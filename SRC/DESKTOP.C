/*****************************************************************
	7UP
	Modul: DESKTOP.C
	(c) by TheoSoft '90

	Iconbedienung

	1997-03-25 (MJK): nur lokal benîtigte Funktionen static 
	                  deklariert
	1997-03-27 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <ext.h>
#else
#	include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
# define event_timer( time ) evnt_timer( (int)(time >> 16), (int)(time & 0xFFFF) )
#else
#	include <gem.h>
# define event_timer( time ) evnt_timer( time )
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "macro.h"
#include "alert.h"
#include "falert.h"
#include "windows.h"
#include "forms.h"
#ifndef ENGLISH											/* (GS) */
	#include "7UP.h"
#else
	#include "7UP_eng.h"
#endif
#include "undo.h"
#include "language.h"
#include "7up3.h"
#include "resource.h"
#include "fileio.h"
#include "block.h"
#include "findrep.h"
#include "editor.h"
#include "printer.h"
#include "wind_.h"
#include "graf_.h"

#include "desktop.h"

#define FLAGS15 0x8000
#define ICON_MAX 12 /* max. 30 Zeichen im Icon */

char menu_ctrl[7][5]={" ^F1"," ^F2"," ^F3"," ^F4"," ^F5"," ^F6"," ^F7"}; 

void iconposition(int icon, int *x, int *y, int *w, int *h)
{
	if(nodesktop)
	{
		*x=*y=*w=*h=0;
		return;
	}
	objc_offset(desktop,icon,x,y);
	*w=desktop[icon].ob_width;
	*h=desktop[icon].ob_height;
}

