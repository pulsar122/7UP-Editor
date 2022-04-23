/*****************************************************************
	7UP
	Modul: WINDOWS.C
	(c) by TheoSoft '90

	Window Bibliothek
	
	1997-04-08 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-09 (MJK): MSDOS-Teile entfernt
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
	1997-04-16 (MJK): Mehrzahl der W...-Funktionen von (int *) auf
	                  (GRECT *) umgestellt.
	PL02:
	1997-06-20 (MJK): beim "Namesvergleich" links verfolgen

	PL06:
	2000-06-16 (GS) :	Bei wind_get "|0x8000" ausgeklammert da dies in der
										GEMLIB 38gs nicht unterstÅtzt wird.
	2000-06-23 (GS) : Curserbewegung im Fenster nur falls das Fenster
										nicht iconifiziert ist.

	PL08:
	2002-09-28 (GS) : Der Fensterinhalt wird auch bei den PC spezifischen
										Tasten gescrollt ( Bild hoch, Bild runter, Ende ).

*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <tos.h>
#else
#	include <osbind.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <gem.h>
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#ifndef ENGLISH											/* (GS) */
	#include "7UP.h"
#else
	#include "7UP_eng.h"
#endif
#include "undo.h"
#include "7up3.h"
#include "resource.h"
#include "config.h"
#include "toolbar.h"
#include "desktop.h"
#include "vafunc.h"
#include "editor.h"
#include "fbinfo.h"
#include "wfindblk.h"
#include "fontsel.h"
#include "fileio.h"
#include "findrep.h"
#include "wind_.h"
#include "graf_.h"

#include "windows.h"

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

#ifndef _AESrshdr
#	ifdef TCC_GEM
#		define _AESrshdr ((RSHDR *)(*((long *)&_GemParBlk.global[7])))
#	else
#		define _AESrshdr ((RSHDR *)(*((long *)&aes_global[7])))
#	endif
#endif

#define NOWINDOW	 -1
#define MIN_WIDTH  128
#define MIN_HEIGHT 144
#define VERTICAL	1
#define HORIZONTAL 2
#define notnull(a) ((a>0)?(a):(1))
#define HORIZ_OFFSET 5

#define SCREEN			1
#define PLOTTER		 11
#define PRINTER		 21
#define METAFILE		31
#define CAMERA		  41
#define TABLET		  51

#define EXOB_TYPE(x) (x>>8)

static int x_desk,y_desk,w_desk,h_desk;

WINDOW _wind[MAXWINDOWS]=
{
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,0L,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0,
	NOWINDOW,0,0,0,0,0,0,0,0,0L,0L,INSERT+INDENT,0,0,0,0,STRING_LENGTH,0L,0L,0L,0L,8,16,0,0,(int(*)())Wclear,0L,10,1,3,0L,0L,0L,0L,0L,0L,0
};					  /* | | */

int align(register int x, register int n)	  /* Umrechnung einer Koordinate auf die nÑchste  */
{									/* durch n teilbare Stelle */
	x += (n >> 2) - 1;		/* Runden und */
	x = n * (x / n);			/* Rest entfernen */
	return(x);
}

#ifdef TC_GEM
int rc_intersect(register GRECT *g1, register GRECT *g2)
{
  register int tx = max(g1->g_x, g2->g_x);
  register int ty = max(g1->g_y, g2->g_y);
  register int tw = min(g1->g_x + g1->g_w, g2->g_x + g2->g_w);
  register int th = min(g1->g_y + g1->g_h, g2->g_y + g2->g_h);

  g2->g_x = tx;
  g2->g_y = ty;
  g2->g_w = tw - tx;
  g2->g_h = th - ty;

  return ((tw > tx) && (th > ty));
}
#endif

