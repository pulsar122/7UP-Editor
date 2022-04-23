/*****************************************************************
	7UP
	Modul: AVPROT.C
	(c) by TheoSoft '90
	
	AV-Protokoll

	1997-03-25 (MJK): void-Deklatarion einiger Funktionen erg„nzt,
	                  static-Deklaration einer nur lokal ben”tigten
	                  Funktion erg„nzt
	1997-03-26 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <gem.h>
#endif
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#	include <ext.h>
#else
#	include <osbind.h>
#endif

#include "alert.h"
#include "7up.h"
#include "windows.h"
#include "undo.h"
#include "vafunc.h"
#include "dragdrop.h"
#include "di_fly.h"
#include "language.h"
#include "undo.h"
#include "7up3.h"
#include "fileio.h"
#include "editor.h"
#include "block.h"
#include "findrep.h"
#include "graf_.h"

#include "AVprot.h"

#ifndef _AESversion
#	ifdef TCC_GEM
#		define _AESversion (_GemParBlk.global[0])
#	else
#		define _AESversion (aes_global[0])			/* 10.05.2000 GS */
#	endif
#endif

#ifndef _AESnumapps
#	ifdef TCC_GEM
#		define _AESnumapps (_GemParBlk.global[1])
#	else
#		define _AESnumapps (aes_global[1])			/* 10.05.2000 GS */
#	endif
#endif

/* AVSTUFF *****************************************************************/

#define AP_TERM         50 /* MultiTOS Shutdown */
#define AP_TFAIL        51 /* Kein Shutdown */
#define AP_RESCHG       57 /* Resolution change */
#define SPOOLER_ACK    101
#define AV_SENDKEY  0x4710

/***************************************************************************/

/* zerlegung des comdatastrings in commandvektor und commandcounter */
static int _get_args(int *argc, char *argv[], char *comdata)
{
	register int i;
	register char c;

	*argc=1;
	argv[0] = "";
	for(i=1; i<MAXWINDOWS; )
	{
		while (isspace(*comdata)) /* fhrende blanks raus */
			comdata++;
		if (*comdata == '\0')     /* kein argument */
			break;
		argv[i++] = comdata;      /* argvektor setzen */
		while ( (*comdata != '\0') && (isspace(*comdata)==0)  )
			comdata++;              /* pointer hochz„hlen, solange nicht
			                         * NULL oder blank */
		c = *comdata;             /* *comdata merken */
		*comdata++ = '\0';        /* den n„chsten NULL setzen */
		if (c == '\0')            /* und auf stringende testen */
			break;
	}
	*argc=i;
	return(*argc-1);            /* 0 wenn nur name */
}

