/*****************************************************************
	7UP
	Modul: DEADKEY.C
	(c) by TheoSoft '92
	
	Deadkeys

	1997-03-27 (MJK): ben�tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	                  int-deklarierte Funktionen ohne Return-Wert
	                  auf void korrigiert
	1997-04-11 (MJK): Eingeschr�nkt auf GEMDOS
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
# define event_timer( time ) evnt_timer( (int)(time >> 16), (int)(time & 0xFFFF) )
#else
#	include <gem.h>
# define event_timer( time ) evnt_timer( time )
#endif
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <ext.h>
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#ifndef ENGLISH											/* (GS) */
	#include "7UP.h"
#else
	#include "7UP_eng.h"
#endif
#include "alert.h"
#include "falert.h"
#include "di_fly.h"
#include "resource.h"
#include "fileio.h"
#include "forms.h"
#include "config.h"
#include "menuikey.h"
#include "objc_.h"

#include "deadkey.h"

char shortcutfile[32]="";
int scaktiv;

typedef struct /* Deadkeystruktur */
{
	unsigned char dead,sym,sign;
} DK;

static DK dk[]= /* Deadkeyliste  */
{
	'\'','a','�',
	'\'','e','�',
	'\'','E','�',
	'\'','i','�',
	'\'','o','�',
	'\'','u','�',

	'`','a','�',
	'`','A','�',
	'`','e','�',
	'`','i','�',
	'`','o','�',
	'`','u','�',

	'^','a','�',
	'^','e','�',
	'^','i','�',
	'^','o','�',
	'^','u','�',

	'~','a','�',
	'~','A','�',
	'~','n','�',
	'~','N','�',
	'~','o','�',
	'~','O','�',

	'o','a','�',
	'o','A','�',

	'/','o','�',
	'/','O','�',

	',','c','�',
	',','C','�',

	'_','a','�',
	'_','o','�',

	'\"','e','�',
	'\"','i','�',
	'\"','y','�',

	'a','e','�',
	'A','E','�',
	'<','<','�',
	'>','>','�',
	'!','!','�',
	'?','?','�',
	'f','f','',
	'F','F','',

	'1','2','�',
	'1','4','�',

	'+','-','�',
	'>','=','�',
	'<','=','�',

	'^','2','�',
	'^','3','�'
};

static char *shortcut=NULL;

int deadkey(int dead, int key, char **cp)				/* Zeichen wandeln			  */
{
	register int i;
	register char sct[4];
	
	sct[0]=dead;
	sct[1]=key;
	sct[2]='=';
	sct[3]=0;
	*cp=NULL;	
	if(shortcut && (*cp=strstr(shortcut,sct))!=NULL)/* Shortcut aus Datei */
	{																/* nachtr�glich erweitert */
		(*cp)+=3;
		return(0);
	}
	
	for(i=0; i<sizeof(dk)/3; i++)
	{
		if(dk[i].dead == dead)				  /* Deadkey gefunden			 */
			for(/*i*/; dk[i].dead == dead; i++)  /* weiter mit den Symbolen	*/
				if(dk[i].sym == key)			 /* Zeichen gefunden			 */
					return(dk[i].sign);		  /* Sonderzeichen zur�ckgeben */
	}
	return(key);
}

int isdeadkey(int dead)			  /* geh�rt das Zeichen zu den Deadkeys */
{
	register int i;
	register char *cp;
	
	if(shortcut)						 /* nachladbare Shortcuts */
	{
		cp=shortcut;
		while((cp=strchr(cp,'='))!=NULL)
		{
			if((*(cp-2) == dead) || (*(cp-3) == dead))
				return(dead);
			cp++;
		}
	}

	for(i=0; i<sizeof(dk)/3; i++) /* erstes Zeichen der Liste abklappern */
	{
		if(dk[i].dead == dead)	  /* Zeichen gefunden						  */
			return(dead);
	}
	return(0);
}