WINDOW *Wcreate(int kind, int x, int y, int w, int h)
{
	register WINDOW *wp=(WINDOW *)0L;
	register int i,handle;
	int xw,yw,ww,hw,wh,ret;
	char pathname[PATH_MAX];

	if(Wcount(CREATED)<(MAXWINDOWS-1))
	{
		wind_get(0,WF_WORKXYWH /*|0x8000*/ ,&x_desk,&y_desk,&w_desk,&h_desk);
		wh=wind_create(kind,x,y,w,h);
		if(wh>0 /* && wh<MAXWINDOWS */ )
		{
			handle=open_work(SCREEN);
			for(i=1; i<MAXWINDOWS; i++)
				if(!(_wind[i].w_state & CREATED)) /* jetzt: wh != arrayindex */
				{
					wp = &_wind[i];
					break;
				}
			wp->wihandle = wh;
			wp->vdihandle= handle;
			for(i=DESKICN1; i<=DESKICN7; i++)
				if(desktop[i].ob_flags & HIDETREE) /* Icon suchen */
				{
					wp->icon=i;
					break;
				}
			if(wp->fontid==1)
			{
#ifdef DEBUG
printf("Set Window font: Id=%2d size=%2dpt\n", wp->fontid, wp->fontsize);
#endif
				wp->fontsize = vst_point(wp->vdihandle,wp->fontsize,&ret,&ret,&wp->wscroll,&wp->hscroll);
				vqt_width(wp->vdihandle, 'W', &wp->wscroll, &ret, &ret);  /* Breite der Zeichen */
#ifdef DEBUG
printf("Got Window font: Id=%2d size=%2dpt\n", wp->fontid, wp->fontsize);
#endif
			}
			wp->kind	  = kind;
			wp->w_state |= CREATED;
			
			if(toolbar_zeigen && rsrc_load(find_7upinf(pathname,"bar",0)))
			{  /* Register fÅr rsrc_free() sichern */
				wp->toolbaraddress=*(long *)_AESrshdr;
/*
				if(toolbar_senkrecht)
		      	rsrc_gaddr(R_TREE,1,&wp->toolbar);
		      else
*/
		      	rsrc_gaddr(R_TREE,0,&wp->toolbar);
		      toolbar_inst(winmenu, wp, wp->toolbar);
			}
			if( ! (wp->work.g_x && wp->work.g_y && wp->work.g_w, wp->work.g_h))
			{/* Fensterarray wurde noch nicht benutzt, erstmalige Einstellung */
			   _wind_get(0,WF_TOP,&wh,&ret,&ret,&ret);
			   if(Wp(wh))
			   {
				   _wind_get(wh,WF_WORKXYWH,&xw,&yw,&ww,&hw);
				   x=xw;
				   y=yw;
				   w=ww;
				   h=hw;
			   }
				_wind_calc(wh,WC_WORK,wp->kind,x,y,w,h,
					&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
   
				wp->work.g_x=align(wp->work.g_x,wp->wscroll);
				wp->work.g_w=align(wp->work.g_w,wp->wscroll);
				wp->work.g_h=align(wp->work.g_h,wp->hscroll);
				wp->work.g_x+=wp->wscroll; /* Fenster nicht ganz groû, wg. Cursor */
				wp->work.g_w-=wp->wscroll;
			}
			else /* im Fensterarray stehen schon Werte drin */
			{
				wp->work.g_x=align(wp->work.g_x,wp->wscroll);
				wp->work.g_w=align(wp->work.g_w,wp->wscroll);
				wp->work.g_h=align(wp->work.g_h,wp->hscroll);
			}
			if(wp->kind & VSLIDE)
				wind_set(wp->wihandle,WF_VSLSIZE,1000,0,0,0);
			if(wp->kind & HSLIDE)
				wind_set(wp->wihandle,WF_HSLSIZE,1000,0,0,0);
		}
	}
	return(wp);
}

int Wopen(register WINDOW *wp)
{
	int x,y,w,h,xi,yi,wi,hi;
	register int i;
/*
	int msgbuf[8];
*/
	if(!wp)
		return(0);
	if(!(wp->w_state & CREATED))
		return(0);
	if(wp->w_state & OPENED)
	{
		Wtop(wp);
		return(1);
	}
/*
/* Reaktion auf AC_CLOSE */
   if(wp->wihandle == -2) /* Es kam ein AC_CLOSE, OPENED-Flag gelîscht */
   {
      wp->wihandle = wind_create(wp->kind,x_desk,y_desk,w_desk,h_desk);
      if(wp->wihandle < 0)
         return(0);
   }
*/
	if((wp->kind & NAME) && wp->name)
		wind_set_str(wp->wihandle,WF_NAME,wp->name);
	if((wp->kind & INFO) && wp->info)
		wind_set_str(wp->wihandle,WF_INFO,wp->info);
	_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
		&x,&y,&w,&h);
	if(y<y_desk) /* Korrektur, wenn jetzt mit Infozeile */
	{
		y=y_desk;
		h-=y_desk;
		_wind_calc(wp->wihandle,WC_WORK,wp->kind,x,y,w,h,
			&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
		wp->work.g_h=align(wp->work.g_h,wp->hscroll);
		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
			&x,&y,&w,&h);
	}
	iconposition(Wh(wp),&xi,&yi,&wi,&hi);
	graf_growbox(xi,yi,wi,hi,x,y,w,h);
	if(wind_open(wp->wihandle,x,y,w,h)>0)
	{
		for(i=1; i<MAXWINDOWS; i++)
			_wind[i].w_state &= ~ONTOP; /* bit zurÅcksetzen */
		wp->w_state |= OPENED;
		wp->w_state |= ONTOP;
		_wind_get(wp->wihandle,WF_WORKXYWH,&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
		Wslupdate(wp,1+2+4+8);
		
		toolbar_adjust(wp);
		
		Wfindblk(wp,&blkwp,&begcut,&endcut);
		AVAccOpenedWindow(wp->wihandle);
		return(1);
	}
	return(0);
}

void Wattrchg(WINDOW *wp, int newkind)
{
	int xb,yb,wb,hb;
	if(wp && newkind!=wp->kind)
	{
		graf_mouse_on(0);
		Wcursor(wp);
		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,&xb,&yb,&wb,&hb);
		if(wp->w_state & OPENED)
			wind_close(wp->wihandle);
		wind_delete(wp->wihandle);
		wp->wihandle=wind_create(newkind,x_desk,y_desk,w_desk,h_desk);
		wp->kind=newkind;
		_wind_calc(wp->wihandle,WC_WORK,newkind,xb,yb,wb,hb,&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
		wp->work.g_h=align(wp->work.g_h,wp->hscroll);
		_wind_calc(wp->wihandle,WC_BORDER,newkind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,&xb,&yb,&wb,&hb);
/* neu *********************************************************************/
		if((wp->kind & NAME))
		{
			wind_set_str(wp->wihandle,WF_NAME,wp->name);
		}
		if((wp->kind & INFO))
			Wnewinfo(wp,"");
		adjust_best_position(wp);
		if(wp->row > wp->work.g_h/wp->hscroll-1)/* Korrektur, wenn Cursor zu tief */
		{
			if(wp->cstr->prev)
			{
				wp->cstr=wp->cstr->prev;
				wp->row--;
			}
		}
		if(wp->w_state & OPENED)
		{
			wind_open(wp->wihandle,xb,yb,wb,hb);
			_wind_get(wp->wihandle,WF_WORKXYWH,&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
			Wsetrcinfo(wp);
			wp->slhpos=wp->slwpos=0;
			Wslupdate(wp,1+2+4+8);
		}
		Wcursor(wp);
		graf_mouse_on(1);
	}
}

char *Wname(WINDOW *wp)
{
#define APPNAMELEN 6 
	if(wp && (wp->kind & NAME))
	{
		if(_AESnumapps != 1)
		{
         return( wp->name[APPNAMELEN] == '*' ? &wp->name[APPNAMELEN+2] : &wp->name[APPNAMELEN]);
		}
		else
		{
         return( *wp->name == '*' ? &wp->name[2] : wp->name);
		}
	}
	return(NULL);
}

void Wnewname(register WINDOW *wp, const char* name)
{
	char newname[PATH_MAX];

	if(wp && (wp->kind & NAME) && name)
	{
		if(_AESnumapps != 1) /* mehr als eine Applikation gleichzeitig */
			strcpy(newname,"[7UP] ");
		else
			newname[0]=0;
		if(wp->w_state & CHANGED)
		{
			strcat(strcat(newname,"* "),name);
		}
		else
		{
			strcat(newname,name);
		}
		if(strcmp(wp->name,newname))
		{
			wind_set_str(wp->wihandle,WF_NAME,strcpy(wp->name,newname));
		}
	}
}

void Wnewinfo(register WINDOW *wp, const char* newinfo)
{
	if(wp && (wp->kind & INFO) && strcmp(wp->info,newinfo))
		wind_set_str(wp->wihandle,WF_INFO,strcpy(wp->info,newinfo));
}

int Wnewfont(WINDOW *wp, int fontid, int fontsize)
{
	if(wp)
	{
		wp->fontid=fontid;
		vst_font(wp->vdihandle,fontid);
		return(Wfontsize(wp,fontsize));
	}
	return(0);
}

int Wfontsize(register WINDOW *wp, int font)
{
	int array[4],oldw,oldh;
	int ret;
	long k;
	if(wp)
	{
		k=wp->hsize/wp->hscroll;
		oldh=wp->hscroll;
		oldw=wp->wscroll;
		wp->fontsize = vst_point(wp->vdihandle,font,&ret,&ret,&wp->wscroll,&wp->hscroll);
		vqt_width(wp->vdihandle, 'W', &wp->wscroll, &ret, &ret);  /* Breite der Zeichen */

		wp->hsize =k*wp->hscroll;
		wp->wsize=STRING_LENGTH*(long)wp->wscroll;
		wp->hfirst=wp->hfirst*wp->hscroll/oldh; /* NICHT a*=x/y; !!! */
		wp->wfirst=wp->wfirst*wp->wscroll/oldw;

		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
			&array[0],&array[1],&array[2],&array[3]);
		graf_mouse_on(0);
		Wcursor(wp);
		Wmovesize(wp,array2grect(array));
		if(wp->w_state & OPENED)
			Wredraw(wp,&wp->work); /* komplettes Redraw */
		Wcursor(wp);
		graf_mouse_on(1);
		return(wp->fontsize);
	}
	return(0);
}

static void _Wicndraw(const register WINDOW *wp, int dir, int clip[])
{
  MFDB	  s, d;
  BITBLK *bitblk;
  int     pxy [8];
  int     index [2];
  
	if(wp)
	{
		bitblk = userimg [WICON_KLEIN].ob_spec.bitblk;
		
		d.fd_addr  = NULL; /* screen */
		s.fd_addr  = (void *)bitblk->bi_pdata;
		s.fd_w = bitblk->bi_wb << 3;
		s.fd_h  = bitblk->bi_hl;
		s.fd_wdwidth = s.fd_w/16;
		s.fd_stand  = 0;
		s.fd_nplanes  = 1;
		
		pxy [0] = 0;
		pxy [1] = 0;
		pxy [2] = s.fd_w - 1;
		pxy [3] = s.fd_h - 1;
		pxy [4] = wp->work.g_x;
		pxy [5] = wp->work.g_y;
		pxy [6] = wp->work.g_x+wp->work.g_w-1;
		pxy [7] = wp->work.g_y+wp->work.g_h-1;
		
		index [0] = BLACK;
		index [1] = WHITE;
		vr_recfl(wp->vdihandle,clip);				/* weiûes rechteck in workspace */
		vrt_cpyfm (wp->vdihandle, MD_REPLACE, pxy, &s, &d, index);	 /* copy it */
	}
}

void Wredraw(register WINDOW *wp, GRECT *rect)
{
  int area[4],clip[4];

  if(!wp)
	  return;
#if MiNT
	wind_update(BEG_UPDATE);
#endif
	_wind_get(wp->wihandle, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
	while( area[2] && area[3] )
	{
		if(rc_intersect(array2grect(&x_desk),array2grect(area)))
		{
			if(rc_intersect(rect,array2grect(area)))
			{
				clip[0]=area[0];
				clip[1]=area[1];
				clip[2]=area[0]+area[2]-1;
				clip[3]=area[1]+area[3]-1;
				vs_clip(wp->vdihandle,1,clip);

		   	if(wp->w_state & ICONIFIED)
			   	_Wicndraw(wp,VERTICAL+HORIZONTAL,clip);
			   else
				   wp->draw(wp,VERTICAL+HORIZONTAL,clip);
			}
		}
		_wind_get(wp->wihandle, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
	}
#if MiNT
	wind_update(END_UPDATE);
#endif
}

void Wtop(register WINDOW *wp)
{
	register int i;

	for(i=1; i<MAXWINDOWS; i++)
	{
		_wind[i].w_state &= ~ONTOP; /* bit zurÅcksetzen */
	}
	if(wp)
	{
		wp->w_state |= ONTOP;
		wp->cspos=wp->col; /* CursormÅll vermeiden */
		wind_set(wp->wihandle,WF_TOP,0,0,0,0);
		Wfindblk(wp,&blkwp,&begcut,&endcut);

		toolbar_adjust(wp);
   }
}

void Wbottom(register WINDOW *wp)
{
	if(wp)
	{
		wind_set(wp->wihandle,WF_BOTTOM,0,0,0,0);
   }
}

void Wfull(register WINDOW *wp)
{
	int nx,ny,nw,nh,ox,oy,ow,oh;
	register long i, oldrow;
	long oldhfirst;

	if(wp)
	{
		_wind_get(wp->wihandle,WF_CURRXYWH,&ox,&oy,&ow,&oh);
		if(wp->w_state & FULLED && wp->w_state & OPENED)
		{
			_wind_get(wp->wihandle,WF_PREVXYWH,&nx,&ny,&nw,&nh);
			graf_shrinkbox(nx,ny,nw,nh,ox,oy,ow,oh);
		}
		else
		{
			_wind_get(0,WF_WORKXYWH,&nx,&ny,&nw,&nh);
			graf_growbox(ox,oy,ow,oh,nx,ny,nw,nh);
		}
		_wind_calc(wp->wihandle,WC_WORK,wp->kind,nx,ny,nw,nh,&wp->work.g_x,&wp->work.g_y,
			&wp->work.g_w,&wp->work.g_h);

		wp->work.g_x=align(wp->work.g_x,wp->wscroll);
		wp->work.g_w=align(wp->work.g_w,wp->wscroll);
		wp->work.g_h=align(wp->work.g_h,wp->hscroll);

		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
			&nx,&ny,&nw,&nh);
		wind_set(wp->wihandle,WF_CURRXYWH,nx,ny,nw,nh);
		_wind_get(wp->wihandle,WF_WORKXYWH,&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
		
		toolbar_adjust(wp);
		
		if(wp->type)		/* windowtype == GRAPHIC ? */
			wp->hfirst=0;
		else
		{
			oldhfirst=wp->hfirst;
			oldrow=wp->row;
			if(wp->hsize < wp->work.g_h) /* wenn text < work.g_h */
				wp->hfirst=0;
			if(wp->hfirst + wp->work.g_h > wp->hsize)
				wp->hfirst=wp->hsize - wp->work.g_h;
			if(wp->hfirst < 0)
				wp->hfirst=0;
			if(wp->col > wp->work.g_w/wp->wscroll-1)
				wp->col=wp->work.g_w/wp->wscroll-1;
			if(wp->row > wp->work.g_h/wp->hscroll-1)
				wp->row=wp->work.g_h/wp->hscroll-1;
			if(wp->row < oldrow)
				for(i=0; i<(oldrow-wp->row); i++)
				{
					 wp->cstr=wp->cstr->prev;
				}
			if(wp->hfirst<oldhfirst)
			{
				for(i=0; i<(oldhfirst-wp->hfirst)/wp->hscroll; i++)
				{
					 wp->wstr=wp->wstr->prev;
				}
				Wredraw(wp,&wp->work);
			}
		}
		Wslupdate(wp,1+2+4+8);
		wp->w_state ^= FULLED;
	}
}

void Wadjust(WINDOW *wp)
{
	GRECT grect;
	int xdesk,ydesk,wdesk,hdesk;

	if(wp)
	{
		_wind_get(0,WF_WORKXYWH,&xdesk,&ydesk,&wdesk,&hdesk);
		_wind_calc(wp->wihandle,WC_WORK,wp->kind,xdesk,ydesk,wdesk,hdesk,
			&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
							  /* anpassen an Umbruch */
		wp->work.g_w=min(wp->umbruch*wp->wscroll,wp->work.g_w);
		wp->work.g_x=align(wp->work.g_x,wp->wscroll);
		wp->work.g_w=align(wp->work.g_w,wp->wscroll);
		wp->work.g_h=align(wp->work.g_h,wp->hscroll);
		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
			&grect.g_x,&grect.g_y,&grect.g_w,&grect.g_h);
		Wmovesize(wp,&grect);
	}
}

void Wmovesize(register WINDOW *wp, GRECT *nxywh)
{
	register long i, oldrow, oldhfirst;

	if(wp /*&& wp->w_state & OPENED*/ && !(wp->w_state & ICONIFIED) ) /* (GS) */
	{
		if(nxywh->g_w<MIN_WIDTH)
			nxywh->g_w=MIN_WIDTH;
		if(nxywh->g_h<MIN_HEIGHT)
			nxywh->g_h=MIN_HEIGHT;
		_wind_calc(wp->wihandle,WC_WORK,wp->kind,nxywh->g_x,nxywh->g_y,nxywh->g_w,nxywh->g_h,
			&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);

		wp->work.g_x=align(wp->work.g_x,wp->wscroll);
		wp->work.g_w=align(wp->work.g_w,wp->wscroll);
		wp->work.g_h=align(wp->work.g_h,wp->hscroll);

		toolbar_adjust(wp);

		if(!(wp->w_state & OPENED))
			return;

		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
			&nxywh->g_x,&nxywh->g_y,&nxywh->g_w,&nxywh->g_h);

		wind_set(wp->wihandle,WF_CURRXYWH,nxywh->g_x,nxywh->g_y,nxywh->g_w,nxywh->g_h);
		_wind_get(wp->wihandle,WF_WORKXYWH,&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
		
		toolbar_adjust(wp);
		
		if(wp->type)		/* windowtype == GRAPHIC ? */
			wp->hfirst=0;
		else
		{
			oldhfirst=wp->hfirst;
			oldrow=wp->row;
			if(wp->hsize < wp->work.g_h) /* wenn text < work.g_h */
				wp->hfirst=0;
			if(wp->hfirst + wp->work.g_h > wp->hsize)
				wp->hfirst=wp->hsize - wp->work.g_h;
			if(wp->hfirst < 0)
				wp->hfirst=0;
			if(wp->col > wp->work.g_w/wp->wscroll-1)
				wp->col=wp->work.g_w/wp->wscroll-1;
			if(wp->row > wp->work.g_h/wp->hscroll-1)
				wp->row=wp->work.g_h/wp->hscroll-1;
			if(wp->row < oldrow)					 /* kleiner */
				for(i=0; i<(oldrow-wp->row); i++)
				{
					 wp->cstr=wp->cstr->prev;
				}
			if(wp->hfirst<oldhfirst)
			{
				for(i=0; i<(oldhfirst-wp->hfirst)/wp->hscroll; i++)
				{
					 wp->wstr=wp->wstr->prev;
				}
				if(wp->w_state & OPENED)
					Wredraw(wp,&wp->work);
			}
			if(wp->row > oldrow)					 /* grîûer */
				for(i=0; i<-(oldrow-wp->row); i++)
				{
					 wp->cstr=wp->cstr->next;
				}
			if(wp->hfirst>oldhfirst)
			{
				for(i=0; i<-(oldhfirst-wp->hfirst)/wp->hscroll; i++)
				{
					 wp->wstr=wp->wstr->next;
				}
				if(wp->w_state & OPENED)
					Wredraw(wp,&wp->work);
			}
		}
		Wslupdate(wp,1+2+4+8);
		wp->w_state &= ~FULLED;
	}
	else
	{
		wind_set(wp->wihandle,WF_CURRXYWH,nxywh->g_x,nxywh->g_y,nxywh->g_w,nxywh->g_h);
		wind_get(wp->wihandle,WF_WORKXYWH,&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h); /* (GS): ohne Toolboxberechnung*/
	}
}

static void _Wtile(void)
{
   typedef struct
   {
      GRECT rect[MAXWINDOWS];
   }TWindpos;

   TWindpos pos;
   int i,k,xd,yd,wd,hd;

   wind_get(0,WF_WORKXYWH /*| 0x8000*/ ,&xd,&yd,&wd,&hd);

   switch(Wcount(OPENED))
   {
      case 1:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd;
         pos.rect[1].g_h = hd;

         break;
      case 2:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd;
         pos.rect[1].g_h = hd/2;

         pos.rect[2].g_x = xd;
         pos.rect[2].g_y = yd+hd/2;
         pos.rect[2].g_w = wd;
         pos.rect[2].g_h = hd/2;

         break;
      case 3:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd;
         pos.rect[1].g_h = hd/3;

         pos.rect[2].g_x = xd;
         pos.rect[2].g_y = yd+hd/3;
         pos.rect[2].g_w = wd;
         pos.rect[2].g_h = hd/3;

         pos.rect[3].g_x = xd;
         pos.rect[3].g_y = yd+2*hd/3;
         pos.rect[3].g_w = wd;
         pos.rect[3].g_h = hd/3;

         break;
      case 4:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd/2;
         pos.rect[1].g_h = hd/2;

         pos.rect[2].g_x = xd;
         pos.rect[2].g_y = yd+hd/2;
         pos.rect[2].g_w = wd/2;
         pos.rect[2].g_h = hd/2;

         pos.rect[3].g_x = xd+wd/2;
         pos.rect[3].g_y = yd;
         pos.rect[3].g_w = wd/2;
         pos.rect[3].g_h = hd/2;

         pos.rect[4].g_x = xd+wd/2;
         pos.rect[4].g_y = yd+hd/2;
         pos.rect[4].g_w = wd/2;
         pos.rect[4].g_h = hd/2;

         break;
      case 5:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd/2;
         pos.rect[1].g_h = hd/2;

         pos.rect[2].g_x = xd;
         pos.rect[2].g_y = yd+hd/2;
         pos.rect[2].g_w = wd/2;
         pos.rect[2].g_h = hd/2;

         pos.rect[3].g_x = xd+wd/2;
         pos.rect[3].g_y = yd;
         pos.rect[3].g_w = wd/2;
         pos.rect[3].g_h = hd/3;

         pos.rect[4].g_x = xd+wd/2;
         pos.rect[4].g_y = yd+hd/3;
         pos.rect[4].g_w = wd/2;
         pos.rect[4].g_h = hd/3;

         pos.rect[5].g_x = xd+wd/2;
         pos.rect[5].g_y = yd+2*hd/3;
         pos.rect[5].g_w = wd/2;
         pos.rect[5].g_h = hd/3;

         break;
      case 6:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd/2;
         pos.rect[1].g_h = hd/3;

         pos.rect[2].g_x = xd;
         pos.rect[2].g_y = yd+hd/3;
         pos.rect[2].g_w = wd/2;
         pos.rect[2].g_h = hd/3;

         pos.rect[3].g_x = xd;
         pos.rect[3].g_y = yd+2*hd/3;
         pos.rect[3].g_w = wd/2;
         pos.rect[3].g_h = hd/3;

         pos.rect[4].g_x = xd+wd/2;
         pos.rect[4].g_y = yd;
         pos.rect[4].g_w = wd/2;
         pos.rect[4].g_h = hd/3;

         pos.rect[5].g_x = xd+wd/2;
         pos.rect[5].g_y = yd+hd/3;
         pos.rect[5].g_w = wd/2;
         pos.rect[5].g_h = hd/3;

         pos.rect[6].g_x = xd+wd/2;
         pos.rect[6].g_y = yd+2*hd/3;
         pos.rect[6].g_w = wd/2;
         pos.rect[6].g_h = hd/3;

         break;
      case 7:
         pos.rect[1].g_x = xd;
         pos.rect[1].g_y = yd;
         pos.rect[1].g_w = wd/2;
         pos.rect[1].g_h = hd/3;

         pos.rect[2].g_x = xd;
         pos.rect[2].g_y = yd+hd/3;
         pos.rect[2].g_w = wd/2;
         pos.rect[2].g_h = hd/3;

         pos.rect[3].g_x = xd;
         pos.rect[3].g_y = yd+2*hd/3;
         pos.rect[3].g_w = wd/2;
         pos.rect[3].g_h = hd/3;

         pos.rect[4].g_x = xd+wd/2;
         pos.rect[4].g_y = yd;
         pos.rect[4].g_w = wd/2;
         pos.rect[4].g_h = hd/4;

         pos.rect[5].g_x = xd+wd/2;
         pos.rect[5].g_y = yd+hd/4;
         pos.rect[5].g_w = wd/2;
         pos.rect[5].g_h = hd/4;

         pos.rect[6].g_x = xd+wd/2;
         pos.rect[6].g_y = yd+2*hd/4;
         pos.rect[6].g_w = wd/2;
         pos.rect[6].g_h = hd/4;

         pos.rect[7].g_x = xd+wd/2;
         pos.rect[7].g_y = yd+3*hd/4;
         pos.rect[7].g_w = wd/2;
         pos.rect[7].g_h = hd/4;

         break;
   }
   for(i=1, k=1; i<MAXWINDOWS; i++)
      if(_wind[i].w_state & CREATED && _wind[i].w_state & OPENED)
         if(pos.rect[k].g_w && pos.rect[k].g_h)
            Wmovesize(&_wind[i],&pos.rect[k++]);
}

void Warrange(int how)
{
	GRECT grect;
	int i,k,count,xstep,ystep,diff;
/*
	int msgbuf[8];
*/	
	if(how==1)
	{
		_Wtile();
		return;
	}

	count = Wcount(OPENED);
	if(count>1)
	{
		xstep = w_desk/count;
		diff=(count-1)*xstep+MIN_WIDTH - w_desk;
		if(diff>0)
			xstep-=diff/count;

		ystep = h_desk/count;
		diff=(count-1)*ystep+MIN_HEIGHT - h_desk;
		if(diff>0)
			ystep-=diff/count;

		for(i=1, k=0; i<MAXWINDOWS; i++)
			if((_wind[i].w_state & CREATED) && (_wind[i].w_state & OPENED))
			{
				switch(how)
				{
					case 1: /* untereinander */
						grect.g_x = x_desk;
						grect.g_y = k * ystep + y_desk;
						grect.g_w = w_desk;
						grect.g_h = ystep;
						break;
					case 2: /* nebeneinander */
						grect.g_x = k * xstep + x_desk;
						grect.g_y = y_desk;
						grect.g_w = xstep;
						grect.g_h = h_desk;
						break;
					case 3: /* Åberlappend */
						grect.g_x = k * y_desk + x_desk;
						grect.g_y = k * y_desk + y_desk;
						grect.g_w = w_desk-(count-1)*y_desk;
						grect.g_h = h_desk-(count-1)*y_desk;
						break;
				}
				Wmovesize(&_wind[i],&grect);
				Wtop(&_wind[i]);
				k++;
			}
	}
	else
		Wfull(Wgettop());
}


/* 11.9.1993 */
void Wiconify(WINDOW *wp, int nxywh[])
{
	if(wp)
	{
/*
      Wmovesize(wp, nxywh);
*/
		graf_shrinkbox(nxywh[0],nxywh[1],nxywh[2],nxywh[3],
		          wp->work.g_x, wp->work.g_y,
                wp->work.g_w, wp->work.g_h);
      wind_set(wp->wihandle, 
               WF_ICONIFY,
               nxywh[0],nxywh[1],nxywh[2],nxywh[3]);
		
      wind_get(wp->wihandle, WF_WORKXYWH,
		         &wp->work.g_x, &wp->work.g_y,
               &wp->work.g_w, &wp->work.g_h);

		graf_mouse_on(0);
		Wcursor(wp);
		Wcuroff(wp);
		graf_mouse_on(1);
		wp->w_state |= ICONIFIED;
	}
}

void Wuniconify(WINDOW *wp, int nxywh[])
{
	if(wp)
	{
/*
      Wmovesize(wp, nxywh);
*/
		graf_growbox(wp->work.g_x, wp->work.g_y,
                   wp->work.g_w, wp->work.g_h,
                   nxywh[0],nxywh[1],nxywh[2],nxywh[3]);

      wind_set(wp->wihandle, 
               WF_UNICONIFY,
               nxywh[0],nxywh[1],nxywh[2],nxywh[3]);

      _wind_get(wp->wihandle, WF_WORKXYWH,
		         &wp->work.g_x, &wp->work.g_y,
               &wp->work.g_w, &wp->work.g_h);

		graf_mouse_on(0);
		Wcuron(wp);
		Wcursor(wp);
		graf_mouse_on(1);
		wp->w_state &= ~ICONIFIED;
	}
}

void Wcycle(register WINDOW *wp)
{
	register int i,k;

	if(wp)
	{
		for(i=1,k=1; i<MAXWINDOWS; i++,k++)
			if(_wind[i].wihandle==wp->wihandle)
				break;
		for(++i; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & OPENED)
			{
				Wtop(&_wind[i]);
				return;
			}
		for(i=1; i<k; i++)
			if(_wind[i].w_state & OPENED)
			{
				Wtop(&_wind[i]);
				return;
			}
	}
}

int Wcount(int state)
{
	int i,count=0;
	for(i=1; i<MAXWINDOWS; i++)
		if(_wind[i].w_state & state)
			count++;
	return(count);
}

static void _Wscroll(register WINDOW *wp, int dir, int delta, GRECT *rect)
{
  register int x, y, w, h;
  MFDB s, d;
  register int pxyarray[8];

  x = rect->g_x; /* Arbeitsbereich */
  y = rect->g_y;
  w = rect->g_w;
  h = rect->g_h;

  s.fd_addr = 0x0L;
  d.fd_addr = 0x0L;

  pxyarray [0] = x;
  pxyarray [1] = y;
  pxyarray [2] = x+w-1;
  pxyarray [3] = y+h-1;
  vs_clip(wp->vdihandle,1,pxyarray);

  if (dir & VERTICAL)										 /* Vertikales Scrolling */
  {
	 if(h>abs(delta) && h>wp->hscroll)										/* Bereich kleiner delta */
	 {
		if (delta > 0)											 /* AufwÑrts Scrolling */
		{
		  pxyarray [0] = x;									 /* Werte fÅr vro_cpyfm */
		  pxyarray [1] = y + delta;
		  pxyarray [2] = x + w - 1;
		  pxyarray [3] = y + h - 1;
		  pxyarray [4] = x;
		  pxyarray [5] = y;
		  pxyarray [6] = pxyarray [2];
		  pxyarray [7] = pxyarray [3] - delta;

		  y = y + h - delta;			  /* Unterer Bereich nicht gescrollt,... */
		  h = delta;								  /* ...muû neu gezeichnet werden */
		}												/* Es muû mehr gezeichnet werden */
		else															/* AbwÑrts Scrolling */
		{
		  pxyarray [0] = x;									 /* Werte fÅr vro_cpyfm */
		  pxyarray [1] = y;
		  pxyarray [2] = x + w - 1;
		  pxyarray [3] = y + h + delta - 1;
		  pxyarray [4] = x;
		  pxyarray [5] = y - delta;
		  pxyarray [6] = pxyarray [2];
		  pxyarray [7] = pxyarray [3] - delta;

		  h = -delta;							/* Oberen Bereich noch neu zeichnen */
		}
		vro_cpyfm(wp->vdihandle, 3, pxyarray, &s, &d);/* Eigentliches Scrolling */
		pxyarray[0] = x;								 /* neuzuzeichnendes Rechteck */
		pxyarray[1] = y;
		pxyarray[2] = x+w-1;
		pxyarray[3] = y+h-1;
	 }
  }
  else														  /* Horizontales Scrolling */
  {
	 if(w>abs(delta) && w>wp->wscroll)										/* Bereich kleiner delta */
	 {
		if (delta > 0)												 /* Links Scrolling */
		{
		  pxyarray [0] = x + delta;						  /* Werte fÅr vro_cpyfm */
		  pxyarray [1] = y;
		  pxyarray [2] = x + w - 1;
		  pxyarray [3] = y + h - 1;
		  pxyarray [4] = x;
		  pxyarray [5] = y;
		  pxyarray [6] = pxyarray [2] - delta;
		  pxyarray [7] = pxyarray [3];

		  x = x + w - delta;			  /* Rechter Bereich nicht gescrollt,... */
		  w = delta;								  /* ...muû neu gezeichnet werden */
		}												/* Es muû mehr gezeichnet werden */
		else															 /* Rechts Scrolling */
		{
		  pxyarray [0] = x;									 /* Werte fÅr vro_cpyfm */
		  pxyarray [1] = y;
		  pxyarray [2] = x + w + delta - 1;
		  pxyarray [3] = y + h - 1;
		  pxyarray [4] = x - delta;
		  pxyarray [5] = y;
		  pxyarray [6] = pxyarray [2] - delta;
		  pxyarray [7] = pxyarray [3];

		  w = -delta;							/* Linken Bereich noch neu zeichnen */
		}
		vro_cpyfm(wp->vdihandle, 3, pxyarray, &s, &d);/* Eigentliches Scrolling */
		pxyarray[0] = x;								 /* neuzuzeichnendes Rechteck */
		pxyarray[1] = y;
		pxyarray[2] = x+w-1;
		pxyarray[3] = y+h-1;
	 }
  }
  vs_clip(wp->vdihandle,1,pxyarray);
  wp->draw(wp,dir,pxyarray);
  return;
}

void Wscroll(register WINDOW *wp, int dir, int delta, GRECT *rect)
{
	int area[4];
#if MiNT
	wind_update(BEG_UPDATE);
#endif
	_wind_get(wp->wihandle, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
	while( area[2] && area[3] )
	{
		if(rc_intersect(array2grect(&x_desk),array2grect(area)))
		{
			if(rc_intersect(rect,array2grect(area)))
			{
				_Wscroll(wp,dir,delta,array2grect(area));
			}
		}
		_wind_get(wp->wihandle, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
	}
#if MiNT
	wind_update(END_UPDATE);
#endif
}

void Wslide(register WINDOW *wp, int newslide, int which)
{
	register long newpos,i,delta;
/*
	int area[4];
*/
	if(wp->kind & VSLIDE & which)
	{
		newpos=(long)newslide*(wp->hsize-wp->work.g_h)/1000L;
		newpos-=(newpos%wp->hscroll);
		delta=newpos-wp->hfirst;
		if(delta)
		{
			wp->hfirst=newpos;
			if(delta > 0)
			{
				for(i=0; i<delta/wp->hscroll; i++)
				{
					wp->wstr=wp->wstr->next;
					wp->cstr=wp->cstr->next;
				}
			}
			else
			{
				for(i=0; i<(-delta/wp->hscroll); i++)
				{
					wp->wstr=wp->wstr->prev;
					wp->cstr=wp->cstr->prev;
				}
			}
			if(labs(delta) >= wp->work.g_h)
			{
				Wredraw(wp, &wp->work);	 /* Bereich ganz neu zeichnen */
			}
			else
			{
				Wscroll(wp,VERTICAL,(int)delta,&wp->work);
			}
		}
	}
	if(wp->kind & HSLIDE & which)
	{
		newpos=(long)newslide*(wp->wsize-wp->work.g_w)/1000L;
		newpos-=(newpos%wp->wscroll);
		delta=newpos-wp->wfirst;
		if(delta)
		{
			wp->wfirst=newpos;				 /*-newpos%wp->wscroll;*/
			if(labs(delta) >= wp->work.g_w)
			{
				Wredraw(wp, &wp->work);	 /* Bereich ganz neu zeichnen */
			}
			else
			{
				Wscroll(wp,HORIZONTAL,(int)delta,&wp->work);
			}
		}
	}
	Wslupdate(wp,1+2+4+8);
}

int Warrow(register WINDOW *wp, int arrow)
{
	register long newpos,oldpos,i,delta=0;
/*
	int area[4];
*/
	if(arrow<=WA_DNLINE)
	{
		oldpos=newpos=wp->hfirst;
		switch(arrow)
		{
			case WA_UPPAGE /*0*/:
				if((newpos-=wp->work.g_h) < 0)
					newpos=0;
				break;
			case WA_DNPAGE /*1*/:
				if((newpos+=wp->work.g_h) > (wp->hsize-wp->work.g_h))
					newpos=wp->hsize-wp->work.g_h;
				break;
			case WA_UPLINE /*2*/:
				if(wp->hsize > wp->work.g_h)
				{
					if((newpos-=wp->hscroll) < 0)
						newpos=0;
					if(++wp->row > wp->work.g_h/wp->hscroll-1)
						wp->row=wp->work.g_h/wp->hscroll-1;
				}
				break;
			case WA_DNLINE /*3*/:
				if(wp->hsize > wp->work.g_h)
				{
					if((newpos+=wp->hscroll) > (wp->hsize-wp->work.g_h))
						newpos=wp->hsize-wp->work.g_h;
					if(--wp->row < 0)
						wp->row=0;
				}
				break;
		}
		wp->hfirst=newpos;
		delta=newpos-oldpos;
		if(delta)
		{
			if(delta > 0)
			{
				for(i=0; i<delta/wp->hscroll; i++)
				{
					wp->wstr=wp->wstr->next;
				}
			}
			else
			{
				for(i=0; i<(-delta/wp->hscroll); i++)
				{
					wp->wstr=wp->wstr->prev;
				}
			}
			wp->cstr=wp->wstr;
			for(i=0; i<wp->row; i++)
			{
				wp->cstr=wp->cstr->next;
			}
			if(labs(delta) >= wp->work.g_h)
			{
				Wredraw(wp, &wp->work);	 /* Bereich ganz neu zeichnen */
			}
			else
			{
				Wscroll(wp,VERTICAL,(int)delta,&wp->work);
			}
			return((int)delta);
		}
	}
	else
	{
		oldpos=newpos=wp->wfirst;
		switch(arrow)
		{
			case WA_LFPAGE /*4*/:
				if((newpos-=wp->work.g_w) < 0)
					newpos=0;
				break;
			case WA_RTPAGE /*5*/:
				if((newpos+=wp->work.g_w) > (wp->wsize-wp->work.g_w))
					newpos=wp->wsize-wp->work.g_w;
				break;
			case WA_LFLINE /*6*/:
				if(wp->wsize > wp->work.g_w)
					if((newpos-=(HORIZ_OFFSET*wp->wscroll)) < 0)
						newpos=0;
					else
						wp->cspos=(wp->col+=HORIZ_OFFSET);
				break;
			case WA_RTLINE /*7*/:
				if(wp->wsize > wp->work.g_w)
					if((newpos+=(HORIZ_OFFSET*wp->wscroll)) > (wp->wsize-wp->work.g_w))
						newpos=wp->wsize-wp->work.g_w;
					else
						wp->cspos=(wp->col-=HORIZ_OFFSET);
				break;
		}
		wp->wfirst=newpos;
		delta=newpos-oldpos;
		if(delta)
		{
			if(labs(delta) >= wp->work.g_w)
			{
				Wredraw(wp, &wp->work);	 /* Bereich ganz neu zeichnen */
			}
			else
			{
				Wscroll(wp,HORIZONTAL,(int)delta,&wp->work);
			}
			return((int)delta);
		}
	}
	return(0);
}
/*
int _Warrow(register WINDOW *wp, int mesag[])
{
   register int scrollx, scrolly, speed, linesperpage, columnsperpage;
   if(wp)
   {
      if((mesag[ 5] < 0) || (mesag[ 7] < 0))
      {
	      linesperpage=wp->work.g_h/wp->hscroll;
	      columnsperpage=wp->work.g_w/wp->wscroll;
	      scrollx = scrolly = 0;
	      speed = (mesag[ 5] < 0) ? -mesag[ 5] : 1;
	      switch (mesag[ 4]) 
	      {
					case WA_UPLINE: scrolly = -speed; break;
					case WA_DNLINE: scrolly = speed; break;
					case WA_LFLINE: scrollx = -speed; break;
					case WA_RTLINE: scrollx = speed; break;
					case WA_UPPAGE: scrolly = -(speed * linesperpage); break;
					case WA_DNPAGE: scrolly = (speed * linesperpage); break;
					case WA_LFPAGE: scrollx = -(speed * columnsperpage); break;
					case WA_RTPAGE: scrollx = (speed * columnsperpage); break;
	      }
	      if (mesag[ 7] < 0) 
	      {
	         speed = -mesag[ 7];
	         switch (mesag[ 6]) 
	         {
	            case WA_UPLINE: scrolly = scrolly - speed; break;
	            case WA_DNLINE: scrolly = scrolly + speed; break;
	            case WA_LFLINE: scrollx = scrollx - speed; break;
	            case WA_RTLINE: scrollx = scrollx + speed; break;
	            case WA_UPPAGE: scrolly = scrolly - (speed * linesperpage); break;
	            case WA_DNPAGE: scrolly = scrolly + (speed * linesperpage); break;
	            case WA_LFPAGE: scrollx = scrollx - (speed * columnsperpage); break;
	            case WA_RTPAGE: scrollx = scrollx + (speed * columnsperpage); break;
	         }
	      }
/*
	      if (scrollx != 0) 
            Wscroll(wp, HORIZONTAL, scrollx * wp->wscroll, &wp->work.g_x)
	      if (scrolly != 0)
            Wscroll(wp, VERTICAL, scrolly * wp->hscroll, &wp->work.g_x)
*/
	      if (scrollx != 0) 
            Warrow(wp, msgbuf, scrollx);
	      if (scrolly != 0)
            Warrow(wp, msgbuf, scrolly);
		}
		else
			Warrow(wp, mesag[ 4]);
   }
}
*/
void Wslupdate(register WINDOW *wp, int what)
{
	register long newpos;
	int slider,ret;

	if(wp)
	{
		if((wp->kind & HSLIDE) && (what & 4))
		{
			_wind_get(wp->wihandle,WF_HSLSIZE,&slider,&ret,&ret,&ret);
			newpos=1000L*wp->work.g_w/notnull(wp->wsize);
			if((slider == 1000 && newpos < slider) ||
				(slider <  1000 && newpos != slider))
				wind_set(wp->wihandle,WF_HSLSIZE,(int)min(1000,newpos),0,0,0);
		}
		if((wp->kind & VSLIDE) && (what & 8))
		{
			_wind_get(wp->wihandle,WF_VSLSIZE,&slider,&ret,&ret,&ret);
			newpos=1000L*wp->work.g_h/notnull(wp->hsize);
			if((slider == 1000 && newpos < slider) ||
				(slider <  1000 && newpos != slider))
				wind_set(wp->wihandle,WF_VSLSIZE,(int)min(1000,newpos),0,0,0);
		}
		if((wp->kind & HSLIDE) && (what & 1))
		{
			if((newpos=(int)((1000L*wp->wfirst)/notnull(wp->wsize-wp->work.g_w)))!=wp->slwpos)
				wind_set(wp->wihandle,WF_HSLIDE,wp->slwpos=(int)newpos,0,0,0);
		}
		if((wp->kind & VSLIDE) && (what & 2))
		{
			if((newpos=(int)((1000L*wp->hfirst)/notnull(wp->hsize-wp->work.g_h)))!=wp->slhpos)
				wind_set(wp->wihandle,WF_VSLIDE,wp->slhpos=(int)newpos,0,0,0);
		}
	}
}

void Wclose(register WINDOW *wp)
{
	int x,y,w,h,xi,yi,wi,hi;
	int xy[4];										/* (GS) */

	if(wp && wp->w_state & OPENED)
	{
		if(wp->w_state & ICONIFIED)		/* (GS), damit GRECT work wieder korrekt ist */
		{
			_wind_get(wp->wihandle,WF_UNICONIFY,&xy[0],&xy[1],&xy[2],&xy[3]);
			Wuniconify(wp, &xy[0]);
		}
		_wind_get(wp->wihandle,WF_CURRXYWH,&x,&y,&w,&h);
		wind_close(wp->wihandle);
		iconposition(Wh(wp),&xi,&yi,&wi,&hi);
		graf_shrinkbox(xi,yi,wi,hi,x,y,w,h);
		wp->w_state &= ~OPENED;
		wp->w_state &= ~ONTOP;
		AVAccClosedWindow(wp->wihandle);
		if((wp=Wgettop()) != NULL)
		{
			Wfindblk(wp,&blkwp,&begcut,&endcut);
		}
	}
}

void Wopenall(void)
{
	register int i;

	for(i=1; i<MAXWINDOWS; i++)
		if(_wind[i].w_state & CREATED && !(_wind[i].w_state & OPENED))
			Wopen(&_wind[i]);
}

void Wcloseall(void)
{
	register int i;

	for(i=1; i<MAXWINDOWS; i++)
		if(_wind[i].w_state & OPENED)
			Wclose(&_wind[i]);
}

void Wreset(WINDOW *wp)
{
	if(wp)
	{
		wp->wihandle  = NOWINDOW;
		wp->vdihandle = 0;
		wp->icon		= 0;
		wp->kind		= 0;
		wp->w_state	&= ~CREATED;
		wp->w_state	&= ~OPENED;
		wp->w_state	&= ~FULLED;
		wp->w_state	&= ~ONTOP;
		wp->w_state	&= ~CURSOR;
		wp->w_state	&= ~CHANGED;
		wp->w_state	&= ~CURSON;
		wp->w_state	&= ~GEMFONTS;
		wp->w_state	&= ~PROPFONT;
		wp->w_state	&= ~CBLINK;
		wp->type		= 0;
		wp->hfirst/=wp->hscroll;	 /* in Zeilenzahl umwandeln */
		wp->wfirst/=wp->wscroll;
		wp->hsize	  = 0;
		wp->wsize	  = 0;
		wp->slhpos	 = 0;
		wp->slwpos	 = 0;
		wp->draw		= (int(*)())Wclear;
		wp->outspec	= 0L;
		wp->fstr=wp->wstr=wp->cstr=0L;
		wp->toolbar=NULL;
		wp->toolbaraddress=0L;
		wp->tabbar =NULL;
		wp->tos_domain = 0;
	}
}

void Wdelete(register WINDOW *wp)
{
	int obj;
	OBJECT *ob;

	if(wp)
	{
		if(wp->w_state & OPENED)
			Wclose(wp);
		if(wp->w_state & CREATED)
		{
			free(wp->name);
			wp->name=NULL;
			free(wp->info);
			wp->info=NULL;
			wind_delete(wp->wihandle);
			close_work(wp->vdihandle,SCREEN);
			if(wp->toolbar && wp->toolbaraddress)
			{	
				obj = 0;
			   do 
			   {
					ob = &wp->toolbar[++obj];
					if(ob->ob_type==G_USERDEF)
						free((char *)((TEDINFO *)ob->ob_spec.userblk->ub_parm)->te_ptext);
				   if(EXOB_TYPE(ob->ob_type)==0xFF)
						free(ob->ob_spec.tedinfo->te_ptext);
				} 
				while (! (ob->ob_flags & LASTOB));

				/* Register fÅr rsrc_free() setzen */
				_AESrshdr=(RSHDR *)wp->toolbaraddress;
				rsrc_free();
			}
			Wreset(wp);
		}
	}
}

void Wnew(void) /* neu: MT 22.9.94 */
{
	register int i;
	WINDOW *wp;
	int xy[4];										/* (GS) */

	if(_AESversion>=0x0140)
	{
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & CREATED)
			{
				wp=Wp(_wind[i].wihandle);
				Wfree(wp);
				if(wp->w_state&OPENED)
					AVAccClosedWindow(wp->wihandle);
				
				if(wp->toolbar)
				{	
				  /* Register fÅr rsrc_free() setzen */
					_AESrshdr=(RSHDR *)wp->toolbaraddress;
					rsrc_free();
				}
				if(wp->w_state & ICONIFIED)		/* (GS), damit GRECT work wieder korrekt ist */
				{
					_wind_get(wp->wihandle,WF_UNICONIFY,&xy[0],&xy[1],&xy[2],&xy[3]);
					Wuniconify(wp, &xy[0]);
				}

				if(_AESnumapps == 1) /* Singletasking */
					wind_delete(wp->wihandle);
				close_work(wp->vdihandle,SCREEN);
				Wreset(wp);
			}
		if(_AESnumapps != 1) /* Multitasking */
			wind_new();
	}
	else
	{
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & CREATED)
			{
				Wfree(Wp(_wind[i].wihandle));
				Wdelete(Wp(_wind[i].wihandle));
			}
	 }
}
/*
void Wnew(void)
{
	register int i;
	int wh;
	WINDOW *wp;
	if(_AESversion>=0x0140)
	{
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & CREATED)
			{
				wp=Wp(_wind[i].wihandle);
				Wfree(wp);
				if(wp->w_state&OPENED)
					AVAccClosedWindow(wp->wihandle);
				close_work(wp->vdihandle,SCREEN);
				Wreset(wp);
			}
   	if( ! (get_cookie('MiNT') && 
   	   (_AESversion>=0x0400) && 
   	   (_AESnumapps != 1)))
			wind_update(BEG_UPDATE); /* wg. wind_new() */
		wind_new();
	}
	else
	{
		for(i=1; i<MAXWINDOWS; i++)
			if(_wind[i].w_state & CREATED)
			{
				Wfree(Wp(_wind[i].wihandle));
				Wdelete(Wp(_wind[i].wihandle));
			}
	 }
}
*/