void hndl_AVProt(int msgbuf[])
{
	char pathname[256],*clp;
	WINDOW *wp;
	int i,k,av_buf[8],overwrite=0;
	int gargc;
	char *gargv[MAXWINDOWS+1];
	/*static*/ char fpattern[FILENAME_MAX]="";
	
	extern int av_font;
	extern int av_point;
	extern int gl_apid,terminate_at_once;
	extern char alertstr[];
	extern OBJECT *winmenu;
	extern WINDOW _wind[];

	if (AVProcessMsg(msgbuf))
	{
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & OPENED)
				AVAccOpenedWindow(_wind[i].wihandle);
	}
	switch(msgbuf[0])
	{
		case AC_OPEN:
#if MiNT
			wind_update(BEG_UPDATE);
#endif
			menu_bar(winmenu,1);
#if MiNT
			wind_update(END_UPDATE);
#endif
			break;
		case AP_TERM: /* MultiTOS shutdown */
		case AP_RESCHG: /* Resolution change */
			if(Wcount(CHANGED))
			{
				av_buf[0]=AP_TFAIL;
				av_buf[1]=0;
				shel_write(10,0,0,(char *)av_buf,"");
				form_alert(1,Aavprot[2]);
			}
			else
			{
				terminate_at_once=1;
				exit(0);
			}
			break;
		case AC_CLOSE:
/*
/* Reaktion auf AC_CLOSE */
		   for(i=1; i<MAXWINDOWS; i++)
		      if(_wind[i].w_state & OPENED)
		      {
		      	_wind[i].w_state &= ~OPENED;
		      	_wind[i].wihandle = -2;     /* AC_CLOSE-Kennung */
		      }
*/
			break;
		case SPOOLER_ACK:
			if((wp=Wgettop()) != NULL)
			{
				if(wp->kind & INFO)
					wind_set_str(wp->wihandle,WF_INFO,CALCLOCKMSG);
				else
					form_alert(1,Aavprot[3]);
			}
			else
				form_alert(1,Aavprot[3]);
			break;
		case AV_PROTOKOLL:
			av_buf[0]=VA_PROTOSTATUS;
			av_buf[1]=gl_apid;
			av_buf[2]=0;
			av_buf[3]=1|2|16; /* AV_SENDKEY + AV_ASKFILEFONT + AV_OPENWIND */
			av_buf[4]=0;
			av_buf[5]=0;
			*(char **)(&av_buf[6])="7UP     ";
			appl_write(msgbuf[1],16,av_buf);
			break;
		case AV_ASKFILEFONT:
			av_buf[0]=VA_FILEFONT;
			av_buf[1]=gl_apid;
			av_buf[2]=0;
			av_buf[3]=av_font; /* AV_ASKFILEFONT */
			av_buf[4]=av_point;
			appl_write(msgbuf[1],16,av_buf);
			break;
		case AP_DRAGDROP: /* MultiTOS Drag&Drop Protokoll */
			pathname[0]=0;
			rec_ddmsg(msgbuf,pathname);
			goto DRAGDROPENTRY;  /* to be proofed */
/*			break;	1997-03-26 (MJK): berflssig */
		case	VA_DRAGACCWIND:
			strcpy(pathname,*((char **)(&msgbuf[6])));/* Pfadname */
DRAGDROPENTRY:
			if((wp=Wp(msgbuf[3]))!=NULL)
			{
				if((clp=strchr(pathname,' ')) != NULL)
					*clp=0;
				if(!cut && begcut && endcut)
				{  /* wenn was markiert, erst weg damit */
					overwrite=1;
					graf_mouse_on(0);
					Wcursor(wp);
					free_undoblk(wp,undo.blkbeg); /* Block weg */
					if((wp->w_state&COLUMN))
					{
						cut=cut_col(wp,begcut,endcut);
						undo.flag=copy_col(wp,begcut,endcut,&undo.blkbeg,&undo.blkend);
					}
					else
					{
						cut=cut_blk(wp,begcut,endcut);
						undo.flag=copy_blk(wp,begcut,endcut,&undo.blkbeg,&undo.blkend);
					}
					Wcuron(wp);
					Wcursor(wp);
					graf_mouse_on(1);
					free_blk(wp,begcut);
				}
				else
/* nach Klemmbretteinschaltung kann noch ein Ramblock existieren */
					free_blk(wp,begcut);		  /* ...weg damit */

				if(_read_blk(wp,pathname,&begcut,&endcut)>0)
				{
					store_undo(wp, &undo, begcut, endcut, WINEDIT, overwrite?CUTPAST:EDITCUT);
					i=endcut->used;
					graf_mouse_on(0);
					Wcursor(wp);
					if((wp->w_state&COLUMN))
						paste_col(wp,begcut,endcut);
					else
						paste_blk(wp,begcut,endcut);
					Wcursor(wp);
					graf_mouse_on(1);
					endcut->endcol=i;
					if(!(wp->w_state&COLUMN))
						hndl_blkfind(wp,begcut,endcut,SEAREND);
					else
						free_blk(wp,begcut); /* Spaltenblock freigeben */
				}
				begline=endline=0L;
				begcut=endcut=NULL;
			}
			else /* Fensterhandle -1, laut MTOS-Doku neues Fenster */
			{
				if(*pathname)
				{
					if(_get_args(&gargc, gargv, pathname))
						for(i=1,k=1; i<gargc && k<MAXWINDOWS-Wcount(CREATED); i++)
							Wreadtempfile(strupr(gargv[i]),0);
				}
			}
			break;
		case VA_START:
			if((msgbuf[3] || msgbuf[4]) && (clp=malloc(strlen(*((char **)(&msgbuf[3])))+1))!=NULL)
			{
				strcpy(clp,*((char **)(&msgbuf[3])));			  /* Pfadname */
				if(_get_args(&gargc, gargv, clp))
					for(i=1,k=1; i<gargc && k<MAXWINDOWS-Wcount(CREATED); i++)
						Wreadtempfile(strupr(gargv[i]),0);
			  free(clp);
			}
#if MiNT
			wind_update(BEG_UPDATE);
#endif
			menu_bar(winmenu,1);
#if MiNT
			wind_update(END_UPDATE);
#endif
			break;
		case AV_OPENWIND:
			if(Wcount(CREATED)<(MAXWINDOWS-1))
			{
				strcpy(pathname,*((char **)(&msgbuf[3])));			  /* Pfadname */
				if(!strchr(*((char **)(&msgbuf[5])),'*') &&	 /* Wildcard drin? */
					!strchr(*((char **)(&msgbuf[5])),'?') &&
					*(*((char **)(&msgbuf[5]))))	  /* steht berhaupt was drin? */
				{
					strcat(pathname,*((char **)(&msgbuf[5])));
					Wreadtempfile(pathname,0);
				}
				else /* ja, kein vollst„ndiger Pfadname */
				{
					strcpy(fpattern,*((char **)(&msgbuf[5])));
					loadfiles(pathname,fpattern);
				}
			}
			av_buf[0]=VA_WINDOPEN;
			av_buf[1]=gl_apid;
			av_buf[2]=0;
			av_buf[3]=0;
			av_buf[4]=0;
			appl_write(msgbuf[1],16,av_buf);
#if MiNT
			wind_update(BEG_UPDATE);
#endif
			menu_bar(winmenu,1);
#if MiNT
			wind_update(END_UPDATE);
#endif
			break;
		case AV_SENDKEY:
			hndl_keybd(msgbuf[3],msgbuf[4]);
			break;
	}
}

