/*****************************************************************
	7UP
	Modul: FBINFO.C
	(c) by TheoSoft '91

	Dateiinformationen Datum, Uhrzeit ...

	1997-03-27 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	                  Bug in textcompare() behoben
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
	1997-04-21 (MJK): Unabh„ngig von den TC/PC-Libraries
	2000-03-25 (GS) : Y2K-Fehler beim Fileinfo behoben
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <tos.h>
#	include <ext.h>
#	define _DISKINFO DISKINFO
#else
#	include <dirent.h>
#	include <osbind.h>
#	include <support.h>
#	define ltoa(a,b,c) _ltoa(a,b,c)
#	include <stat.h>
#	include <time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
# define event_timer( time ) evnt_timer( (int)(time >> 16), (int)(time & 0xFFFF) )
#else
#	include <aesbind.h>
#	include <vdibind.h>
# define event_timer( time ) evnt_timer( time )
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "alert.h"

#include "7up.h"
#include "windows.h"
#include "version.h"
#include "macro.h"
#include "language.h"
#include "resource.h"
#include "desktop.h"
#include "7up3.h"
#include "forms.h"
#include "fileio.h"
#include "printer.h"
#include "findrep.h"
#include "editor.h"
#include "deadkey.h"
#include "di_fly.h"
#include "graf_.h"

#include "fbinfo.h"

#define INFONAME_LEN 20

/* dta.h(dateistruktur) 14.04.87  */

typedef struct datetimerec
{
	unsigned  hr  : 5;
	unsigned  min : 6;
	unsigned  sec : 5;
	unsigned  yr  : 7;
	unsigned  mon : 4;
	unsigned  day : 5;
}DATETIMEREC;

typedef struct dtarec
{
	char res[21];
	char attr;
	DATETIMEREC datim;
	long size;
	char filename[FILENAME_MAX];
}MYDTA;

char *make_newpathname(char *name, char *new)
{
	char *cp;
	char temp[PATH_MAX];

	strcpy(temp,name);
	if((cp=strrchr(temp,'\\'))!=NULL || (cp=strrchr(temp,'/'))!=NULL)
	{
		*(cp+1)=0;
		strcat(temp,new);
		strcpy(new,temp);
	}
	return(new);
}

static void make_infoname(char *filename, int mode)
{
	register int i,k;
	char temp[FILENAME_MAX];

   if(mode)               /* 1 = komprimieren */
   {
      k=(int)strlen(filename);
      for(i=k; i<12; i++)
         filename[i]=0;
      strncpy(temp,filename,8);
      temp[8]=0;
      for(i=0; i<8; i++)
         if(temp[i]==' ')
            temp[i]=0;
      if(filename[8] && filename[8]!=' ')
      {
         strcat(temp,".");
         filename[11]=0;
         strcat(temp,&filename[8]);
      }
      for(i=0; i<12; i++)
         if(temp[i]==' ')
            temp[i]=0;
      temp[12]=0;
      strcpy(filename,temp);
   }
   else                  /* 0 = expandieren */
   {
      k=(int)strlen(filename);
		for(i=0; i<9; i++)
         if(filename[i] && filename[i]=='.')
         {
            filename[i]=0;
				break;
         }
		if(k>i && filename[i+1] && filename[i+1]!=' ')
      {
         sprintf(temp,"%-8s%-3s",filename,&filename[i+1]);
         strcpy(filename,temp);
         for(i=9; i<12; i++)
            if(filename[i]==' ')
               filename[i]=0;
      }
   }
}

