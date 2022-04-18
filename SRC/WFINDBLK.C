/*****************************************************************
	7UP
	Modul: WFINDBLK.C
	(c) by TheoSoft '92

	Sucht und markiert Block bei wiederge”ffneten Fenster
	
	1997-04-08 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
*****************************************************************/
#include <stdio.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <aesbind.h>
#endif

#include "windows.h"
#include "undo.h"
#include "block.h"
#include "7up3.h"
#include "editor.h"

#include "wfindblk.h"

WINDOW *Wfindblk(WINDOW *wp, WINDOW **blkwp, LINESTRUCT **beg, LINESTRUCT **end)
{
	LINESTRUCT *line;		 /* markierten Block im neuen Fenster suchen */
	long bl=0L;

	if(wp && beg && end)
	{
		for(line=wp->fstr; line; line=line->next,bl++)
		{
			if(line->attr & SELECTED)
			{
				if(cut && *beg && *end)	  /* erst noch Ramblock l”schen */
					free_blk(wp,*beg);
				begline=endline=bl;
				lasthfirst=bl*wp->hscroll;
				lastwstr=line;

				(*beg)=(*end)=line;
				for(line=line->next; line && line->next; line=line->next)
				{
					if(!(line->attr & SELECTED))
					{
						if((*beg)->begcol==0 && (*end)->endcol==STRING_LENGTH)
							endline++;
						(*blkwp)=wp;
						return(wp);
					}
					else
					{
						endline++;
						(*end)=line;
					}
				}
				(*blkwp)=wp;
				return(wp);
			}
		}
		if(!cut) /* zumindest kein Block in diesem Fenster */
		{
			(*beg)=(*end)=NULL;
			begline=endline=0L;
		}
	}
	return(NULL);
}

int Wrestblk(WINDOW *wp, UNDO *undo, LINESTRUCT **beg, LINESTRUCT **end)
{
	register long i;
	LINESTRUCT *line;
	register int y;
	
	if(wp)
	{
		lasthfirst=undo->wline*wp->hscroll;
		begline=undo->begline;
		endline=undo->endline;
		line=wp->fstr;
		for(i=0; i<undo->wline; i++, line=line->next)
			;
		lastwstr=line;
		for(/*i*/; i<begline; i++, line=line->next)
			;
		*end=*beg=line;
		(*beg)->begcol=undo->begcol;
		(*beg)->endcol=undo->endcol;
		for(/*i*/; i<endline; i++, line=line->next)
		{
			(*end)=line;
			(*end)->attr|=SELECTED;
			(*end)->begcol=undo->begcol;
			(*end)->endcol=undo->endcol+(undo->blktype==COLUMN?undo->begcol:0);
		}
		if(undo->blktype==COLUMN)
			wp->cspos=(wp->col+=undo->endcol);
		if((undo->blktype!=COLUMN) && ((*beg) != (*end))) /* mehr als eine zeile */
		{
			(*beg)->endcol=STRING_LENGTH;
			(*end)->begcol=0;
			for(line=(*beg)->next; line && line != (*end); line=line->next)
			{
				line->begcol=0;
				line->endcol=STRING_LENGTH;
			}
		}
  		endline--;
		if((*beg)->begcol==0 && (*end)->endcol>=(*end)->used)
			endline++;
		for(line=wp->wstr,i=0,y=wp->work.g_y;
			 line && y<(wp->work.g_y+wp->work.g_h-1);
			 line=line->next, i++, y+=wp->hscroll)
			if(line->attr & SELECTED)
			{
				mark_line(wp,line,(int)i);
			}
		Wcuroff(wp);
		return(1);
	}
	return(0);
}