#pragma warn -par
int relay( int myid, int apid, int argc, char *argv[])
{
	char filename[FILENAME_MAX], *cp;
	register int i;
	static int av_buf[8]; /* muá static sein, sonst weg! */

	for(i=1; i<argc; i++)
	{
		if((cp=strrchr(argv[i],'\\'))!=NULL || (cp=strrchr(argv[i],'/'))!=NULL)
		{										 /* Pfad und Namen trennen */
			strcpy(filename,++cp);
			*cp=0;
			av_buf[0]=AV_OPENWIND;
			av_buf[1]=gl_apid;
			av_buf[2]=0;
			*((char **)&av_buf[3])=argv[i];
			*((char **)&av_buf[5])=filename;
			if(appl_write(apid,16,av_buf)>0)
			{
				do /* auf Anwort warten, wenn appl_write = OK */
				{
					evnt_mesag(av_buf);
				}
				while(av_buf[0]!=VA_WINDOPEN && av_buf[1]!=apid);
			}
			else
			{
				form_alert(1,Aavprot[1]);
				return(0); /* Fehler, hat nicht geklappt, aktiv bleiben */
			}
		}
	}
	if(argc==1) /* Menleiste durch VA_START toppen lassen */
	{
		av_buf[0]=VA_START;
		av_buf[1]=gl_apid;
		av_buf[2]=0;
		av_buf[3]=0;
		av_buf[4]=0;
		av_buf[5]=0;
		av_buf[6]=0;
		av_buf[7]=0;
		appl_write(apid,16,av_buf);
	}
	return(1);
}
#pragma warn .par