void hndl_fileinfo(WINDOW *wp, WINDOW *blkwp)
{
	register LINESTRUCT *line;
	char *cp,filename[FILENAME_MAX];
	long lines,chars,frei;
#if defined( __TURBOC__ ) && !defined( __MINT__ )
	struct ffblk fileRec;
	MYDTA mydta;
#else
	struct stat finfo;
	struct tm *lt;
	DIR *dir;
	struct dirent *dent;
#endif
	int hfirst=0;
	_DISKINFO dtable;
	long tt_ram;

	int exit_obj,maxlen=0,i,k,count,do_blink=1;

	if(wp && !is_selected(desktop,DESKICN1,DESKICND))
	{
		i=wp->icon;
		sel_icons(desktop,i,i,1);
		do_blink=0;
	}
	if(nodesktop)
		goto NODESKTOP;
	for(i=DESKICN1; i<=DESKICND; i++)
	{
		if(desktop[i].ob_state & SELECTED)
		{
			wp=Wicon(i);						/* nimm Icon fr Info */
			if(do_blink)
			{
				desel_icons(desktop,i,i,1);		  /* 3 x blinken lassen */
				event_timer(125);
				sel_icons(desktop,i,i,1);
				event_timer(125);
				desel_icons(desktop,i,i,1);
				event_timer(125);
				sel_icons(desktop,i,i,1);
			}
			switch(i)
			{
				case DESKICN1:
				case DESKICN2:
				case DESKICN3:
				case DESKICN4:
				case DESKICN5:
				case DESKICN6:
				case DESKICN7:
NODESKTOP:
					if(wp && wp->fstr)
					{
						if(wp==blkwp && topwin && !cut && begcut && endcut)
						{
							form_write(infomenu,INFOTITL,BLOCKINFO,0);
							Wblksize(wp,begcut,endcut,&lines,&chars);
						}
						else
							Wtxtsize(wp,&lines,&chars);

/* Dateiname TOS und MiNTversion */
						if(get_cookie('MiNT'))
						{
							infomenu[INFOTNAME].ob_flags|=HIDETREE;
							infomenu[INFOTNAME].ob_flags&=~EDITABLE;
							infomenu[INFOMNAME].ob_flags&=~HIDETREE;
							strcpy(filename,split_fname((char *)Wname(wp)));
							if(strlen(filename)>INFONAME_LEN)
							{
								infomenu[INFOZUR  ].ob_flags&=~HIDETREE;
								infomenu[INFOVOR  ].ob_flags&=~HIDETREE;
							}
							else
							{
								infomenu[INFOZUR  ].ob_flags|=HIDETREE;
								infomenu[INFOVOR  ].ob_flags|=HIDETREE;
							}
							strncpy(alertstr,filename,INFONAME_LEN);
							alertstr[INFONAME_LEN]=0;
							form_write(infomenu,INFOMNAME,alertstr,0);
						}
						else
						{
							infomenu[INFOTNAME].ob_flags&=~HIDETREE;
							infomenu[INFOTNAME].ob_flags|=EDITABLE;
							infomenu[INFOMNAME].ob_flags|=HIDETREE;
							infomenu[INFOZUR  ].ob_flags|=HIDETREE;
							infomenu[INFOVOR  ].ob_flags|=HIDETREE;
							strcpy(filename,split_fname((char *)Wname(wp)));
							make_infoname(filename,0);
							form_write(infomenu,INFOTNAME,filename,0);
						}
#if defined( __TURBOC__ ) && !defined( __MINT__ )
						if(!findfirst((char *)Wname(wp),(struct ffblk *)&mydta,0))
						{
							sprintf(alertstr,"%02d.%02d.%02d",
								mydta.datim.day,mydta.datim.mon,mydta.datim.yr+80);
							form_write(infomenu,INFODATE,alertstr,0);
							sprintf(alertstr,"%02d:%02d:%02d",
								mydta.datim.hr, mydta.datim.min, mydta.datim.sec*2);
							form_write(infomenu,INFOTIME,alertstr,0);
							while(!findnext((struct ffblk *)&mydta)); /* weiter bis zum Ende */
						}
#else
						if(!stat(Wname(wp),&finfo))
						{
							lt = localtime( &finfo.st_mtime );
							sprintf(alertstr,"%02d.%02d.%02d",
							        lt->tm_mday, lt->tm_mon+1, lt->tm_year % 100);
							form_write(infomenu,INFODATE,alertstr,0);
							sprintf(alertstr,"%02d:%02d:%02d",
							        lt->tm_hour,lt->tm_min,lt->tm_sec);
							form_write(infomenu,INFOTIME,alertstr,0);
						}
#endif
						else
						{
							form_write(infomenu,INFODATE,"--.--.--",0);
							form_write(infomenu,INFOTIME,"--:--:--",0);
						}
/* Editiert */
						if(wp->w_state & CHANGED)
							*(char *)infomenu[INFOEDIT].ob_spec.index='*';
						else
							*(char *)infomenu[INFOEDIT].ob_spec.index=' ';
/* Zeichen */
						sprintf(alertstr,"%8ld",chars);
						form_write(infomenu,INFOSIZE,alertstr,0);
/* Zeilen */
						sprintf(alertstr,"%5ld",lines);
						form_write(infomenu,INFOLINE,alertstr,0);
/* Seiten */
						sprintf(alertstr,"%5ld",lines%zz ? lines/zz+1 : lines/zz);
						/* Wenn kein Divisionsrest, Seitenzahl ganzzahlig,
							andernfalls eine Seite mehr angeben */
						form_write(infomenu,INFOPAGE,alertstr,0);
/* l„ngste Zeile */
						for(line=wp->fstr; line; line=line->next)
							maxlen=max(line->used,maxlen);
						for(lines=1,line=wp->fstr; line; lines++,line=line->next)
							if(line->used==maxlen)
								break;
						sprintf(alertstr,"%5ld/%3d",lines,maxlen);
						form_write(infomenu,INFOLONG,alertstr,0);
/* in Arbeit */
						sprintf(alertstr,"%d",Wcount(CREATED));
						form_write(infomenu,INFOTOTL,alertstr,0);
/* Gesamtgr”áe aller Dateien */
						for(frei=0L, k=1; k<MAXWINDOWS; k++)
							if(_wind[k].w_state & CREATED)
							{
								Wtxtsize(Wp(_wind[k].wihandle),&lines,&chars);
								frei+=chars; /* nicht frei, aber richtiger Datentyp long */
							}
						sprintf(alertstr,"%8ld",frei);
						form_write(infomenu,INFOTBYT,alertstr,0);
/* Ram */
						tt_ram=max(0,(long)Mxalloc(-1L,0)) + 
								 max(0,(long)Mxalloc(-1L,1));
						sprintf(alertstr,"%8ld",tt_ram?tt_ram:(long)Malloc(-1L));
						form_write(infomenu,INFOFRAM,alertstr,0);
/* Dialog ausgeben */
						form_exopen(infomenu,0);
						do
						{
							exit_obj=(form_exdo(infomenu,0)&0x7FFF);
							switch(exit_obj)
							{
								case INFOZUR:
								   if(hfirst>0)
								      hfirst--;
									break;
								case INFOVOR:
								   if(hfirst<(int)(strlen(filename)-INFONAME_LEN)) /* 20 Zeichen lang */
								   	hfirst++;
									break;
								case INFOHELP:
									form_alert(1,Afbinfo[0]);
									objc_change(infomenu,exit_obj,0,infomenu->ob_x,infomenu->ob_y,infomenu->ob_width,infomenu->ob_height,infomenu[exit_obj].ob_state&~SELECTED,1);
									break;
								
							}
							if(get_cookie('MiNT'))
							{
								strncpy(alertstr,&filename[hfirst],INFONAME_LEN);
								alertstr[INFONAME_LEN]=0;
								form_write(infomenu,INFOMNAME,alertstr,1);
							}
						}
						while(!(exit_obj==INFOABBR || exit_obj==INFOOK));
						form_exclose(infomenu,exit_obj,0);
						
						if(!get_cookie('MiNT') && (exit_obj==INFOOK))
						{
							form_read(infomenu,INFOTNAME,filename);
                     make_infoname(filename,1);
/*
printf("\33H%s %s",(char *)split_fname((char *)Wname(wp)),filename);
*/
							if(*filename && 
							   *filename!=' ' && 
							   strcmp((char *)split_fname((char *)Wname(wp)),
							   	filename))
							{
								if((cp=make_newpathname((char *)Wname(wp),filename)) != NULL)
								{
									wp->w_state|=CHANGED;
									Wnewname(wp,cp);
									ren_icon(desktop, wp->icon);
								}
							}
						}
						form_write(infomenu,INFOTITL,DATEIINFO,0);
					}
					break;
				case DESKICN8: /* Papierkorb */
				case DESKICND: /* voller Papierkorb */
					sprintf(alertstr,Afbinfo[1],countdeletedfiles);
					form_alert(1,alertstr);
					break;
				case DESKICN9: /* Drucker	 */
					if(is_busy())
						form_alert(1,Afbinfo[2]);
					else
						form_alert(1,Afbinfo[3]);
					break;
				case DESKICNA: /* Diskette	*/
					if((wp=Wgettop()) != NULL)
					{
						graf_mouse(BUSY_BEE,0L);
						cp=(char *)Wname(wp);
						Dfree(&dtable,cp[0]-'A'+1);
						frei=(long)dtable.b_free*(long)dtable.b_secsiz*(long)dtable.b_clsiz;
						sprintf(alertstr,Afbinfo[4],
							(char *)split_fname((char *)Wname(wp)),frei);
						graf_mouse(ARROW,0L);
						form_alert(1,alertstr);
					}
					else
						form_alert(1,Afbinfo[5]);
					break;
				case DESKICNB: /* Klemmbrett */
					count=0;
					graf_mouse(BUSY_BEE,0L);
					scrp_read(filename);
#if defined( __TURBOC__ ) && !defined( __MINT__ )
					complete_path(filename);
/*
					if(filename[strlen(filename)-1]!='\\')
						strcat(filename,"\\");
*/
					strcat(filename,"SCRAP.*");
					if(!findfirst(filename,&fileRec,0))
					{
						count++;
						while(!findnext(&fileRec))
							count++;
					}
#else
					if ( ( dir = opendir( filename ) ) != NULL ) {
						while ( ( dent = readdir( dir ) ) != NULL )
							if ( !strnicmp( dent->d_name, "SCRAP.", 6 ) )
								count++;
						closedir( dir );
					}
#endif
					sprintf(alertstr,Afbinfo[6],count);
					graf_mouse(ARROW,0L);
					form_alert(1,alertstr);
					break;
				case DESKICNC:
					form_alert(1,Afbinfo[7]);
					break;
				default:
					break;
			}
			if(!do_blink)
				desel_icons(desktop,i,i,1);
		}
	}
}

