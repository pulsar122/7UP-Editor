/*****************************************************************
	7UP
	Modul: EDITOR.C
	(c) by TheoSoft '90

	Texteingabe

	1997-03-26 (MJK):	void- und static-Deklarationen ergÑnzt
	1997-03-27 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
*****************************************************************/
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
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#	include <ext.h>
#else
#	include <osbind.h>
#endif

#include "macro.h"
#include "alert.h"
#include "7up.h"
#include "forms.h"
#include "windows.h"
#include "undo.h"
#include "language.h"
#include "resource.h"
#include "fileio.h"
#include "block.h"
#include "7up3.h"
#include "findrep.h"
#include "toolbar.h"
#include "numerik.h"
#include "deadkey.h"
#include "helpmsg.h"
#include "markblk.h"
#include "textmark.h"
#include "desktop.h"
#include "wind_.h"
#include "graf_.h"

#include "editor.h"

void _exit( int status ); /* Dieser Prototyp fehlt in vielen Bindings */

#ifndef TXT_NORMAL
#	define TXT_NORMAL 0x0000
#endif

#define CR 0x0D /* dÅrfen nicht verwendet werden */
#define LF 0x0A
#define VERTICAL	1
#define HORIZONTAL 2
#define DEZTAB '#'

LINESTRUCT *lastwstr;
long        lasthfirst,begline,endline;
WINDOW     *copywindow;
/*
long *ckbd = NULL; /*Compose Keyboard von Pascal Fellerich*/
*/

