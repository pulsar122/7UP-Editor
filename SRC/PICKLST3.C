/*****************************************************************
	7UP
	Modul: PICKLIST.C
	(c) by TheoSoft '93

	Liste der zuletzt editierten Dateien
	
	1997-04-07 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-09 (MJK): Vertikalen Listbox-Slider mit der zugehîrigen
	                  LISTBOX-Funktion verwalten statt mit einer
	                  (jetzt static) FONTSEL-Funktion.
	1997-04-24 (MJK): Mehr als MAXPICKFILES-EintrÑge in der Liste
	                  verhindert.
	PL02
	1997-06-20 (MJK): Pickliste kann beliebig viel EintrÑge
	                  enthalten. Links werden in der Pickliste
	                  verfolgt.
	PICKLST3
	1998-03-07 (MJK): Pickliste wird inklusive Dateiidentifikation
	                  geladen und gespeichert.
	Bemerkung: Die gesamte Dateienlistbox gehîrt eigentlich auf
	           LISTBOX-Funktionen umgestellt!
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <ext.h>
#else
# include <stat.h>
#	include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <aesbind.h>
#	include <vdibind.h>
#endif

#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "alert.h"
#include "forms.h"
#include "windows.h"
#include "7up.h"
#include "version.h"
#include "language.h"
#include "config.h"
#include "7up3.h"
#include "fileio.h"
#include "findrep.h"
#include "listbox.h"
#include "objc_.h"

#include "picklist.h"

#define MAXENTRIES      8
#define MAXNAMELETTERS 42
#define MAXINFOLETTERS 41
#define notnull(a) (((a)>0)?(a):(1))
#define FLAGS15 0x8000

typedef struct
{
	char  name[PATH_MAX];
	long  line;
	long  dev; /* mit -1 wird ein ungÅltiger dev_t signalisiert */
	ino_t ino;
	char  info[MAXINFOLETTERS];
}PICKLISTE;

static PICKLISTE *pl=NULL;
static int pl_entries=0;

#define NEWVERSIONNAME  "7UP 2.33 Freeware"
#define NEWVERSIONSTRLEN (strlen(NEWVERSIONNAME)+1)

void free_picklist( void )
{
	if(pl)
		free(pl);
	pl=NULL;
	pl_entries=0;
	return;
}

int load_picklist( void )
{
	FILE *fp;
	fpos_t start;
	char filename[PATH_MAX];
	int count = 0;
	struct stat st;

	if((fp=fopen(find_7upinf(filename,"PCK",0),"r"))!=NULL)
	{															/* Pickfile îffnen */
		graf_mouse(BUSY_BEE,NULL);
		fgets(filename,(int)(VERSIONSTRLEN+1),fp);
		filename[strlen(filename)-1]=0; /*CR weg*/
		if(!strcmp(filename,VERSIONNAME) || !strcmp(filename,NEWVERSIONNAME) ) {
			/* 1997-06-20 (MJK): Picklist dynamisch anpassen */
			for ( pl_entries = 0, fgetpos(fp, &start);
			      fgets(filename,PATH_MAX,fp) && /* Dateiname */
			       fgets(filename,30,fp) && /* Zeile, dev, ino */
			       fgets(filename,MAXINFOLETTERS,fp);   /* Info */
			      pl_entries++ );
			if( pl_entries > 0 ) {
				fsetpos(fp,&start); /* ZurÅck auf Anfang der Liste */
				if((pl=calloc(pl_entries,sizeof(PICKLISTE)))!=NULL)	{
					for( count = 0; 
					     count < pl_entries && fgets(pl[count].name,PATH_MAX,fp);
					     count += (pl[count].dev != -1) ? 1 : 0 ) {
						pl[count].name[strlen(pl[count].name)-1]=0; /*CR weg*/
						fscanf( fp, "%ld", &(pl[count].line) );
						if ( fgetc( fp ) != ' ' ) {
							if ( !stat( pl[count].name, &st ) ) {
								pl[count].dev = st.st_dev;
								pl[count].ino = st.st_ino;
							} else {
								pl[count].dev = -1;
								pl_entries--;
							}
						} else {
							fscanf( fp, "%ld %lu",&(pl[count].dev),&(pl[count].ino));
							fgetc( fp ); /* '\n' Åberlesen */
						}
						
						fgets(pl[count].info,MAXINFOLETTERS,fp);
						pl[count].info[strlen(pl[count].info)-1]=0; /*CR weg*/
					}
				} else {
					form_alert(1,Apicklist[3]);
					count = -1;
				}
			}
		}	else if(filelength(fileno(fp)) > 0)
			form_alert(1,Apicklist[2]);
		fclose(fp);
		graf_mouse(ARROW,NULL);	
	}
	return count;
}

