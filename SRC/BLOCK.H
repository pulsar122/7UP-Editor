/*****************************************************************
	7UP
	Header: BLOCK.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-08 (MJK): Ben�tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef BLOCK_H
#define BLOCK_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

void blank(char *s, int n); /* evtl. mit blanks auff�llen */
char *strins(char *dst, char *ins, int idx);

LINESTRUCT *beg_blk(WINDOW *wp, LINESTRUCT *begcut, LINESTRUCT *endcut); /* blockanfang */
LINESTRUCT *end_blk(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut);	/* blockende */
void mark_all(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut); /* alles markieren */

int cut_blk(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end); /* ausschneiden */
int copy_blk(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end,
             LINESTRUCT **begcopy, LINESTRUCT **endcopy); /* kopieren */
void paste_blk(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end); /* einf�gen */

int cut_col(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end); /* ausschneiden */
int copy_col(WINDOW *wp, LINESTRUCT *begcut, LINESTRUCT *endcut,
             LINESTRUCT **begcopy, LINESTRUCT **endcopy); /* kopieren */
void paste_col(WINDOW *wp,LINESTRUCT *beg, LINESTRUCT *end); /* einf�gen */

void hide_blk(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end); /* blockmarkierung l�schen */
void free_blk(WINDOW *wp,LINESTRUCT *line); /* block aus dem speicher l�schen */

void shlf_line(WINDOW *wp, LINESTRUCT *begcut, LINESTRUCT *endcut);
void shrt_line(WINDOW *wp, LINESTRUCT *begcut,  LINESTRUCT *endcut);

#endif /* BLOCK_H */