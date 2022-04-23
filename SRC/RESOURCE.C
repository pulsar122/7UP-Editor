/*****************************************************************
	7UP
	Modul: RESOURCE.C
	(c) by TheoSoft '90
	
	Resourcen kompilieren
	
	1997-04-07 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-09 (MJK): MSDOS-Teile entfernt
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <tos.h>
#	include <ext.h>
#else
#	include <osbind.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <gem.h>
#endif

#include "windows.h"
#include "forms.h"
#include "7UP.h"
#include "macro.h"
#include "undo.h"
#include "language.h"
#include "desktop.h"
#include "config.h"
#include "userdef.h"
#include "7up3.h"
#include "deadkey.h"
#include "toolbar.h"
#include "numerik.h"
#include "fileio.h"
#include "shell.h"
#include "printer.h"
#include "objc_.h"
#include "wind_.h"

#include "resource.h"

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

#define SCREEN			 1
#define PLOTTER		  11
#define PRINTER		  21
#define METAFILE		31
#define CAMERA		  41
#define TABLET		  51

#define DFONT	  0x0080				 /* extended obtype fÅr font */
#define DALT		0x0081

#define FLAGS15 0x8000

#define CRLF 1
#define LF	2
#define CR	3

OBJECT *winmenu,*prtmenu,*popmenu,
		 *desktop,*findmenu,*gotomenu,
		 *fontmenu,*tabmenu,*infomenu,
		 *copyinfo,*shellmenu,*chartable,
		 /**progress,*/*shareware,*userimg,
		 *shell2,*fkeymenu,*umbrmenu,
		 *pinstall,*preview,*layout,
		 *replmenu,*markmenu,
		 *popmenu,*divmenu,*bracemenu,
		 *sortmenu,*gdospop,*fselbox,
		 *paperpop,*grepmenu,*distpop,
		 *nummenu,*picklist,*menueditor,
		 *registmenu,*formatmenu,*listbox;

int align(int, int);
void *get_cookie(long CookieId);
WINDOW *Wmentry(int mentry);

