/*****************************************************************
	7UP
	Modul: UNDO.C
	(c) by TheoSoft '92

	simple UNDO/REDO Funktion
	
	1997-04-08 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
# define event_timer( time ) evnt_timer( (int)(time >> 16), (int)(time & 0xFFFF) )
#else
#	include <aesbind.h>
# define event_timer( time ) evnt_timer( time )
#endif

#include "alert.h"
#include "7up.h"
#include "windows.h"
#include "wfindblk.h"
#include "block.h"
#include "editor.h"
#include "7up3.h"
#include "resource.h"
#include "graf_.h"

#include "undo.h"

#ifndef _AESapid
#	ifdef TCC_GEM
#		define	_AESapid (_GemParBlk.global[2])
#	else
#		error First define _AESapid as global[2]
#	endif
#endif

#define LINEUNDO  (-1)
#define LINEPAST  (-2)
#define CUTLINE	(-3)
#define CUTPAST	(-4)
#define PASTCUT	(-5)
#define BACKSPACE (0x0008)
#define RETURN	 (0x000D)

UNDO undo={0,0,NULL,NULL,0L,0L,0L,0,0,0,"",NULL,-1,-1,1};

void free_undoblk(WINDOW *wp, LINESTRUCT *line)
{
	if(!wp)
		return;
	if(line)
	{
		do
		{
			if(line->string)
				free(line->string);
			if(line->prev)
				free(line->prev);
			if(line->next)
				line=line->next;
			else
			{
				free(line);
				line=NULL;
			}
		}
		while(line);
		undo.blkbeg=undo.blkend=NULL;
	}
}

static char savestr[STRING_LENGTH+2];

void store_undo(WINDOW *wp, UNDO *undo, LINESTRUCT *beg, LINESTRUCT *end, int menu, int item)
{												/* Ausschneideinformationen sichern */
	long lines, chars;
	if(wp)
	{
		Wblksize(wp,beg,end,&lines,&chars);
		undo->menu=menu;
		undo->item=item;
		undo->wline=wp->hfirst/wp->hscroll;
		undo->begline=wp->row+wp->hfirst/wp->hscroll;
		undo->begcol=(int)(wp->col+wp->wfirst/wp->wscroll);
		undo->endline=undo->begline+lines;
		undo->endcol=end->used;

		if(end->used==0)
		{  /* eine ganze Zeile mit Zeilenumbruch z.B. ^Y */
			undo->endline--;
			undo->endcol=STRING_LENGTH;
		}

		if(((undo->endline-undo->begline)==1) && (undo->endcol<STRING_LENGTH))
		{  /* ein Ausschnitt aus einer Zeile */
			undo->endcol=undo->begcol+end->used;
		}
/*
printf("\33H*%ld %ld %d %d %d*",
			  undo->begline,
			  undo->endline,
			  undo->begcol,
			  undo->endcol,
			  end->used);
form_alert(1,"[0][ ][OK]");
*/
		undo->endcol=min(undo->endcol,STRING_LENGTH);
		undo->blktype=wp->w_state&COLUMN;
	}
}

static void ucut(WINDOW *wp)
{
	free_undoblk(wp,undo.blkbeg);
	Wrestblk(wp, &undo, &undo.blkbeg, &undo.blkend);
/*
printf("\33H*%ld %ld %d %d %d %d*",
			  begline,endline,
			  undo.blkbeg->begcol,
			  undo.blkbeg->endcol,
			  undo.blkend->begcol,
			  undo.blkend->endcol);
form_alert(1,"[0][ ][OK]");
*/
	event_timer(125);
	if(undo.blktype==COLUMN)
		cut_col(wp,undo.blkbeg,undo.blkend);
	else
		cut_blk(wp,undo.blkbeg,undo.blkend);
	Wcuron(wp);
	undo.menu=WINEDIT;
	undo.item=EDITPAST;
}

