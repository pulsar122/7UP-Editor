/*****************************************************************
	7UP
	GEM-Texteditor fÅr Atari ST und PC
	Modul: 7UP.C
	(c) by TheoSoft '90
	Entwicklungsbegin: 01.07.90
	Version 0.90b    : 26.09.90
	Version 0.91b    : 29.09.90
	Version 0.92b    : 08.10.90
	Version 0.93b    : 16.10.90
	Version 0.94b    : 31.10.90
	Version 0.95b    : 01.11.90
	Version 1.0      : 21.12.90
	Version 1.01     : 25.12.90
	Version 1.02     : 11.01.91
	Version 2.00     : 28.06.91
	Version 2.1 sw   : 31.10.92
	Version 2.1      : 19.11.92
	Version 2.11b    : 01.12.92
	Version 2.15b    : 01.08.93 
	Version 2.16b    : ? 
	Version 2.17b    : ?	
	Version 2.18b    : 01.11.93
	Version 2.19b    : 07.12.93
	Version 2.2      : 16.01.94 MAXON PD
	Version 2.2      : 22.02.94 Golden Code
	                   24.02.94 Bug "weiche CRs"
	Version 2.3      : 23.02.95 Golden Code
	Version 2.31     : 01.04.95 
	Version 2.32     : 08.05.95 
	Version 2.33     : 31.12.95 filegrep Zieldatei nicht nochmal auswerten
	Freigabe der Sourcen Februar 1997
	
	1997-03-25 (MJK): TAB2,
	                  Prototypes aus Headefiles laden,
	                  mit aktivierten Warnings compilierbar,
	                  Variable registriert deaktiviert
	1997-04-09 (MJK): MSDOS-Teile entfernt
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
	PL2:
	1997-06-19 (MJK): Wiederherstellen der Selektion beim Redraw
	                  funktioniert nun wieder.
	PL3:
	1998-03-06 (MJK): Makrorekorder wieder angeschlossen
*****************************************************************/

#define PL7UP "03"	/* Patchlevel von 7Up */

#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <tos.h>
#	include <ext.h>
#else
#	include <mintbind.h>
#	include <signal.h>
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

#include "vaproto.h"
#include "fsel_inp.h"
#include "alert.h"
#include "windows.h"
#include "forms.h"
#include "7UP.h"
#include "undo.h"
#include "version.h"

#include "language.h"

#include "7UP3.h"
#include "desktop.h"
#include "printer.h"
#include "findrep.h"
#include "tabulat.h"
#include "editor.h"
#include "fbinfo.h"
#include "resource.h"
#include "toolbar.h"
#include "avprot.h"
#include "picklist.h"
#include "config.h"
#include "shell.h"
#include "userdef.h"
#include "fileio.h"
#include "grepfile.h"
#include "block.h"
#include "format.h"
#include "macro.h"
#include "deadkey.h"
#include "helpmsg.h"
#include "fontsel.h"
#include "fkeys.h"
#include "preview.h"
#include "sort.h"
#include "textmark.h"
#include "numerik.h"
#include "menuikey.h"
#include "markblk.h"
#include "wrealscr.h"
#include "wirth.h"
#include "VAfunc.h"
#include "objc_.h"
#include "wind_.h"
#include "graf_.h"

#ifndef SQUARED	/* error at gemfast? */
#	define SQUARED SQUARE
#endif

#ifndef _AESversion
#	ifdef TCC_GEM
#		define _AESversion (_GemParBlk.global[0])
#	else
#		error First define _AESversion to global[0]
#	endif
#endif

#ifndef _AESnumapps
#	ifdef TCC_GEM
#		define _AESnumapps (_GemParBlk.global[1])
#	else
#		error First define _AESnumapps to global[1]
#	endif
#endif

/* lÑuft nicht bei Blockoperationen gecancelt 10.7.94
WINDOW *Wgetwind(register int wum); */

#define TEXT	     0  /* windowtype */
#define GRAPHIC    1
#define PRT		     0
#ifndef DESK	/* In der MiNTLIB z. B. bereits definiert */
#	define DESK	     0
#endif
#define VERTICAL	 1
#define HORIZONTAL 2
#define notnull(a) ((a>0)?(a):(1))

#define BELL 7

#define EXOB_TYPE(x) (x>>8)

#define SPOOLER_ACK 101

int gl_apid;
int aeshandle,vdihandle,userhandle,exitcode=0;
int xdesk,ydesk,wdesk,hdesk;
int msgbuf[8];
int charw,charh,boxw,boxh,norm_point,small_point;
int work_in[103],work_out[57];
int topwin=FALSE;
int terminate_at_once=FALSE;

unsigned long backuptime=0XFFFFFFFFL; /* timer fÅr autom. Backup */
unsigned long codeid=0x12345678L;


/* AVSTUFF *****************************************************************/

#define AP_TERM		50	  /* MultiTOS Shutdown */
#define AV_PROTOKOLL 0x4700
#define AV_SENDKEY	0x4710
int av_font=1;
int av_point=10;
int umlautwandlung=FALSE,toolbar_zeigen=FALSE,scrollreal;

/***************************************************************************/
/* in diesen Speicherbereich sollen die Konfigurationen geschrieben werden */
int clipbrd=TRUE;
int nodesktop,vastart;

/* hier sind die Iconpositionen abgespeichert */
ICNCOORDS iconcoords[]=
{
  0,0, /* 1 */
  0,0, /* 2 */
  0,0, /* 3 */
  0,0, /* 4 */
  0,0, /* 5 */
  0,0, /* 6 */
  0,0, /* 7 */
  0,0, /* Papierkorb */
  0,0, /* Drucker	 */
  0,0, /* Diskette	*/
  0,0  /* Klemmbrett */
};
unsigned int iconcoordcnt = (int)(sizeof(iconcoords)/sizeof(ICNCOORDS));

/***************************************************************************/

#define SINGLECLICK 1
#define DOUBLECLICK 2
#define TRIPLECLICK 3
#define LEFTBUTTON  1
#define RIGHTBUTTON 2
#define LEFTPRESS	1
#define RIGHTPRESS  2

#define SCREEN			1
#define PLOTTER		 11
#define PRINTER		 21
#define METAFILE		31
#define CAMERA		  41
#define TABLET		  51

#define E_TIME 500L

static MEVENT mevent=
{
	MU_MESAG|MU_TIMER|MU_KEYBD|MU_BUTTON|MU_M1,
/*
	0x0102,3,0,
*/
	TRIPLECLICK,1,1,
	1,0,0,1,1,
	0,0,0,0,0,
	msgbuf,
	E_TIME,
	0,0,0,0,0,0,
/* nur der VollstÑndigkeit halber die Variablen von XGEM */
	0,0,0,0,0,
	0,
	0L,
	0L,0L
};

static unsigned long timer=0L; /* fÅr Autosave */

char alertstr[256];
/*
extern long *ckbd; /* Compose Keyboard von Pascal Fellerich */
*/
long *winx;

WINDOW *blkwp=NULL;
LINESTRUCT *begcut=NULL, *endcut=NULL, *begcopy=NULL, *endcopy=NULL;
int			cut=FALSE;
/* Es gibt keine Registrierung mehr (MJK 3/97)
int         registriert; */

int open_work (int device)
{
	register int i;
	int handle;

	for (i = 0; i < 103; i++) work_in [i] = 1;
	work_in [0]  = device;								 /* device handle */
	work_in [10] = 2;								 /* Raster Koordinaten */

	if (device == SCREEN)
	{
	  handle=aeshandle;
	  v_opnvwk (work_in, &handle, work_out);	  /* virtuell îffnen */
	}
	else													 /* nicht Bildschirm */
	{
	  v_opnwk (work_in, &handle, work_out);  /* physikalisch îffnen */
	}
	return (handle);
}

void close_work (int handle, int device)
{
  switch (device)
  {
	 case SCREEN:
		 v_clsvwk (handle);
		 break;
	 default:
		 v_clswk (handle);
		 break;
  }
}

#pragma warn -par
void Wmarkline(WINDOW *wp, LINESTRUCT *line, int fchar, int x, int y)
{
	register int width, pxyarray[4];

	if((width=(line->endcol-line->begcol)) == 0) /* 1997-06-19 (MJK): == statt != */
		return;

	pxyarray[0]=(int)(x - wp->wfirst + line->begcol*wp->wscroll);
	pxyarray[1]=y;
	pxyarray[2]=pxyarray[0] + width*wp->wscroll -1;
	pxyarray[3]=pxyarray[1] + wp->hscroll -1;
	vsf_color(wp->vdihandle,BLACK);
	vswr_mode(wp->vdihandle,MD_XOR);
	vr_recfl (wp->vdihandle,pxyarray);		/* markieren */
	vsf_color(wp->vdihandle,WHITE);
	vswr_mode(wp->vdihandle,MD_REPLACE);
}
#pragma warn .par