int Wsetscreen(WINDOW *wp)
{
	register long i,k;
	LINESTRUCT *help;
	int x,y,w,h;

	if(wp)
	{
		if(wp->fontid==1)					/* wenn Standardfont */
			Wnewfont(wp,1,wp->fontsize); /* letzte Fontgrîûe */
		else									 /* sonst Fonts laden */
		{
			graf_mouse(BUSY_BEE,0L);
			vq_extnd(wp->vdihandle,0,work_out);
			if(vq_gdos())
			{
				additional=vst_load_fonts(wp->vdihandle,0)+work_out[10];
				wp->w_state|=GEMFONTS;
			}
			else
				additional=work_out[10]; /* Nur 6x6 system font */
			graf_mouse(ARROW,0L);

			if(additional==work_out[10])	 /* Fehler, kein Fontladen mîglich */
			{
				Wnewfont(wp,1,norm_point);  /* Standartfont bei Fehler */
				wp->w_state&=~PROPFONT;
			}
			else
			{
				if(wp->fontid==vst_font(wp->vdihandle,wp->fontid)) /* falls in ASSIGN.SYS etwas geÑndert wurde */
					Wnewfont(wp,wp->fontid,wp->fontsize); /* letzter Font und Grîûe */
				else
					Wnewfont(wp,1,norm_point);  /* Standartfont bei Fehler */
			}
		}
		_wind_calc(wp->wihandle,WC_BORDER,wp->kind,wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
			&x,&y,&w,&h);
																	/*MT 12.9.94 '<=' statt '<' */
		if(x<(x_desk+w_desk-1) && y<(y_desk+h_desk-1) && w<=w_desk && h<=h_desk)
		{
			help=wp->fstr;
			k=wp->hfirst;
			wp->hfirst*=wp->hscroll;		  /* erweitern auf pixelformat */
			wp->wfirst*=wp->wscroll;
			for(i=0; i<k && help; i++, help=help->next)
				;
			if(help)
			{
				wp->wstr=help;
				k=wp->row;
				for(i=0;
					 i<k && i<(wp->work.g_h/wp->hscroll-1) && help;
					 i++,help=help->next)
					;
				if(help)
				{
					wp->cstr=help;

					for(/*i*/;
						 i<k && wp->wstr->next && wp->cstr->next;
						 i++)
					{
						wp->wstr=wp->wstr->next;
						wp->cstr=wp->cstr->next;
						wp->hfirst+=wp->hscroll;
					}
					adjust_best_position(wp);
					wp->col=wp->cspos;
					return(1);
				}
			}
		}
		_wind_calc(wp->wihandle,WC_WORK,wp->kind,x_desk,y_desk,w_desk,h_desk,
			&wp->work.g_x,&wp->work.g_y,&wp->work.g_w,&wp->work.g_h);
		wp->work.g_x=align(wp->work.g_x,wp->wscroll);
		wp->work.g_w=align(wp->work.g_w,wp->wscroll);
		wp->work.g_h=align(wp->work.g_h,wp->hscroll);
		wp->work.g_x+=wp->wscroll; /* Fenster nicht ganz groû, wg. Cursor */
		wp->work.g_w-=wp->wscroll;
		wp->cstr=wp->wstr=wp->fstr;
		wp->hfirst=wp->wfirst=0;
		wp->cspos=wp->col=wp->row=0;
	}
	return(0);
}

