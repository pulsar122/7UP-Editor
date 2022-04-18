/*****************************************************************
	7UP
	Modul: WREALSCR.C
	(c) by TheoSoft '92

	Live scrolling des Fensterinhaltes 
	
	1997-04-08 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#endif
#include <stdio.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <aesbind.h>
#endif

#include "windows.h"
#include "7up3.h"
#include "fbinfo.h"
#include "wind_.h"
#include "graf_.h"

#include "wrealscr.h"

#define VERTICAL	1
#define HORIZONTAL 2
#define notnull(a) ((a>0)?(a):(1))

#define EXOB_TYPE(x) (x>>8)
#define WP_YWORK(y) (wp->toolbar?(EXOB_TYPE(wp->toolbar->ob_type)==0?(y-wp->toolbar->ob_height):y):y)
#define WP_HWORK(h) (wp->toolbar?(EXOB_TYPE(wp->toolbar->ob_type)==0?(h+wp->toolbar->ob_height):h):h)
#define WP_XWORK(x) (wp->toolbar?(EXOB_TYPE(wp->toolbar->ob_type)==1?(x-wp->toolbar->ob_width ):x):x)
#define WP_WWORK(w) (wp->toolbar?(EXOB_TYPE(wp->toolbar->ob_type)==1?(w+wp->toolbar->ob_width ):w):w)

int isvertical(WINDOW *wp, int e_mx, int e_my)
{						  /* Slider kann links oder rechts sein */
	int x,y,w,h;
	wind_calc(WC_BORDER,wp->kind,
		wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
		&x,&y,&w,&h);
	if(((e_mx>WP_XWORK(wp->work.g_x)+WP_WWORK(wp->work.g_w) && e_mx<x+w) || /* rechts */
		 (e_mx>x && e_mx<WP_XWORK(wp->work.g_x))) &&				 /* links  */
		(e_my>WP_YWORK(wp->work.g_y)+boxh && e_my<WP_YWORK(wp->work.g_y)+WP_HWORK(wp->work.g_h)-boxh) &&
		wp->hsize>WP_HWORK(wp->work.g_h))
		return(1);					 /* Maus im vert. Slider */
	else
		return(0);
}

int ishorizontal(WINDOW *wp, int e_mx, int e_my)
{									 /* Slider kann nur unten sein */
	int x,y,w,h;
	wind_calc(WC_BORDER,wp->kind,
		wp->work.g_x,wp->work.g_y,wp->work.g_w,wp->work.g_h,
		&x,&y,&w,&h);
	if((e_my>WP_YWORK(wp->work.g_y)+WP_HWORK(wp->work.g_h) && e_my<y+h) &&
		(e_mx>WP_XWORK(wp->work.g_x)+boxw*2 && e_mx<WP_XWORK(wp->work.g_x)+WP_WWORK(wp->work.g_w)-boxw*2) &&
		wp->wsize>WP_WWORK(wp->work.g_w))
		return(1);					  /* Maus im hor. Slider */
	else
		return(0);
}