void save_picklist(void)
{
	FILE *fp;
	char filename[PATH_MAX];
	int i;

	if((fp=fopen(find_7upinf(filename,"PCK",1),"w"))!=NULL) {													/* Pickfile îffnen */
		graf_mouse(BUSY_BEE,NULL);	
		fprintf(fp,"%s\n",NEWVERSIONNAME);
		for(i=0; i<pl_entries; i++)
			if(*pl[i].name && pl[i].dev != -1L)	{
				fprintf(fp,"%s\n",pl[i].name);
				fprintf(fp,"%ld %ld %lu\n",pl[i].line,pl[i].dev,pl[i].ino);
				fprintf(fp,"%s\n",pl[i].info);
			}
		fclose(fp);												/* Datei schlieûen */
		graf_mouse(ARROW,NULL);	
	}
}

static int getentry(char *pathname) {
	int i;
	struct stat s,ss;
	if ( pl ) {
		if ( stat( pathname, &s ) ) {
			for ( i = 0; i < pl_entries; i++ )
				if ( samefile( pl[i].name, pathname ) )
					return i;
		} else {
			for ( i = 0; i < pl_entries; i++ )
				if ( pl[i].dev != -1L && (int)pl[i].dev == s.st_dev && pl[i].ino == s.st_ino ) {
					if ( stat( pl[i].name, &ss ) || s.st_dev != ss.st_dev || s.st_ino != ss.st_ino )
						strcpy( pl[i].name, pathname );
					return i;
				}
		}
	}
	return -1;
}

void append_picklist(OBJECT * tree, char *pathname, long line)
{
	FILE *fp;
	char filename[PATH_MAX];
	int i, j;
	struct stat st;
	
	if(Wcount(CREATED)==0)
		return;

	if(!(tree[PICKAKTIV].ob_state & SELECTED)) /* ist ausgeschaltet 8.1.95 */
		return;

	if( load_picklist() < 0 )
		/* Wenn die Pickliste nicht geladen werden konnte, erfolgt
		 * auch kein Eintrag in dieselbe */
		return;
	
	if((fp=fopen(find_7upinf(filename,"PCK",1),"w"))!=NULL) { /* Pickfile îffnen */
		graf_mouse(BUSY_BEE,NULL);	
		fprintf(fp,"%s\n",NEWVERSIONNAME);
		/* Erst die neuen bzw. geÑnderten, ... */
		if ( pathname ) {
			fprintf(fp,"%s\n",pathname);
			if ( (i = getentry(pathname)) >= 0) {
				*pl[i].name = '\0';
				fprintf(fp,"%ld %ld %lu\n", line, pl[i].dev, pl[i].ino);
				fprintf(fp,"%s\n",pl[i].info);
			} else {
				if ( stat( pathname, &st ) )
					fprintf(fp,"%ld\n", line );
				else
					fprintf(fp,"%ld %ld %lu\n", line, (long)st.st_dev, st.st_ino );
				fputc( '\n', fp );
			}
		} else for ( j = 0; j<MAXWINDOWS; j++ ) {
			if( _wind[j].w_state & CREATED ) {
				fprintf(fp,"%s\n",Wname(&_wind[j]));
				if ( (i = getentry( Wname(&_wind[j]) )) >= 0 ) {
					*pl[i].name = '\0';
					fprintf(fp,"%ld %ld %lu\n",_wind[j].row + _wind[j].hfirst/_wind[j].hscroll + 1, pl[i].dev, pl[i].ino );
					fprintf(fp,"%s\n",pl[i].info);
				} else {
					if ( stat( Wname(&_wind[j]), &st ) ) 
						fprintf(fp,"%ld\n",_wind[j].row + _wind[j].hfirst/_wind[j].hscroll + 1 );
					else
						fprintf(fp,"%ld %ld %lu\n",_wind[j].row + _wind[j].hfirst/_wind[j].hscroll + 1, (long)st.st_dev, st.st_ino );
					fputc( '\n', fp );
				}
			}
		}
		/* dann die alten bzw. unverÑnderten */
		for(i=0; i<pl_entries; i++)
			if(*pl[i].name)	{
				fprintf(fp,"%s\n",pl[i].name);
				fprintf(fp,"%ld %ld %lu\n",pl[i].line,pl[i].dev,pl[i].ino);
				fprintf(fp,"%s\n",pl[i].info);
			}
		fclose(fp);												/* Datei schlieûen */
		graf_mouse(ARROW,NULL);	
	}
	
	free_picklist();
}