static void _loadshortcuts(char *filename)
{
	FILE *fp;
	register char *cp;
	
	if((fp=fopen(filename,"rb"))!=NULL)	
	{
		form_write(menueditor,MENUTFILE,split_fname(filename),0);
		graf_mouse(BUSY_BEE,NULL);
		if(shortcut)
			free(shortcut);
		if((shortcut=malloc(filelength(fileno(fp))+1))!=NULL)
		{
			fread(shortcut,1,filelength(fileno(fp)),fp);
			shortcut[filelength(fileno(fp))]=0; /* letztes Zeichen eine 0 */
/*
			strcpy(filename,(char *)split_fname(filename));
			if((cp=strchr(filename,'.'))!=NULL)
				*cp=0;
*/
         strcpy(shortcutfile,(char *)split_fname(filename)); /* wird in Infozeile eingeblendet */

			if((cp=strrchr(shortcut,'#'))!=NULL) /* Kommentarkopf abschneiden */
				if((cp=strchr(cp,'\n'))!=NULL)
					memmove(shortcut,cp+1,strlen(cp+1)+1);
			cp=shortcut;
			while((cp=strchr(cp,'='))!=NULL)		 /* Leerzeichen vor den '=' l�schen */
			{
				if(*(cp-1)==' ')
					memmove(cp-1,cp,strlen(cp)+1);
				else
					cp++;
			}
		}
		else
			my_form_alert(1,Adeadkey[0]);
		fclose(fp);
		graf_mouse(ARROW,NULL);
	}
}

void loadshortcuts(char *filename)
{
	char pathname[PATH_MAX];
	
	search_env(pathname,filename,0); /* READ */
	_loadshortcuts(pathname);
}