void Wrealscroll(WINDOW *wp, int e_mx, int e_my, int dir)
{
	int mouse_click,pos, ret, slsize;
	int mx,my,nx,ny;
	
	wind_update(BEG_MCTRL);
	graf_mouse_on(0);
	Wcursor(wp);
	graf_mouse_on(1);
	graf_mouse(FLAT_HAND,NULL);
	nx=mx=e_mx;
	ny=my=e_my;
	if(dir==VERTICAL)
	{
		_wind_get(wp->wihandle,WF_VSLSIZE,&slsize,&ret,&ret,&ret);
/*
		pos=(int)((long)((my-max((boxh+2)/2,(long)(wp->work.g_h-2*(boxh+2))*slsize/1000L/2))-(wp->work.g_y+(boxh+2)))*1000L/(long)notnull((long)(wp->work.g_h-2*(boxh+2))-max((boxh+2),(long)(wp->work.g_h-2*(boxh+2))*slsize/1000L)));
*/
		pos=(int)((long)((my-max((boxh+2)/2,(long)(WP_HWORK(wp->work.g_h)-2*(boxh+2))*slsize/1000L/2))-(WP_YWORK(wp->work.g_y)+(boxh+2)))*1000L/(long)notnull((long)(WP_HWORK(wp->work.g_h)-2*(boxh+2))-max((boxh+2),(long)(WP_HWORK(wp->work.g_h)-2*(boxh+2))*slsize/1000L)));
		graf_mouse_on(0);
		Wslide(wp,min(max(0,pos),1000),VSLIDE); /* entsprechend Maus... */
		graf_mouse_on(1);
		do
		{
			graf_mkstate(&mx,&my,&mouse_click,&ret); /* Position */
			if(ny!=my && my>WP_YWORK(wp->work.g_y) && my<WP_YWORK(wp->work.g_y)+WP_HWORK(wp->work.g_h))
			{
/*
				pos=(int)((long)((my-max((boxh+2)/2,(long)(wp->work.g_h-2*(boxh+2))*slsize/1000L/2))-(wp->work.g_y+(boxh+2)))*1000L/(long)notnull((long)(wp->work.g_h-2*(boxh+2))-max((boxh+2),(long)(wp->work.g_h-2*(boxh+2))*slsize/1000L)));
*/
				pos=(int)((long)((my-max((boxh+2)/2,(long)(WP_HWORK(wp->work.g_h)-2*(boxh+2))*slsize/1000L/2))-(WP_YWORK(wp->work.g_y)+(boxh+2)))*1000L/(long)notnull((long)(WP_HWORK(wp->work.g_h)-2*(boxh+2))-max((boxh+2),(long)(WP_HWORK(wp->work.g_h)-2*(boxh+2))*slsize/1000L)));
				graf_mouse_on(0);
				Wslide(wp,min(max(0,pos),1000),VSLIDE); /* einstellen */
				graf_mouse_on(1);
				Wsetrcinfo(wp);
				nx=mx;
				ny=my;
			}
		}
		while(mouse_click); /* solange Maustaste gedrckt */
	}
	else
	{
		_wind_get(wp->wihandle,WF_HSLSIZE,&slsize,&ret,&ret,&ret);
		pos=(int)((long)((mx-max((boxw*2+2)/2,(long)(WP_WWORK(wp->work.g_w)-2*(boxw*2+2))*slsize/1000L/2))-(WP_XWORK(wp->work.g_x)+(boxw*2+2)))*1000L/(long)notnull((long)(WP_WWORK(wp->work.g_w)-2*(boxw*2+2))-max((boxw*2+2),(long)(WP_WWORK(wp->work.g_w)-2*(boxw*2+2))*slsize/1000L)));
		graf_mouse_on(0);
		Wslide(wp,min(max(0,pos),1000),HSLIDE); /* entsprechend Maus... */
		graf_mouse_on(1);
		do
		{
			graf_mkstate(&mx,&my,&mouse_click,&ret); /* Position */
			if(nx!=mx && mx>WP_XWORK(wp->work.g_x) && mx<WP_XWORK(wp->work.g_x)+WP_WWORK(wp->work.g_w))
			{
				pos=(int)((long)((mx-max((boxw*2+2)/2,(long)(WP_WWORK(wp->work.g_w)-2*(boxw*2+2))*slsize/1000L/2))-(WP_XWORK(wp->work.g_x)+(boxw*2+2)))*1000L/(long)notnull((long)(WP_WWORK(wp->work.g_w)-2*(boxw*2+2))-max((boxw*2+2),(long)(WP_WWORK(wp->work.g_w)-2*(boxw*2+2))*slsize/1000L)));
				graf_mouse_on(0);
				Wslide(wp,min(max(0,pos),1000),HSLIDE); /* einstellen */
				graf_mouse_on(1);
				Wsetrcinfo(wp);
				nx=mx;
				ny=my;
			}
		}
		while(mouse_click);
	}
	graf_mouse(ARROW,NULL);
	graf_mouse_on(0);
	Wcursor(wp);
	graf_mouse_on(1);
	wind_update(END_MCTRL);
}
/*
		pos=(int)((long)((my-max((boxh+2)/2,(long)(WP_HWORK-2*(boxh+2))*slsize/1000L/2))-(WP_YWORK+(boxh+2)))*1000L/(long)notnull((long)(WP_HWORK-2*(boxh+2))-max((boxh+2),(long)(WP_HWORK-2*(boxh+2))*slsize/1000L)));
*/