static char *stradj(char *dest, char *src, int maxlen)
{
	register int len;
	dest[0]=' ';
	if((len=(int)strlen(src))>maxlen)
	{
		strncpy(&dest[1],src,maxlen/2);
		strncpy(&dest[maxlen/2+1],&src[len-maxlen/2],maxlen/2);
		dest[maxlen/2-1+1]='.';
		dest[maxlen/2+0+1]='.';
		dest[maxlen/2+1+1]='.';
	}
	else
	{
		strcpy(&dest[1],src);
		memset(&dest[len+1],' ',maxlen-len+1);
	}
	dest[maxlen+1]=' ';
	dest[maxlen+2]=0;
	return(dest);
}

static int list_do(OBJECT *tree, int exit_obj, long *first, int *which, int count, int *done)
{
	int my,ret,newpos,oby,kstate;
/*	
	*which=-1;
*/
	graf_mkstate(&ret,&my,&ret,&kstate);
	switch(exit_obj)
	{
		case PICKUP:
			if(kstate & (K_LSHIFT|K_RSHIFT))
			{
				if((*first-MAXENTRIES)>0)
					*first-=MAXENTRIES;
				else
					*first=0;
			}
			else
			{
				if(*first>0)
					(*first)--;
				else
					exit_obj=-1;
			}
			break;
		case PICKDN:
			if(kstate & (K_LSHIFT|K_RSHIFT))
			{
				if((*first+MAXENTRIES)<(count-MAXENTRIES))
					*first+=MAXENTRIES;
				else
					*first=count-MAXENTRIES;
			}
			else
			{
				if(*first<count-MAXENTRIES)
					(*first)++;
				else
					exit_obj=-1;
			}
			break;
		case PICKSLID:
			graf_mouse(FLAT_HAND,NULL);
			newpos=graf_slidebox(tree,PICKBOX,PICKSLID,1);
			graf_mouse(ARROW,NULL);
			*first=((newpos*(count-MAXENTRIES))/1000L);
			break;
		case PICKBOX:
			objc_offset(tree,exit_obj+1,&ret,&oby);
			if(my>oby)
			{
				if((*first+MAXENTRIES)<(count-MAXENTRIES))
					*first+=MAXENTRIES;
				else
					*first=count-MAXENTRIES;
			}
			else
			{
				if((*first-MAXENTRIES)>0)
					*first-=MAXENTRIES;
				else
					*first=0;
			}
			break;
		case PICKFIRST:
		case PICKFIRST+1:
		case PICKFIRST+2:
		case PICKFIRST+3:
		case PICKFIRST+4:
		case PICKFIRST+5:
		case PICKFIRST+6:
		case PICKLAST:
			*which=(int)(exit_obj-PICKFIRST+*first);
			break;
	}
	if((exit_obj) & 0x8000) /* Verlassen mit Doppelklick */
	{
		switch((exit_obj) &= 0x7FFF)
		{
			case PICKUP:
				if(*first>0)
					*first=0;
				else
					/* Wenn bereits ein Objekt selektiert dies behalten,
					 * sonst Ñndern (MJK 3/97) */
				if(*which>-1)
					exit_obj=-1;
				break;
			case PICKDN:
				if(*first<count-MAXENTRIES)
					*first=count-MAXENTRIES;
				else
					/* Wenn bereits ein Objekt selektiert dies behalten,
					 * sonst Ñndern (MJK 3/97) */
				if(*which>-1)
					exit_obj=-1;
				break;
			case PICKFIRST:
			case PICKFIRST+1:
			case PICKFIRST+2:
			case PICKFIRST+3:
			case PICKFIRST+4:
			case PICKFIRST+5:
			case PICKFIRST+6:
			case PICKLAST:
				*which=(int)(exit_obj-PICKFIRST+*first);
				*done=1;
				break;
		}
	}
	return(exit_obj);
}