int Wblksize(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end, long *lines, long *chars)
{
	register LINESTRUCT *line;
	if(wp && wp->fstr)
	{
		(*lines)=(*chars)=0;
		line=beg;
		do
		{
			(*lines)++;
			(*chars)+=min(line->used,line->endcol)-line->begcol;
			line=line->next;
		}
		while(line != end->next);
		return(1);
	}
	return(0);
}

int Wtxtsize(WINDOW *wp, long *lines, long *chars)
{
	register LINESTRUCT *line;
	if(wp && wp->fstr)
	{
		(*lines)=(*chars)=0;
		line=wp->fstr;
		do
		{
			(*lines)++;
			(*chars)+=line->used+2;
			line=line->next;
		}
		while(line);
		(*chars)-=2;
		return(1);
	}
	return(0);
}

WINDOW *Wgetwp(char *filename)
{
	register int i;
	for(i=1; i<MAXWINDOWS; i++)
		if((_wind[i].w_state & CREATED) &&
		   samefile(Wname(&_wind[i]),filename)) /* 1997-06-20 (MJK) */
			return(&_wind[i]);
	return((WINDOW *)0L);
}

WINDOW *Wp(int wh)
{
	register int i;
	for(i=1; i<MAXWINDOWS; i++)
		if((_wind[i].w_state&CREATED) && (wh==_wind[i].wihandle))
			return(&_wind[i]);
	return((WINDOW *)0L);
}