static void upaste(WINDOW *wp)
{
	LINESTRUCT *begcopy=NULL, *endcopy=NULL;
	
	if((undo.flag=copy_blk(wp,undo.blkbeg,undo.blkend,&begcopy,&endcopy))>0)
	{
		/* store_undo auf nach dem Einfge verschoben (MJK 3/97) 
		store_undo(wp, &undo, begcopy, endcopy, WINEDIT, EDITCUT); */
#if DEBUG
printf("\33H*%ld %ld %d %d %d %d*",
			  begline,endline,
			  undo.blkbeg->begcol,
			  undo.blkbeg->endcol,
			  undo.blkend->begcol,
			  undo.blkend->endcol);
form_alert(1,"[0][ ][OK]");
#endif
		if(undo.blktype==COLUMN)
			paste_col(wp,begcopy,endcopy);
		else
  			paste_blk(wp,begcopy,endcopy);
		store_undo(wp, &undo, begcopy, endcopy, WINEDIT, EDITCUT);
		/* wir bereits von store_undo erledigt (MJK 3/97)
		undo.menu=WINEDIT;
		undo.item=EDITCUT;
		*/
	}
}

static void uline(WINDOW *wp)
{
	char *cp;
	int savecol;

	strcpy(savestr,wp->cstr->string);
	if(wp->cstr->len < (strlen(undo.string)+1))
	{
		if((cp=realloc(wp->cstr->string,(strlen(undo.string)+1))) != NULL)
		{
			wp->cstr->string=cp;
			wp->cstr->len=(int)(strlen(undo.string)+1);
		}
		else
		{
			undo.item=0;
			return;
		}
	}
	strcpy(wp->cstr->string,undo.string);
	wp->cstr->used=(int)strlen(undo.string);
	savecol=wp->col;
	wp->col=undo.col;				  /* Cursor setzen */
	undo.col=savecol;
	refresh(wp, wp->cstr, max(0,(min(wp->col,savecol)-1)), wp->row);
	strcpy(undo.string,savestr);
	undo.item=LINEUNDO;
}

void do_undo(WINDOW *wp)										 /* Undo ausfhren */
{
	int msgbuf[8];
	LINESTRUCT *dummy=NULL;
	
	if(wp)
	{
		if(undo.flag!=1)
		{
			free_undoblk(wp, undo.blkbeg);
			form_alert(1,Aundo[0]);
			undo.item=0;
			return;
		}
		graf_mouse_on(0);
		Wcursor(wp);
		switch(undo.item)
		{
			case EDITCUT:
				ucut(wp);
				break;
			case EDITPAST:
				upaste(wp);
				break;
			case LINEUNDO:
				uline(wp);
				break;
			case LINEPAST:
				uline(wp);
				upaste(wp);
				undo.item=CUTLINE;
				break;
			case CUTLINE:
				ucut(wp);
				uline(wp);
				undo.item=LINEPAST;
				break;
			case CUTPAST:
				Wrestblk(wp, &undo, &begcut, &endcut);
				event_timer(125);
				if(undo.blktype==COLUMN)
					cut_col(wp, begcut, endcut);
				else
					cut_blk(wp, begcut, endcut);
				free_blk(wp,begcut);
				Wcuron(wp);
				upaste(wp);
				undo.item=0;
				break;
			case PASTCUT:
				break;
			case WINCLOSE:
				msgbuf[0]=MN_SELECTED;
				msgbuf[1]=_AESapid;
				msgbuf[2]=Wh(wp);
				msgbuf[3]=undo.menu;
				msgbuf[4]=undo.item;
				menu_tnormal(winmenu,msgbuf[3],0);
				appl_write(msgbuf[1],16,msgbuf);
				break;	
		}
		wp->cspos=wp->col;
		Wcursor(wp);
		graf_mouse_on(1);
		switch(undo.item)
		{
			case BACKSPACE:
				editor(wp,0,BACKSPACE,&dummy,&dummy);
				undo.item=RETURN;
				break;
			case RETURN:
				editor(wp,0,RETURN,&dummy,&dummy);
				undo.item=BACKSPACE;
				break;
		}
	}
}