int Wtxtdraw(register WINDOW *wp, int dir, int clip[])
{
/*	static int oldeffect = 0;*/
	
	register int fline,lline,i,fchar;
	int x,y/*,dummy*/;
	register LINESTRUCT *line;
		
	x = wp->work.g_x;			  /* x-Koordinate fÅr alle Zeilen */
	y = wp->work.g_y;
	fchar=(int)(wp->wfirst/wp->wscroll);
	fline = (int)(wp->hfirst/wp->hscroll);
	lline = (int)(fline + wp->work.g_h/wp->hscroll);

	if(lline > wp->hsize/wp->hscroll)
		lline=(int)(wp->hsize/wp->hscroll);

	line=wp->wstr;
/*
	vs_clip(wp->vdihandle,TRUE,clip);
*/
	vr_recfl(wp->vdihandle,clip);				/* weiûes rechteck in workspace */

	twp=wp; /* topwindow setzen, damit userdef Tabbar zeichnen kann */
	
   if(wp->toolbar && (dir==(HORIZONTAL+VERTICAL)))
   {
		if(EXOB_TYPE(wp->toolbar->ob_type) == 1) /* senkrecht */
		{
			wp->toolbar->ob_x     =wp->work.g_x-wp->toolbar->ob_width-wp->wscroll/2;
			wp->toolbar->ob_y     =wp->work.g_y;
			wp->toolbar->ob_height=wp->work.g_h;
	      objc_draw(wp->toolbar, ROOT, MAX_DEPTH,
	      	clip[0], clip[1],	clip[2]-clip[0]+1, clip[3]-clip[1]+1);
		}
		else
		{
			wp->toolbar->ob_x    =wp->work.g_x-wp->wscroll/2;
			wp->toolbar->ob_y    =wp->work.g_y-wp->toolbar->ob_height;
			wp->toolbar->ob_width=wp->work.g_w+wp->wscroll/2;
			wp->toolbar->ob_height--;
	      objc_draw(wp->toolbar, ROOT, MAX_DEPTH,
	      	clip[0], clip[1],	clip[2]-clip[0]+1, clip[3]-clip[1]+1);
			wp->toolbar->ob_height++;
		}
   }

	for(i=fline; i<lline && line ; i++, line=line->next, y+=wp->hscroll)
	{
		if((y + wp->hscroll)>clip[1] && y<clip[3])
		{
			if(fchar < line->used)
			{
/*			
				if((line->attr & TABCOMP) && tabexp)
				{
					stpexpan(iostring, &line->string[fchar], wp->tab, STRING_LENGTH,&dummy);
					outtext(wp->vdihandle,x,y,iostring,
						strlen(iostring),wp->work.g_w,wp->wscroll);
				}
				else
*/
/*	
					if(line->effect != oldeffect) /* normal, fett, kursiv */
					{
						vst_effects(wp->vdihandle, line->effect);
						oldeffect = line->effect;
					}
*/
					v_gtext(wp->vdihandle,x,y,&line->string[fchar]);  /* ...normale ausgabe */	
			}
			if(line->attr & SELECTED)
				Wmarkline(wp,line,fchar,x,y);
		}
	}
	
	if(scrollreal)
	{
		if(dir&HORIZONTAL)
		{
			Wslupdate(wp,1);
			Wtabbarupdate(wp);
		}
		if(dir&VERTICAL)
			Wslupdate(wp,2);
	}

	return(TRUE);
}

void Wdefattr(WINDOW *wp)
{
	int ret;
	if(wp)
	{
		vst_alignment(wp->vdihandle,0,5,&ret,&ret); /* Ausrichtung */
		vst_rotation(wp->vdihandle,0);
		vswr_mode(wp->vdihandle,MD_REPLACE);			 /* replace */
		vsf_interior(wp->vdihandle,FIS_SOLID);			 /* FÅllung */
		vsf_color(wp->vdihandle,WHITE);				  /* farbe weiû */
		vst_color(wp->vdihandle,BLACK);
		vst_point(wp->vdihandle,wp->fontsize,&ret,&ret,&ret,&ret);
		vqt_width(wp->vdihandle,'W',&wp->wscroll,&ret,&ret);  /* Breite der Zeichen */
	}
}

void Wcbattr(int handle)					 /* Cursor- und Blockschreibattibute */
{
	int ret;
	vswr_mode	(handle,MD_XOR);			/* Cursor */
	vsf_interior(handle,FIS_SOLID);
	vsf_color	(handle,BLACK);

	vsl_type	 (handle, USERLINE);		/* Iconrahmen */
	vsl_width	(handle, 1);
	vsl_color	(handle, BLACK);
	vsl_ends	 (handle, SQUARED, SQUARED);

	vst_alignment(handle,0,5,&ret,&ret); /* Ausrichtung */
	vst_point(handle,10,&ret,&ret,&ret,&ret);
}

#pragma warn -par
static int getsysfontheight(int handle, 
                            int id, 
                            int *npoint,
                            int *spoint,
                            int boxw, int boxh)
{
	int pt=0,ch,cw,ch2,cw2,ret;
   if(_AESversion>=0x0399)
   {
      appl_getinfo(0,&pt,&ret,&ret,&ret);
		vst_height(handle,pt,&ret,&ret,&cw2,&ch2);
		pt=0;
		do
		{
			vst_point(handle,++pt,&ret,&ret,&cw,&ch);
			if (pt > 99)
			{
	        Cconws("\33HUngeeigneter Systemfont! Programmende.  ");
			  exit(-1);
			}
#ifdef DEBUG
printf("pt=%2d - ch=%2d ch2=%2d cw=%2d cw2=%2d\n", pt, ch, ch2, cw, cw2);
#endif
		}
		while(!(ch==ch2 && cw==cw2));
      *npoint=pt;
      
      appl_getinfo(1,&pt,&ret,&ret,&ret);
      *spoint=pt;
   }
   else
   {
		do
		{
			vst_point(handle,++pt,&ret,&ret,&cw,&ch);
			if (pt > 99)
			{
	        Cconws("\33HUngeeigneter Systemfont! Programmende.  ");
			  exit(-1);
			}
#ifdef DEBUG
printf("pt=%2d - ch=%2d boxh=%2d cw=%2d boxw=%2d\n", pt, ch, boxh, cw, boxw);
#endif
		}
		while(!(ch==boxh && cw==boxw));
		*npoint = pt;
		*spoint = 8;
	}
	return(pt);
}
#pragma warn .par

#define _CONTERM 1156L

long _keyoff(void)
{
	return *(unsigned char *)_CONTERM &= ~2; /* Autorepeat aus */
}

long _keyon(void)
{
	return *(unsigned char *)_CONTERM |=  2; /* Autorepeat ein */
}

void pexit(void)
{
	int attr[10];
   
	graf_mouse(BUSY_BEE,0L);
	append_picklist(picklist, NULL,0L); /* Pickliste erweitern */
/*
	writenames(); /* Dateinamen sichern */
*/
	sicons();	  /* Iconpositionen sichern */
	Wnew();								 /* alles dicht machen */
	if((mevent.e_ks & (K_RSHIFT|K_LSHIFT)) ||
		(divmenu[DIVSAVE].ob_state & SELECTED))
		saveconfig(TRUE);
	if(!nodesktop)
		wind_set(0,WF_NEWDESK,0,0,0,0);
#if MiNT
	wind_update(BEG_UPDATE);
#endif
	menu_bar(winmenu,FALSE);
#if MiNT
	wind_update(END_UPDATE);
#endif
	vqt_attributes(aeshandle,attr);
	if (vq_gdos() /*&& (attr[0]!=1)*/)
	  vst_unload_fonts(userhandle,0);
	AVExit(); /* abmelden; 1997-04-23 (MJK): gl_apid wird nicht mehr benîtigt */
	Supexec(_keyon);
	close_work(userhandle,SCREEN); /**/
	close_work(vdihandle,SCREEN);
	graf_mouse(ARROW,0L);
	appl_exit();
}