int Windex(WINDOW *wp)
{
	register int i;
	if(wp)
	{
		for(i=1; i<MAXWINDOWS; i++)
			if((wp->w_state&CREATED) && (wp->wihandle==_wind[i].wihandle))
				return(i);
	}
	return(0);
}

WINDOW *Wmentry(int mentry)
{
	register int i;
	for(i=1; i<MAXWINDOWS; i++)
		if((_wind[i].w_state&CREATED) && (mentry==_wind[i].mentry))
			return(&_wind[i]);
	return((WINDOW *)0L);
}

WINDOW *Wicon(int icon)
{
	register int i;
	for(i=1; i<MAXWINDOWS; i++)
		if((_wind[i].w_state&CREATED) && (icon==_wind[i].icon))
			return(&_wind[i]);
	return((WINDOW *)0L);
}

int Wh(register WINDOW *wp)
{
	if(wp)
		return(wp->wihandle);
	else
		return(0);
}

WINDOW *Wgettop(void)
{
	WINDOW *wp=NULL;
	int wh,ret;

	_wind_get(0,WF_TOP,&wh,&ret,&ret,&ret);
	if((wp=Wp(wh)) != NULL )
		wp->w_state|=ONTOP;
	return(wp);
}

void Wclear(register WINDOW *wp)
{
	register int pxyarray[4];
	if(wp && wp->w_state & OPENED)
	{
		graf_mouse_on(0);
		vswr_mode(wp->vdihandle,1);		 /* Åberschreiben */
		vsf_interior(wp->vdihandle,1);	 /* muster */
		vsf_color(wp->vdihandle,0);		 /* farbe weiû */
		pxyarray[0]=wp->work.g_x;
		pxyarray[1]=wp->work.g_y;
		pxyarray[2]=wp->work.g_x + wp->work.g_w - 1;
		pxyarray[3]=wp->work.g_y + wp->work.g_h - 1;
		vr_recfl(wp->vdihandle,pxyarray); /* weiûes rechteck in workspace */
		graf_mouse_on(1);
	}
}