void Wsetrcinfo(WINDOW *wp)
{
	char str[80];
	int caps=0;
   
	if(wp)
		Wnewname(wp,(char *)Wname(wp));

	if(wp && (wp->kind&INFO))
	{
		caps=(int)(Kbshift(-1)&16);
		sprintf(str," F:%d S:%-4ld Z:%-4ld S:%-3ld L:%-3d Tab:%d U:%-3d Blk:%-4ld %c%c%c %s %s %s %s",
			Windex(wp),
			(wp->row+wp->hfirst/wp->hscroll)/zz+1,
			wp->row+1+wp->hfirst/wp->hscroll,
			wp->col+1+wp->wfirst/wp->wscroll,
			wp->cstr->used,
			wp->tab,
			wp->umbruch,
			(wp/*==blkwp*/)?(((/*begline&&*/endline)?labs(endline-begline)+(wp->w_state&COLUMN?1L:0L):0L)):0L,
			wp->w_state&BLOCKSATZ?'B':' ',
			wp->w_state&COLUMN?'C':' ',
			wp->w_state&INDENT?'I':' ',
			caps?"CAPS":"",
			macro.rec ?"MREC ":"",
			macro.play?"MPLAY":"",
			shortcutfile);

		Wnewinfo(wp,str);
	}
}