void mark_line(WINDOW *wp, LINESTRUCT *str, int line)
{																  /* zeile markieren */
	register int pxyarray[4],area[4],full[4],clip[4];

	clip[0]=(int)(wp->work.g_x - wp->wfirst + str->begcol*wp->wscroll);
	clip[1]=wp->work.g_y + line * wp->hscroll;
	clip[2]=(str->endcol-str->begcol)*wp->wscroll;
	clip[3]=wp->hscroll;

#if MiNT
   wind_update(BEG_UPDATE);
#endif
	_wind_get(0, WF_WORKXYWH, &full[0], &full[1], &full[2], &full[3]);
	_wind_get(wp->wihandle, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
	while( area[2] && area[3] )
	{
		if(rc_intersect(array2grect(full),array2grect(area)))
		{
			if(rc_intersect(array2grect(clip),array2grect(area)))
			{
				pxyarray[0]=area[0];
				pxyarray[1]=area[1];
				pxyarray[2]=area[0]+area[2]-1;
				pxyarray[3]=area[1]+area[3]-1;
				vs_clip(vdihandle,1,pxyarray);
				vr_recfl(vdihandle,pxyarray);		/* schwarz markieren */
			}
		}
		_wind_get(wp->wihandle, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
	}
	vs_clip(vdihandle,0,pxyarray);
#if MiNT
   wind_update(END_UPDATE);
#endif
}

#pragma warn -par
void refresh(register WINDOW *wp, LINESTRUCT *line, register int col, register int row)
{
	register GRECT rect;

	rect.g_x=wp->work.g_x + col * wp->wscroll;
	rect.g_y=wp->work.g_y + row * wp->hscroll;
	rect.g_w=wp->work.g_w - col * wp->wscroll;
	rect.g_h=wp->hscroll;
	Wredraw(wp,&rect);
}
#pragma warn .par

int ins_char(WINDOW *wp, register LINESTRUCT *line, int c) /* zeichen einfÅgen */
{
	register long abscol;
	char *help;
	if(line->used < STRING_LENGTH)
	{
		abscol=wp->col+wp->wfirst/wp->wscroll; /* absolute position */
		if(abscol<=line->used)					  /* hîchstens zeilenende */
		{
			if(!(line->used < line->len))
			{
				if((line->len += NBLOCKS) > STRING_LENGTH)
					line->len = STRING_LENGTH;
				help=realloc(line->string, line->len +1);
				if(help!=NULL)
					line->string=help;
				else
				{
					return(0);
				}
				line->string[line->used]=0;
			}
			if(wp->w_state & INSERT)									/* einfÅgen */
			{
				/* vom zeichen unter cursor rest nach rechts verschieben, insertmodus */
				memmove(&line->string[abscol+1],
						 &line->string[abscol],
						 strlen(&line->string[abscol]) +1);

				line->string[abscol]=c;			  /* zeichen setzen */
				line->used++;							/* lÑnge Ñndern */
				line->string[line->used]=0;		 /* ende setzen */
			}
			else											/* Åberschreiben */
			{
				line->string[abscol]=c;			  /* zeichen setzen */
				if(abscol==line->used)
				{
					line->used++;
					line->string[line->used]=0;	 /* ende setzen */
				}
			}
			return(1);
		}
	}
	return(0);
}

static int ins_dezimal(WINDOW *wp, register LINESTRUCT *line, int c) /* zeichen einfÅgen */
{
	register int abscol;
	register int i;
	
	if(line->used < STRING_LENGTH)
	{
		abscol=(int)(wp->col+wp->wfirst/wp->wscroll); /* absolute position */
		if(abscol<=line->used)					  /* hîchstens zeilenende */
		{
			for(i=abscol-1; i>=0; i--)
				if(line->string[i] == ' ')
				{
					memmove(&line->string[i],&line->string[i+1],abscol-i-1);
					line->string[abscol-1]=c;
					return(1);
				}
		}
	}
	return(0);
}

static int can_ins_dezimal(WINDOW *wp, register LINESTRUCT *line) /* prÅfen ob num-zeichen eingefÅgt werden kann */
{
	register int abscol;
	register int i;
	
	if(line->used < STRING_LENGTH)
	{
		abscol=(int)(wp->col+wp->wfirst/wp->wscroll); /* absolute position */
		if(abscol<=line->used)					  /* hîchstens zeilenende */
		{
			for(i=abscol-1; i>=0; i--)
				if(line->string[i] == ' ')
					return(1);
		}
	}
	return(0);
}

int ins_line(WINDOW *wp)			/* neue zeile einfÅgen */
{
	register LINESTRUCT *insline;
	register long abscol;
	char *help;

	if((insline=malloc(sizeof(LINESTRUCT)))==NULL) /* insert */
		return(0);
	insline->prev=0L;
	insline->next=0L;
	if((insline->string=(char *)malloc(NBLOCKS+1))==NULL)
	{
		free(insline);
		return(0);
	}
	insline->string[0]=0;
	insline->used=0;
	insline->len=NBLOCKS;
	insline->attr=0;
	insline->effect=TXT_NORMAL;

	if(wp->cstr) /* normale arbeitsweise */
	{
		abscol=wp->col+wp->wfirst/wp->wscroll; /* absolute position */
		if(wp->cstr->next)
		{
			insline->next=wp->cstr->next; /* ersten hinten verknÅpfen */
			wp->cstr->next->prev=insline;
		}
		wp->cstr->next=insline;			 /* dann vorne */
		insline->prev=wp->cstr;
		if(abscol < wp->cstr->used)
		{
			help=realloc(insline->string,strlen(&wp->cstr->string[abscol])+1);
			if(help != NULL)
				insline->string=help;
			else
			{
				form_alert(1,Aeditor[0]);
				_exit(-1); /* ohne atexit Vektor */
			}
			strcpy(insline->string,&wp->cstr->string[abscol]);
			insline->used=insline->len=(int)strlen(insline->string);
			wp->cstr->string[abscol]=0;
			wp->cstr->used=(int)strlen(wp->cstr->string);
			refresh(wp,wp->cstr,wp->col,wp->row);
		}
		wp->col=0;
		wp->cstr=insline;
	}
	else /* anfang des textes erzeugen */
	{
		wp->cstr=wp->wstr=wp->fstr=insline;
	}
	wp->hsize+=wp->hscroll;
	return(1);
}

static int cat_line(WINDOW *wp)
{
	register int i,diff;
	register char save;
	register long hfirst;
	void *temp;

	if(wp->cstr->prev)
	{
		if(wp->cstr->prev->used==0)
		{
			if(wp->cstr==wp->wstr) /* falls erste Zeile im Fenster */
			{
/* Koos Kuil 26.04.93 */
				if(Warrow(wp,WA_UPLINE))
				{
/* 
					wp->cstr=wp->cstr->next;
					wp->row=1;
*/
				}
			}
			wp->cstr=wp->cstr->prev;	  /* einfach vorherige zeile lîschen */
			wp->col=0;
			wp->row--;
			hfirst=wp->hfirst;						  /* letzte position merken */
			del_line(wp);
			if(wp->hfirst!=hfirst)		  		/* wurde anfang zurÅckversetzt */
			{
				if(wp->cstr->next)
				{
					wp->cstr=wp->cstr->next;	/* kann cursor versetzt werden */
					wp->row++;
				}
			}
			return(0);
		}
		else
		{
			if(wp->cstr==wp->wstr) /* falls erste Zeile im Fenster */
			{
/* Koos Kuil 26.04.93 */
				if(Warrow(wp,WA_UPLINE))
				{
/*
					wp->cstr=wp->cstr->next;
					wp->row=1;
*/
				}
			}
			if(wp->cstr->used > 0) /* umkopieren */
			{
				if((temp=realloc(wp->cstr->prev->string,wp->cstr->prev->used+wp->cstr->used+1))==NULL)
					return(0);

				wp->cstr->prev->string=temp;

				diff = STRING_LENGTH - wp->cstr->prev->used;
				wp->col = wp->cstr->prev->used; /* cursor in die spalte hinter zeile */
				wp->row--;
				if(wp->cstr->used > diff)
				{
					save=wp->cstr->string[diff];
					wp->cstr->string[diff]=0;
				}
				strcat(wp->cstr->prev->string,wp->cstr->string);
				wp->cstr->prev->len=wp->cstr->prev->used=
					(int)strlen(wp->cstr->prev->string);
				if(wp->cstr->used > diff)
				{
					wp->cstr->string[diff]=save;
				}
				wp->cstr=wp->cstr->prev;
				refresh(wp, wp->cstr,wp->col,wp->row);
				wp->cstr=wp->cstr->next;
				wp->row++;
				if(wp->cstr->used > diff)
				{
					strcpy(wp->cstr->string,&wp->cstr->string[diff]);
					wp->cstr->used=(int)strlen(wp->cstr->string);
					refresh(wp, wp->cstr, 0, wp->row );
					wp->cstr=wp->cstr->prev;
				}
				else
				{
					hfirst=wp->hfirst;					  /* letzte position merken */
					temp=wp->cstr->next;
					del_line(wp);
					if(wp->hfirst==hfirst)		  /* wurde anfang zurÅckversetzt */
					{
						if(wp->cstr->prev && wp->cstr==temp)
						{
							wp->cstr=wp->cstr->prev;	/* kann cursor versetzt werden */
							wp->row--;
						}
					}
				}
			}
			else
			{
				hfirst=wp->hfirst;					  /* letzte position merken */
				temp=wp->cstr->next;
				del_line(wp);
				wp->col=wp->cstr->used;
				if(wp->hfirst==hfirst)		  /* wurde anfang zurÅckversetzt */
				{
					if(wp->cstr->prev && wp->cstr==temp)
					{
						wp->cstr=wp->cstr->prev;	/* kann cursor versetzt werden */
						wp->col=wp->cstr->used;
						wp->row--;
					}
				}
			}
			if(wp->col > wp->work.g_w/wp->wscroll-1)
			{
				i=wp->col/(wp->work.g_w/wp->wscroll);
				wp->col -= i*wp->work.g_w/wp->wscroll;
				while(i--)
					Warrow(wp,WA_RTPAGE);
			}
		}
	}
	return(0);
}

static int backspace(WINDOW *wp, register LINESTRUCT *line)
{											 /* zeichen links vom cursor lîschen */
	register long abscol;

	abscol=wp->col+wp->wfirst/wp->wscroll - 1; /* zeichen vor dem cursor, absolute position */

	/* vom zeichen unter cursor ein zeichen nach links schieben, zeichen...*/
	/* ... wird dabei automatisch gelîscht */
	strcpy(&line->string[abscol],&line->string[abscol+1]);
	line->used--;							  /* lÑnge kÅrzen */
	return(1);
}

int del_char(WINDOW *wp, register LINESTRUCT *line)
{													  /* zeichen unter cursor lîschen */
	register long abscol;
	GRECT rect;

	abscol=wp->col+wp->wfirst/wp->wscroll; /* zeichen vor dem cursor, absolute position */
	if(!abscol && !line->used)
	{
		del_line(wp);
		return(1); /* 0 */
	}
	if(abscol < line->used)
	{
		strcpy(&line->string[abscol],&line->string[abscol+1]);
		line->used--;							  /* lÑnge kÅrzen */
		rect.g_x=wp->work.g_x + wp->col * wp->wscroll;
		rect.g_y=wp->work.g_y + wp->row * wp->hscroll;
		rect.g_w=wp->work.g_w - wp->col * wp->wscroll;
		rect.g_h=wp->hscroll;
		Wscroll(wp,HORIZONTAL,wp->wscroll,&rect);
		return(1); /* 0 */
	}
	else
		return(0);
}

int del_line(WINDOW *wp)
{
	LINESTRUCT *begcut, *endcut;
	int savecol;

	if(! wp->cstr->prev && !wp->cstr->next) /* erste und einzige zeile */
	{
		wp->cstr->string[0]=0;
		wp->cstr->used=0;
		refresh(wp,wp->cstr,0,wp->row);
	}
	else
	{
		lastwstr=wp->wstr;
		lasthfirst=wp->hfirst;
		begcut=endcut=wp->cstr;	 /* anfang = ende */
		begline=wp->row+wp->hfirst/wp->hscroll;  /* zeilen = 1 */
		endline=begline+1;
		begcut->begcol=endcut->begcol=0;
		begcut->endcol=endcut->endcol=STRING_LENGTH;

		savecol=wp->col;
		cut=cut_blk(wp, begcut, endcut);	  /* cuten */
		free_blk(wp,begcut);					 /* hwds! */
		wp->col=savecol;
	}
	return(0);
}

static void del_eoln(WINDOW *wp)
{
	wp->cstr->string[wp->col+wp->wfirst/wp->wscroll]=0;
	wp->cstr->used=(int)strlen(wp->cstr->string);
	refresh(wp, wp->cstr, wp->col, wp->row);
}

int adjust_best_position(WINDOW *wp) /* evtl. unten anpassen */
{
	register int i,fline,lline;
	if(wp->hfirst!=0 && wp->hfirst + wp->work.g_h > wp->hsize)
	{
		lline=(int)(wp->hfirst/wp->hscroll);	 /* erste zeile merken */
		wp->hfirst = wp->hsize-wp->work.g_h; /* neu setzen */
		if(wp->hfirst<0)
			wp->hfirst=0;
		fline=(int)(wp->hfirst/wp->hscroll);	  /* neue erste zeile */
		if(fline<lline)  /* kann nur kleiner sein, weil zurÅckpositioniert */
			for(i=fline; i<lline; i++)	 /* zeiger zurÅck setzen */
			{
				wp->wstr=wp->wstr->prev;	/* window und cursor */
			}
		return(1);
	}
	return(0);
}

#pragma warn -par
int hndl_umbruch(OBJECT *tree, WINDOW *wp, int start)
{
   int umbruch=0;
	if(wp)
	{
		sprintf(tree[UMBRUCH].ob_spec.tedinfo->te_ptext,"%d",wp->umbruch-1);
		if(form_exhndl(tree,0,0)==UMBRABBR)
			return(wp->umbruch);
		umbruch=atoi(form_read(tree,UMBRUCH,alertstr))+1;
		if(umbruch<2)
			umbruch=2;
		if(umbruch>STRING_LENGTH)
			umbruch=STRING_LENGTH;
	}
	return(umbruch);
}
#pragma warn .par

void blockformat(char *str, int diff) /* mit blanks expandieren */
{
	register int i,k,endlos=1;

	if(diff<0)
		return;

	k=(int)strlen(str);
	do
	{
		for(i=0; i<k; i++) /* fÅhrende Blanks Åberspringen */
			if(str[i]!=' ')
				break;
		for(/*i*/; i<k && diff; i++)
		{
			if(str[i]==' ' /*|| str[i]=='\t'*/)
			{
				memmove(&str[i+1],&str[i],strlen(&str[i])+1);
				str[i]=' ';
				i+=2;
				k++;
				diff--;
				endlos=0;
			}
		}
		if(endlos)
		  break;
	}
	while(diff>0 && !endlos);
}

int findlastspace(char *str, int umbr, int abs)
{
	int saveabs;
	saveabs = abs;
	
	while(abs >= umbr) /* zurÅck zum Umbruch */
		abs--;
	while(str[abs] != ' ' && str[abs]!='\t' && abs>0)
		abs--;
	if(abs==0)
	{
		while(saveabs > umbr) /* zurÅck zum Umbruch */
		{
			if(str[saveabs] == ' ')
				return(saveabs);
			else
				saveabs--;
		}
	}
	return(abs/*?abs:umbr*/); /* 10.7.94 */
}

static int ispossible(LINESTRUCT *line)  /* beim zweiten RETURN kein EinrÅcken */
{
	int i;
	if(line)
	{
		for(i=0; i<line->used; i++)
			if(line->string[i]!=' ')
				return(1);
		return(0);
	}
	return(1);
}

int isgerman(char key)
{
	char *cp;
	static char german[]=
	{
		'Ñ','é','î','ô','Å','ö','û','·','_',0
	};
	cp=german;
	do
	{
		if(key == *cp)
			return(1);
	}
	while(*++cp);
	return(0);
}

static void _editor(register WINDOW *wp, int state, int key, LINESTRUCT **begcut, LINESTRUCT **endcut)
{
	Wcursor(wp);
	editor(wp,state,key,begcut,endcut);
	graf_mouse_on(0);
	Wcursor(wp);
}

void editor(register WINDOW *wp, int state, int key, LINESTRUCT **begcut, LINESTRUCT **endcut) /* zeicheneingabe auswerten */
{
	register int abscol,i,nexttab,prevtab;
	int /*errcode=0,*/col,block=0;
	GRECT rect;
	char *cp;

	static int dk[2]={0,0};

	if(wp)
	{
		/* markierten Block gleich ersetzen. Sichern in UNDOpuffer */
		if((char)key != 0x1B)	/* != ESC */
		{	
	      /* aktuelle Zeile fÅr Zeilenundo sichern */
			if((undo.cp  != wp->cstr->string) &&
				(undo.row != wp->hfirst/wp->hscroll+wp->row)) /* fÅr undo string retten */
			{	/* auch row merken, weil sich beim einfÅgen adresse Ñndert */
				strcpy(undo.string,wp->cstr->string);
				undo.cp =wp->cstr->string;
				undo.col=wp->col;
				undo.row=(int)(wp->row+wp->hfirst/wp->hscroll);
			}
			if(!cut && (*begcut) && (*endcut)) /* markierten Block lîschen */
			{
            if((*begcut)==wp->fstr && (*endcut)->next==NULL)
            {
               if(form_alert(2,Aeditor[6])==1)
                  return /*errcode*/;
            }
     			if((*begcut) == (*endcut))
     			{
     			   if(endline != begline) /* ganze Zeile */
       			   undo.item=LINEPAST;
     			   else
     			      undo.item=LINEUNDO;
     			}
     			else
     			   undo.item=LINEPAST;
				hndl_blkfind(wp,(*begcut),(*endcut),SEARBEG);
				graf_mouse_on(0);
				Wcursor(wp);
   			free_undoblk(wp,undo.blkbeg); /* Block weg */
				if((wp->w_state&COLUMN))
				{
					cut=cut_col(wp,(*begcut),(*endcut));
               undo.flag=copy_col(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
				}
				else
				{
					cut=cut_blk(wp,(*begcut),(*endcut));
               undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
				}
				Wcuron(wp);
				Wcursor(wp);
/*
				graf_mouse_on(1);
*/
   			free_blk(wp,(*begcut)); /* Block weg */
   			if(undo.item==LINEPAST)
   			{
			   	strcpy(undo.string,wp->cstr->string); /* nochmal fÅr LINEPASTE */
		   		undo.cp =wp->cstr->string;
	   			undo.col=wp->col;
   				undo.row=(int)(wp->row+wp->hfirst/wp->hscroll);
				}
				block=1;
			}
      }
		if(!(state & (K_CTRL|K_ALT)) && !(key & 0x8000))/* keine sondertaste (control) */
		{
			graf_mouse_on(0);
			Wcursor(wp);
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
			switch(key)
			{
				case 0x0009:	/*  tab  */
					wp->w_state|=CHANGED; /* es wurde editiert */
					if(!tabexp)
						goto INS_CHAR;
					if(!block)
					{
						undo.item=LINEUNDO;
						if(tabbar)
						{
							nexttab=Wgetnexttab(wp);
						}
						else
						{
							nexttab=wp->tab-(wp->col%wp->tab);
							nexttab=(nexttab ? nexttab : wp->tab);
						}
						if(wp->w_state & INSERT)
						{
							for(i=0; i<nexttab; i++)
							{
								if(ins_char(wp,wp->cstr,' '))
								{
									refresh(wp, wp->cstr, wp->col , wp->row);
									if(++wp->col > wp->work.g_w/wp->wscroll-1) /* scrollen */
									{
										wp->col--;
										if(Warrow(wp,WA_RTLINE))
											wp->col++;	  /*-=4;*/
									}
								}
							}
						}
						else
						{
							if((wp->col+=nexttab) > wp->work.g_w/wp->wscroll-1) /* scrollen */
							{
								wp->col-=nexttab;
								if(Warrow(wp,WA_RTLINE))
									/*wp->col++*/;  /*-=4;*/
							}
						}
					}
					break;
				case 0x0209:	/*  shift tab : eigentlich quatsch */
					if(tabbar)
					{
						prevtab=Wgetprevtab(wp/*, wp->toolbar*/);
					}
					else
					{
						prevtab=wp->col%wp->tab;
						prevtab=(prevtab ? prevtab : wp->tab);
					}
					wp->col-=prevtab;
					break;
				case 0x000D: /* RETURN */
				case 0x020D: /* S " */
				case 0x400D: /* ENTER auf Zehnerblock */
				case 0x420D: /* S " */
					undo.item=0;
					if((wp->w_state & INSERT) || !wp->cstr->next)
					{
						if(ins_line(wp))
						{
							wp->w_state|=CHANGED; /* es wurde editiert */
							if(++wp->row > wp->work.g_h/wp->hscroll-1 )/* scrollen */
							{
								refresh(wp,wp->cstr,0,wp->row);
								Warrow(wp,WA_DNLINE);
								Wslide(wp,0,HSLIDE);
								wp->col=0;
							}
							else
							{
								rect.g_x=wp->work.g_x;
								rect.g_y=wp->work.g_y + (wp->row) * wp->hscroll;
								rect.g_w=wp->work.g_w;
								rect.g_h=wp->work.g_h - (wp->row) * wp->hscroll;
								Wscroll(wp,VERTICAL,-wp->hscroll,&rect);
								Wslide(wp,0,HSLIDE);
								wp->col=0;
							}
							if((wp->w_state & INDENT) && ispossible(wp->cstr->prev)) /* cursor einrÅcken, auto-indent */
							{
								for(i=0; i<wp->cstr->prev->used && wp->cstr->prev->string[i] == ' '; i++)
									if(ins_char(wp,wp->cstr,' '))
									{
										wp->col++;
									}
								refresh(wp, wp->cstr, 0, wp->row);
								if(wp->col==0) /* nicht eingerÅckt */
							      undo.item=BACKSPACE;
							}
						}
					}
					else
					{
						wp->col=0;
						if(wp->cstr->next)
						{
							if(++wp->row > wp->work.g_h/wp->hscroll-1 )/* scrollen */
							{
								Warrow(wp,WA_DNLINE);
							}
							else
								wp->cstr=wp->cstr->next; /* Cursor vor	 */
						}
					}
					undo.cp=NULL; /* lîschen, weil sonst fehlerhaft */
					undo.row=-1;
					break;
				case 0x007F: /* DELETE */
					wp->w_state|=CHANGED; /* es wurde editiert */
					if(!block)
					{
						undo.item=LINEUNDO;
						if(!del_char(wp,wp->cstr))
							if(wp->cstr->next)
							{
								undo.item=0;
								wp->cstr=wp->cstr->next;
								wp->col=0;
								wp->row++;
								cat_line(wp);
								undo.item=RETURN;
							}
					}
					break;
				case 0x0008: /* BACKSPACE */
				case 0x0208: /* S " */
					wp->w_state|=CHANGED; /* es wurde editiert */
					if(!block)
					{
						if(!(wp->wfirst == 0 && wp->col == 0))
						{
							undo.item=LINEUNDO;
							backspace(wp,wp->cstr);
							wp->col--;
							rect.g_x=wp->work.g_x + wp->col * wp->wscroll;
							rect.g_y=wp->work.g_y + wp->row * wp->hscroll;
							rect.g_w=wp->work.g_w - wp->col * wp->wscroll;
							rect.g_h=wp->hscroll;
							Wscroll(wp,HORIZONTAL,wp->wscroll,&rect);
						}
						else
						{
							undo.item=RETURN;
							cat_line(wp);
						}
					}
					break;
				case 0x001B: /* ESC */
/*
					if(ckbd) /*CKBD geladen, Åberlassen wirs diesem Treiber*/
						goto DEADKEY;
*/
					if (!scaktiv) /* Shortcuts nicht aktiv */
						goto NODEADKEY;
						
					if(dk[0]!=0x1B)
						dk[0]=0x1B;
					else
						dk[0]=0;
					break;
					
/* GEMDOS */default:
INS_CHAR:
					key=(char)key;
/*
					if(ckbd) /*CKBD geladen, Åberlassen wirs diesem Treiber*/
						goto DEADKEY;
*/
					if (!scaktiv)  /* Shortcuts nicht aktiv */
						goto NODEADKEY;
						
					if(dk[0]==0x1B) /* ESC = deadkey */
					{
						if(!dk[1])
						{
							dk[1]=isdeadkey(key);
							if(!dk[1])
							{
								dk[0]=0;
								goto NODEADKEY;
							}
						}
						else
						{
                     cp=NULL;
							key=deadkey(dk[1],key,&cp);
							dk[0]=dk[1]=0;
							if(key) /* einzelnes Zeichen */
							   goto NODEADKEY;
							if(cp)  /* Shortcutzeichenkette */
							{
                        col=-1; /* zurÅcksetzen auf ungÅltig */
								while((*cp!='\r') && (*cp!=0))
								{
									Wcursor(wp);
/*
									graf_mouse_on(1);
*/
                           if(*cp!='~')
   									editor(wp,0,(int)*cp,begcut,endcut);
   								else
   								   col=wp->col;
									graf_mouse_on(0);
									Wcursor(wp);
									cp++;
								}
								if(col>-1)    /* abfragen und setzen */
   								wp->col=col; /* Tilde in Shortcut */
 							}
						}
					}
					else
					{
NODEADKEY:
						if(!block)
							undo.item=LINEUNDO;
						if(key && key!=CR && key!=LF)
						{
							wp->w_state|=CHANGED; /* es wurde editiert  */
							if(umlautwandlung)    /* TeX-Umlautwandlung */
							{
								switch(key)
								{
									case 'Ñ':
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'a',begcut,endcut);
										goto ENDE;
										/*break;*/
									case 'é':
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'A',begcut,endcut);
										goto ENDE;
										/*break;*/
									case 'î':
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'o',begcut,endcut);
										goto ENDE;
										/*break;*/
									case 'ô':
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'O',begcut,endcut);
										goto ENDE;
										/*break;*/
									case 'Å':
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'u',begcut,endcut);
										goto ENDE;
										/*break;*/
									case 'ö':
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'U',begcut,endcut);
										goto ENDE;
										/*break;*/
									case 0x9E:    /*'û'*/
									case 0xE1:    /*'û'*/
										_editor(wp,0,'"',begcut,endcut);
										_editor(wp,0,'s',begcut,endcut);
										goto ENDE;
										/*break;*/
/*
									case 'Ñ':
										_editor(wp,0,'a',begcut,endcut);
										_editor(wp,0,'e',begcut,endcut);
										goto ENDE;
										break;
									case 'é':
										_editor(wp,0,'A',begcut,endcut);
										_editor(wp,0,'e',begcut,endcut);
										goto ENDE;
										break;
									case 'î':
										_editor(wp,0,'o',begcut,endcut);
										_editor(wp,0,'e',begcut,endcut);
										goto ENDE;
										break;
									case 'ô':
										_editor(wp,0,'O',begcut,endcut);
										_editor(wp,0,'e',begcut,endcut);
										goto ENDE;
										break;
									case 'Å':
										_editor(wp,0,'u',begcut,endcut);
										_editor(wp,0,'e',begcut,endcut);
										goto ENDE;
										break;
									case 'ö':
										_editor(wp,0,'U',begcut,endcut);
										_editor(wp,0,'e',begcut,endcut);
										goto ENDE;
										break;
									case 0x9E:    /*'û'*/
									case 0xE1:    /*'û'*/
										_editor(wp,0,'s',begcut,endcut);
										_editor(wp,0,'s',begcut,endcut);
										goto ENDE;
										break;
*/
								}
							}
							if(isnum(key) && (Wgettab(wp)==DEZTAB) && can_ins_dezimal(wp,wp->cstr))
							{
								if(ins_dezimal(wp,wp->cstr,key))
								{
									rect.g_x=wp->work.g_x;
									rect.g_y=wp->work.g_y + wp->row * wp->hscroll;
									rect.g_w=wp->work.g_w;
									rect.g_h=wp->hscroll;
									Wredraw(wp,&rect);
								}
							}
							else
							{
								if(ins_char(wp,wp->cstr,key))
								{
									if((wp->w_state & INSERT) && (wp->col+wp->wfirst/wp->wscroll < wp->cstr->used-1))
									{
										rect.g_x=wp->work.g_x + wp->col * wp->wscroll;
										rect.g_y=wp->work.g_y + wp->row * wp->hscroll;
										rect.g_w=wp->work.g_w - wp->col * wp->wscroll;
										rect.g_h=wp->hscroll;
										Wscroll(wp,HORIZONTAL,-wp->wscroll,&rect);
									}
									else
									{
										rect.g_x=wp->work.g_x + wp->col * wp->wscroll;
										rect.g_y=wp->work.g_y + wp->row * wp->hscroll;
										rect.g_w=wp->wscroll;
										rect.g_h=wp->hscroll;
										Wredraw(wp,&rect);
									}
									wp->col++;
								}
							}
  							col=abscol=(int)(wp->col+wp->wfirst/wp->wscroll);
							if(abscol >= wp->umbruch)
							{
								abscol=findlastspace(wp->cstr->string, wp->umbruch, abscol);
								if(abscol>0)
								{
									wp->cspos=wp->col=(int)(abscol-wp->wfirst/wp->wscroll+1);
									wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
									Wcursor(wp);
/*
									graf_mouse_on(1);
*/
									editor(wp,0,0x000D,begcut,endcut);
									graf_mouse_on(0);
									Wcursor(wp);

									wp->col+=(int)((col-abscol)-wp->wfirst/wp->wscroll-1);

									if(wp->w_state & BLOCKSATZ)
									{
										blockformat(wp->cstr->prev->string, wp->umbruch-wp->cstr->prev->used);
										wp->cstr->prev->used=(int)strlen(wp->cstr->prev->string);
										refresh(wp, wp->cstr->prev, 0, wp->row-1);
									}
								}
							}
						}
					}
					break;
			}