void pinit(char *inffile)
{
	int i,ret,wh,attr[10];

	gl_apid=appl_init();
	if(gl_apid < 0)
		exit(-1);
	graf_mouse(BUSY_BEE,0L);
	if(pexec)
	{
		if(_AESversion>=0x0104) /* evtl. Deskaccessories schlieûen */
		{
			wind_update(BEG_UPDATE);
			wind_new();
		}
		else
		{
			do
			{
				_wind_get(0,WF_TOP,&wh,&i,&i,&i);
				if(wh>0)
				{
					wind_close(wh);
					wind_delete(wh);
				}
			}
			while(wh>0);
		}
	}
	wind_update(BEG_UPDATE);
	_wind_get(DESK,WF_WORKXYWH,&xdesk,&ydesk,&wdesk,&hdesk);
	aeshandle=graf_handle(&boxw,&boxh,&charw,&charh);
	vdihandle=open_work(SCREEN);
	Wcbattr(vdihandle);
	userhandle=open_work(SCREEN);

	vqt_attributes(aeshandle,attr);
	if (vq_gdos() /*&& (attr[0]!=1)*/)
	  vst_load_fonts(userhandle,0);
	vst_font(userhandle,attr[0]); /* Systemfont einstellen */

#ifdef DEBUG
printf("grafhandle boxh=%2d boxw=%2d\n", boxh, boxw);
#endif

#ifdef DEBUG
printf("Systemfont Id=%2d\n", attr[0]);
#endif

	getsysfontheight(userhandle, attr[0], &norm_point, &small_point, boxw, boxh);

#ifdef DEBUG
printf("Systemfont normal=%2dpt, small=%2dpt\n", norm_point, small_point);
#endif

	for(i=1;i<MAXWINDOWS;i++)
	{
/*		_wind[i].fontid  =attr[0];
*/
		_wind[i].fontsize=norm_point;/*font einstellen*/
	}

	/* Font fÅr AV_PROTOKOLL */
	av_font=attr[0];
	av_point=norm_point;

	actbutcolor=WHITE;
	dialbgcolor=WHITE;
	if(_AESversion>=0x0340 /*&& mindestens_16_Farben()*/)
	{  /* erst prÅfen, ob implementiert */
		if(objc_sysvar(0,4/*ACTBUTCOL*/,0,0,&actbutcolor,&ret)>0)
			objc_sysvar(0,5/*BACKGRCOL*/,0,0,&dialbgcolor,&ret);
		else
			actbutcolor=WHITE;
	}

	if(_AESversion >= 0x0400)
		shel_write(9,1,0,"","");		  /* AP_TERM anmelden */

	if(!rsrc_init("7UP.RSC",inffile))
	{
		 form_alert(1,A7up[0]);
		 close_work(userhandle,SCREEN); /**/
		 close_work(vdihandle,SCREEN);
		 wind_update(END_UPDATE);
		 appl_exit();
		 exit(-1);
	}

	graf_mouse(ARROW,0L);
	wind_update(END_UPDATE);
}

void Wwindmsg(WINDOW *wp, int *msgbuf)
{
	int ret,mouse_click;
	void hndl_button();

	graf_mouse_on(FALSE);
	Wcursor(wp);
	switch(msgbuf[0])
	{
		case WM_ARROWED:
/*
			_Warrow(wp,msgbuf); /* incl. WinX-UnterstÅtzung */
*/
			if(winx)
			{
				graf_mouse_on(FALSE);
				Warrow(wp,msgbuf[4]);
				graf_mouse_on(TRUE);
			}
			else
			{
				graf_mouse_on(TRUE);
				wind_update(BEG_MCTRL);
				do
				{
					graf_mouse_on(FALSE);
					Warrow(wp,msgbuf[4]);
					graf_mouse_on(TRUE);
					graf_mkstate(&ret,&ret,&mouse_click,&ret);
				}
				while(mouse_click & 1); /* linke Taste */
				wind_update(END_MCTRL);
				graf_mouse_on(FALSE);
			}
			undo.item=FALSE;
			break;
		case WM_REDRAW:
			Wredraw(wp,array2grect(&msgbuf[4]));
			break;
/* Fehler: hat nichts mit WM_TOPPED zu tun und fÅhrte zu einer
	énderung in MultiTOS seitens Atari. Absolut lachhaft, diese
	Diletanten. Die Ñndern TOS wg. 7UP!
		case WM_NEWTOP:
*/
		case WM_TOPPED:
			Wtop(wp);
			checkmenu(winmenu,Wgettop());
			undo.item=FALSE;
			break;
		case 33/*WM_BOTTOMED*/:
			Wbottom(wp); /* WiNX 14.4.94 */
			break;
		case WM_ONTOP:		  /* 7UP wurde nach oben gebracht */
			inst_trashcan_icon(desktop,DESKICN8,DESKICND,FALSE);
			inst_clipboard_icon(desktop,DESKICNB,DESKICNC,FALSE);
			break;
		case WM_MOVED:
		case WM_SIZED:
			Wmovesize(wp,array2grect(&msgbuf[4]));
			undo.item=FALSE;
			break;
		case WM_VSLID:
			Wslide(wp,msgbuf[4],VSLIDE);
			undo.item=FALSE;
			break;
		case WM_HSLID:
			Wslide(wp,msgbuf[4],HSLIDE);
			undo.item=FALSE;
			break;
		case WM_FULLED:
			if(mevent.e_ks & (K_LSHIFT|K_RSHIFT))
				Wadjust(wp);
			else
				Wfull(wp);
			undo.item=FALSE;
			break;
		/* 11.9.1993 */
		case WM_ICONIFY:
			/*Wiconify(wp,&msgbuf[4]);*/
			undo.item=FALSE;
			break;
		case WM_UNICONIFY:
			/*Wuniconify(wp,&msgbuf[4]);*/
			undo.item=FALSE;
			break;
		case WM_ALLICONIFY:
			undo.item=FALSE;
			break;
		case WM_CLOSED:
/* Iconifytest
			if(mevent.e_ks & (K_LSHIFT|K_RSHIFT))
			{
				wind_calc(WC_BORDER,wp->kind,
						wp->work.g_x+16,wp->work.g_y+16,72,72,
						&msgbuf[4],&msgbuf[5],&msgbuf[6],&msgbuf[7]);
			   if(wp->w_state & ICONIFIED)
   				Wuniconify(wp,&msgbuf[4]);
			   else
   				Wiconify(wp,&msgbuf[4]);
   		}
			else
*/
				Wclose(wp);
			checkmenu(winmenu,Wgettop());
			undo.item=FALSE;
			break;
	}
	Wcursor(wp);
	graf_mouse_on(TRUE);
	return;
}

int delete(WINDOW *wp, OBJECT *tree, int desk_obj)
{
	char filename[PATH_MAX];
	if(wp && wp->w_state & CHANGED)
	{
		sprintf(alertstr,A7up[1],split_fname((char *)Wname(wp)));
		switch(form_alert(3,alertstr))
		{
			case 1:
				wp->w_state&=~CHANGED;
				break;
			case 2:
				return(FALSE);
			case 3:
				write_file(wp, FALSE);
				strcpy(filename,(char *)Wname(wp));
				unlink(change_ext(filename,".$$$")); /* <NAME>.$$$ lîschen */
				break;
		}
	}
	append_picklist(picklist, (char *)Wname(wp),wp->row + /* Pickliste erweitern */
												 wp->hfirst/wp->hscroll + 1); 
	del_icon(tree, desk_obj);
	Wfree(wp);
	Wdelete(wp);
	menu_ienable(winmenu,WINNEW,TRUE);	/* Menueintrag wieder freigeben */
	menu_ienable(winmenu,WINOPEN,TRUE);
	return(TRUE);
}

void hndl_mesag(int mbuf[])
{
	if( mbuf[0] != MN_SELECTED ) /* WINDOW MESSAGES */
	{
		if(mbuf[0]>=AC_OPEN)
			hndl_AVProt(mbuf);
		else
			Wwindmsg(Wp(mbuf[3]),mbuf);
	}
	else
		hndl_menu(mbuf[3],mbuf[4]);
}

