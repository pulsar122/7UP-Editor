/*****************************************************************
	7UP
	Modul: MACROREC.C
	(c) by TheoSoft '94

	Aufzeichnung und Wiedergabe von TastaturdrÅcken,
	siehe auch mevent.c
	
	1997-04-07 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "windows.h"
#ifndef ENGLISH											/* (GS) */
	#include "7UP.h"
#else
	#include "7UP_eng.h"
#endif
#include "version.h"
#include "alert.h"
#include "falert.h"
#include "config.h"
#include "fileio.h"
#include "mevent.h"									/* (GS)	*/


#include "macro.h"

#define MAXLONGINT 0x7FFFFFFFL
#define MACROPLAYTIME 10	/* ms zwischen zwei Abspielaktionen */

TMACRO macro = {VERSIONNAME,0x0100,(int)(sizeof(TMACRO)-sizeof(void *)),0L,0L,0L,0L,0L,0L,0L,NULL};

int record_macro(int command, int kstate, int key) 
{
	TMACROBUFF *ip;
	
	switch(command)
	{
		case MACREC:
			if(macro.mp)
				free(macro.mp);
		   macro.mp=(TMACROBUFF *)malloc(MAX_MACROS*sizeof(TMACROBUFF));
			if(macro.mp)
			{
				macro.size  = MAX_MACROS;
				macro.rec   = 1;
				macro.play  = 0;
				macro.macroindex = 0;
				macro.lastmacro = -1;
				macro.lastrep = 1;
				memset(macro.mp, 0, MAX_MACROS*sizeof(TMACROBUFF));
			}
			return(0);
			/*break;*/
		case MACSTOP:
			if(macro.rec)
				macro.rec = 0;
			return(0);
			/*break;*/
		case MACPLAY:
			if(macro.mp)
			{
				macro.macroindex = 0;
				macro.repindex   = 0;
				macro.rec  = 0;
				macro.play = 1;
			}
			return(0);
			/*break;*/
		default:
			if(macro.rec)
			{
				if(macro.lastmacro==-1) /* als erstes die Wiederholrate */
				{
					if(isdigit(key)) /* Ziffer? */
					{
						macro.lastrep=(int)((char)key-'0'); /* 1-9 erlaubt */
						macro.lastmacro++;
					}
					else /* Ziffer!!! */
					{
						macro.rec = 0; /* Fehler, halt! */
						my_form_alert(1,Amacrorec[0]);
					}
					return(0);
				}
				else
				{
					if(macro.lastmacro < macro.size) /* paût das Zeichen noch in den Puffer */
					{
						macro.mp[macro.lastmacro].kstate = kstate;
						macro.mp[macro.lastmacro].key    = key;
						macro.lastmacro++;
					}
					else /* Makroplatz ist zu klein, erweitern wir ihn */
					{
						if(macro.size + MAX_MACROS <= MAXLONGINT) 
						{  /* immer um 256 erweitern, aber nicht mehr als 2^31-1 */
							ip = (TMACROBUFF *)realloc(macro.mp,(macro.size+MAX_MACROS)*sizeof(TMACROBUFF));
							if(ip) /* der Platz reicht */
							{
								macro.mp    = ip;      /* Pointer auf neuen Wert */
								macro.size += MAX_MACROS; /* Puffer grîûer machen */
								/* das letzte Zeichen nehmen wir aber noch mit! */
								macro.mp[macro.lastmacro].kstate = kstate;
								macro.mp[macro.lastmacro].key    = key;
								macro.lastmacro++;
							}
							else /* leider kein Speicher mehr frei */
							{
								my_form_alert(1,Amacrorec[1]);
								macro.rec = 0;
							}
						}
						else /* max. Makrogrîûe erreicht */
						{	
							my_form_alert(1,Amacrorec[1]);
							macro.rec = 0;
						}
					}
				}
			}
			return(MU_KEYBD);
			/*break;*/
	}
}