void checkmenu(OBJECT *tree, WINDOW *wp)
{
	static WINDOW *oldwp=NULL;
	WINDOW *wp2;
	int i,desk_obj;
	int wh;

	if(wp)
	{
		if(wp!=oldwp) /* nur dann, wenn Fenster nicht gewechselt wurde */
		{
			if(undo.item!=WINABORT && undo.item!=WINCLOSE)
				undo.item=FALSE;
			oldwp=wp;
		}
		i=is_selected(desktop,DESKICN8,DESKICND);
		menu_ienable(tree,WINREAD, !i && !(begcut && endcut));
		menu_ienable(tree,WINCLOSE,!i);

		menu_ienable(tree,WINSAVE, !i &&
						 (wp->w_state&CHANGED?TRUE:FALSE) &&
						 strcmp((char *)Wname(wp),NAMENLOS));
		menu_ienable(tree,WINSAVAS,!i);
		menu_ienable(tree,WINABORT,!i &&
						 (wp->w_state&CHANGED?TRUE:FALSE) &&
						 strcmp((char *)Wname(wp),NAMENLOS));
		menu_ienable(tree,WINNEXT2,(Wcount(OPENED)>1)?TRUE:FALSE); /* next gewÑhren */

		menu_ienable(tree,WINOPALL,(Wcount(CREATED)-Wcount(OPENED))?TRUE:FALSE);
		menu_ienable(tree,WINCLALL,Wcount(OPENED)?TRUE:FALSE);
		menu_ienable(tree,WINARR1,(Wcount(OPENED)>1)?TRUE:FALSE);
		menu_ienable(tree,WINARR2,(Wcount(OPENED)>1)?TRUE:FALSE);
		menu_ienable(tree,WINARR3,(Wcount(OPENED)>1)?TRUE:FALSE);
		menu_ienable(tree,WINFULL,TRUE);
		menu_ienable(tree,WINPRINT,!i);

		i = !cut && begcut && endcut;
		if(i)
			menu_ienable(tree,BLKCOL,FALSE);
		else
			menu_ienable(tree,BLKCOL,TRUE);
		menu_ienable(tree,EDITCUT,i);
		menu_ienable(tree,EDITCOPY,i);
		menu_ienable(tree,EDITPAST,clipbrd
		|| (!clipbrd &&  cut &&	begcut && endcut));

		menu_ienable(tree,EDITUNDO,undo.item); /* UNDO gewÑhren */
		menu_ienable(tree,EDITSHLF,i);
		menu_ienable(tree,EDITSHRT,i);
		menu_ienable(tree,EDITSORT,i);
		menu_ienable(tree,EDITTOGL,TRUE);
		menu_ienable(tree,EDITBIG,TRUE);
		menu_ienable(tree,EDITSMAL,TRUE);
		menu_ienable(tree,EDITCAPS,TRUE);
		menu_ienable(tree,EDITFORM,wp->w_state&COLUMN?FALSE:TRUE);
		menu_ienable(tree,SEARBEG,i);
		menu_ienable(tree,SEAREND,i);

		i = (wp->w_state&COLUMN) && begcut && endcut;
		menu_ienable(tree,BLKCNT,i);
		menu_ienable(tree,BLKSUM,i);
		menu_ienable(tree,BLKMEAN,i);
		menu_ienable(tree,BLKSDEV,i);
		menu_ienable(tree,BLKMWST,i);
		menu_ienable(tree,BLKINTER,i);
		menu_ienable(tree,BLKALL,i);

		menu_ienable(tree,FORMUMBR,TRUE);
		menu_ienable(tree,FORMBLK,TRUE);
		menu_icheck(tree,FORMBLK,wp->w_state &  BLOCKSATZ ? TRUE:FALSE ); /* HÑkchen fÅr Blocksatz */
		menu_icheck(tree,BLKCOL,wp->w_state & COLUMN ? TRUE:FALSE );

		menu_ienable(tree,FORMINS,TRUE);
		menu_icheck(tree,FORMINS,wp->w_state&INSERT?TRUE:FALSE); /* HÑkchen fÅr Insert */

		menu_ienable(tree,FORMIND,TRUE);
		menu_icheck(tree,FORMIND,wp->w_state&INDENT?TRUE:FALSE); /* HÑkchen fÅr Indent */

		menu_ienable(tree,OPTLIN,TRUE);
		menu_ienable(tree,OPTBRACE,TRUE);

		if(!topwin)
		{
			menu_ienable(tree,WINREAD ,TRUE);
			menu_ienable(tree,WININFO ,TRUE);
			menu_ienable(tree,WINCLOSE,TRUE);
			menu_ienable(tree,WINSAVE ,TRUE);
			menu_ienable(tree,WINSAVAS,TRUE);
			menu_ienable(tree,WINABORT,TRUE);
			menu_ienable(tree,WINPRINT,TRUE);

			if(nodesktop)
				menu_ienable(tree,EDITALL,TRUE);
			menu_ienable(tree,SEARFIND,TRUE);
			menu_ienable(tree,SEARNEXT,TRUE);
			menu_ienable(tree,SEARSEL ,TRUE);
			menu_ienable(tree,SEARSMRK,TRUE);
			menu_ienable(tree,SEARGMRK,TRUE);
			menu_ienable(tree,SEARGOTO,TRUE);
			menu_ienable(tree,SEARPAGE,TRUE);

			menu_ienable(tree,FORMTAB,TRUE);
			menu_ienable(tree,OPTFONT,TRUE);
			menu_ienable(tree,OPTCHARS,TRUE);
			menu_ienable(tree,OPTVIEW,TRUE);

			topwin=TRUE;
		}										  /* NICHT VOR "if(!topwin)" !!!	 */
		if((i=wind_create(0,0,0,0,0))>0) /* gibt es ein weiteres Fenster ? */
		{
			wind_delete(i);
			if(Wcount(CREATED)<MAXWINDOWS-1)			  /* wg. WINX */
			{
				menu_ienable(tree,WINNEW,TRUE);  /* wenn ja, enablen				*/
				menu_ienable(tree,WINOPEN,TRUE);
				menu_ienable(tree,WINPICK,TRUE);
			}
			else
			{
				menu_ienable(tree,WINNEW,FALSE);  /* wenn nein, disablen		  */
				menu_ienable(tree,WINOPEN,FALSE);
				menu_ienable(tree,WINPICK,FALSE);
			}
		}
		else
		{
			menu_ienable(tree,WINNEW,FALSE);  /* wenn nein, disablen		  */
			menu_ienable(tree,WINOPEN,FALSE);
			menu_ienable(tree,WINPICK,FALSE);
		}
	}
	else
	{
		if(topwin)
		{
			menu_ienable(tree,WINREAD ,FALSE);
			menu_ienable(tree,WININFO ,FALSE);
			menu_ienable(tree,WINCLOSE,FALSE);
			menu_ienable(tree,WINSAVE ,FALSE);
			menu_ienable(tree,WINSAVAS,FALSE);
			menu_ienable(tree,WINABORT,FALSE);
			menu_ienable(tree,WINPRINT,FALSE);

			menu_ienable(tree,EDITUNDO,FALSE);
			menu_ienable(tree,EDITCUT,FALSE);
			menu_ienable(tree,EDITCOPY,FALSE);
			menu_ienable(tree,EDITPAST,FALSE);
			menu_ienable(tree,EDITSHLF,FALSE);
			menu_ienable(tree,EDITSHRT,FALSE);
			menu_ienable(tree,EDITSORT,FALSE);
			menu_ienable(tree,EDITTOGL,FALSE);
			menu_ienable(tree,EDITBIG,FALSE);
			menu_ienable(tree,EDITSMAL,FALSE);
			menu_ienable(tree,EDITCAPS,FALSE);
			menu_ienable(tree,EDITFORM,FALSE);

			menu_ienable(tree,SEARFIND,FALSE);
			menu_ienable(tree,SEARNEXT,FALSE);
			menu_ienable(tree,SEARSEL ,FALSE);
			menu_ienable(tree,SEARBEG ,FALSE);
			menu_ienable(tree,SEAREND ,FALSE);
			menu_ienable(tree,SEARSMRK,FALSE);
			menu_ienable(tree,SEARGMRK,FALSE);
			menu_ienable(tree,SEARGOTO,FALSE);
			menu_ienable(tree,SEARPAGE,FALSE);

			menu_ienable(tree,BLKCNT,FALSE);
			menu_ienable(tree,BLKSUM,FALSE);
			menu_ienable(tree,BLKMEAN,FALSE);
			menu_ienable(tree,BLKSDEV,FALSE);
			menu_ienable(tree,BLKMWST,FALSE);
			menu_ienable(tree,BLKINTER,FALSE);
			menu_ienable(tree,BLKALL,FALSE);

			menu_ienable(tree,WINCLALL,FALSE);
			menu_ienable(tree,WINARR1,FALSE);
			menu_ienable(tree,WINARR2,FALSE);
			menu_ienable(tree,WINARR3,FALSE);
			menu_ienable(tree,WINFULL,FALSE);

			menu_ienable(tree,OPTFONT,FALSE);
			menu_ienable(tree,OPTCHARS,FALSE);
			menu_ienable(tree,OPTVIEW,FALSE);
			menu_ienable(tree,OPTLIN,FALSE);
			menu_ienable(tree,OPTBRACE,FALSE);
			topwin=FALSE;
		}
		_wind_get(0,WF_TOP,&wh,&i,&i,&i);
		/* mindestens ein eigenes Fenster, oberstes gehîrt aber nicht zu 7UP */
		menu_ienable(tree,WINNEXT2,(Wcount(OPENED) && !Wp(wh))?TRUE:FALSE); /* next gewÑhren */

		menu_ienable(tree,WININFO, TRUE);
		menu_ienable(tree,WINCLOSE,TRUE);
		menu_ienable(tree,WINSAVE, TRUE);
		menu_ienable(tree,WINSAVAS,TRUE);
		menu_ienable(tree,WINPRINT,TRUE);
		menu_ienable(tree,EDITALL,!nodesktop);

		menu_ienable(tree,FORMTAB,FALSE);
		menu_ienable(tree,FORMUMBR,FALSE);
		menu_ienable(tree,FORMBLK,FALSE);
		menu_ienable(tree,BLKCOL,FALSE);
		menu_ienable(tree,FORMINS,FALSE);
		menu_ienable(tree,FORMIND,FALSE);

		menu_icheck(tree,FORMTAB,FALSE);
		menu_icheck(tree,FORMBLK,FALSE);
		menu_icheck(tree,BLKCOL,FALSE);
		menu_icheck(tree,FORMINS,FALSE);
		menu_icheck(tree,FORMIND,FALSE);
		if(is_selected(desktop,DESKICN1,DESKICN7)==1)
		{
			for(desk_obj=DESKICN1; desk_obj<=DESKICN7; desk_obj++)
			{
				if(desktop[desk_obj].ob_state & SELECTED)
				{
					wp=Wicon(desk_obj);				/* nimm Icon  */

					menu_ienable(tree,WINSAVE,(wp->w_state&CHANGED?TRUE:FALSE) &&
									 strcmp((char *)Wname(wp),NAMENLOS));
					menu_ienable(tree,FORMTAB,TRUE);
					menu_ienable(tree,FORMUMBR,TRUE);
					menu_ienable(tree,FORMBLK,TRUE);
					menu_ienable(tree,BLKCOL,TRUE);
					menu_ienable(tree,FORMINS,TRUE);
					menu_ienable(tree,FORMIND,TRUE);
					menu_icheck(tree,FORMBLK,wp->w_state&BLOCKSATZ?TRUE:FALSE); /* HÑkchen fÅr Blocksatz */
					menu_icheck(tree,BLKCOL,wp->w_state&COLUMN?TRUE:FALSE);
					menu_icheck(tree,FORMINS,wp->w_state&INSERT?TRUE:FALSE); /* HÑkchen fÅr Insert */
					menu_icheck(tree,FORMIND,wp->w_state&INDENT?TRUE:FALSE); /* HÑkchen fÅr Indent */
				}
			}
		}
		menu_ienable(tree,WINOPALL,Wcount(CREATED)?TRUE:FALSE);
		menu_ienable(tree,OPTCHARS,TRUE);
		menu_ienable(tree,OPTVIEW,TRUE);
		menu_ienable(tree,OPTLIN,TRUE);
		menu_ienable(tree,OPTBRACE,TRUE);
		if(!is_selected(desktop,DESKICN1,DESKICN7))
		{
			menu_ienable(tree,WINCLOSE,FALSE);
			menu_ienable(tree,WINSAVE, FALSE);
			menu_ienable(tree,WINSAVAS,FALSE);
			menu_ienable(tree,WINPRINT,FALSE);
			menu_ienable(tree,OPTCHARS,FALSE);
			menu_ienable(tree,OPTVIEW,FALSE);
			menu_ienable(tree,OPTLIN,FALSE);
			menu_ienable(tree,OPTBRACE,FALSE);
		}
		else
			goto WEITER;
		if(!is_selected(desktop,DESKICN8,DESKICND))
		{
			menu_ienable(tree,WININFO,FALSE);  /* info erstmal lîschen */
		}
	}
WEITER:
	if(nodesktop)
		for(i=WINDAT1; i<=WINDAT7; i++)
		  if(!(tree[i].ob_state&DISABLED) && (wp2=Wmentry(i))!=NULL)
				menu_icheck(tree,i,wp2->w_state&OPENED?TRUE:FALSE);

	menu_ienable(tree, MACSAVE, macro.mp?TRUE:FALSE); /* Macro laden */

	if(Wcount(OPENED)>=2)
		menu_ienable(tree,OPTCOMP,TRUE);  /* Textvergleich */
	else
		menu_ienable(tree,OPTCOMP,FALSE);
	return;
}