void hndl_menu(int menu, int item)
{
	WINDOW *wp;
	int newname=FALSE;
	int i,k,desk_obj,overwrite=FALSE,abbruch;
	char *cp,filename[PATH_MAX],tempname[PATH_MAX];
	static char fpattern[FILENAME_MAX]="*.*";

#define MAXPATHS 7

	wp=Wgettop();
	switch(item)
		{
			case WINABOUT:
				strcpy(alertstr,__DATE__);
				alertstr[3]=alertstr[6]=0;
				sprintf((char *)copyinfo[CDATE].ob_spec.index,
					/* Versionsstring angepaût (MJK 3/97) (MJK 4/97)*/
#ifdef ENGLISH
					"Version 2.33PL"PL7UP" %s %s. %s",&alertstr[0],&alertstr[4],&alertstr[7]);
#else
					"Version 2.33PL"PL7UP" %s. %s %s",&alertstr[4],&alertstr[0],&alertstr[7]);
#endif
/*
				if(!registriert)
					shareware[SHAREREGIST].ob_flags |= HIDETREE;
*/
				if(!windials)
				{
					/* Registrierbutton und Registrierdialog entfernt
					 * (MJK 3/97) */
					if(form_exhndl(copyinfo,0,0) == CINFO)
						form_exhndl(shareware,0,0);
				}
				else
					if(form_exhndl(copyinfo,0,0)==CINFO)
					{
						if(windials)
						{
							graf_mouse_on(FALSE);
							Wcursor(wp);		 /* ausschalten */
							evnt_event(&mevent); /* Dummyaufruf um Redraw zu killen */
							Wredraw(wp,array2grect(&msgbuf[4]));
							Wcursor(wp);
							graf_mouse_on(TRUE);
							msgbuf[3]=3; /* MenÅtitel wird Åberschrieben */
						}
						/* Registrierbutton und Registrierdialog entfernt
						 * (MJK 3/97) */
						form_exhndl(shareware,0,0);
					}
				break;
			case WINNEW:
				Wnewfile(NAMENLOS);
				break;
			case WINOPEN:
				if(is_selected(desktop,DESKICN1,DESKICND))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICND; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							click_icon(desktop,desk_obj);
						}
					}
				}
				else
				{
					loadfiles("","");
				}
				break;
			case WINPICK:
				hndl_picklist(picklist);
				break;
			case WINREAD:
				if(wp && !begcut && !endcut)
				{
/* mit SELECTRIC ist gewÑhlte die Reihenfolge nicht gewÑhrleistet */
					if( slct_check(0x0100))
					{
	 					slct_morenames(0, MAXPATHS,lname); /* we want more */
	 				}
					filename[0]=0;
					if(!getfilename(filename,fpattern,"",fselmsg[0]))
						break;
					if(!filename[0]) /* Kunde will nicht */
						break;
					if(slct && (slct->out_count>1))
					{
						slct->out_count=min(slct->out_count,MAXPATHS);
						for(i=0; i<slct->out_count; i++)
						{  /* Pfadnamen zusammenbasteln */
							if((cp=strrchr(filename,'\\'))!=NULL || (cp=strrchr(filename,'/'))!=NULL)
							{
								cp[1]=0;
								strcat(filename,lpath[i]);
								switch(_read_blk(wp, filename, &begcut, &endcut))
								{
									case -1:	/* kein ram frei */
										sprintf(alertstr,A7up[3],(char *)split_fname(filename));
										form_alert(1,alertstr); /* kein break, es geht weiter */
									case TRUE: /* ok */
										wp->w_state|=CHANGED;
										k=endcut->used; /* k nicht i */
										graf_mouse_on(FALSE);
										Wcursor(wp);
										if((wp->w_state&COLUMN))
											paste_col(wp,begcut,endcut);
										else
											paste_blk(wp,begcut,endcut);
										Wcursor(wp);
										graf_mouse_on(TRUE);
										endcut->endcol=k;
										if(!(wp->w_state&COLUMN))
											hndl_blkfind(wp,begcut,endcut,SEAREND);
										else
											free_blk(wp,begcut); /* Spaltenblock freigeben */
										graf_mouse_on(FALSE);
										Wcursor(wp);
										Wredraw(wp,&wp->work);
										Wcursor(wp);
										graf_mouse_on(TRUE);
										break;
									case FALSE:
										break;
								}
								begcut=endcut=NULL;
							}
						}
						undo.item=FALSE;
					}
					else
					{
						switch(_read_blk(wp, filename, &begcut, &endcut))
						{
							case -1:	/* kein ram frei */
								sprintf(alertstr,A7up[3],split_fname(filename));
								form_alert(1,alertstr); /* kein break, es geht weiter */
							case TRUE: /* ok */
								store_undo(wp, &undo, begcut, endcut, WINEDIT, EDITCUT);
								wp->w_state|=CHANGED;
								i=endcut->used;
								graf_mouse_on(FALSE);
								Wcursor(wp);
								if((wp->w_state&COLUMN))
									paste_col(wp,begcut,endcut);
								else
									paste_blk(wp,begcut,endcut);
								Wcursor(wp);
								graf_mouse_on(TRUE);
								endcut->endcol=i;
								if(!(wp->w_state&COLUMN))
									hndl_blkfind(wp,begcut,endcut,SEAREND);
								else
									free_blk(wp,begcut); /* Spaltenblock freigeben */
								graf_mouse_on(FALSE);
								Wcursor(wp);
								Wredraw(wp,&wp->work);
								Wcursor(wp);
								graf_mouse_on(TRUE);
								break;
							case FALSE:
								sprintf(alertstr,A7up[4],(char *)split_fname(filename));
								form_alert(1,alertstr);
								undo.item=FALSE;	/* EDITCUT */
								break;
						}
						begcut=endcut=NULL;
					}
				}
				break;
			case WININFO:
				hndl_fileinfo(wp,blkwp);
				break;
			case WINCLOSE:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							delete(wp,desktop,desk_obj);
						}
					}
				}
				else
				{
					if(wp)
					{
						desk_obj=wp->icon;
						delete(wp,desktop,desk_obj);
					}
				}
				break;
			case WINSAVAS:
				if(topwin && !cut && begcut && endcut) /* markierter Block */
				{
					write_block(wp,begcut,endcut);
					break;							 /* hier mu· ein break hin !!! */
				}
				else					  /* ganze Datei */
					newname=TRUE; /* kein break; */
			case WINSAVE:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							if(!strcmp((char *)Wname(wp),NAMENLOS))
								write_file(wp,TRUE);
							else
							{
								write_file(wp,FALSE);
								strcpy(tempname,(char *)Wname(wp));
								unlink(change_ext(tempname,".$$$")); /* <NAME>.$$$ lîschen */
							}
						}
					}
				}
				else
				{
					if(!strcmp((char *)Wname(wp),NAMENLOS))
						newname=TRUE;
					write_file(wp,newname);
					strcpy(tempname,(char *)Wname(wp));
					unlink(change_ext(tempname,".$$$")); /* <NAME>.$$$ lîschen */
				}
				timer=0L;		 /* Autosave zurÅcksetzen */
				newname=FALSE;
				break;
			case WINABORT:
				if(wp->w_state&CHANGED)
				{
					if(!cut)
						hide_blk(wp,begcut,endcut);
					strcpy(&alertstr[100],(char *)Wname(wp));  /* alerstr von delete gebraucht */
					desk_obj=wp->icon;
					if(delete(wp,desktop,desk_obj))
						Wreadtempfile(&alertstr[100],TRUE); /* hier steht der alte Name drin */
				}
				break;
			case WINSHELL:
				hndl_shell(shellmenu,0);
				break;
			case WINLAY:
				hndl_layout(layout,0);
				break;
			case WINPRINT:
				if(topwin && !cut && begcut && endcut)
				{
					print_block(wp,begcut,endcut);
				}
				else
				{
					if(is_selected(desktop,DESKICN1,DESKICN7))
					{
						for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
						{
							if(desktop[desk_obj].ob_state & SELECTED)
							{
								wp=Wicon(desk_obj);				/* nimm Icon  */
								hndl_prtmenu(prtmenu,layout,pinstall,wp);
							}
						}
					}
					else
						hndl_prtmenu(prtmenu,layout,pinstall,wp);
				}
				break;
			case WINQUIT:
/*
				if(!Wcount(CHANGED) ||
				 	(mevent.e_ks & (K_RSHIFT|K_LSHIFT)) ||
					(divmenu[DIVSAVE].ob_state & SELECTED))
				{
					terminate_at_once=TRUE;
					exit(exitcode); /* schon hier raus, sonst sind die Namen weg! */
				}