ENDE:
			wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
	      Wcuron(wp);
			Wcursor(wp);
/*
			graf_mouse_on(1);
*/
		}
	}
	/*return(errcode);*/
}

WINDOW *twp;
void hndl_chartable(WINDOW *wp, OBJECT *tree)
{
	int c,ret,kstate,ende;
	LINESTRUCT *dummy=NULL;
	
	if(wp)
	{
		ende = 0;
		twp=wp;
		form_exopen(tree,0);
		do
		{
			c=(form_exdo(tree,0)&0x7FFF);
			switch(c)
			{
				case CHARHELP:
					form_alert(1,Aeditor[5]);
					objc_change(tree,c,0,tree->ob_x,tree->ob_y,
						tree->ob_width,tree->ob_height,tree[c].ob_state&~SELECTED,1);
					break;
				case CHAROK:
					ende = 1;
					break;
				default:
					graf_mkstate(&ret,&ret,&ret,&kstate);
					if(kstate&(K_LSHIFT|K_RSHIFT))
					{
						sprintf(alertstr,Aeditor[1],
							(c-FCHAR)&0xff,(c-FCHAR)&0xff,(c-FCHAR)&0xff);
						if(form_alert(2,alertstr) == 2)
						{
							ende = 1;
						}
					}
					else
					{
						ende = 1;
					}
					objc_change(tree,c,0,
						tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,
						NORMAL,1);
					break;
			}
		}
		while(!ende);
		form_exclose(tree,c,0);
		tree[FCHAR+256].ob_state&=~SELECTED;

		Wdefattr(wp);

		if(c>0 && c<(FCHAR+256))
			editor(wp,0,(c-FCHAR)&0xff,&dummy,&dummy);
	}
}