#define C_Y 2
#define C_W 2
#define C_H 4
/*
#define C_Y 3
#define C_W 1
#define C_H 6
*/
void Wcursor(register WINDOW *wp)
{
	register int pxyarray[4/*26*/],area[4],clip[4];
	register LINESTRUCT *line;

	if(!wp)
		return;
	if(!(wp->w_state & OPENED))
		return;

	wp->row=0;
	for(line = wp->wstr; line && line != wp->cstr; line = line->next)
		wp->row++;

	wp->col=wp->cspos; /* festen Cursor gewÑhrleisten */
	if(!(wp->col + wp->wfirst/wp->wscroll < wp->cstr->used))
		wp->col = (int)(wp->cstr->used-wp->wfirst/wp->wscroll); /* max. an ende des strings */
	if(!(wp->w_state & CURSON))
		return;
	if(!(wp->w_state & CURSOR))
		return;
	if(wp->w_state&INSERT) /* INSERT */
	{
/*
		pxyarray[0]=max(wp->work.g_x,wp->work.g_x+wp->col*wp->wscroll - 1); /* Strichcursor */
*/
		pxyarray[0]=wp->work.g_x+wp->col*wp->wscroll; /* Strichcursor */
		pxyarray[1]=wp->work.g_y+wp->row*wp->hscroll - C_Y;
		pxyarray[2]=C_W;
		pxyarray[3]=wp->hscroll + C_H;
		wp->w_state |= CBLINK;
	}
	else		/* OVERWRITE */
	{
		pxyarray[0]=wp->work.g_x+wp->col*wp->wscroll; /* Blockcursor */
		pxyarray[1]=wp->work.g_y+wp->row*wp->hscroll;
		pxyarray[2]=wp->wscroll;
		pxyarray[3]=wp->hscroll;
		wp->w_state &= ~CBLINK;
	}
#if MiNT
	wind_update(BEG_UPDATE);
#endif
	_wind_get(wp->wihandle, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
	while( area[2] && area[3] )
	{
		if(rc_intersect(array2grect(&x_desk),array2grect(area)))
		{
			if(rc_intersect(&wp->work,array2grect(area))) /*4.6.94 wg. Toolbar*/
			{
				if(rc_intersect(array2grect(pxyarray),array2grect(area)))
				{
					clip[0]=area[0];
					clip[1]=area[1];
					clip[2]=area[0]+area[2]-1;
					clip[3]=area[1]+area[3]-1;

/* ÅberflÅssig, ist doch ge-rc_intersect-et, leider nicht 18.3.95 */
					vs_clip(vdihandle,1,clip);
					vr_recfl(vdihandle,clip); /* cursor ^ cursor  */
				}
			}
		}
		_wind_get(wp->wihandle, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
	}
/*
	vs_clip(vdihandle,0,clip);
*/
#if MiNT
	wind_update(END_UPDATE);
#endif
}

int Wmxycursor(register WINDOW *wp,int mx,int my)
{
	register int i;
	if(wp)
	{
		if(mx >= wp->work.g_x &&
			mx <= (wp->work.g_x + wp->work.g_w -1) &&
			my >= wp->work.g_y &&
			my <= (wp->work.g_y + wp->work.g_h -1))
		{
			graf_mouse_on(0);
			Wcursor(wp);
			wp->col=(mx-wp->work.g_x)/wp->wscroll;
			if(wp->w_state & INSERT)
				if(((mx-wp->work.g_x) % wp->wscroll) >= wp->wscroll/2)
					wp->col++;
			wp->cspos=wp->col;
			wp->row=(my-wp->work.g_y)/wp->hscroll;
			for(i=0,wp->cstr=wp->wstr;
				 i<wp->row && wp->cstr->next;
				 i++,wp->cstr=wp->cstr->next)
				;
			Wshiftpage(wp,0,wp->cstr->used);
			Wcuron(wp);
			Wcursor(wp);
			graf_mouse_on(1);
			return(1);
		}
	}
	return(0);
}

void _Wcblink(WINDOW *wp, int mx, int my)
{
	int hide=0,pxyarray[8];
	if(wp && (wp->w_state & CURSOR) && (wp->w_state & CBLINK))
	{
		pxyarray[0]=mx-16;
		pxyarray[1]=my-16;
		pxyarray[2]=3*16;
		pxyarray[3]=3*16;
		pxyarray[4]=max(wp->work.g_x,wp->work.g_x+wp->col*wp->wscroll-1); /* Strichcursor */
		pxyarray[5]=wp->work.g_y+wp->row*wp->hscroll-2;
		pxyarray[6]=2;
		pxyarray[7]=wp->hscroll+4;
		if( rc_intersect(array2grect(pxyarray),array2grect(&pxyarray[4])))
		{
			hide=graf_mouse_on(0);
		}
		if(wp->w_state & CURSON)
		{
			Wcursor(wp);
			wp->w_state &= ~CURSON;
		}
		else
		{
			wp->w_state |= CURSON;
			Wcursor(wp);
		}
		if(hide)
			graf_mouse_on(1);
	}
}

void Wcuron(register WINDOW *wp)
{
	if(wp)
	{
		wp->w_state |= CURSOR;
		wp->w_state |= CURSON;
	}
}

void Wcuroff(register WINDOW *wp)
{
	if(wp)
	{
		wp->w_state &= ~CURSOR;
		wp->w_state &= ~CURSON;
	}
}

int Wshiftpage(WINDOW *wp, int len, int used)
{
	register int abscol,pos;
	if(wp)
	{
		abscol=(int)(wp->col+wp->wfirst/wp->wscroll);
		abscol=min(abscol, used);
		abscol=max(0,abscol);
		if(((abscol-abs(len))<(int)(wp->wfirst/wp->wscroll) ||
			 (abscol+abs(len))>(int)(wp->wfirst+wp->work.g_w)/wp->wscroll-1))
		{
			graf_mouse_on(0);
			pos=abscol+len;
			pos=max(0,pos);
			pos=min(pos,STRING_LENGTH);
			Wslide(wp,(int)(1000L*pos/STRING_LENGTH),HSLIDE);
	/*
			graf_mouse_on(1);
	*/
		}
		wp->col=abscol-(int)(wp->wfirst/wp->wscroll);
		return(wp->col);
	}
	return 0;
}

int inw(char c); /* IN_WORD: kein Space oder Sonderzeichen */

#pragma warn -par
int Whndlkbd(register WINDOW *wp, int state, int key)
{  /* bei CAPS ist 0x1000 ausmaskiert!!! */
	register long i=0,fline,lline;
	int abscol,arrowed=0;
	register LINESTRUCT *help;

	if(! wp) 														/* Kein Fenster 							*/
		return 0;

	if(wp->w_state & ICONIFIED)					/* (GS) Ist Fenster iconify ?	*/
		return 0;		

	if(! (key & 0x8000)) 							/* Scancodebit muû gesetzt sein */
		return(0);

	switch(key)
	{
/**************************** NORMAL *************************************/
		case 0x8048:	/*  up	*/
			graf_mouse_on(0);
			Wcursor(wp);
			if(wp->cstr->prev)
			{
				if(--wp->row < 0)/* scrollen */
					Warrow(wp,WA_UPLINE);
				else
					wp->cstr=wp->cstr->prev; /* Cursor zurÅck */
				wp->col=(int)min(wp->cstr->used-wp->wfirst/wp->wscroll,wp->cspos);
			}
			Wshiftpage(wp,0,wp->cstr->used);
			undo.item=0;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x8050:	/* down  */
			graf_mouse_on(0);
			Wcursor(wp);
			if(wp->cstr->next)
			{
				if(++wp->row > wp->work.g_h/wp->hscroll-1 )/* scrollen */
					Warrow(wp,WA_DNLINE);
				else
					wp->cstr=wp->cstr->next; /* Cursor vor	 */
				wp->col=(int)min(wp->cstr->used-wp->wfirst/wp->wscroll,wp->cspos);
			}
			Wshiftpage(wp,0,wp->cstr->used);
			undo.item=0;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x804B:	/* left  */
			graf_mouse_on(0);
			Wcursor(wp);
			wp->col--;
			if(wp->col < 1 && wp->wfirst > 0)
			{
			}
			else
			{
				if(wp->col < 0)
				{
					if(wp->cstr->prev)
					{
						if(--wp->row < 0)/* scrollen */
							Warrow(wp,WA_UPLINE);
						else
							wp->cstr=wp->cstr->prev; /* Cursor zurÅck */
						wp->col = wp->cstr->used;
					}
				}
			}
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x804D:	/* right */
			graf_mouse_on(0);
			Wcursor(wp);
			if(wp->col+wp->wfirst/wp->wscroll < wp->cstr->used)
			{
				wp->col++;
			}
			else /* bei Zeilenende Cursor eine Zeile weiter vom Anfang */
			{
				if(wp->cstr->next)
				{
					if(++wp->row > wp->work.g_h/wp->hscroll-1 )/* scrollen */
					{
						Warrow(wp,WA_DNLINE);
					}
					else
						wp->cstr=wp->cstr->next; /* Cursor vor	 */
					wp->col=(int)(-wp->wfirst/wp->wscroll);
				}
			}
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x8047:	/* Home */
			graf_mouse_on(0);
			Wcursor(wp);
			wp->col=(int)(-wp->wfirst/wp->wscroll);
			if(wp->hsize > wp->work.g_h && wp->hfirst)
			{
				wp->wfirst=wp->hfirst=0;
				wp->row=0;
				wp->cstr=wp->wstr=wp->fstr;	/* alles an den Anfang */
				Wredraw(wp,&wp->work);
			}
			else /* doc < window, cursor auf 0 */
			{
				wp->cstr=wp->fstr;
				wp->row=0;
			}
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			undo.item=0;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
/**************************** SHIFT **************************************/
		case 0x8049:	/* PC Bild hoch [GS] */
		case 0x8248:	/*  shift up	*/
			graf_mouse_on(0);
			Wcursor(wp);
			if(wp->cstr == wp->wstr)
				Warrow(wp,WA_UPPAGE);
			else
				wp->cstr=wp->wstr;
			wp->col=(int)min(wp->cstr->used-wp->wfirst/wp->wscroll,wp->cspos);
			Wshiftpage(wp,0,wp->cstr->used);
			undo.item=0;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x8051:	/* PC Bild runter [GS] */
		case 0x8250:	/* shift down  */
			graf_mouse_on(0);
			Wcursor(wp);
			if(wp->hsize > wp->work.g_h)
			{
				for(i=0, help=wp->wstr;	/* letzte Fensterzeile suchen */
					 i<(wp->work.g_h/wp->hscroll-1) && help->next;
					 i++, help=help->next)
					 ;
				if(wp->cstr == help)
					Warrow(wp,WA_DNPAGE);
				else
					wp->cstr=help;
			}
			else
			{
				while(wp->cstr->next)
					wp->cstr=wp->cstr->next;
			}
			wp->col=(int)min(wp->cstr->used-wp->wfirst/wp->wscroll,wp->cspos);
			Wshiftpage(wp,0,wp->cstr->used);
			undo.item=0;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x824B:	/* shift left  */
			graf_mouse_on(0);
			Wcursor(wp);
			wp->col=(int)(-wp->wfirst/wp->wscroll);
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x824D:	/* shift right */
			graf_mouse_on(0);
			Wcursor(wp);
			wp->col=(int)(wp->cstr->used-wp->wfirst/wp->wscroll);
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x804f:	/* PC Ende [GS] */
		case 0x8247:	/* Clr (shift Home) */
			graf_mouse_on(0);
			Wcursor(wp);
			wp->col=(int)(-wp->wfirst/wp->wscroll);
			fline=wp->hfirst/wp->hscroll;
			if(wp->hsize > wp->work.g_h && wp->hfirst != wp->hsize-wp->work.g_h)
			{
				wp->hfirst = wp->hsize-wp->work.g_h;
				if(wp->hfirst < 0)
					wp->hfirst = 0;
				lline=wp->hfirst/wp->hscroll;
				if(lline > fline)
				{
					for(i=fline; i<lline; i++)
					{
						wp->wstr=wp->wstr->next;
						wp->cstr=wp->cstr->next;
					}
					Wredraw(wp,&wp->work);
				}
			}
			while(wp->cstr->next)
			{
				wp->cstr=wp->cstr->next;
				wp->row++;
			}
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			undo.item=0;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
/**************************** CONTROL ************************************/
		case 0x8448:	 /* ctrl up */
		case 0x8450:    /* ctrl dn */
			arrowed=1;
		   break;
		case 0x8473:	 /* ctrl left  */
			graf_mouse_on(0);
			Wcursor(wp);
			abscol=(int)(wp->col+wp->wfirst/wp->wscroll);
DACAPO_B:
			if(wp->col == 0 && wp->wfirst == 0)
			{
				if(wp->cstr->prev)
				{
					if(--wp->row < 0)/* scrollen */
					{
						Warrow(wp,WA_UPLINE);
					}
					else
						wp->cstr=wp->cstr->prev; /* Cursor zurÅck */
					abscol = wp->col = wp->cstr->used;
				}
			}
			abscol=find_prev_letter(wp->cstr->string, abscol);
			wp->col=(int)(abscol-wp->wfirst/wp->wscroll);
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			if(inw(wp->cstr->string[abscol]) && wp->cstr->prev)
				goto DACAPO_B;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
		case 0x8474:	 /* ctrl right */
			graf_mouse_on(0);
			Wcursor(wp);
			abscol=(int)(wp->col+wp->wfirst/wp->wscroll);
DACAPO_F:
			if(!(wp->col+wp->wfirst/wp->wscroll < wp->cstr->used))
			{
				if(wp->cstr->next)
				{
					if(++wp->row > wp->work.g_h/wp->hscroll-1 )/* scrollen */
					{
						Warrow(wp,WA_DNLINE);
					}
					else
						wp->cstr=wp->cstr->next; /* Cursor vor	 */
					wp->col=(int)(-wp->wfirst/wp->wscroll);
					abscol=-1;
				}
			}
			abscol=find_next_letter(wp->cstr->string, abscol);
			wp->col=(int)(abscol-wp->wfirst/wp->wscroll);
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			if(inw(wp->cstr->string[abscol]) && wp->cstr->next)
				goto DACAPO_F;
			arrowed=1;
			Wcuron(wp);
			Wcursor(wp);
			break;
/************************ SHIFT-CONTROL ***********************************/
		case 0x8648:	 /* shiftctrl up */
		case 0x8650:    /* shiftctrl dn */
		case 0x864B:	 /* shiftctrl left  */
		case 0x8673:
		case 0x864D:	 /* shiftctrl right */
		case 0x8674:
			arrowed=1;
			break;
	}
	return(arrowed);
}
#pragma warn .par