#ifdef TCC_GEM
int appl_getinfo(int ap_gtype, int *ap_gout1, int *ap_gout2, int *ap_gout3, int *ap_gout4)
{
	AESPB aespb=
	{
		_GemParBlk.contrl,
		_GemParBlk.global,
		_GemParBlk.intin,
		_GemParBlk.intout,
		(int *)_GemParBlk.addrin,
		(int *)_GemParBlk.addrout
	};

	_GemParBlk.contrl[0] = 130;
	_GemParBlk.contrl[1] = 1;
	_GemParBlk.contrl[2] = 5;
	_GemParBlk.contrl[3] = 0;
	_GemParBlk.contrl[4] = 0;
	
	_GemParBlk.intin[0]  = ap_gtype;
	
	_crystal(&aespb);
	
	*ap_gout1 = _GemParBlk.intout[1];
	*ap_gout2 = _GemParBlk.intout[2];
	*ap_gout3 = _GemParBlk.intout[3];
	*ap_gout4 = _GemParBlk.intout[4];
	return(_GemParBlk.intout[0]);
}

int appl_search( int ap_smode, char *ap_sname, int *ap_stype, int *ap_sid )
{
	AESPB aespb=
	{
		_GemParBlk.contrl,
		_GemParBlk.global,
		_GemParBlk.intin,
		_GemParBlk.intout,
		(int *)_GemParBlk.addrin,
		(int *)_GemParBlk.addrout
	};

	_GemParBlk.intin [0] = ap_smode;
	_GemParBlk.addrin[0] = ap_sname;

	_GemParBlk.contrl[0]=18;	
	_GemParBlk.contrl[1]=1;	
	_GemParBlk.contrl[2]=3;	
	_GemParBlk.contrl[3]=1;	

	_crystal(&aespb);

	*ap_stype = _GemParBlk.intout[1];
	*ap_sid	= _GemParBlk.intout[2];

	return(_GemParBlk.intout[0]);
}
#endif /* TCC_GEM */

#define MAXPAPPS 15
#define APP_FIRST 0
#define APP_NEXT  1

int My_appl_find(int my_id, char *ap_sname)
{
	int i, ap_stype, ap_sid;
	char name[32];

	if(_AESnumapps != 1)
	{
		if(get_cookie('MagX'))
		{
			if((ap_sid=appl_find(ap_sname))>=0 && ap_sid!=my_id)
				return(ap_sid);
			for(i=(my_id+1); i<_AESnumapps; i++)
			{
				sprintf(name,"? %c",(char)i);
				name[1]=0;
				if((ap_sid=appl_find(name))==1)
					if(!strcmp(ap_sname,name))
						return(i);
			}
		}
		if(_AESversion>=0x0400)
		{
			if(appl_search( APP_FIRST, name, &ap_stype, &ap_sid )==1)
			{
				if((ap_sid != my_id) && !strncmp(ap_sname,name,8))
					return(ap_sid);
				while(appl_search( APP_NEXT, name, &ap_stype, &ap_sid )==1)
				{
					if((ap_sid != my_id) && !strncmp(ap_sname,name,8))
						return(ap_sid);
				}
			}
		}
		else
		{
			return(appl_find(ap_sname));
		}
	}
	return(my_id);
}

/* 1997-04-22 (MJK): In der neuen VAFUNC bereits enthalten
void AVExit(int myapid)
{
	int apid;
	static int av_buf[8];

	if((apid=appl_find(AVName))>=0)
	{
		av_buf[0]=AV_EXIT;
		av_buf[1]=myapid;
		av_buf[2]=0;
		av_buf[3]=myapid;
		av_buf[4]=0;
		av_buf[5]=0;
		av_buf[6]=0;
		av_buf[7]=0;
		appl_write(apid,16,av_buf);
	}
/*
printf("%d",c_pas(2));	
*/
}
*/