int play_macro(unsigned int *kstate, unsigned int *key)
{
	if(macro.repindex < macro.lastrep)
	{
		if(macro.macroindex < macro.lastmacro)
		{
		 	*kstate = macro.mp[macro.macroindex].kstate;
			*key	  = macro.mp[macro.macroindex].key;
			macro.macroindex++;
			return MU_KEYBD;
		}
		macro.macroindex=0;
		macro.repindex++;
		return(0);
	}
	macro.repindex=1;
	macro.play=0;
	return(0);
}

void _loadmacro(char *filename)
{
	FILE *fp;

	if((fp=fopen(filename,"rb"))!=NULL)
	{
		if(macro.mp) /* alten MÅll freigeben */
			free(macro.mp);
		fread(&macro,sizeof(TMACRO),1,fp); /* Kopf laden */
	   macro.mp=(TMACROBUFF *)malloc(macro.size*sizeof(TMACROBUFF));
		if(macro.mp) /* Speicher angefordert */
			fread(macro.mp,macro.size*sizeof(TMACROBUFF),1,fp); /* hinein damit */
		fclose(fp);
	}
}

void loadmacro(void)
{
	char *cp, filename[PATH_MAX]="";
	/*static*/ char fpattern[FILENAME_MAX]="*.*";

	strcpy(fpattern,"*.mac");
	find_7upinf(filename,"mac",1 /*0*/);
	if((cp=strrchr(filename,'\\'))!=NULL)
		strcpy(&cp[1],fpattern);
	else
		*filename=0;
	if(getfilename(filename,fpattern,"@",fselmsg[28]))
		_loadmacro(filename);
}

void _savemacro(char *filename)
{
	FILE *fp;

	if(!macro.mp)
		return;

	if((fp=fopen(filename,"wb"))!=NULL)
	{
		fwrite(&macro,sizeof(TMACRO),1,fp); /* Kopfdaten schreiben */
		fwrite(macro.mp,macro.size*sizeof(TMACROBUFF),1,fp); /* Makros schreiben */
		fclose(fp);
	}
}

void savemacro(void)
{
	char *cp, filename[PATH_MAX]="";
	/*static*/ char fpattern[FILENAME_MAX]="*.*";

	strcpy(fpattern,"*.mac");
	find_7upinf(filename,"mac",1 /*0*/);
	if((cp=strrchr(filename,'\\'))!=NULL)
		strcpy(&cp[1],fpattern);
	else
		*filename=0;
	if(getfilename(filename,fpattern,"@",fselmsg[29]))
		_savemacro(filename);
}

/*
	1998-03-06 (MJK): énderungen an der evnt_event-Routine fÅr den
	                  Macrorecorder hier einfÅgen
*/

int mevnt_event(MEVENT *mevent) {
	int retval, flags;
	unsigned long time;
	if( macro.play && (mevent->e_flags & MU_KEYBD) ) {
		/* WÑhrend des Abspielens von MU_KEYBD werden nur noch
		 * MU_MESAG durchgelassen (wegen Redraws)
		 * Damit das funktioniert muû zusÑtzlich ein Timerevent
		 * ausgefÅhrt werden.
		 */
		flags = mevent->e_flags;
		time = mevent->e_time;
		mevent->e_flags &= MU_MESAG;
		mevent->e_flags |= MU_TIMER;
		if ( !(flags & MU_TIMER) || time > MACROPLAYTIME )
			mevent->e_time = MACROPLAYTIME;
		retval = evnt_mevent( mevent );
		retval |= play_macro(&(mevent->e_ks),&(mevent->e_kr));
		mevent->e_flags = flags;
		mevent->e_time = time;
		if ( time > MACROPLAYTIME )
			flags &= ~MU_TIMER;
		retval &= flags;
	} else if ( ( retval = evnt_mevent( mevent ) ) & MU_KEYBD ) {
		if ( !record_macro(0,mevent->e_ks,mevent->e_kr) )
			retval &= ~MU_KEYBD;
	}
	return retval;
}
