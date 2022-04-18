/*****************************************************************
	7UP
	Header: FINDREP.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-07 (MJK): benîtigtes Windows.h wird zugeladen
*****************************************************************/

#ifndef FINDREP_H
#define FINDREP_H

#include "Windows.h"

/*****************************************************************
	Typen
*****************************************************************/

typedef struct {
	long absrow,abscol;
	LINESTRUCT *line;
	char *string;
	WINDOW *wp;
} LINEMARK;

/*****************************************************************
	Variablen
*****************************************************************/

extern char searchstring[];

/*****************************************************************
	Prototypen
*****************************************************************/

void redraw(register WINDOW *wp, register LINESTRUCT *line, register long i, int replace);

char *stristr(char *s1, char *s2);
int __tolower(int c);
int __toupper(int c);

int hndl_find(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, int item);
void hndl_blkfind(WINDOW *wp, LINESTRUCT *begcut, LINESTRUCT *endcut, int item);
int hndl_goto(WINDOW *wp, OBJECT *tree, long line);
void hndl_page(WINDOW *wp, OBJECT *tree, long line);

int iserrfile(WINDOW *wp);
int isregfile(WINDOW *wp);
int inw(char c);

void gotomark(WINDOW *wp, LINEMARK *mark);

int find_next_letter(char *str, int col); /* fÅr control-cursor zum nÑchsten Wort */
int find_prev_letter(char *str, int col); /* von Wort zu Wort mit CONTROL+CURSOR */

void changeletters(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end, int mode);

void check_braces(OBJECT *tree, WINDOW *wp, LINESTRUCT *begcut, LINESTRUCT *endcut);

#endif /* FINDREP_H */