static int (*cmp)(char *s, char *t)=NULL;

static int strlogcmp(register char *s, register char *t)
{
	register char *u=t;
	do
	{
		while(isspace(*s))
			s++;
		while(isspace(*t))
			t++;
		if(*s != *t)
			return((int)((long)t-(long)u));
		s++,t++;
	}
	while(*s);
	return(-1);
}

static int strnorcmp(register char *s, register char *t)
{
	register char *u=t;
	for(; *s == *t; s++, t++)
		if(!*s)
			return(-1);
	return((int)((long)t-(long)u));
}

static int strmcmp(char **string, int *col) /* String Multiple Compare */
{
	register int i,k/*,len*/,found=0;
	for(i=1; i<MAXWINDOWS; i++)
	{
		if(string[i])
			found++;
	}
	for(i=1; i<MAXWINDOWS; i++)
	{
		if(string[i])
			break;
	}
	if(found<2)
	{
		*col=0;
		return(-1);
	}
/*	len=(int)strlen(string[i]);*/
	for(k=i+1; k<MAXWINDOWS; k++)
		if(_wind[k].w_state & OPENED)
		{
			if(string[k])
			{
				if((*col=cmp(string[i],string[k]))>=0)
				{
/*
					for(*col=0; *col<len; (*col)++)
						if(string[i][*col] != string[k][*col])
							break;
*/
					return(k);
				}
			}
			else
			{
				*col=0;
				return(k);
			}
		}
	return(0);
}