static void Iredraw(OBJECT *tree, int start_obj, int depth)
{
	GRECT t1,t2;

	if(nodesktop)
		return;

	objc_offset(tree,start_obj,&t2.g_x,&t2.g_y);
	t2.g_w = tree[start_obj].ob_width;
	t2.g_h = tree[start_obj].ob_height;
	_wind_get(0,WF_FIRSTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
	while(t1.g_w && t1.g_h)
	{
		if(rc_intersect(&t2,&t1))
			objc_draw(tree,start_obj,depth,t1.g_x,t1.g_y,t1.g_w,t1.g_h); /* 1997-03-27 (MJK): tree statt desktop */
		_wind_get(0,WF_NEXTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
	}
}

void Dredraw(OBJECT *tree, int x, int y, int w, int h)
{
	GRECT t1,t2;

	if(nodesktop)
		return;

	t2.g_x=x;
	t2.g_y=y;
	t2.g_w=w;
	t2.g_h=h;
	_wind_get(0,WF_FIRSTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
	while(t1.g_w && t1.g_h)
	{
		if(rc_intersect(&t2,&t1))
			objc_draw(tree,ROOT,MAX_DEPTH,t1.g_x,t1.g_y,t1.g_w,t1.g_h); /* 1997-03-27 (MJK): tree statt desktop */
		_wind_get(0,WF_NEXTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
	}
}

void add_icon(OBJECT *tree, int icon)
{
	int i;
	WINDOW *wp;

	if((wp=Wicon(icon)) != NULL)
	{
		for(i=WINDAT1;i<=WINDAT7;i++)
			if(winmenu[i].ob_state & DISABLED)
			{
				if(nodesktop)
					winmenu[i].ob_flags&=~HIDETREE;
				menu_ienable(winmenu,i,1);
				wp->mentry=i;
				break;
			}
		if(nodesktop) 	
		{
			winmenu[WINDAT1-1].ob_flags&=~HIDETREE;
			winmenu[WINOPALL-1].ob_height=(WINDAT7-WINOPALL+1)*boxh;
			for(i=WINDAT7;i>=WINDAT1;i--)
				if(winmenu[i].ob_state&DISABLED)
				{
					winmenu[i].ob_flags|=HIDETREE;
					winmenu[WINOPALL-1].ob_height-=boxh;
				}
				else
					break;
		}
	}		
	tree[icon].ob_flags&=~HIDETREE;

	if(nodesktop)
		return;

	Iredraw(tree,icon,0);
}

#define MENU_LEN 20

void ren_icon(OBJECT *tree, int icon)
{
	int len;
	WINDOW *wp;
	char *cp,name[25]="";

	if((wp=Wicon(icon)) != NULL)
	{
		cp=(char *)Wname(wp);
		len=(int)strlen(cp);
		if(len>20-4)
		{
			strncpy(&name[2],cp,2);
			name[4]=0;
			strcpy(&name[4],"..");
			strcpy(&name[6],&cp[len-(13-4)-3]);
		}
		else
		{
			memset(name,' ',20-2);
			name[20-2]=0;
			memmove(&name[2],cp,strlen(cp));
		}
		name[0]=name[1]=' ';
		strcat(name,menu_ctrl[wp->mentry-WINDAT1]); /* Ctrlsequenz anhÑngen */
	 	menu_text(winmenu,wp->mentry,name);
	}		
	strncpy(tree[icon].ob_spec.iconblk->ib_ptext,split_fname((char *)Wname(wp)),ICON_MAX);
	tree[icon].ob_spec.iconblk->ib_ptext[ICON_MAX]=0;

	if(nodesktop)
		return;

	Iredraw(tree,icon,0);
}

void del_icon(OBJECT *tree, int icon)
{
	int i,offx,offy;
	char name[25]="";

	WINDOW *wp;
	
	if((wp=Wicon(icon)) != NULL)
	{
		strcpy(name,"  Datei X            ");
		name[8]=(char)(wp->mentry-WINDAT1)+'1';
  		menu_text(winmenu,wp->mentry,name);
		menu_icheck(winmenu,wp->mentry,0);
		menu_ienable(winmenu,wp->mentry,0);

		if(nodesktop) 	
		{
			winmenu[WINOPALL-1].ob_height=(WINDAT7-WINOPALL+1)*boxh;
			for(i=WINDAT7;i>=WINDAT1;i--)
				if(winmenu[i].ob_state&DISABLED)
				{
					winmenu[i].ob_flags|=HIDETREE;
					winmenu[WINOPALL-1].ob_height-=boxh;
				}
				else
					break;
			if(winmenu[WINOPALL-1].ob_height==(WINDAT1-WINOPALL)*boxh)
			{
				winmenu[WINDAT1-1].ob_flags|=HIDETREE;
				winmenu[WINOPALL-1].ob_height-=boxh;
			}
		}
		wp->mentry=0;
	}
	tree[icon].ob_state&=~SELECTED;
	tree[icon].ob_flags|=HIDETREE;

	if(nodesktop)
		return;

	objc_offset(tree,icon,&offx,&offy); /* Desktop, nicht Icon neuzeichnen!!! */
	Dredraw(tree,offx,offy,tree[icon].ob_width,tree[icon].ob_height);
}

int is_selected(OBJECT *tree, int from, int to)
{
	register int i,count=0;

	if(nodesktop)
		return(0);

	for(i=from; i<=to; i++)		  /* icon selektieren */
		if(tree[i].ob_state & SELECTED)
			count++;
	return(count);
}

void sel_icons(OBJECT *tree, int from, int to, int mode)
{
	register int i;

	if(nodesktop)
		return;

	for(i=from; i<=to; i++)		  /* icon selektieren */
		if(!(tree[i].ob_state & SELECTED) && !(tree[i].ob_flags & HIDETREE))
		{
			tree[i].ob_state |= SELECTED;
			if(mode)
				Iredraw(tree,i,1);
		}
}

void desel_icons(OBJECT *tree, int from, int to, int mode)
{
	register int i;

	if(nodesktop)
		return;

	for(i=from; i<=to; i++)		  /* icon deselektieren */
		if(tree[i].ob_state & SELECTED && !(tree[i].ob_flags & HIDETREE))
		{
			tree[i].ob_state &= ~SELECTED;
			tree[i].ob_flags &= ~FLAGS15;			/* lîschen */
			if(mode)
				Iredraw(tree,i,1);
		}
}

static MEVENT mevent=
{
	MU_BUTTON,
	1,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	NULL,
	0L,
	0,0,0,0,0,0,
/* nur der VollstÑndigkeit halber die Variablen von XGEM */
	0,0,0,0,0,
	0,
	0L,
	0L,0L
};

#define odd(i) ((i)&1)

static void set_style (int handle, int expr)
{
  vsl_udsty (handle, odd (expr) ? 0xAAAA : 0x5555);
} /* set_style */

static void drawframe(int handle,int x, int y, int w, int h)
{
	register int xy[10];

	xy [0] = x;
	xy [1] = y;
	xy [2] = xy [0];
	xy [3] = xy [1] + h - 1;
	xy [4] = xy [0] + w - 1;
	xy [5] = xy [3];
	xy [6] = xy [4];
	xy [7] = xy [1];
	xy [8] = xy [0];
	xy [9] = xy [1];

	set_style (handle, xy [0] + xy [1]);
	v_pline (handle, 2, &xy [0]);
	set_style (handle, xy [3]);
	v_pline (handle, 2, &xy [2]);
	set_style (handle, xy [4] + xy [5]);
	v_pline (handle, 2, &xy [4]);
	set_style (handle, xy [7]);
	v_pline (handle, 2, &xy [6]);
}

static GRECT *size_of_all_icons(OBJECT *tree, int from, int to)
{
	static GRECT rect;
	int i,x,y;

	rect.g_x=INT_MAX;
	rect.g_y=INT_MAX;
	rect.g_w=0;
	rect.g_h=0;

	for(i=from; i<=to; i++)				/* x,y */
	{
		if(tree[i].ob_state & SELECTED)
		{
			objc_offset(tree,i,&x,&y);
			rect.g_x= min(rect.g_x,x);
			rect.g_y= min(rect.g_y,y);
		}
	}
	for(i=from; i<=to; i++)				/* w,h */
	{
		if(tree[i].ob_state & SELECTED)
		{
			objc_offset(tree,i,&x,&y);
			rect.g_w= max(rect.g_w,x+tree[i].ob_width-rect.g_x);
			rect.g_h= max(rect.g_h,y+tree[i].ob_height-rect.g_y);
		}
	}
	return(&rect);
}

#pragma warn -par
static int graf_exdragbox(OBJECT *tree, int icon,
	int width, int height, int offx, int offy,
	int xd, int yd, int wd, int hd, int *xret, int *yret)
{
	int i,mx,my,ret,pxy[4],origin,ooum=0,noum=0;
	register int lw,rw,uh,lh;
	int xoff,yoff,oldx,oldy;

	GRECT *rect;
	ICONBLK *iconblk;

	oldx=offx;
	oldy=offy;

	pxy[0]=xd;
	pxy[1]=yd;
	pxy[2]=xd+wd-1;
	pxy[3]=yd+hd-1;
	vs_clip(vdihandle, 1, pxy);

	rect=size_of_all_icons(tree,DESKICN1,DESKICND);
	lw=offx - rect->g_x;
	rw=rect->g_x + rect->g_w - offx;
	uh=offy - rect->g_y;
	lh=rect->g_y + rect->g_h - offy;

	graf_mouse_on(0);
	for(i=DESKICN1; i<=DESKICND; i++)
	{
		if(tree[i].ob_state & SELECTED)
		{
			objc_offset(tree,i,&xoff,&yoff);
			iconblk=tree[i].ob_spec.iconblk;
			drawframe(vdihandle,xoff+iconblk->ib_xicon+(offx-oldx),
							  yoff+iconblk->ib_yicon+(offy-oldy),
									 iconblk->ib_wicon,
									 iconblk->ib_hicon);
			drawframe(vdihandle,xoff+iconblk->ib_xtext+(offx-oldx),
							  yoff+iconblk->ib_ytext+(offy-oldy),
									 iconblk->ib_wtext,
									 iconblk->ib_htext);
			tree[i].ob_flags |= FLAGS15; /* als FLAGS15 markieren */
		}
	}
	graf_mouse_on(1);

	graf_mkstate(&mx,&my,&ret,&ret);
	origin=objc_find(tree,ROOT,MAX_DEPTH,mx,my);

	do
	{
		mevnt_event(&mevent);
		if(mevent.e_mx != mx || mevent.e_my != my)
		{
			graf_mouse_on(0);
			for(i=DESKICN1; i<=DESKICND; i++)
			{
				if(tree[i].ob_flags & FLAGS15 && tree[i].ob_state & SELECTED)
				{
					objc_offset(tree,i,&xoff,&yoff);
					iconblk=tree[i].ob_spec.iconblk;
					drawframe(vdihandle,xoff+iconblk->ib_xicon+(offx-oldx),
									  yoff+iconblk->ib_yicon+(offy-oldy),
											 iconblk->ib_wicon,
											 iconblk->ib_hicon);
					drawframe(vdihandle,xoff+iconblk->ib_xtext+(offx-oldx),
									  yoff+iconblk->ib_ytext+(offy-oldy),
											 iconblk->ib_wtext,
											 iconblk->ib_htext);
				}
			}

			mx=mevent.e_mx;
			my=mevent.e_my;
			noum=objc_find(tree,ROOT,MAX_DEPTH,mx,my);  /* Objekt finden */

			if(noum!=ooum && !(tree[noum].ob_state & SELECTED))
			{
				if(ooum>ROOT && ooum!=origin)
					desel_icons(tree,ooum,ooum,1);/* altes Objekt desel. */
				if(noum>ROOT && noum!=origin)
					sel_icons(tree,noum,noum,1);  /* neues Objekt sel.	*/
				ooum=noum;
			}

			offx=mx-width/2;			  /* neuen Dragrahmen errechenen */
			offy=my-height/2;

			if(offx-lw<xd)
				offx=xd+lw;
			if(offy-uh<yd)
				offy=yd+uh;
			if(offx+rw>xd+wd-1)
				offx=xd+wd-1-rw;
			if(offy+lh>yd+hd-1)
				offy=yd+hd-1-lh;

			for(i=DESKICN1; i<=DESKICND; i++)
			{
				if(tree[i].ob_flags & FLAGS15 && tree[i].ob_state & SELECTED)
				{
					objc_offset(tree,i,&xoff,&yoff);
					iconblk=tree[i].ob_spec.iconblk;
					drawframe(vdihandle,xoff+iconblk->ib_xicon+(offx-oldx),
									  yoff+iconblk->ib_yicon+(offy-oldy),
											 iconblk->ib_wicon,
											 iconblk->ib_hicon);
					drawframe(vdihandle,xoff+iconblk->ib_xtext+(offx-oldx),
									  yoff+iconblk->ib_ytext+(offy-oldy),
											 iconblk->ib_wtext,
											 iconblk->ib_htext);
				}
			}
			graf_mouse_on(1);
		}
	}
	while(mevent.e_mb & 0x0001);
	graf_mouse_on(0);
	for(i=DESKICN1; i<=DESKICND; i++)
	{
		if(tree[i].ob_flags & FLAGS15 && tree[i].ob_state & SELECTED)
		{
			objc_offset(tree,i,&xoff,&yoff);
			iconblk=tree[i].ob_spec.iconblk;
			drawframe(vdihandle,xoff+iconblk->ib_xicon+(offx-oldx),
							  yoff+iconblk->ib_yicon+(offy-oldy),
									 iconblk->ib_wicon,
									 iconblk->ib_hicon);
			drawframe(vdihandle,xoff+iconblk->ib_xtext+(offx-oldx),
							  yoff+iconblk->ib_ytext+(offy-oldy),
									 iconblk->ib_wtext,
									 iconblk->ib_htext);
		}
	}
	graf_mouse_on(1);
	vs_clip(vdihandle, 0, pxy);

	*xret=offx;											 /* return-Werte */
	*yret=offy;
	return(ooum);
}
#pragma warn .par

void drag_icon(OBJECT *tree, int icon)
{
	LINESTRUCT *line;
	WINDOW *wp,*wp2;
	static char filename[PATH_MAX]; /* mu· static sein, weil Spooler sonst */
									  /* den lokalen String nicht findet	  */

	int x,y,offx,offy,width,height,objc;
	int xoff,yoff,i,mx,my,mc,ret;

	if(nodesktop)
		return;

	wp=Wgettop();
	if(!(tree[icon].ob_state & SELECTED)) /* wenn es nocht nicht selektiert ist */
	{												 /* Rest desel */
		desel_icons(tree,DESKICN1,DESKICND,1);
		sel_icons(tree,icon,icon,1);
	}												 /* sonst mehrere Icons draggen */
	event_timer(125);
	graf_mkstate(&mx,&my,&mc,&ret);
	if(!mc)											/* nur invertieren, fÅr Info... */
		return;

	graf_mouse(FLAT_HAND,0L);
	objc_offset(tree,icon,&offx,&offy);
/*
	printf("\33H%3d %3d %3d ",ydesk,offy,tree[icon].ob_height);
*/

	width =tree[icon].ob_spec.iconblk->ib_wtext;
	height=tree[icon].ob_spec.iconblk->ib_hicon
			+tree[icon].ob_spec.iconblk->ib_htext;
	objc=graf_exdragbox(tree,icon,width,height,offx,offy,
							  xdesk,ydesk,wdesk,hdesk,&x,&y);
	graf_mouse(ARROW,0L);
	graf_mkstate(&mx,&my,&ret,&ret);

	if((ret=wind_find(mx,my))>0)  /* Klemmbretticon ins oberste Fenster */
	{
		if(ret==Wh(wp))
		{
			switch(icon)
			{
				case DESKICN1:
				case DESKICN2:
				case DESKICN3:
				case DESKICN4:
				case DESKICN5:
				case DESKICN6:
				case DESKICN7:
					wp2=Wicon(icon);
					if(!cut)
			 			hide_blk(wp,begcut,endcut);
			 		else
						free_blk(wp,begcut);
					if(_read_blk(wp,wp2->name,&begcut,&endcut)>0)
					{
						store_undo(wp, &undo, begcut, endcut, WINEDIT, EDITCUT);
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
						hndl_blkfind(wp,begcut,endcut,SEAREND);
					}
					begline=endline=0L;
					begcut=endcut=NULL;
					break;
				case DESKICNB:
					if(!cut)
			 			hide_blk(wp,begcut,endcut);
			 		else
						free_blk(wp,begcut);
					if(read_clip(wp,&begcut,&endcut))
					{
						store_undo(wp, &undo, begcut, endcut, WINEDIT, EDITCUT);
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
						hndl_blkfind(wp,begcut,endcut,SEAREND);
					}
					begcut=endcut=NULL;
					break;
				default:
					my_form_alert(1,Adesktop[0]);
					break;
			}
		}
/*
		desel_icons(tree,DESKICN1,DESKICND,1);
*/
		return;
	}

	for(i=DESKICN1; i<=DESKICND; i++)
	{
		if(tree[i].ob_flags & FLAGS15 && tree[i].ob_state & SELECTED)
		{
			switch(objc) /* auf dieses Ziel wurde Icon gezogen */
			{
				case DESKICN1:
				case DESKICN2:
				case DESKICN3:
				case DESKICN4:
				case DESKICN5:
				case DESKICN6:
				case DESKICN7:
						my_form_alert(1,Adesktop[0]);
					break;
				case DESKICN8: /* Papierkorb */
				case DESKICND: /* voller Papierkorb */
					switch(i) /* dies ist das bewegte Icon */
					{
						case DESKICN1:
						case DESKICN2:
						case DESKICN3:
						case DESKICN4:
						case DESKICN5:
						case DESKICN6:
						case DESKICN7:
							Gsave(Wicon(i));
							delete(Wicon(i),tree,i);
							break;
						case DESKICN8: /* Papierkorb */
						case DESKICND: /* voller Papierkorb */
						case DESKICN9: /* Drucker	 */
						case DESKICNA: /* DISK		 */
							my_form_alert(1,Adesktop[0]);
							break;
						case DESKICNB: /* Klemmbrett */
							graf_mouse(BUSY_BEE,NULL);
							scrp_clear_own();
							inst_clipboard_icon(tree,DESKICNB,DESKICNC,0);
							graf_mouse(ARROW,NULL);
							break;
						case DESKICNC:
							my_form_alert(1,Adesktop[1]);
							break;
					}
					break;
				case DESKICN9: /* Drucker	 */
					switch(i)
					{
						case DESKICN1:
						case DESKICN2:
						case DESKICN3:
						case DESKICN4:
						case DESKICN5:
						case DESKICN6:
						case DESKICN7:
							hndl_prtmenu(prtmenu,layout,pinstall,Wicon(i));
							break;
						case DESKICN8: /* Papierkorb */
						case DESKICND: /* voller Papierkorb */
						case DESKICN9: /* Drucker	 */
						case DESKICNA: /* DISK		 */
							my_form_alert(1,Adesktop[0]);
							break;
						case DESKICNB: /* Klemmbrett */
							if(my_form_alert(2,Adesktop[2])==2)
							{
								scrp_read(filename);
								if(*filename)
								{
									complete_path(filename);
/*
									if(filename[strlen(filename)-1]!='\\')
										strcat(filename,"\\");
*/
									strcat(filename,"SCRAP.TXT");
									spool(filename,1,0);
								}
							}
							break;
						case DESKICNC:
							my_form_alert(1,Adesktop[1]);
							break;
					}
					break;
				case DESKICNA: /* Disk		 */
					switch(i)
					{
						case DESKICN1:
						case DESKICN2:
						case DESKICN3:
						case DESKICN4:
						case DESKICN5:
						case DESKICN6:
						case DESKICN7:
							write_file(Wicon(i), 1); /* immer SAVE AS..., weil besser */
							break;
						case DESKICN8: /* Papierkorb */
						case DESKICND: /* voller Papierkorb */
						case DESKICN9: /* Drucker	 */
						case DESKICNA: /* DISK		 */
							my_form_alert(1,Adesktop[0]);
							break;
						case DESKICNB: /* Klemmbrett */
							my_form_alert(1,Adesktop[3]);
							break;
						case DESKICNC: /* leeres Klemmbrett */
							my_form_alert(1,Adesktop[1]);
							break;
					}
					break;
				case DESKICNB: /* Klemmbrett */
				case DESKICNC: /* leeres K. */
					switch(i)
					{
						case DESKICN1:
						case DESKICN2:
						case DESKICN3:
						case DESKICN4:
						case DESKICN5:
						case DESKICN6:
						case DESKICN7:
							wp=Wicon(icon);
							for(line=wp->fstr; line->next; line=line->next)
							{
								line->begcol=0;
								line->endcol=STRING_LENGTH;
							}
							write_clip(wp, wp->fstr, line);
							break;
						case DESKICN8: /* leerer Papierkorb */
						case DESKICND: /* voller Papierkorb */
						case DESKICN9: /* Drucker	 */
						case DESKICNA: /* DISK		 */
						case DESKICNB: /* Klemmbrett */
						case DESKICNC: /* ungÅltig */
							my_form_alert(1,Adesktop[0]);
							break;
					}
					break;
				case ROOT:
					if(x != offx || y != offy) /* nur draggen */
					{
						objc_offset(tree,i,&xoff,&yoff);
						tree[i].ob_x=align(xoff+(x-offx),8);
						tree[i].ob_y=align(yoff+(y-ydesk-offy),8);
						Dredraw(tree,xoff,yoff,tree[i].ob_width,tree[i].ob_height);
						switch(i) /* welches Icon wurde verschoben? */
						{
							case DESKICN8: /* leerer P */
								tree[DESKICND].ob_x=tree[i].ob_x;
								tree[DESKICND].ob_y=tree[i].ob_y;
								break;
							case DESKICND: /* voller P */
								tree[DESKICN8].ob_x=tree[i].ob_x;
								tree[DESKICN8].ob_y=tree[i].ob_y;
								break;
							case DESKICNB: /* volles K */
								tree[DESKICNC].ob_x=tree[i].ob_x;
								tree[DESKICNC].ob_y=tree[i].ob_y;
								break;
							case DESKICNC: /* leeres K */
								tree[DESKICNB].ob_x=tree[i].ob_x;
								tree[DESKICNB].ob_y=tree[i].ob_y;
								break;
						}
					}
					break;
			}
			Iredraw(tree,i,1);
		}
	}
	desel_icons(tree,objc,objc,1);
	return;
}

static GRECT *rubber(int x, int y)
{
	static GRECT rub;
	rub.g_x=x;
	rub.g_y=y;
	graf_rubbox(x,y,-32767,-32767,&rub.g_w,&rub.g_h);
	if(rub.g_w < 0)
	{
		rub.g_x +=  rub.g_w;
		rub.g_w  = -rub.g_w;
	}
	if(rub.g_h < 0)
	{
		rub.g_y +=  rub.g_h;
		rub.g_h  = -rub.g_h;
	}
	return(&rub);
}

int mark_icons(OBJECT *tree, int mx, int my)
{
	ICONBLK *iconblk;
	GRECT *grect;
	int xoff,yoff,icon=0,mc,ret,p1[4],p2[4],p3[4],p4[4];

	if(nodesktop)
		return(0);

	event_timer(125);
	graf_mkstate(&ret,&ret,&mc,&ret);
	ret=0;
	if(!mc)											/* nur invertieren, fÅr Info... */
		return(0);
	graf_mouse(POINT_HAND,NULL);
	grect=rubber(mx,my);
	graf_mouse(ARROW,NULL);

	for(icon=DESKICN1; icon<=DESKICND; icon++)
	{
		objc_offset(tree,icon,&xoff,&yoff);
		iconblk=tree[icon].ob_spec.iconblk;
		p1[0]=p2[0]=grect->g_x; /*- xdesk;*/
		p1[1]=p2[1]=grect->g_y; /*- ydesk;*/
		p1[2]=p2[2]=grect->g_w;
		p1[3]=p2[3]=grect->g_h;

		p3[0]=iconblk->ib_xicon+xoff;
		p3[1]=iconblk->ib_yicon+yoff;
		p3[2]=iconblk->ib_wicon;
		p3[3]=iconblk->ib_hicon;

		p4[0]=iconblk->ib_xtext+xoff;
		p4[1]=iconblk->ib_ytext+yoff;
		p4[2]=iconblk->ib_wtext;
		p4[3]=iconblk->ib_htext;

		if(rc_intersect(array2grect(p3),array2grect(p1)) ||
		   rc_intersect(array2grect(p4),array2grect(p2)))
		{
			sel_icons(tree,icon,icon,1);
			ret=1;
		}
	}
	return(ret);
}

WINDOW *Wgetwp(char *filename);

void click_icon(OBJECT *tree, int icon)
{
	char *cp,filename[PATH_MAX];

	if(nodesktop)
		return;

	sel_icons(tree,icon,icon,1);
	if(icon>=DESKICN1 && icon<=DESKICN7) /* Datei îffnen */
	{
		Wopen(Wicon(icon));
		undo.menu=WINFILE;
		undo.item=WINCLOSE;
	}
	if(icon==DESKICN8 || icon==DESKICND) /* Trashcan */
		inst_trashcan_icon(tree,DESKICN8,DESKICND,1);
	if(icon==DESKICND)						/* Gemini-Trashcan */
	{
		if((cp=getenv("TRASHDIR"))!=NULL)
		{
			strcpy(filename,cp);
			complete_path(filename);
/*
			if(filename[strlen(filename)-1]!='\\')
				strcat(filename,"\\*.*");
*/
			if(gettrashname(/*strupr*/(filename),EMPTYTRASH))
			{
				Wreadfile(filename,0);
				unlink(filename);
				inst_trashcan_icon(tree,DESKICN8,DESKICND,0);
			}
		}
	}
	if(icon==DESKICN8)/* MÅlleimer */
		delete_file();
	if(icon==DESKICN9)/* Drucker */
		spool_file();
	if(icon==DESKICNA) /* Fileselectbox zeigen */
		loadfiles("","");
	if(icon==DESKICNB || icon==DESKICNC) /* Clipbrd */
		icon=inst_clipboard_icon(tree,DESKICNB,DESKICNC,1);
	if(icon==DESKICNB) /* Clipbrd */
	{
		scrp_read(filename);
		if(*filename)
		{
			complete_path(filename);
/*
			if(filename[strlen(filename)-1]!='\\')
				strcat(filename,"\\");
*/
			strcat(filename,"SCRAP.TXT");
			Wreadfile(filename,0);
		}
	}
	if(icon==DESKICNC)
	{ /* leer */
		my_form_alert(1,Adesktop[1]);
	}
	desel_icons(tree,icon,icon,1);
}

int inst_trashcan_icon(OBJECT *tree, int old, int new, int mode)
{
	char *cp, filename[PATH_MAX];

	if(nodesktop)
		return(0);

	if((divmenu[DIVPAPER].ob_state & SELECTED) &&
		(cp=getenv("TRASHDIR"))!=NULL)
	{
		strcpy(filename,cp);
		complete_path(filename);
/*
		if(filename[strlen(filename)-1]!='\\')
			strcat(filename,"\\");
*/
		strcat(filename,"*.*");
		if(!access(filename,0))
		{
			if(mode)
				tree[new].ob_state|=SELECTED;
			del_icon(tree,old);
			add_icon(tree,new);
			return(new);
		}
	}
	if(mode)
		tree[old].ob_state|=SELECTED;
	del_icon(tree,new);
	add_icon(tree,old);
	return(old);
}

int inst_clipboard_icon(OBJECT *tree, int full, int empty, int mode)
{
	char filename[PATH_MAX];

	if(nodesktop)
		return(0);

	scrp_read(filename);
	if(!*filename)
	{
		if(create_clip())
			scrp_read(filename);
	}
	if(*filename)
	{
		complete_path(filename);
/*
		if(filename[strlen(filename)-1]!='\\')
			strcat(filename,"\\");
*/
		strcat(filename,"SCRAP.TXT");
		if(!access(filename,0))
		{
			if(mode)
				tree[full].ob_state|=SELECTED;
			del_icon(tree,empty);
			add_icon(tree,full);
			return(full);
		}
	}
	if(mode)
		tree[empty].ob_state|=SELECTED;
  	del_icon(tree,full);
	add_icon(tree,empty);
	return(empty);
}