*/
				if(!Wcount(CHANGED))
				{
					terminate_at_once=TRUE;
					exit(exitcode);
				}
				abbruch=FALSE;
				for(k=1; !abbruch && k<MAXWINDOWS; k++)
				{
					if(_wind[k].w_state & CREATED)
					{
						if(_wind[k].w_state & CHANGED)
						{
							sprintf(alertstr,A7up[1],split_fname((char *)Wname(&_wind[k])));
							switch(form_alert(3,alertstr))
							{
								case 1:
									_wind[k].w_state&=~CHANGED;
									break;
								case 2:
									abbruch=TRUE;
									break;
								case 3:
									write_file(&_wind[k], FALSE);
									strcpy(alertstr,(char *)Wname(&_wind[k]));
									unlink(change_ext(alertstr,".$$$")); /* <NAME>.$$$ lîschen */
									break;
							}
						}
						if(!Wcount(CHANGED) && !abbruch)
  						{
							terminate_at_once=TRUE;
							exit(exitcode);
						}
						if(!abbruch)
						{
							del_icon(desktop, _wind[k].icon);
							Wfree(&_wind[k]);
							Wdelete(&_wind[k]);
							menu_ienable(winmenu,WINNEW,TRUE);  /* Menueintrag wieder freigeben */
							menu_ienable(winmenu,WINOPEN,TRUE);
						}
					}
				}
				terminate_at_once=FALSE;
				exitcode=0;				  				/* zurÅcksetzen */
				break;
			case EDITUNDO:
				do_undo(wp);
				break;
			case EDITALL:
				if(wp)
				{
					graf_mouse_on(FALSE);
					Wcursor(wp);
					mark_all(wp,&begcut,&endcut);
					Wcuroff(wp);
					Wcursor(wp);
					graf_mouse_on(TRUE);
				}
				else
					sel_icons(desktop,DESKICN1,DESKICND,TRUE);
				break;
			case EDITCUT:
				if(wp && !cut && begcut && endcut)
				{
					graf_mouse_on(FALSE);
					Wcursor(wp);
					free_undoblk(wp, undo.blkbeg);
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
					graf_mouse_on(TRUE);
					if(clipbrd)
					{
						write_clip(wp,begcut,endcut);
						free_blk(wp,begcut);
					}
					undo.menu=WINEDIT;
					undo.item=EDITPAST;
				}
				break;
			case EDITCOPY:
				if(wp && !cut && begcut && endcut)
				{
					if(clipbrd)
					{
						write_clip(wp,begcut,endcut);
					}
					else
					{
						graf_mouse_on(FALSE);
						Wcursor(wp);
						if((wp->w_state&COLUMN))
							cut=copy_col(wp,begcut,endcut,&begcopy,&endcopy);
						else
							cut=copy_blk(wp,begcut,endcut,&begcopy,&endcopy);
						Wcuron(wp);
						Wcursor(wp);
						graf_mouse_on(TRUE);
						switch(cut)
						{
							case  TRUE:
								begcut=begcopy;
								endcut=endcopy;
								break;
							case  FALSE:
								break;
							case -1:
								free_blk(wp,begcopy);		 /* Kopie freigeben	 */
								form_alert(1,A7up[5]);
								break;
						}
					}
				}
				break;
			case EDITPAST:
				if(wp && (clipbrd?1:(cut && begcut && endcut)))
				{
					if(clipbrd)
					{
						if(!cut && begcut && endcut)
						{  /* wenn was markiert, erst weg damit */
							overwrite=TRUE;
							graf_mouse_on(FALSE);
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
							graf_mouse_on(TRUE);
							free_blk(wp,begcut);
						}
						else
						{
							if(begcut && endcut)		  /* Block existiert */
							{
										  /* nach Klemmbretteinschaltung kann */
												/* noch ein Ramblock existieren */
								free_blk(wp,begcut);		  /* ...weg damit */
							}
							else				 /* ^B gedrÅckt, aber nicht ^E */
							{						/* also noch MÅll aufrÑumen */
						 		cut=FALSE;
						  		begline=endline=0L;
						 		begcut=endcut=0L;
							}
						}
						if(read_clip(wp,&begcut,&endcut))
						{
							store_undo(wp, &undo, begcut, endcut, WINEDIT, overwrite?CUTPAST:EDITCUT);
							i=endcut->used;
							graf_mouse_on(FALSE);
							Wcursor(wp);
							if((wp->w_state&COLUMN))
								paste_col(wp,begcut,endcut);
							else
								paste_blk(wp,begcut,endcut);
							Wcursor(wp);
							graf_mouse_on(TRUE);
							endcut->endcol=i;
							if(!(wp->w_state&COLUMN))
								hndl_blkfind(wp,begcut,endcut,SEAREND);
							else
								free_blk(wp,begcut); /* Spaltenblock freigeben */
						}
						begcut=endcut=NULL;
					}
					else
					{
						graf_mouse_on(FALSE);
						Wcursor(wp);
						cut=copy_blk(wp,begcut,endcut,&begcopy,&endcopy);
						switch(cut)
						{
							case  TRUE:
								store_undo(wp, &undo, begcopy, endcopy, WINEDIT, EDITCUT);
								i=endcopy->used;
								if((wp->w_state&COLUMN))
									paste_col(wp,begcopy,endcopy);
								else
									paste_blk(wp,begcopy,endcopy);
								endcopy->endcol=i;
								break;
							case  FALSE:
								break;
							case -1:
								free_blk(wp,begcopy);		 /* Kopie freigeben	 */
								form_alert(1,A7up[5]);
								cut=FALSE;
								undo.item=FALSE;	/* EDITCUT */
								break;
						}
						Wcursor(wp);
						graf_mouse_on(TRUE);
						if(!(wp->w_state&COLUMN))
						{
							if(cut)
								hndl_blkfind(wp,begcopy,endcopy,SEAREND);
						}
					}
				}
				break;
			case EDITSHLF:
				shlf_line(wp,begcut,endcut);
				break;
			case EDITSHRT:
				shrt_line(wp,begcut,endcut);
				break;
			case EDITFORM:
				if(!cut)
				{
/*
					textformat(wp,&begcut,&endcut);
*/
					hndl_textformat(wp,formatmenu,&begcut,&endcut);
				}
				break;
			case EDITSORT:
				hndl_sort(wp,sortmenu,&begcut,&endcut);
				break;

			case EDITTOGL:
			case EDITBIG:
			case EDITSMAL:
			case EDITCAPS:
				if(!cut)
					changeletters(wp,begcut,endcut,item);
				break;