static MEVENT mevent=
{
	MU_TIMER|MU_KEYBD,
	2,1,1,
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

/* mit ALT-Taste Dezimalwert eingeben, nur GEMDOS */
int altnum(int *state, int *key)
{
	char num[4];
	register int i=1,event;
/*
	if(ckbd) return(0); /*Wenn Compose Keyboard geladen ist, raus.*/
*/
/* geht auch dann nicht
	if(_GemParBlk.global[0]>=0x0400) /* Falcon TOS kanns selbst */
		return(0);
*/						 /* & geht nicht wg. Shift*/
	if((*state == K_ALT) && isdigit(*key))
	{  /*  1		2		3		4  */
		num[0]=num[1]=num[2]=num[3]=0;
		num[0]=(char)(*key);
      mevent.e_time=0;
		do
		{
			 event=mevnt_event(&mevent);
			 if(event & MU_KEYBD)
			 {
				 if(!isdigit(mevent.e_kr))
					 return(0);
				 num[i++]=(char)mevent.e_kr;
			 }
		}
		while(i<4 && (mevent.e_ks == K_ALT));
		if((i=atoi(num))>0)
		{
			*state=0;
			*key=i;
			return(1);
		}
	}
	return(0);
}

int special(WINDOW *wp, WINDOW **blkwp, int state, int key, LINESTRUCT **begcut, LINESTRUCT **endcut)
{
	LINESTRUCT *line;
	int i,mx,my,len,used,wordend;
	char *cp;
	extern int exitcode;
	extern char errorstr[];
	extern OBJECT *markmenu,*divmenu;
	extern WINDOW _wind[];
	/*GRECT rect;*/
   
	if(wp) /* ab jetzt nur mit offenem Fenster */
	{
		if(!state)
		{
			switch(key)
			{
				case 0x8052: /* INSERT */
               graf_mouse_on(0);
               Wcursor(wp);
					menu_icheck(winmenu,FORMINS,(wp->w_state^=INSERT)&INSERT?1:0);
					Wcuron(wp);
               Wcursor(wp);
/*
               graf_mouse_on(1);
*/
					return(1);
				case 0x8061: /* UNDO */
					do_undo(wp);
					return(1);
				case 0x8062: /* HELP */
					if(!help())
					{
                  sprintf(alertstr,Aeditor[2],(char *)(divmenu[DIVHDA].ob_spec.index/*+16L*/));
						form_alert(1,alertstr);
					}
					return(1);
			}
			return(0);
		}
		if(state & (K_LSHIFT | K_RSHIFT))
		{
			switch(key)
			{
				case 0x8252: /* Shift Insert */
					return(1);
				case 0x8261: /* Shift Undo */
					return(1);
				case 0x8262: /* Shift Help = Compilerfehlermeldung anzeigen */
					if(*errorstr=='\"' && errorstr[strlen(errorstr)-1]=='\"')
					{
						if(wp->kind & INFO)
							wind_set(wp->wihandle,WF_INFO,errorstr);
						else
							form_alert(1,Aeditor[3]);
					}
					else
						form_alert(1,Aeditor[4]);
					return(1);
				case 0x027F: /* Shift DELETE */
					if(cut)
						free_blk(wp,(*begcut));
					if(!wp->cstr->next && !wp->cstr->used)
						return(1); /* die letzte Zeile ist leer, beenden! */
					undo.item=0;
					if(!(*begcut) && !(*endcut))
					{
						mx=wp->work.g_x+wp->col*wp->wscroll+1;
						my=wp->work.g_y+wp->row*wp->hscroll+1;
						if(Wdclickline(wp,begcut,endcut,mx,my))
						{
							graf_mouse_on(0);
							line=wp->cstr->next;
							if(line)
							{
	   						Wcursor(wp);
								free_undoblk(wp, undo.blkbeg);
								cut=cut_blk(wp,(*begcut),(*endcut));
								undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
								wp->col=(int)(-wp->wfirst/wp->wscroll);
								wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
								/* ^Y Korrektur am Textende */
								if(wp->cstr != line && wp->cstr->next)
									wp->cstr=wp->cstr->next;
								Wcuron(wp);
								Wcursor(wp);
								free_blk(wp,(*begcut));
							}
							else /* Cursor in letzter Zeile */
							{
	   						Wcursor(wp);
								free_undoblk(wp, undo.blkbeg);
								undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
								wp->col=(int)(-wp->wfirst/wp->wscroll);
								wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
							   wp->cstr->used=0;
							   wp->cstr->string[0]=0;
   							refresh(wp, wp->cstr, wp->col, wp->row);
   							Wcuron(wp);
	   						Wcursor(wp);
								hide_blk(wp,*begcut,*endcut);
							}
							undo.menu=WINEDIT;
							undo.item=EDITPAST;
/*
							graf_mouse_on(1);
*/
							return(1);
						}
					}
				   break;
			}
			return(0);
		}
		if(state & K_CTRL)
		{
/* 1.10.94 Dialog
			switch(key) /* ^F8 ist fast wie ^a, deshalb hier*/
			{
				/* Textformatierung ^F8  = linksbÅndig */
				/*                  ^F9  = zentriert   */
				/*                  ^F10 = rechtsbÅndig */
				case 0x8442: /* ^F8  */
				case 0x8443: /* ^F9  */
				case 0x8444: /* ^F10 */
					textformat2(wp, begcut, endcut, key, ...);
					return(1);
					break;
			}
*/
			switch(__tolower((char)key))
			{
			   case 'y':
					if(cut)
						free_blk(wp,(*begcut));
					if(!wp->cstr->next && !wp->cstr->used)
						return(1); /* die letzte Zeile ist leer, beenden! */
					undo.item=0;
					if(!(*begcut) && !(*endcut))
					{
						mx=wp->work.g_x+wp->col*wp->wscroll+1;
						my=wp->work.g_y+wp->row*wp->hscroll+1;
						if(Wdclickline(wp,begcut,endcut,mx,my))
						{
							graf_mouse_on(0);
							line=wp->cstr->next;
							if(line)
							{
	   						Wcursor(wp);
								free_undoblk(wp, undo.blkbeg);
								cut=cut_blk(wp,(*begcut),(*endcut));
								undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
								wp->col=(int)(-wp->wfirst/wp->wscroll);
								wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
								/* ^Y Korrektur am Textende */
								if(wp->cstr != line && wp->cstr->next)
									wp->cstr=wp->cstr->next;
								Wcuron(wp);
								Wcursor(wp);
								
								if(clipbrd)
								{
									mevent.e_time=250;
   								if(!(mevnt_event(&mevent)&MU_KEYBD))
	   								write_clip(wp,(*begcut),(*endcut));
									free_blk(wp,(*begcut));
								}
							}
							else /* Cursor in letzter Zeile */
							{
	   						Wcursor(wp);
								free_undoblk(wp, undo.blkbeg);
								undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
								wp->col=(int)(-wp->wfirst/wp->wscroll);
								wp->cspos=Wshiftpage(wp,0,wp->cstr->used);
							   wp->cstr->used=0;
							   wp->cstr->string[0]=0;
							   wp->w_state|=CHANGED;
   							refresh(wp, wp->cstr, wp->col, wp->row);
   							Wcuron(wp);
	   						Wcursor(wp);
   							if(clipbrd)
   							{
                	mevent.e_time=250;
   								if(!(mevnt_event(&mevent)&MU_KEYBD))
   		   						write_clip(wp,(*begcut),(*endcut));
								}
								hide_blk(wp,*begcut,*endcut);
							}
							undo.menu=WINEDIT;
							undo.item=EDITPAST;
/*
							graf_mouse_on(1);
*/
							return(1);
						}
					}
					break;
				case 'e': /* E */
					if(wp==*blkwp && *begcut && !(*endcut))  /* Reihenfolge NICHT vertauschen */
					{
						graf_mouse_on(0);
						Wcursor(wp);
						*endcut=end_blk(wp,begcut,endcut);
						Wcuroff(wp);
						Wcursor(wp);
/*
						graf_mouse_on(1);
*/
						if(*endcut==NULL ||						  /* Fehler? oder	 */
						  ((*begcut)->begcol == (*begcut)->endcol)) /* gleiche Stelle? */
						{
							hide_blk(wp,*begcut,*endcut);
							*blkwp=NULL;
						}
						return(1);
					}
					break;
            case 'a': /* A bei SysKey */
				case 'b': /* B */
					if(!cut)								 /* gibt es erst noch mÅll zu lîschen */
						hide_blk(wp,*begcut,*endcut);
					else
						free_blk(wp,*begcut);
					if(!(*begcut) && !(*endcut))
					{
						*begcut=beg_blk(wp,*begcut,*endcut);
						*endcut=NULL;
						cut=0;
						*blkwp=wp;							 /* diesem Fenster gehîrt der Block */
						return(1);
					}
					break;
				case '9': /* Durchfallen und exitcode immer erhîhen */
					exitcode++;
				case '8':
					exitcode++;
				case '7': 
					exitcode++;
				case '6':
					exitcode++;
				case '5':
					exitcode++;
				case '4':
					exitcode++;
				case '3':
					exitcode++;
				case '2':
					exitcode++;
				case '1':
					exitcode++;
				case '0': /* CTRL-Ziffernblock = Exitcode */
					if(exitcode)
					{
                  graf_mouse_on(1);/* nur bei eventgesteuerter Maus */
/*
					   if(wp->w_state & CHANGED)
					   {
							if(!strcmp((char *)Wname(wp),NAMENLOS))
								write_file(wp,1);
							else
   		   			   write_file(wp,0);
   		   	   }
				   	for(i=1;i<MAXWINDOWS;i++)
			   		   _wind[i].w_state &= ~CHANGED;
*/
/* alles sichern MT 18.6.95 */
				   	for(i=1;i<MAXWINDOWS;i++)
			   		   if(_wind[i].w_state & CHANGED)
			   		   {
								if(!strcmp((char *)Wname(wp),NAMENLOS))
									write_file(wp,1);
								else
   			   			   write_file(wp,0);
				   		   _wind[i].w_state &= ~CHANGED;
			   		   }
		   			exit(exitcode);
					}
					break;
			}
			switch(key)
			{
            case 0x8452: /* ^Insert */
               return(1);
            case 0x8461: /* ^Undo */
               return(1);
            case 0x8462: /* ^Help */
               return(1);
				case 0x0408: /* ^BACKSPACE */
					if(cut)
						free_blk(wp,(*begcut));
					if(!(*begcut) && !(*endcut))
					{
						wp->w_state|=CHANGED; /* es wurde editiert */
						lastwstr=wp->wstr;
						lasthfirst=wp->hfirst;
						begline=wp->row+wp->hfirst/wp->hscroll;
						endline=begline;
						*begcut=*endcut=wp->cstr;
						(*begcut)->endcol=(*endcut)->endcol=(int)(wp->col+wp->wfirst/wp->wscroll);
						for(wordend=(*begcut)->endcol; wordend>0; wordend--)
							if((*begcut)->string[wordend]==' ')
								break;
						for(/*wordend*/; wordend>0; wordend--)
							if((*begcut)->string[wordend-1]!=' ')
								break;
						(*begcut)->begcol=(*endcut)->begcol=wordend;
						(*begcut)->attr  =((*endcut)->attr|=SELECTED);

						if((*begcut)->begcol==0 && (*begcut)->endcol==wp->cstr->used)
							endline++; /* Rest der Zeile */

						graf_mouse_on(0);
						Wcursor(wp);
/*GEMDOS*/			mark_line(wp,wp->cstr,wp->row);
						event_timer(25);
						cut=cut_blk(wp,(*begcut),(*endcut));
						free_undoblk(wp, undo.blkbeg);
						undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
						Wcuron(wp);
						Wcursor(wp);
/*
						graf_mouse_on(1);
*/
						undo.menu=WINEDIT;
						undo.item=EDITPAST;
						return(1);
					}
					break;
				case 0x047F: /* ^DELETE */
					if(cut)
						free_blk(wp,(*begcut));
					if(!(*begcut) && !(*endcut))
					{
						wp->w_state|=CHANGED; /* es wurde editiert */
						lastwstr=wp->wstr;
						lasthfirst=wp->hfirst;
						begline=wp->row+wp->hfirst/wp->hscroll;
						endline=begline;
						*begcut=*endcut=wp->cstr;
						(*begcut)->begcol=(*endcut)->begcol=(int)(wp->col+wp->wfirst/wp->wscroll);
						for(wordend=(*begcut)->begcol; wordend<wp->cstr->used; wordend++)
							if((*begcut)->string[wordend]==' ')
								break;
						for(/*wordend*/; wordend<wp->cstr->used; wordend++)
							if((*begcut)->string[wordend]!=' ')
								break;
						(*begcut)->endcol=(*endcut)->endcol=wordend;
						(*begcut)->attr  =((*endcut)->attr|=SELECTED);

						if((*begcut)->begcol==0 && (*begcut)->endcol==wp->cstr->used)
							endline++; /* Rest der Zeile */

						graf_mouse_on(0);
						Wcursor(wp);
/*GEMDOS*/			mark_line(wp,wp->cstr,wp->row);
						event_timer(25);
						cut=cut_blk(wp,(*begcut),(*endcut));
						free_undoblk(wp, undo.blkbeg);
						undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
						Wcuron(wp);
						Wcursor(wp);
/*
						graf_mouse_on(1);
*/
						undo.menu=WINEDIT;
						undo.item=EDITPAST;
						return(1);
					}
					break;
			}
			return(0);
		}
		if(state & K_ALT)
		{
			switch(__tolower((char)key))
			{
				case '1':				/* Textmarken anspringen */
				case '2':
				case '3':
				case '4':
				case '5':
					if(state & (K_LSHIFT|K_RSHIFT))
					{
						textmarker(wp,markmenu,0,state,(char)key-'1');
						return(1);
					}
					break;
				case '~': /* ALT~ = linetoggle */
				   if(wp->cstr->next)
				   {
						wp->w_state		|= CHANGED; /* es wurde editiert */
						used				 = wp->cstr->next->used;
						len				 = wp->cstr->next->len;
						cp					 = wp->cstr->next->string;
						wp->cstr->next->string = wp->cstr->string,
						wp->cstr->next->used	  = wp->cstr->used;
						wp->cstr->next->len	  = wp->cstr->len;
						wp->cstr->used	= used;
						wp->cstr->len	 = len;
						wp->cstr->string = cp;
						graf_mouse_on(0);
						Wcursor(wp);
						refresh(wp,wp->cstr,		  0,wp->row  );
						refresh(wp,wp->cstr->next,0,wp->row+1);
						Wcursor(wp);
/*
						graf_mouse_on(1);
*/
						undo.item=0;
						return(1);
					}
					break;
			}
			switch(key)
			{
/* 2.10.94
				/* Texteffekte */
				case 0x883E: /* ALT-F4 */
				case 0x883F: /* ALT-F5 */
				case 0x8840: /* ALT-F6 */
					switch(key)
					{
						case 0x883E: /* ALT-F4 */
							wp->cstr->effect  = TXT_NORMAL;
							break;
						case 0x883F: /* ALT-F5 */
							wp->cstr->effect ^= TXT_THICKENED;
							break;
						case 0x8840: /* ALT-F6 */
							wp->cstr->effect ^= TXT_SKEWED;
							break;
					}
					graf_mouse_on(0);
					Wcursor(wp);
					rect.g_x=wp->work.g_x;
					rect.g_y=wp->work.g_y + wp->row * wp->hscroll;
					rect.g_w=wp->work.g_w;
					rect.g_h=wp->hscroll;
					Wredraw(wp,&rect);
			      Wcuron(wp);
					Wcursor(wp);
/*
					graf_mouse_on(1);
*/
					return(1);				
*/
            case 0x8861: /* ALT-UNDO */
               return(1);
				case 0x087F: /* ALT-DELETE, bis zum Ende der Zeile lîschen */
					if(cut)
						free_blk(wp,(*begcut));
					if(!(*begcut) && !(*endcut))
					{
						wp->w_state|=CHANGED; /* es wurde editiert */
						lastwstr=wp->wstr;
						lasthfirst=wp->hfirst;
						begline=wp->row+wp->hfirst/wp->hscroll;
						endline=begline;
						*begcut=*endcut=wp->cstr;
						(*begcut)->begcol=(*endcut)->begcol=(int)(wp->col+wp->wfirst/wp->wscroll);
						(*begcut)->endcol=(*endcut)->endcol=wp->cstr->used;
						(*begcut)->attr  =((*endcut)->attr|=SELECTED);
						if((*begcut)->begcol == 0) /* ganze Zeile */
							endline++;
						graf_mouse_on(0);
						Wcursor(wp);
/*GEMDOS*/			mark_line(wp,wp->cstr,wp->row);
						event_timer(25);
						cut=cut_blk(wp,(*begcut),(*endcut));
						free_undoblk(wp, undo.blkbeg);
						undo.flag=copy_blk(wp,(*begcut),(*endcut),&undo.blkbeg,&undo.blkend);
						Wcuron(wp);
						Wcursor(wp);
/*
						graf_mouse_on(1);
*/
						undo.menu=WINEDIT;
						undo.item=EDITPAST;
						return(1);
					}
					break;
			}
			return(0);
		}
	}
	else
	{
		if(!state)
		{
			switch(key)
			{
				case 0x8062: /* Help */
					if(!help())
						form_alert(1,Aeditor[2]);
					return(1);
				case 0x8061: /* UNDO */
					desel_icons(desktop,DESKICN1,DESKICNC,1);
					return(1);
			}
			return(0);
		}
		if(state & K_CTRL)
		{
		   switch(__tolower((char)key))
		   {
				case '9': /* Durchfallen und exitcode immer erhîhen */
					exitcode++;
				case '8':
					exitcode++;
				case '7': 
					exitcode++;
				case '6':
					exitcode++;
				case '5':
					exitcode++;
				case '4':
					exitcode++;
				case '3':
					exitcode++;
				case '2':
					exitcode++;
				case '1':
					exitcode++;
				case '0': /* CTRL-Ziffernblock = Exitcode */
					if(exitcode)
					{
/*
             graf_mouse_on(1);/* nur bei eventgesteuerter Maus */
					   if(wp->w_state & CHANGED)
					   {
							if(!strcmp((char *)Wname(wp),NAMENLOS))
								write_file(wp,1);
							else
   		   			   write_file(wp,0);
   		   	   }
				   	for(i=1;i<MAXWINDOWS;i++)
			   		   _wind[i].w_state &= ~CHANGED;
		   			exit(exitcode);
*/
					}
					break;
			}
			return(0);
		}
		if(state & K_ALT)
		{
			return(0);
		}
	}
	return(0);
}