static int _hndl_picklist(OBJECT *tree, char *filename, long *line)
{
	static long first2=0;

	long first,newpos;
	int which=-1,count,count2,exit_obj,done=0,changed=0;
	int infochanged=0; /* (MJK 3/97) */
	int i,k;
	struct stat st;
	
	char string[MAXINFOLETTERS];

	count=load_picklist();
	if(count<=0)
		return(-1);
	count2=count;
/*	
	hsort(pl,count,sizeof(char *),strcmp);
*/
/**************************************************************************/
/* letzte OK-Werte einstellen, falls verstellt und Abbruch gedrÅckt wurde */
/**************************************************************************/
	first=first2;

	for(i=PICKFIRST; i<=PICKLAST; i++)
	{
		tree[i].ob_state&=~SELECTED;
		tree[i].ob_flags&=~SELECTABLE;
	}
	tree[PICKSLID].ob_height=
			 (int)max(boxh,MAXENTRIES*tree[PICKBOX].ob_height/max(MAXENTRIES,count));
	newpos=/*(int)*/((first*1000L)/notnull(count-MAXENTRIES));
	newpos=max(0,newpos);
	newpos=min(newpos,1000);
	setsliderpos(tree,PICKSLID,(int)newpos); /* 1997-04-09 (MJK): LISTBOX-Funktion */

	for(i=(int)first,k=0; i<count && k<MAXENTRIES; i++,k++)
	{
		stradj(string,pl[i].name,(int)(MAXNAMELETTERS-2));
		form_write(tree,PICKFIRST+k,string,0);
		tree[PICKFIRST+k].ob_flags|=SELECTABLE;
	}
/*MT 16.7.94*/
	form_write(tree,PICKCOMMENT,"",0);

	memset(string,' ',MAXNAMELETTERS);
	string[MAXNAMELETTERS]=0;
	for(; k<MAXENTRIES; k++)
		form_write(tree,PICKFIRST+k,string,0);
/*
	if(get_cookie('MiNT') && (_GemParBlk.global[1] != 1)) /* MultiTOS */
		tree[PICKBOX].ob_spec.obspec.fillpattern=4;
*/
	tree[PICKDEL].ob_state|=DISABLED;
	tree[PICKCLEAN].ob_state|=DISABLED;

	sprintf(string,PICKFILES,count2);
	form_write(tree,PICKINFO,string,0);
/*   
	if(_GemParBlk.global[0] >=0x0340) /* Ab Falcon Muster Ñndern */
	{
		tree[PICKBOX ].ob_spec.obspec.fillpattern=4;
	}
*/
	form_exopen(tree,0);
	do
	{
		exit_obj=form_exdo(tree,0);

		/* énderungen des Infotexte feststellen und zur spÑteren
		 * Sicherung merken. (MJK 3/97) */
		if(which>-1 && strcmp(form_read(tree,PICKCOMMENT,string),pl[which].info))
		{
			strcpy(pl[which].info,string);
			infochanged=1;
		}

		exit_obj=list_do(tree, exit_obj, &first, &which, count, &done);
		switch(exit_obj)
		{
			case PICKHELP:
				form_alert(1,Apicklist[0]);
				objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
				break;
			case PICKDEL:
				if(which>-1)
				{
					graf_mouse(BUSY_BEE,NULL);	
				   if(tree[PICKCLEAN].ob_state & SELECTED)
				   {
				      for(i=0; i<count; i++)
				      {
/* Lîschen, wenn A | B | N | (U & (A | B)) | 'nicht existent' */
				         if((toupper(pl[i].name[0]) == 'A') ||
				            (toupper(pl[i].name[0]) == 'B') ||
				            (toupper(pl[i].name[0]) == 'N') ||
				            ((toupper(pl[i].name[0]) == 'U') && 
				             ((toupper(pl[i].name[3]) == 'A') ||
				              (toupper(pl[i].name[3]) == 'B'))) ||
				            (stat(pl[i].name,&st)) )
							{
									*pl[i].name=0; /* Lîschen */
									if((i+PICKFIRST-first)>=PICKFIRST && 
									   (i+PICKFIRST-first)<=PICKLAST)
									{
										tree[i+PICKFIRST-first].ob_state&=~SELECTED;
										memset(string,' ',MAXNAMELETTERS);
										string[MAXNAMELETTERS]=0;
										form_write(tree,(int)(i+PICKFIRST-first),string,1);
									}
							} else {
								pl[i].dev = st.st_dev;
								pl[i].ino = st.st_ino;
							}
				     }
				   }
				   else
					{
						*pl[which].name=0; /* Lîschen */
						tree[which+PICKFIRST-first].ob_state&=~SELECTED;
						memset(string,' ',MAXNAMELETTERS);
						string[MAXNAMELETTERS]=0;
						form_write(tree,(int)(which+PICKFIRST-first),string,1);
					}
					save_picklist();
					which=-1;
					changed=1;
				   sprintf(string,PICKFILES,--count2);	
				   form_write(tree,PICKINFO,string,1);
					graf_mouse(ARROW,NULL);	
				}
				objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,
								tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
				tree[PICKCLEAN].ob_state&=~SELECTED;
				tree[PICKCLEAN].ob_state|=DISABLED;
				objc_update(tree,PICKCLEAN,0);
				tree[PICKDEL].ob_state|=DISABLED;
				objc_update(tree,PICKDEL,0);
				break;
			case PICKFIRST:
			case PICKFIRST+1:
			case PICKFIRST+2:
			case PICKFIRST+3:
			case PICKFIRST+4:
			case PICKFIRST+5:
			case PICKFIRST+6:
			case PICKLAST:
			   sprintf(string,PICKFILESSELECTED,count2);	
			   form_write(tree,PICKINFO,string,1);
/*MT 16.7.94*/
				if(which>-1)
					form_write(tree,PICKCOMMENT,pl[which].info,1);
				objc_change(tree,PICKCLEAN,0,tree->ob_x,tree->ob_y,
								tree->ob_width,tree->ob_height,tree[PICKCLEAN].ob_state&~DISABLED,1);
				objc_change(tree,PICKDEL,0,tree->ob_x,tree->ob_y,
								tree->ob_width,tree->ob_height,tree[PICKDEL].ob_state&~DISABLED,1);
				changed=1;
				break;
			case PICKUP:
			case PICKDN:
			case PICKBOX:
			case PICKSLID:
				if(count>MAXENTRIES)
				{
					for(i=PICKFIRST; i<=PICKLAST; i++)
						if(tree[i].ob_state & SELECTED)
							which=(int)(i-PICKFIRST+first);
					newpos=/*(int)*/((first*1000L)/notnull(count-MAXENTRIES));
					newpos=min(max(0,newpos),1000);
					setsliderpos(tree,PICKSLID,(int)newpos);
					objc_update(tree,PICKBOX,MAX_DEPTH);
					for(i=(int)first,k=0; k<(int)MAXENTRIES; i++,k++)
					{
						stradj(string,pl[i].name,(int)(MAXNAMELETTERS-2));
						form_write(tree,PICKFIRST+k,string,1);
					}
					if(which>-1)
						form_write(tree,PICKCOMMENT,pl[which].info,1);
				}
				break;
			case PICKOK:
			case PICKABBR:
				done=1;
				break;
		}
	}
	while(!done);
	form_exclose(tree, exit_obj, 0);
	tree[PICKOK].ob_state&=~SELECTED;
	if(which>-1)
	{
		strcpy(filename,pl[which].name);
		*line = pl[which].line;
		if ( stat( filename, &st ) )
			pl[which].dev = -1;
		else {
			pl[which].dev = st.st_dev;
			pl[which].ino = st.st_ino;
		}
	}

	/* Im Falle der énderung eines oder mehrerer Infotexte, die
	 * geÑnderte Picklist speichern (MJK 3/97) */
	if(infochanged && (exit_obj==PICKOK))
	{
		save_picklist();
		changed=1;
	}
	if(changed)/*nach Lîschen auf null, damit Neuaufbau klappt*/
		first=0;
/*
	if(changed && (exit_obj==PICKOK))
	{
		if(which>-1)
			form_read(tree,PICKCOMMENT,pl[which].info);
		save_picklist();
		first=0;
	}
*/
	free_picklist();
	first2=first;
	switch(exit_obj)
	{
		case PICKFIRST:
		case PICKFIRST+1:
		case PICKFIRST+2:
		case PICKFIRST+3:
		case PICKFIRST+4:
		case PICKFIRST+5:
		case PICKFIRST+6:
		case PICKLAST:
		case PICKOK:
			if(which<=-1)
				*filename = '\0';
			return(1);
	}
	return(0);
}

void hndl_picklist(OBJECT *tree)
{
	char filename[PATH_MAX];
	long line=0;
	WINDOW *wp;
	int a;
	a = tree[PICKAKTIV].ob_state;
	
	switch(_hndl_picklist(tree,filename,&line))
	{
		case 1:
			if(*filename && (wp=Wreadtempfile(filename,0))!=NULL)
				if(line > 1) /* nur wenn positioniert werden soll */
					hndl_goto(wp, NULL, line);
			break;
		case -1:
			form_alert(1,Apicklist[1]);
			/* durchfallen!!! */
		case 0:
			tree[PICKAKTIV].ob_state = a;
			break;
	}
}