/*
			case EDITTOGL:
				if(!cut)
					changeletters(wp,begcut,endcut,item);
				break;
			case EDITBIG:
			case EDITSMAL:
			case EDITCAPS:
				if(!cut)
					textformat2(wp, begcut, endcut, item);
				break;
*/
/*
			case EDITCLIP:
				menu_icheck(winmenu,EDITCLIP,clipbrd^=TRUE);
				break;
*/
			case SEARNEXT:
			case SEARFIND:
			case SEARSEL :
				hndl_find(wp,&begcut,&endcut,item);
				break;
			case SEARFILE:
				hndl_grepmenu(grepmenu,0);
				break;
			case SEARBEG:
			case SEAREND:
				k=endcut->endcol; /* ans ende also 512 gesprungen	  */
				endcut->endcol=min(endcut->used,endcut->endcol);
				hndl_blkfind(wp,begcut,endcut,item);
				endcut->endcol=k;
				break;
			case SEARSMRK:
			case SEARGMRK:
				textmarker(wp,markmenu,item,0,0);
				break;
			case SEARGOTO:
				hndl_goto(wp,gotomenu,-1);
				break;
			case SEARPAGE:
				hndl_page(wp,gotomenu,-1);
				break;
			case BLKCNT:
			case BLKSUM:
			case BLKMEAN:
			case BLKSDEV:
			case BLKMWST:
			case BLKINTER:
			case BLKALL:
				if(!cut)
					rechnen(wp,nummenu,item,begcut,endcut);
				break;
			case BLKKONF:
				hndl_nummenu(nummenu,0,0);
				break;
			case FORMTAB:
				i=hndl_tab(tabmenu,wp);
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							wp->tab=i;
						}
					}
				}
				else
					wp->tab=i;
				break;
			case FORMUMBR:
				i=hndl_umbruch(umbrmenu,wp,0);
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							wp->umbruch=i;
						}
					}
				}
				else
					wp->umbruch=i;
				break;
			case FORMBLK:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							menu_icheck(winmenu,FORMBLK,(wp->w_state^=BLOCKSATZ)&BLOCKSATZ?TRUE:FALSE);
						}
					}
				}
				else
					menu_icheck(winmenu,FORMBLK,(wp->w_state^=BLOCKSATZ)&BLOCKSATZ?TRUE:FALSE);
				break;
			case BLKCOL:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							menu_icheck(winmenu,BLKCOL,(wp->w_state^=COLUMN)&COLUMN?TRUE:FALSE);
						}
					}
				}
				else
					menu_icheck(winmenu,BLKCOL,(wp->w_state^=COLUMN)&COLUMN?TRUE:FALSE);
				break;
			case FORMINS:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							menu_icheck(winmenu,FORMINS,(wp->w_state^=INSERT)&INSERT?TRUE:FALSE);
						}
					}
				}
				else
				{
					graf_mouse_on(FALSE);
					Wcursor(wp);
					menu_icheck(winmenu,FORMINS,(wp->w_state^=INSERT)&INSERT?TRUE:FALSE);
					Wcuron(wp);
					Wcursor(wp);
					graf_mouse_on(TRUE);
				}
				break;
			case FORMIND:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							menu_icheck(winmenu,FORMIND,(wp->w_state^=INDENT)&INDENT?TRUE:FALSE);
						}
					}
				}
				else
					menu_icheck(winmenu,FORMIND,(wp->w_state^=INDENT)&INDENT?TRUE:FALSE);
				break;
			case MACOPEN:
				loadmacro();
				break;
			case MACSAVE:
				savemacro();
				break;
			case MACREC:
			case MACSTOP:
			case MACPLAY:
				record_macro(item,0,0);
				break;
			case MACSCT:
				hndl_shortcuts();
				break;
			case WINNEXT2:
				Wcycle(wp);
				break;
			case WINOPALL:
				Wopenall();
				break;
			case WINCLALL:
				Wcloseall();
				break;
			case WINARR1:
			case WINARR2:
			case WINARR3:
			 	graf_mouse_on(FALSE);
			 	Wcursor(wp);
				Warrange(item-WINARR1+1);
			 	Wcursor(wp);
			 	graf_mouse_on(TRUE);
				break;
			case WINFULL:
			 	graf_mouse_on(FALSE);
			 	Wcursor(wp);
				Wfull(wp);
			 	Wcursor(wp);
			 	graf_mouse_on(TRUE);
				break;
			case WINDAT1:
			case WINDAT2:
			case WINDAT3:
			case WINDAT4:
			case WINDAT5:
			case WINDAT6:
			case WINDAT7:
				Wopen(Wmentry(item));
				break;
			case OPTHELP:
				if(wp && (mevent.e_ks & (K_LSHIFT|K_RSHIFT)))
				{
					if(*errorstr=='\"' && errorstr[strlen(errorstr)-1]=='\"')
					{
						if(wp->kind & INFO)
							wind_set(wp->wihandle,WF_INFO,errorstr);
						else
							form_alert(1,A7up[6]);
					}
					else
					{
						form_alert(1,A7up[7]);
					}
				}
				else
					if(!help())
					{
						sprintf(alertstr,A7up[8],(char *)(divmenu[DIVHDA].ob_spec.index/*+16L*/));
						form_alert(1,alertstr);
					}
				break;
			case OPTFONT:
				hndl_font(wp,fontmenu);
				av_font=wp?wp->fontid:av_font;
				av_point=wp?wp->fontsize:av_point;
				break;
			case OPTCHARS:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							hndl_chartable(wp,chartable);
						}
					}
				}
				else
					hndl_chartable(wp,chartable);
				break;
			case OPTFKT:
				hndl_fkeymenu(fkeymenu,0);
				break;
			case OPTVIEW:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							hndl_preview(wp,preview, begcut, endcut);
						}
					}
				}
				else
					hndl_preview(wp,preview, begcut, endcut);
				break;
			case OPTLIN:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							hndl_lineal(wp,0);
						}
					}
				}
				else
					hndl_lineal(wp,0);
				break;
			case OPTBRACE:
				if(is_selected(desktop,DESKICN1,DESKICN7))
				{
					for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
					{
						if(desktop[desk_obj].ob_state & SELECTED)
						{
							wp=Wicon(desk_obj);				/* nimm Icon  */
							check_braces(bracemenu,wp,NULL,NULL);
						}
					}
				}
				else
					check_braces(bracemenu,wp,begcut,endcut);
				break;
			case OPTCOMP:
				textcompare();
				break;
			case OPTDIV:
				hndl_diverses(divmenu,0);
				break;
			case OPTSAVE:	/* Einstellungen sichern */
/*
				if(form_alert(2,A7up[9])==2)
*/
				{
					sicons();
					saveconfig(FALSE);
				}
				break;
			default:
				break;
		}
		/* Reihenfolge nicht Ñndern! */
		toolbar_tnormal(wp,wp?wp->toolbar:NULL, item, 1);
		wp=Wgettop();
		checkmenu(winmenu,wp);	/* je nach Aktion, Menu (de)aktivieren */
										/* hier mu· es Wgettop() statt wp hei·en, */
										/* weil vielleicht ein Fenster geschlossen wurde */
  		menu_tnormal(winmenu,menu,1);
}

void _hndl_keybd(WINDOW *wp, int kstate, int key)
{
	int menu, item;

	MapKey(&kstate,&key); /* Profibuch Seite 761 */

	if(!Whndlkbd(wp,kstate,key))
	{
		switch(menu_ikey(winmenu,kstate,key,&menu,&item))
		{
			case -1:
				/* MT 16.4.95 disableter MenÅpunkt. Nichts machen */
				break;
			case  0:
				if(wp /*&& ! (wp->w_state & ICONIFIED)*/)
				{
					if(!fkeys(wp,kstate,key,&begcut,&endcut))
					{
						altnum(&kstate,&key);
						if(!special(wp,&blkwp,kstate,key,&begcut,&endcut)) /* versteckte Shortcuts */
							editor(wp,kstate,key,&begcut,&endcut);
					}
				}
				break;
			default:
				hndl_menu( menu, item); /* nur bei geÑndertem menu_ikey() */
				break;
		}
/*
		if(!menu_ikey(winmenu,kstate,key,&menu,&item))
		{
			if(wp /*&& ! (wp->w_state & ICONIFIED)*/)
			{
				if(!fkeys(wp,kstate,key,&begcut,&endcut))
				{
					altnum(&kstate,&key);
					if(!special(wp,&blkwp,kstate,key,&begcut,&endcut)) /* versteckte Shortcuts */
						editor(wp,kstate,key,&begcut,&endcut);
				}
			}
		}
		else
			hndl_menu( menu, item); /* nur bei geÑndertem menu_ikey() */
*/
	}
	else
	{
      if(!_WKeyBlock(wp, &begcut, &endcut, kstate, key))
			if(!cut)
				hide_blk(wp,begcut,endcut);
	}
}

/* lÑuft nicht bei Blockoperationen gecancelt 10.7.94
WINDOW *Wgetwind(register int wum)
{
	WINDOW *wp=NULL;
	
	if(wum) /* window_under_mouse */
		wp=Wp(wind_find(mevent.e_mx, mevent.e_my));
	return(wp?wp:Wgettop());
}
*/

void hndl_keybd(int kstate, int key)
{
	WINDOW *wp;
	register int tkey, repeat_off = FALSE;
/*
	wp=Wgetwind(window_under_mouse);
*/
	wp=Wgettop();
	
	tkey=key>>8;	  /* Tempkey */
	if(
		tkey==0x4B || /* lf 		 */
		tkey==0x4D || /* rt 		 */
		tkey==0x48 || /* up 		 */
		tkey==0x50 || /* dn 		 */
		tkey==0x0E || /* BS 		 */
		tkey==0x53 || /* Del		 */
		tkey==0x1C || /* Return  */
		tkey==0x72)	/* Enter	*/
	{
		Supexec(_keyoff);
		repeat_off=TRUE;
	}
	mevent.e_time=2*E_TIME; /* VerlÑngern, weil jetzt Tasten gedrÅckt werden */

	_hndl_keybd(wp,kstate,key);

	if(repeat_off)
		Supexec(_keyon);
}

int mouse_is_in_icon(OBJECT *tree, int icon, int mx, int my)
{
	ICONBLK *iconblk;
	int xoff, yoff;
	iconblk=tree[icon].ob_spec.iconblk;
	objc_offset(tree,icon,&xoff,&yoff);
	if((mx > xoff+iconblk->ib_xicon && mx < xoff+iconblk->ib_xicon + iconblk->ib_wicon - 1 &&
		 my > yoff+iconblk->ib_yicon && my < yoff+iconblk->ib_yicon + iconblk->ib_hicon + 5 - 1 )
		||
		(mx > xoff+iconblk->ib_xtext && mx < xoff+iconblk->ib_xtext + iconblk->ib_wtext - 1 &&
		 my > yoff+iconblk->ib_ytext && my < yoff+iconblk->ib_ytext + iconblk->ib_htext - 1 ) )
		return(TRUE);
	else
		return(FALSE);
}