long Wline(WINDOW *wp, LINESTRUCT *line)
{
	LINESTRUCT *lp;
	long lc;
	if(wp)
	{
		for(lc=0, lp=wp->fstr; lp; lc++, lp=lp->next)
			if(lp==line)
				return(lc);
		return(lc);
	}
	return(0);
}

void textcompare(void)
{
	register int i,diff;
	int col,ret,kstate;
	long row;
	char ascrow[33];
	char *string[MAXWINDOWS];
	LINESTRUCT *line[MAXWINDOWS];

	graf_mkstate(&ret, &ret, &ret, &kstate);
	if(kstate & (K_RSHIFT|K_LSHIFT))
		cmp=strlogcmp;		  /* logischer Vergleich ohne Whitespaces */
	else
		cmp=strnorcmp;			/* normaler Vergleich */
		
	graf_mouse(BUSY_BEE,NULL);
	for(i=1; i<MAXWINDOWS; i++)
		if(_wind[i].w_state & OPENED)
			line[i]=_wind[i].cstr;
		else
			line[i]=NULL;
	for(i=1; i<MAXWINDOWS; i++)
		if(_wind[i].w_state & OPENED)
			string[i]=
				&line[i]->string[_wind[i].col-_wind[i].wfirst/_wind[i].wscroll];
		else
			string[i]=NULL;
	while((diff=strmcmp(string,&col)) != 0)
	{
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & OPENED)
			{
				if(line[i])
					line[i]=line[i]->next;
			}
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & OPENED)
			{
				if(line[i])
					string[i]=line[i]->string;
				else
					string[i]=NULL;
			}
	}
	switch(diff)
	{
		case -1:
			form_alert(1,Afbinfo[8]);
			break;
		case  0:
			break;
		default:
			sprintf(alertstr,Afbinfo[9], 
				split_fname(Wname(Wp(_wind[diff].wihandle))), 
				row=Wline(&_wind[diff],line[diff])+1, col+1);
			form_alert(1,alertstr);
			ltoa(row,ascrow,10);
			for(i=1; i<MAXWINDOWS; i++)
				if(_wind[i].w_state & OPENED)
				{
					hndl_goto(&_wind[i],NULL,atol(ascrow));
					graf_mouse_on(0);
					Wcursor(&_wind[i]);
					_wind[i].col=_wind[i].cspos=(int)(col-_wind[i].wfirst/_wind[i].wscroll);
					_wind[i].cspos=Wshiftpage(&_wind[i],0,_wind[i].cstr->used); /* 1997-03-27 (MJK): used statt &used! */
					Wcursor(&_wind[i]);
					graf_mouse_on(1);
				}
			Wtop(&_wind[diff]);
			break;
	}
	graf_mouse(ARROW,NULL);
}