#define RSC_CREATE
#ifdef RSC_CREATE
#include "7up.rsh"
#include "7up.rh"

static void fix_tree(int n_tree)
{
  register int tree,	  /* index for trees */
	             object;	/* index for objects */
  OBJECT		 *pobject;

  for (tree = 0; tree < n_tree; tree++) /* fix trees */
  {
	 object  = 0;
	 pobject = rs_trindex [tree];
	 do
	 {
		rsrc_obfix(pobject,object);
	 }
	 while (! (pobject [object++].ob_flags & LASTOB));
  } /* for */
} /* fix_tree */
#endif

#pragma warn -par
int rsrc_init(char *rscname, char *inffile)
{
	unsigned int *ss;
	int /*hiword,loword,*/i,x,y,w,h,area[4];
/*	OBJECT *ob;*/

#ifdef RSC_CREATE
	if(pexec)/* wurde schon mal gestartet */
	{
		menu_bar(winmenu,TRUE);
/*
	if((_AESversion>=0x0400) &&
	   (_wind_get(0,WF_NEWDESK,&hiword,&loword,&i,&i)!=0))
	{
		ob = (OBJECT *)(((long)hiword<<16)|(long)loword);
		if(ob)
		{
			desktop->ob_spec.obspec.interiorcol=ob->ob_spec.obspec.interiorcol;
			desktop->ob_spec.obspec.fillpattern=ob->ob_spec.obspec.fillpattern;
		}
	}
	else
*/
		desktop->ob_spec.obspec.interiorcol=GREEN;
		inst_trashcan_icon(desktop,DESKICN8,DESKICND,FALSE);
		inst_clipboard_icon(desktop,DESKICNB,DESKICNC,FALSE);
		if(!nodesktop || pexec)		/* nur dann MÅll abrÑumen */
		{
			if(nodesktop)
				wind_set(0,WF_NEWDESK,0,0,0,0);
			else
			{
				ss = (unsigned int) &desktop;
				wind_set(0,WF_NEWDESK,ss[0],ss[1],0,0);
			}
/*			wind_set(0,WF_NEWDESK,nodesktop?NULL:desktop,0,0);*/
			_wind_get(0, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
			while( area[2] && area[3] )
			{
				if(nodesktop)
					form_dial(FMD_FINISH,0,0,0,0,area[0],area[1],area[2],area[3]);
				else
					objc_draw(desktop,ROOT,MAX_DEPTH,area[0],area[1],area[2],area[3]);
				_wind_get(0, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
			}
		}
		return(TRUE);
	}
	fix_tree(NUM_TREE);
	winmenu  =(OBJECT *)rs_trindex[WINMENU ];
	prtmenu  =(OBJECT *)rs_trindex[PRTMENU ];
	desktop  =(OBJECT *)rs_trindex[DESKTOP ];
	findmenu =(OBJECT *)rs_trindex[FINDMENU];
	gotomenu =(OBJECT *)rs_trindex[GOTOMENU];
	tabmenu  =(OBJECT *)rs_trindex[TABMENU ];
	infomenu =(OBJECT *)rs_trindex[INFOMENU];
	copyinfo =(OBJECT *)rs_trindex[COPYINFO];
	fontmenu =(OBJECT *)rs_trindex[FONTSEL ];
	shellmenu=(OBJECT *)rs_trindex[SHELMENU];
	chartable=(OBJECT *)rs_trindex[CHARTBL ];
/*
	progress =(OBJECT *)rs_trindex[IOPROG  ];
*/
	shareware=(OBJECT *)rs_trindex[SHARWARE];
	shell2	=(OBJECT *)rs_trindex[SHELL2  ];
	userimg  =(OBJECT *)rs_trindex[USERIMG ];
	fkeymenu =(OBJECT *)rs_trindex[FKEYMENU];
	umbrmenu =(OBJECT *)rs_trindex[UMBRMENU];
	layout	=(OBJECT *)rs_trindex[PRNINST ];
	pinstall =(OBJECT *)rs_trindex[PRNINST2];
	preview  =(OBJECT *)rs_trindex[PREVIEW ];
	replmenu =(OBJECT *)rs_trindex[REPLMENU];
	markmenu =(OBJECT *)rs_trindex[MARKMENU];
	grepmenu =(OBJECT *)rs_trindex[GREPMENU];
	popmenu  =(OBJECT *)rs_trindex[POPMENU ];
	divmenu  =(OBJECT *)rs_trindex[DIVERSES];
	bracemenu=(OBJECT *)rs_trindex[BRACES  ];
	sortmenu =(OBJECT *)rs_trindex[SORTMENU];
	gdospop  =(OBJECT *)rs_trindex[POPPRN  ];
	fselbox  =(OBJECT *)rs_trindex[FSELBOX ];
	paperpop =(OBJECT *)rs_trindex[POPPAPER];
	distpop  =(OBJECT *)rs_trindex[POPDIST ];
	nummenu  =(OBJECT *)rs_trindex[NUMMENU ];
	picklist =(OBJECT *)rs_trindex[PICKLIST];
	menueditor=(OBJECT *)rs_trindex[MENUEDITOR];
	registmenu=(OBJECT *)rs_trindex[REGISTER];
	formatmenu=(OBJECT *)rs_trindex[TEXTFORMAT];
	listbox =  (OBJECT *)rs_trindex[SLISTBOX];
#else
		if(!rsrc_load(rscname))
			return(FALSE);
	rsrc_gaddr(R_TREE,WINMENU ,&winmenu  );
	rsrc_gaddr(R_TREE,PRTMENU ,&prtmenu  );
	rsrc_gaddr(R_TREE,DESKTOP ,&desktop  );
	rsrc_gaddr(R_TREE,FINDMENU,&findmenu );
	rsrc_gaddr(R_TREE,GOTOMENU,&gotomenu );
	rsrc_gaddr(R_TREE,TABMENU ,&tabmenu  );
	rsrc_gaddr(R_TREE,INFOMENU,&infomenu );
	rsrc_gaddr(R_TREE,COPYINFO,&copyinfo );
	rsrc_gaddr(R_TREE,FONTSEL ,&fontmenu );
	rsrc_gaddr(R_TREE,SHELMENU,&shellmenu);
	rsrc_gaddr(R_TREE,CHARTBL ,&chartable);
/*
	rsrc_gaddr(R_TREE,IOPROG  ,&progress );
*/
	rsrc_gaddr(R_TREE,SHARWARE,&shareware);
	rsrc_gaddr(R_TREE,SHELL2  ,&shell2	);
	rsrc_gaddr(R_TREE,USERIMG ,&userimg  );
	rsrc_gaddr(R_TREE,FKEYMENU,&fkeymenu );
	rsrc_gaddr(R_TREE,UMBRMENU,&umbrmenu );
	rsrc_gaddr(R_TREE,PRNINST ,&layout	);
	rsrc_gaddr(R_TREE,PRNINST2,&pinstall );
	rsrc_gaddr(R_TREE,PREVIEW ,&preview  );
	rsrc_gaddr(R_TREE,REPLMENU,&replmenu );
	rsrc_gaddr(R_TREE,MARKMENU,&markmenu );
	rsrc_gaddr(R_TREE,GREPMENU,&grepmenu );
	rsrc_gaddr(R_TREE,POPMENU ,&popmenu  );
	rsrc_gaddr(R_TREE,DIVERSES,&divmenu  );
	rsrc_gaddr(R_TREE,BRACES  ,&bracemenu);
	rsrc_gaddr(R_TREE,SORTMENU,&sortmenu );
	rsrc_gaddr(R_TREE,POPPRN  ,&gdospop  );
	rsrc_gaddr(R_TREE,FSELBOX ,&fselbox  );
	rsrc_gaddr(R_TREE,POPPAPER,&paperpop );
	rsrc_gaddr(R_TREE,POPDIST, &distpop  );
	rsrc_gaddr(R_TREE,NUMMENU, &nummenu  );
	rsrc_gaddr(R_TREE,PICKLIST,&picklist );
	rsrc_gaddr(R_TREE,MENUEDITOR,&menueditor);
	rsrc_gaddr(R_TREE,REGISTER,&registmenu);
	rsrc_gaddr(R_TREE,TEXTFORMAT,&formatmenu);
	rsrc_gaddr(R_TREE,SLISTBOX,&listbox);
#endif

#ifdef RSC_CREATE
#undef RSC_CREATE
#endif

	form_center(prtmenu ,&x,&y,&w,&h);
	form_center(findmenu,&x,&y,&w,&h);
	form_center(gotomenu,&x,&y,&w,&h);
	form_center(tabmenu ,&x,&y,&w,&h);
	form_center(infomenu,&x,&y,&w,&h);
	form_center(copyinfo,&x,&y,&w,&h);
	form_center(fontmenu,&x,&y,&w,&h);
	form_center(shellmenu,&x,&y,&w,&h);
	form_center(chartable,&x,&y,&w,&h);
/*
	form_center(progress,&x,&y,&w,&h);
*/
	form_center(shareware,&x,&y,&w,&h);
	form_center(shell2,&x,&y,&w,&h);
	form_center(fkeymenu,&x,&y,&w,&h);
	form_center(umbrmenu,&x,&y,&w,&h);
	form_center(layout,&x,&y,&w,&h);
	form_center(pinstall,&x,&y,&w,&h);
	form_center(preview,&x,&y,&w,&h);
	form_center(replmenu,&x,&y,&w,&h);
	form_center(markmenu,&x,&y,&w,&h);
	form_center(grepmenu,&x,&y,&w,&h);
	form_center(divmenu,&x,&y,&w,&h);
	form_center(bracemenu,&x,&y,&w,&h);
	form_center(sortmenu,&x,&y,&w,&h);
	form_center(nummenu,&x,&y,&w,&h);
	form_center(picklist,&x,&y,&w,&h);
	form_center(menueditor,&x,&y,&w,&h);
	form_center(registmenu,&x,&y,&w,&h);
	form_center(formatmenu,&x,&y,&w,&h);

	/* Fensterdialoflag beim Laden der Resource aktieren */
	prtmenu->ob_flags&=~FLAGS15;
	findmenu->ob_flags&=~FLAGS15;
	gotomenu->ob_flags&=~FLAGS15;
	tabmenu->ob_flags&=~FLAGS15;
	infomenu->ob_flags&=~FLAGS15;
	copyinfo->ob_flags&=~FLAGS15;
	fontmenu->ob_flags&=~FLAGS15;
	shellmenu->ob_flags&=~FLAGS15;
	chartable->ob_flags&=~FLAGS15;
	shareware->ob_flags&=~FLAGS15;
	shell2->ob_flags&=~FLAGS15;
	fkeymenu->ob_flags&=~FLAGS15;
	umbrmenu->ob_flags&=~FLAGS15;
	layout->ob_flags&=~FLAGS15;
	pinstall->ob_flags&=~FLAGS15;
	preview->ob_flags&=~FLAGS15;
	replmenu->ob_flags&=~FLAGS15;
	markmenu->ob_flags&=~FLAGS15;
	grepmenu->ob_flags&=~FLAGS15;
	divmenu->ob_flags&=~FLAGS15;
	bracemenu->ob_flags&=~FLAGS15;
	sortmenu->ob_flags&=~FLAGS15;
	nummenu->ob_flags&=~FLAGS15;
	picklist->ob_flags&=~FLAGS15;
	menueditor->ob_flags&=~FLAGS15;
	formatmenu->ob_flags&=~FLAGS15;

	form_write(findmenu,FINDSTR,"",FALSE);
	form_write(findmenu,FINDREPL,"",FALSE);
	form_write(gotomenu,GOTOLINE,"",FALSE);
	form_write(tabmenu,TABULAT,"3",FALSE);
	form_write(shellmenu,SHELCOMM,"",FALSE);
	form_write(layout,PRNKZSTR,"",FALSE);
	form_write(layout,PRNFZSTR,"",FALSE);
	form_write(divmenu,DIVBACK2,"",FALSE);
	form_write(grepmenu,GREPPATT,"",FALSE);

	form_write(markmenu,MARK1MEM,"",FALSE);
	form_write(markmenu,MARK2MEM,"",FALSE);
	form_write(markmenu,MARK3MEM,"",FALSE);
	form_write(markmenu,MARK4MEM,"",FALSE);
	form_write(markmenu,MARK5MEM,"",FALSE);

   form_write(bracemenu,FREE1BEG,"",FALSE);
   form_write(bracemenu,FREE1END,"",FALSE);
   form_write(bracemenu,FREE2BEG,"",FALSE);
   form_write(bracemenu,FREE2END,"",FALSE);
   form_write(bracemenu,FREE3BEG,"",FALSE);
   form_write(bracemenu,FREE3END,"",FALSE);
   form_write(bracemenu,FREE4BEG,"",FALSE);
   form_write(bracemenu,FREE4END,"",FALSE);
   form_write(bracemenu,FREE5BEG,"",FALSE);
   form_write(bracemenu,FREE5END,"",FALSE);

	form_write(menueditor,MENUMFILE,"",FALSE);
	form_write(menueditor,MENUTFILE,"",FALSE);

	form_write(registmenu,REGISTNAME,"",FALSE);
	form_write(registmenu,REGISTKEY,"",FALSE);

	desktop->ob_x=xdesk;
	desktop->ob_y=ydesk;
	desktop->ob_width=wdesk;
	desktop->ob_height=hdesk;

	for(i=FKEY1; i<=FKEY10; i++)
		form_write(fkeymenu,i,"",FALSE);
	for(i=SFKEY1; i<=SFKEY10; i++)
	{
		form_write(fkeymenu,i,"",FALSE);
/* 2.10.94
		fkeymenu[i].ob_flags &= ~EDITABLE;
*/
	}
/* erst fixen, dann hiden	
	fkeymenu[FKSHIFT].ob_flags|=HIDETREE;
*/

	w=wdesk/8;
	h=hdesk/6;

	for(i=DESKICN1;i<=DESKICN7;i++)
	{
		desktop[i].ob_x = (i-DESKICN1)*w;
		desktop[i].ob_y = hdesk-desktop[i].ob_height;
	}
	for(i=DESKICN8;i<=DESKICNB;i++)
	{
		desktop[i].ob_x = wdesk-desktop[i].ob_width;
		desktop[i].ob_y = hdesk-desktop[i].ob_height-(i-DESKICN8)*h;
	}
	for(i=DESKICN1;i<=DESKICNB;i++)				  /* koordinaten sichern */
	{
		iconcoords[i-DESKICN1].x = abs2rel(desktop[i].ob_x, xdesk+wdesk);
		iconcoords[i-DESKICN1].y = abs2rel(desktop[i].ob_y, ydesk+hdesk);
	}
	gdospop [POPNO  ].ob_state&=~CHECKED;
	paperpop[POPLETT].ob_state&=~CHECKED;
	distpop [POPD1  ].ob_state&=~CHECKED;

/*********************************************************************/
	restoreconfig(inffile); /* jetzt evtl. Defaultwerte Åberschreiben */
/*********************************************************************/

	gdospop [act_dev  ].ob_state|=CHECKED;
	strcpy((char *)layout[PRNDRUCK].ob_spec.index,
			 (char *)gdospop[act_dev].ob_spec.index);

	paperpop[act_paper].ob_state|=CHECKED;
	strcpy((char *)layout[PRNPAPER].ob_spec.index,
			 (char *)paperpop[act_paper].ob_spec.index);
	distpop[act_dist  ].ob_state|=CHECKED;
	strcpy((char *)layout[PRNDIST].ob_spec.index,
			 (char *)distpop[act_dist].ob_spec.index);
/*
	menu_icheck(winmenu,EDITCLIP,clipbrd);
*/
	if(divmenu[DIVDESK].ob_state & SELECTED)
		nodesktop=FALSE;
	else
		nodesktop=TRUE;

	if(divmenu[DIVCLIP].ob_state & SELECTED)
		clipbrd=TRUE;
	else
		clipbrd=FALSE;

	if(divmenu[DIVTOSDOMIAN].ob_state & SELECTED)
		tosdomain = TRUE;
	else
		tosdomain = FALSE;

  	winmenu[WINOPALL-1].ob_height=((WINFULL-WINOPALL+1)*boxh);
	for(i=WINDAT1-1; i<=WINDAT7; i++)
 		winmenu[i].ob_flags|=HIDETREE;
	
	if(divmenu[DIVPAPER].ob_state & DISABLED)
		divmenu[DIVPAPER].ob_state = NORMAL;
	if(!getenv("TRASHDIR")) /* Disablen falls nicht vorhanden */
		divmenu[DIVPAPER].ob_state = DISABLED;

	if(divmenu[DIVMAUS].ob_state & DISABLED)
		divmenu[DIVMAUS].ob_state = NORMAL;
	if(divmenu[DIVFREE].ob_state & DISABLED)
		divmenu[DIVFREE].ob_state = NORMAL;

	if((void *)get_cookie('VSCR')!=NULL) /* Virtual Screen von BigScreen 2 */
	{
		divmenu[DIVMAUS].ob_state = DISABLED;
		divmenu[DIVFREE].ob_state = DISABLED;
		divmenu[DIVZENT].ob_state = SELECTED;
	}

	if((_AESversion>=0x0340) && 
	   (objc_sysvar(0,4,0,0,&i,&i)>0) &&
	   (TRUE/*mindestens_16_Farben()*/))          /* 3D-Effekt mîglich? */
	{
		threedee=TRUE;
	}
	else
	{
		threedee=FALSE;
	}

	if(!threedee) /* Help- un OK-Knopf vergrîûern */
	{
	   divmenu[DIVHDA].ob_x      -= 0;
   	divmenu[DIVHDA].ob_y      -= 1;
   	divmenu[DIVHDA].ob_width  += 0;
   	divmenu[DIVHDA].ob_height += 2;
   }

	if(_AESnumapps!=1) /* Mehr als eine Applikation gleichzeitig */
	{
		winmenu[WINSHELL].ob_state|=DISABLED; /* kein Shellaufruf */

		if(divmenu[DIVDWIN].ob_state & DISABLED)
			divmenu[DIVDWIN].ob_state = NORMAL;
		if(divmenu[DIVDWIN].ob_state & SELECTED)
			windials=TRUE;
		else
			windials=FALSE;

		if(divmenu[DIVVAST].ob_state & DISABLED)
			divmenu[DIVVAST].ob_state = NORMAL;
		if(divmenu[DIVVAST].ob_state & SELECTED)
			vastart=TRUE;
		else
			vastart=FALSE;
	}
	else
	{
		divmenu[DIVDWIN ].ob_state = DISABLED;
		windials=FALSE;
		divmenu[DIVVAST ].ob_state = DISABLED;
		vastart=FALSE;
	}

	if(divmenu[DIVTABEX].ob_state & SELECTED)
		tabexp=TRUE;
	else
		tabexp=FALSE;

	if(divmenu[DIVKONV ].ob_state & SELECTED)
		eszet=TRUE;
	else
		eszet=FALSE;

	if(divmenu[DIVBLANK].ob_state & SELECTED)
		bcancel=TRUE;
	else
		bcancel=FALSE;

	if(divmenu[DIVSTOOL].ob_state & SELECTED)
		toolbar_zeigen=TRUE;
	else
		toolbar_zeigen=FALSE;

	if(divmenu[DIVCRLF ].ob_state & SELECTED)
		lineendsign=CRLF;
	if(divmenu[DIVLF	].ob_state & SELECTED)
		lineendsign=LF;
	if(divmenu[DIVCR	].ob_state & SELECTED)
		lineendsign=CR;

	if(divmenu[DIVWRET ].ob_state & SELECTED)
		wret=TRUE;
	else
		wret=FALSE;
/*
	if(divmenu[DIVMACTIV].ob_state & SELECTED)
		window_under_mouse=TRUE;
	else
		window_under_mouse=FALSE;
*/
	if(divmenu[DIVUMLAUT].ob_state & SELECTED)
		umlautwandlung=TRUE;
	else
		umlautwandlung=FALSE;

	if(!(divmenu[DIVBUTIM].ob_state & SELECTED))
		backuptime=0xFFFFFFFFL;
	sprintf(divmenu[DIVBACK2].ob_spec.tedinfo->te_ptext,"%02d",
			  (int)min(99,(int)(backuptime/60000L)));

	if(nummenu[NUMNDT].ob_state & SELECTED)
		komma=TRUE;
	else
		komma=FALSE;
/*	
	/* zu Test zwecken */		
	divmenu[DIVTABBAR].ob_state |= SELECTED;
*/
	if(divmenu[DIVTABBAR].ob_state & SELECTED)
		tabbar=TRUE;
	else
		tabbar=FALSE;

	if(divmenu[DIVSCROLL].ob_state & SELECTED)
		scrollreal=TRUE;
	else
		scrollreal=FALSE;

	if(menueditor[MENUTAKTIV].ob_state & SELECTED)
		scaktiv=TRUE;
	else
		scaktiv=FALSE;

	for(i=DESKICN1;i<=DESKICNB;i++) /* Koordinaten setzen, entweder die  */
	{										 /* errechneten, oder die geladenen	*/
		desktop[i].ob_x = rel2abs(iconcoords[i-DESKICN1].x, xdesk+wdesk);
		desktop[i].ob_y = rel2abs(iconcoords[i-DESKICN1].y, ydesk+hdesk);
	}

	for(i=DESKICN1;i<=DESKICNB;i++) /* Koordinaten korrigieren */
	{
		if(desktop[i].ob_x + desktop[i].ob_width > wdesk)
			desktop[i].ob_x = wdesk-desktop[i].ob_width;
		if(desktop[i].ob_y + desktop[i].ob_height > hdesk)
			desktop[i].ob_y = hdesk-desktop[i].ob_height;
	}

	for(i=DESKICN1;i<=DESKICN7;i++)
	{
		desktop[i].ob_state = NORMAL;
		desktop[i].ob_flags = HIDETREE;
	}

	for(i=DESKICN1;i<=DESKICNB;i++) /* auf 8 Pixel justieren */
	{
		desktop[i].ob_x=align(desktop[i].ob_x,8);
		desktop[i].ob_y=align(desktop[i].ob_y,8);
	}

	form_fix(desktop,0);

	/* aktuelles Trashcanicon auswÑhlen */
	desktop[DESKICND].ob_flags|=HIDETREE;
	desktop[DESKICND].ob_x=desktop[DESKICN8].ob_x;
	desktop[DESKICND].ob_y=desktop[DESKICN8].ob_y;
	if(divmenu[DIVPAPER].ob_state & SELECTED)
		inst_trashcan_icon(desktop,DESKICN8,DESKICND,FALSE);

	/* aktuelles Klemmbretticon auswÑhlen */
	desktop[DESKICNC].ob_flags|=HIDETREE;
	desktop[DESKICNC].ob_x=desktop[DESKICNB].ob_x;
	desktop[DESKICNC].ob_y=desktop[DESKICNB].ob_y;
	inst_clipboard_icon(desktop,DESKICNB,DESKICNC,FALSE);
/*
	if(exitcode>0) /* evtl. geÑnderten Exitcode anzeigen */
	{
		*((char *)winmenu[WINQUIT].ob_spec.index+10L)='m';
		*((char *)winmenu[WINQUIT].ob_spec.index+11L)='i';
		*((char *)winmenu[WINQUIT].ob_spec.index+12L)='t';
		*((char *)winmenu[WINQUIT].ob_spec.index+13L)=' ';
		*((char *)winmenu[WINQUIT].ob_spec.index+14L)=(char)(exitcode+'0');
	}
*/
	form_fix(userimg,0);
	form_fix(shell2,TRUE);
	form_fix(winmenu,0);
	form_fix(copyinfo,TRUE);
	form_fix(findmenu,TRUE);
	form_fix(tabmenu,TRUE);
	form_fix(shellmenu,TRUE);
/*
	form_fix(progress,TRUE);
*/
	form_fix(infomenu,TRUE);
	form_fix(shareware,TRUE);
	form_fix(gotomenu,TRUE);
	form_fix(prtmenu,TRUE);
	form_fix(fontmenu,TRUE);
	form_fix(fkeymenu,TRUE);

	for(i=SFKEY1; i<=SFKEY10; i++)
		fkeymenu[i].ob_flags &= ~EDITABLE;
/* MT 2.10.94 erst fixen, dann hiden*/
	fkeymenu[FKSHIFT].ob_flags|=HIDETREE;

	form_fix(umbrmenu,TRUE);
	form_fix(layout,TRUE);
	form_fix(pinstall,TRUE);
	form_fix(preview,TRUE);
	form_fix(replmenu,TRUE);
	form_fix(markmenu,TRUE);
	form_fix(grepmenu,TRUE);
	form_fix(divmenu,TRUE);
	form_fix(bracemenu,TRUE);
	form_fix(sortmenu,TRUE);
	form_fix(nummenu,TRUE);
	form_fix(picklist,TRUE);
	form_fix(menueditor,TRUE);
	form_fix(registmenu,TRUE);
	form_fix(formatmenu,TRUE);

/* da ist nichts zu fixen
	form_fix(listbox,TRUE);
*/

	for(i=FCHAR; i<=(FCHAR+255); i++)
	{
		chartable[i].ob_type |= (DFONT<<8); /* extended obtype setzen */
		chartable[i].ob_flags|= FLAGS15;	  /* wg. form_button() */
	}
	form_fix(chartable,TRUE);

	topwin=TRUE;
	checkmenu(winmenu,NULL);
	topwin=FALSE;
	menu_bar(winmenu,TRUE);

	/* geht nicht unter MTOS auf ST */
/*
	if((_AESversion>=0x0400) &&
	   (_wind_get(0,WF_NEWDESK,&hiword,&loword,&i,&i)!=0))
	{
		ob = (OBJECT *)(((long)hiword<<16)|(long)loword);
		if(ob)
		{
			desktop->ob_spec.obspec.interiorcol=ob->ob_spec.obspec.interiorcol;
			desktop->ob_spec.obspec.fillpattern=ob->ob_spec.obspec.fillpattern;
		}
	}
	else
*/
		desktop->ob_spec.obspec.interiorcol=GREEN;
	if(!nodesktop || pexec)		/* nur dann MÅll abrÑumen */
	{
		if(nodesktop)
			wind_set(0,WF_NEWDESK,0,0,0,0);
		else
		{
			ss = (unsigned int) &desktop;
			wind_set(0,WF_NEWDESK,ss[0],ss[1],0,0);
		}
/*		wind_set(0,WF_NEWDESK,nodesktop?NULL:desktop,0,0);*/
		_wind_get(0, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
		while( area[2] && area[3] )
		{
			if(nodesktop)
				form_dial(FMD_FINISH,0,0,0,0,area[0],area[1],area[2],area[3]);
			else
				objc_draw(desktop,ROOT,MAX_DEPTH,area[0],area[1],area[2],area[3]);
			_wind_get(0, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
		}
	}
	return(TRUE);
}
#pragma warn .par