static void _hndl_shortcuts(OBJECT *tree)
{
	char *cp,str[4],filename[PATH_MAX]="";
	int a, exit_obj, desk=1, m_title, c_title, m_entry, c_entry;
	/*static*/ char fpattern[FILENAME_MAX]="*.*";

	m_title=(winmenu+winmenu->ob_head)->ob_head;
	c_title=(winmenu+m_title)->ob_head;
	m_entry=(winmenu+winmenu->ob_tail)->ob_head;
	c_entry=(winmenu+m_entry)->ob_head;

	tree[MENUTSAVE].ob_flags|=HIDETREE;

	tree[MENUMIDX].ob_flags&=~EDITABLE;
	form_write(tree,MENUMNAME,(char *)winmenu[c_entry].ob_spec.index,0);
	sprintf(str,"%003d",c_entry);
	
	a = tree[MENUTAKTIV].ob_state; /* merken */
	
	form_write(tree,MENUMIDX,str,0);
	form_exopen(tree,0);
   do
   {
      exit_obj=(form_exdo(tree,0)&0x7FFF);
      switch(exit_obj)
      {
			case MENUMVOR:
				form_read(tree,MENUMNAME,(char *)winmenu[c_entry].ob_spec.index);
				c_entry=(winmenu+c_entry)->ob_next;
				if(desk) /* Deskmenu nicht auswerten */
				{
					c_entry=m_entry;
					desk=0;
				}
				if(c_entry == m_entry || c_entry == -1)
				{
					c_title=(winmenu+c_title)->ob_next;
					m_entry=(winmenu+m_entry)->ob_next;
					c_entry=(winmenu+m_entry)->ob_head;
					if(c_title==m_title)
					{
						m_title=(winmenu+winmenu->ob_head)->ob_head;
						c_title=(winmenu+m_title)->ob_head;
						m_entry=(winmenu+winmenu->ob_tail)->ob_head;
						c_entry=(winmenu+m_entry)->ob_head;
						desk=1;
					}
				}
				if(!*(char *)winmenu[c_entry].ob_spec.index ||
					 *(char *)winmenu[c_entry].ob_spec.index == '-')
					c_entry=(winmenu+c_entry)->ob_next;
				while(c_entry>=WINDAT1 && c_entry<=WINDAT7) /* diese ben�tigen wir nicht */
					c_entry=(winmenu+c_entry)->ob_next;
				if(c_entry == m_entry || c_entry == -1)
				{  /* weiter zum n�chsten Men�, wg. WINDAT7 = letzter Eintrag */
					c_title=(winmenu+c_title)->ob_next;
					m_entry=(winmenu+m_entry)->ob_next;
					c_entry=(winmenu+m_entry)->ob_head;
				}
				form_write(tree,MENUMNAME,(char *)winmenu[c_entry].ob_spec.index,1);
				sprintf(str,"%003d",c_entry);
				form_write(tree,MENUMIDX,str,1);
				event_timer(62);
        		break;
			case MENUMHELP:
			   my_form_alert(1,Adeadkey[4]);
				objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
        		break;
			case MENUMSAVE:
				form_read(tree,MENUMNAME,(char *)winmenu[c_entry].ob_spec.index);
				strcpy(fpattern,"*.mnu");
				find_7upinf(filename,"mnu",1);
				if((cp=strrchr(filename,'\\'))!=NULL || (cp=strrchr(filename,'/'))!=NULL)
					strcpy(&cp[1],fpattern);
				else
					*filename=0;
				if(getfilename(filename,fpattern,"@",fselmsg[5]))
					_savemenu(filename);
				tree[exit_obj].ob_state&=~SELECTED;
				if(!windials)
					objc_update(tree,ROOT,MAX_DEPTH);
				else
					objc_update(tree,exit_obj,0);
			   break;
			case MENUMLOAD:
			   /* mu� 1, weil nicht 7UP.MNU sondern auch XYZ.MNU g�ltig ist */
				strcpy(fpattern,"*.mnu");
				find_7upinf(filename,"mnu",1 /*0*/); 
				if((cp=strrchr(filename,'\\'))!=NULL || (cp=strrchr(filename,'/'))!=NULL)
					strcpy(&cp[1],fpattern);
				else
					*filename=0;
				if(getfilename(filename,fpattern,"@",fselmsg[6]))
				{
					_loadmenu(filename);
					form_write(tree,MENUMFILE,split_fname(filename),1);
				}
				
				/* neu aufsetzen */
				m_title=(winmenu+winmenu->ob_head)->ob_head;
				c_title=(winmenu+m_title)->ob_head;
				m_entry=(winmenu+winmenu->ob_tail)->ob_head;
				c_entry=(winmenu+m_entry)->ob_head;

				form_write(tree,MENUMNAME,(char *)winmenu[c_entry].ob_spec.index,1);
				tree[exit_obj].ob_state&=~SELECTED;
				if(!windials)
					objc_update(tree,ROOT,MAX_DEPTH);
				else
					objc_update(tree,exit_obj,0);
			   break;
			case MENUTHELP:
			   my_form_alert(1,Adeadkey[5]);
				objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
        		break;
			case MENUTSAVE:
				break;
			case MENUTLOAD:
				strcpy(fpattern,"*.kbd");
				find_7upinf(filename,"kbd",1 /*0*/);
				if((cp=strrchr(filename,'\\'))!=NULL || (cp=strrchr(filename,'/'))!=NULL)
					strcpy(&cp[1],fpattern);
				else
					*filename=0;
				if(getfilename(filename,fpattern,"@",fselmsg[7]))
				{
					_loadshortcuts(filename);
					form_write(tree,MENUTFILE,split_fname(filename),1);
				}
				tree[exit_obj].ob_state&=~SELECTED;
				if(!windials)
					objc_update(tree,ROOT,MAX_DEPTH);
				else
					objc_update(tree,exit_obj,0);
			   break;
			case MENUOK:
				form_read(tree,MENUMNAME,(char *)winmenu[c_entry].ob_spec.index);
        		break;
      }
   }
   while(exit_obj!=MENUOK);
	form_exclose(tree,exit_obj,0);
	if (exit_obj!=MENUOK)
	{
		tree[MENUTAKTIV].ob_state = a;
	}
	else
	{
		if(tree[MENUTAKTIV].ob_state & SELECTED)
			scaktiv=1;
		else
			scaktiv=0;
	}
}

void hndl_shortcuts(void)
{
	int ret, kstate;

	graf_mkstate(&ret,&ret,&ret,&kstate);
	if(kstate & (K_LSHIFT|K_RSHIFT))
	{
		if(shortcut)
		{
			free(shortcut);
			shortcut=NULL;
			*shortcutfile=0;
			form_write(menueditor,MENUTFILE,"",0);
			return;
		}
	}
	_hndl_shortcuts(menueditor);
}