void hndl_icons(OBJECT *tree, int mx, int my, int key, int click)
{
	int desk_obj,ret,mc;

	if(nodesktop)
		return;

	desk_obj=objc_find(tree,ROOT,MAX_DEPTH,mx,my);
	if((desk_obj>=DESKICN1 && desk_obj<=DESKICND) && mouse_is_in_icon(tree,desk_obj,mx,my))
	{
		if(key & (K_RSHIFT|K_LSHIFT))
		{
			if(tree[desk_obj].ob_state & SELECTED)
			{
				desel_icons(tree,desk_obj,desk_obj,TRUE);
				wind_update(BEG_MCTRL);
				do /* warten, bis mausknopf losgelassen */
				{
					graf_mkstate(&ret,&ret,&mc,&ret);
				}
				while(mc);
				wind_update(END_MCTRL);
			}
			else
			{
				sel_icons(tree,desk_obj,desk_obj,TRUE);
				wind_update(BEG_MCTRL);
				drag_icon(tree,desk_obj);
				wind_update(END_MCTRL);
			}
		}
		else
		{
			if(click==SINGLECLICK)
			{
				wind_update(BEG_MCTRL);
				drag_icon(tree,desk_obj);
				wind_update(END_MCTRL);
			}
			else
			{
				desel_icons(tree,DESKICN1,DESKICND,TRUE);
				click_icon(tree,desk_obj);
			}
		}
	}
	else
	{
		desel_icons(tree,DESKICN1,DESKICND,TRUE);
		mark_icons(tree,mx,my);
	}
}


void hndl_button(void)
{
	WINDOW *wp;
	LINESTRUCT *line;
	int x,y,len;

	wp=Wp(wind_find(mevent.e_mx,mevent.e_my));
   
   if(wp && toolbar_do(wp,wp->toolbar,mevent.e_mx,mevent.e_my,mevent.e_br,mevent.e_ks)>0) /* Toolbar geklickt. Gleich raus */
      return;

	if(!winx && mevent.e_mb==3 && mevent.e_br==1) /* Browsing einleiten */
	{
		if(wp)
		{
			if(isvertical(wp,mevent.e_mx,mevent.e_my))
			{
				Wrealscroll(wp, mevent.e_mx, mevent.e_my, VERTICAL);
				return;
			}
			if(ishorizontal(wp,mevent.e_mx,mevent.e_my))
			{
				Wrealscroll(wp, mevent.e_mx, mevent.e_my, HORIZONTAL);
				return;
			}
		}
	}
	if(mevent.e_br) /* alle Klicks durchlassen */
	{
		if(wp)
		{
			if(!_Wmarkblock(wp,&begcut,&endcut,mevent.e_mx,mevent.e_my,mevent.e_br,mevent.e_ks))
			{
				if(mevent.e_br==SINGLECLICK)
				{
					if(!cut)
						hide_blk(wp,begcut,endcut);
					if(mevent.e_ks & (K_RSHIFT|K_LSHIFT)) /* Blockmarkierung mit Shifttaste */
					{
						begcut=beg_blk(wp,begcut,endcut);
						endcut=NULL;
						cut=FALSE;
						blkwp=wp;							 /* diesem Fenster gehîrt der Block */
					}
					if(Wmxycursor(wp,mevent.e_mx,mevent.e_my))
						undo.item=FALSE;
					if(mevent.e_ks & (K_RSHIFT|K_LSHIFT)) /* Blockmarkierung mit Shifttaste */
					{
						graf_mouse_on(FALSE);
						Wcursor(wp);
						endcut=end_blk(wp,&begcut,&endcut);
						Wcuroff(wp);
						Wcursor(wp);
						graf_mouse_on(TRUE);
						if(endcut==NULL ||						  /* Fehler? oder	 */
						  (begcut->begcol == begcut->endcol)) /* gleiche Stelle? */
						{
							hide_blk(wp,begcut,endcut);
							blkwp=NULL;
							goto WEITER;					  /* Bei Fehler Åberspringen */
						}
					}
					if(mevent.e_ks & K_CTRL)  /* Blockmarkierung mit Controltaste */
					{
						if(cut)					/* gibt es erst noch mÅll zu lîschen */
							free_blk(wp,begcut);
							/* erst Blockende dann Blockanfang, sonst geht es nicht */
						if(wp==blkwp && begcut && !endcut)  /* Reihenfolge NICHT vertauschen */
						{
							graf_mouse_on(FALSE);
							Wcursor(wp);
							endcut=end_blk(wp,&begcut,&endcut);
							Wcuroff(wp);
							Wcursor(wp);
							graf_mouse_on(TRUE);
							if(endcut==NULL ||						  /* Fehler? oder	 */
							  (begcut->begcol == begcut->endcol)) /* gleiche Stelle? */
							{
								hide_blk(wp,begcut,endcut);
								blkwp=NULL;
								goto WEITER;				  /* Bei Fehler Åberspringen */
							}
						}
						if(!begcut && !endcut)
						{
							begcut=beg_blk(wp,begcut,endcut);
							endcut=NULL;
							cut=FALSE;
							blkwp=wp;							 /* diesem Fenster gehîrt der Block */
						}
WEITER:			  ; /* <- hier mu· fÅr PC-TC 2.0 ein Leerstatement sein, grrr.. */
					}
				}
			}
			else
			{
				blkwp=wp;									 /* diesem Fenster gehîrt der Block */
				cut=FALSE;
				mevent.e_br=0;
			}
		}
		else
			hndl_icons(desktop,mevent.e_mx,mevent.e_my,mevent.e_ks,SINGLECLICK);
	}
	if(mevent.e_br == TRIPLECLICK)
	{
		if(wp)
		{
			if(Wdclickline(wp,&begcut,&endcut,mevent.e_mx,mevent.e_my))/* Zeile */
			{
				blkwp=wp;					/* diesem Fenster gehîrt der Block */
				cut=FALSE;
			}
			else
				Bconout(2,7);
		}
	}
	if(mevent.e_br == DOUBLECLICK)
	{
		if(wp)
		{
			if(mevent.e_ks & (K_RSHIFT|K_LSHIFT))
			{
				if(Wdclickline(wp,&begcut,&endcut,mevent.e_mx,mevent.e_my))/* Zeile */
				{
					blkwp=wp;					/* diesem Fenster gehîrt der Block */
					cut=FALSE;
				}
				else
					Bconout(2,7);
			}
			else
			{
				if(Wdclickword(wp,&begcut,&endcut,mevent.e_mx,mevent.e_my))
				{
					blkwp=wp;				  /* diesem Fenster gehîrt der Block */
					cut=FALSE;
					if(iserrfile(wp) && isnumeric(searchstring) && Wcount(OPENED)==2) /* Errorfile ?	  */
					{  /* Errorlisting in <name>.ERR, zweite Datei beliebig */
						/* in searchstr ist angeklickte Zeilenzahl */
						strcpy(alertstr,searchstring);	/* retten, weil durch */
																	/* hide_blk() gelîsch */
						line=begcut;
						if(!cut)
							hide_blk(wp,begcut,endcut);
						else
							free_blk(wp,begcut);
						Wcycle(wp);								 /* Fensterwechsel	  */
						if((wp=Wgettop()) != NULL)			 /* im zweiten Fenster zur Zeile */
						{
							if(!cut)
								hide_blk(wp,begcut,endcut);
							strcpy(searchstring,alertstr);	/* wieder umkopieren  */
							hndl_goto(wp,NULL,atol(searchstring)); /* Zeilennummer angeklickt ? */
							if(wp->kind & INFO)
							{
								strcpy(errorstr,"\"");/* errorstring zusammenbasteln */
								strcat(errorstr,line->string);
								strcat(errorstr,"\"");
								graf_mouse_on(FALSE);
								Wcursor(wp);			/* Cursor korrekt positionieren */
								Wsetrcinfo(wp); /* Infozeile setzen, um Timer zu beruhigen */
								Wcursor(wp);
								graf_mouse_on(TRUE);
								wind_set(wp->wihandle,WF_INFO,errorstr); /* einblenden */
							}
						}
						*searchstring=0;					  /* lîschen				*/
					}
					if(isregfile(wp) && isdigit(begcut->string[0])) /* Regular Expression file ? */
					{
						line=wp->fstr;
						while(strncmp(line->string,"Gesucht: \"",10))
						{										/* wo ist das Suchmuster? */
							line=line->next;
						}
						strcpy(alertstr,&line->string[10]); /* Suchmuster kopieren */
						alertstr[strlen(alertstr)-1]=0;	  /* letztes " killen  */
						prepare(findmenu,grepmenu,alertstr);			/* Weitersuchen ^G	*/
						sscanf(begcut->string,"%d %d %d:",&y,&x,&len);
						line=begcut;
						do
						{
							line=line->prev;
						}
						while(strncmp(line->string,"Datei",5) && line->string[7]!=':');
						strcpy(alertstr,&line->string[6]);
						if(!cut)
							hide_blk(wp,begcut,endcut);
						else
							free_blk(wp,begcut);
						wp=Wreadtempfile(alertstr,FALSE); /* Datei laden */
						if(wp) /* testen, ob gÅltig */
						{
							if(!cut)
								hide_blk(wp,begcut,endcut);
							sprintf(searchstring,"%d",y);
							hndl_goto(wp,NULL,atol(searchstring)); /* Zeilennummer angeklickt ? */
							graf_mouse_on(FALSE); /* Cursor positionieren */
							Wcursor(wp);
							wp->cspos=wp->col=(int)((x-1)-wp->wfirst/wp->wscroll);
							wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
							lastwstr=wp->wstr;
							lasthfirst=wp->hfirst;
							begline=(y-1)+wp->hfirst/wp->hscroll;
							endline=begline;
							begcut=endcut=wp->cstr;
							begcut->begcol=endcut->begcol=x-1;
							begcut->endcol=endcut->endcol=x-1+len;
							begcut->attr=(endcut->attr|=SELECTED);
							mark_line(wp,begcut,y-1);
							Wcuroff(wp);
							Wcursor(wp);
							graf_mouse_on(TRUE);
							strncpy(searchstring,&begcut->string[x-1],len);
							searchstring[len]=0;
							blkwp=wp;
						}
					}
					if((x=isfreepattern(bracemenu,searchstring)) != 0)
					{
						form_read(bracemenu,x,iostring);
						form_read(bracemenu,x+1,&iostring[20]);
						if(Wirth2(wp,&begcut,&endcut,iostring,&iostring[20]))
						{
							x=endcut->endcol; /* ans ende also 512 gesprungen	  */
							endcut->endcol=min(endcut->used,endcut->endcol);
							hndl_blkfind(wp,begcut,endcut,SEAREND);
							endcut->endcol=x;
						}
					}
				}
				else
				{
					if(Wdclickbrace(wp,&begcut,&endcut,mevent.e_mx,mevent.e_my))
					{
						blkwp=wp;		  /* diesem Fenster gehîrt der Block */
						cut=FALSE;
					}
					else
						Bconout(2,7);
				}
			}
		}
		else
		{
			hndl_icons(desktop,mevent.e_mx,mevent.e_my,mevent.e_ks,DOUBLECLICK);
		}
	}
	checkmenu(winmenu,Wgettop()); /* je nach Aktion, Menu (de)aktivieren */
}

void hndl_timer(void)
{
	WINDOW *wp;

	wp=Wgettop();
/*
	wp=Wgetwind(window_under_mouse);
*/
	timer++;
	mevent.e_time=E_TIME; /* wieder kÅrzer blinkend */
	
	_Wcblink(wp, mevent.e_mx, mevent.e_my);
	if(!(timer % 2))					  /* jede 1s */
	{
		Wslupdate(wp,1+2+4+8);
		Wsetrcinfo(wp);
		checkmenu(winmenu,wp);
		if(undo.item==FALSE)
			free_undoblk(wp,undo.blkbeg); /* freigeben, wenn nicht gebraucht */
		Wtabbarupdate(wp);
	}
	if(!(timer % 20)) /* alle 10s */
	{
		graf_mouse_on(TRUE); /* Maus zwangsweise wieder einschalten */
	}
	/* Auswertung des Registrierungscodes rausgeworfen (MJK 3/97)
	if(!(timer %  240))	/* alle 2min Registrierungscode auswerten */
	{
		reg_code();
	}*/
	if(wp && (wp->w_state & CHANGED) && !(timer % (backuptime/(481L)))) /* autom. Backupzeit */
	{
		if(!begcut && !endcut) /* kein Block, ganze Datei */
		{
			if(!strcmp((char *)Wname(wp),NAMENLOS))
				write_file(wp,TRUE);
			else
			{
				strcpy(alertstr,(char *)Wname(wp));
				_write_file(wp,change_ext(alertstr,".$$$"), "wb", FALSE, FALSE, SIGNULL);
				wp->w_state|=CHANGED; /* wieder setzen */
			}
		}
	}
}

#pragma warn -par
void cdecl sighandler(long arg)
{
	register int i;
  	for(i=1; i<MAXWINDOWS; i++)
  	{
		if(_wind[i].w_state & CHANGED)
		{
			if(!strcmp((char *)Wname(&_wind[i]),NAMENLOS))
				sprintf((char *)Wname(&_wind[i]),NAMENLOS".%003d",i);
			_write_file(&_wind[i], (char *)Wname(&_wind[i]), "wb", FALSE, FALSE, SIGTERM);
  		}
  	}
	Psigreturn();
}
#pragma warn .par

void main(int argc,char *argv[])
{
	int event, ap_id;
	
	pinit(isinffile(argc, argv));
	atexit(pexit);


	if(vastart) /* in Diverses einstellbar */
		if((ap_id=My_appl_find(gl_apid,"7UP     "))>=0 && ap_id!=gl_apid) /* schon gelagen */
			if(relay(gl_apid,ap_id,argc,argv)) /* versuchen, Parameter zu Åbergeben */
				exit(0);								/* schon zu Ende */

	if(get_cookie('MiNT')!=NULL)
	{
		Psignal(SIGTERM,
#ifndef __TOS
		        (long)
#endif
		        sighandler); /* SIGTERM abfangen */
	}
/* funktioniert nicht auf dem Falcon
	ckbd = get_cookie('CKBD');
*/
	winx = get_cookie('WINX');
	
	if(_AESversion>=0x0400 && 
		_AESnumapps!=1 && 
		get_cookie('MiNT'))
		menu_register(gl_apid, VERSION);

	AVInit(gl_apid,"7UP     ",1|2|16|1024); /* ,NULL); AV_SENDKEY + AV_ASKFILEFONT + AV_OPENWIND + AV_EXIT */

	/* Registrieranforderung rausgworfen (MJK 3/97)
   registriert = loadlizenz("7UP.KEY");
   
   if(!registriert)
   {
/*
		winmenu[MACOPEN].ob_state|=DISABLED;
		winmenu[MACSAVE].ob_state|=DISABLED;
		winmenu[MACREC ].ob_state|=DISABLED;
		winmenu[MACSTOP].ob_state|=DISABLED;
		winmenu[MACPLAY].ob_state|=DISABLED;
*/
		/* Registrierbutton rausgeworfen (MJK 3/97)
		shareware[SHAREREGIST].ob_flags|=HIDETREE; */
		form_exhndl(shareware,0,0);
		/* Registrierbutton rausgeworfen (MJK 3/97)
		shareware[SHAREREGIST].ob_flags&=~HIDETREE; */
	}*/

	loadmenu("7UP.MNU");
	loadshortcuts("7UP.KBD");	  /* Defaultshortcuts zuerst laden */
	loadsoftkeys("7UP.SFK");
	file_input(argc,argv);
	checkmenu(winmenu,Wgettop()); /* je nach Aktion, Menu (de)aktivieren */

	while(TRUE)
	{
		event=mevnt_event(&mevent);	/* XGEM's event_multi() aber mit Macrofunktion */
		if(event & MU_MESAG)
		{
			hndl_mesag(msgbuf);

/* wg. Macro einkommentiert
			event=0;	  /* vermeidet hakeliges Scrolling, weil timerinterrupt */
*/
		}					/* Åbergangen wird												*/
		if(event & MU_KEYBD)
		{
			hndl_keybd(mevent.e_ks,mevent.e_kr);
			event=0;	  /* s.o. */
		}
		if(event & MU_BUTTON)
		{
			hndl_button();
		}
		if(event & MU_TIMER)
		{
#if MiNT
			wind_update(BEG_UPDATE);
#endif
			hndl_timer();
#if MiNT
			wind_update(END_UPDATE);
#endif
		}
		if(event & MU_M1)
		{
			graf_mouse_on(1);
			mevent.e_m1.g_x=mevent.e_mx;
			mevent.e_m1.g_y=mevent.e_my;
			hndl_mm1(winmenu, mevent.e_mx, mevent.e_my);/*toolbar.c*/
		}
	}
}
