/*****************************************************************
	7UP
	Header: EDITOR.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Benîtigtes AES.H wird geladen
*****************************************************************/

#ifndef EDITOR_H
#define EDITOR_H

#ifndef _GEMFAST_H
#	ifndef __AES__
#		ifdef TCC_GEM
#			include <aes.h>
#		else
#			include <gem.h>
#		endif
#	endif
#endif

/*****************************************************************
	Variablen
*****************************************************************/

extern WINDOW     *twp,        /* Top Window Pointer */
                  *copywindow;
extern LINESTRUCT *lastwstr;
extern long       lasthfirst,
                  begline,endline;

/*****************************************************************
	Prototypen
*****************************************************************/

void mark_line(WINDOW *wp, LINESTRUCT *str, int line); /* Zeile markieren */
void refresh(register WINDOW *wp, LINESTRUCT *line, register int col, register int row);
int ins_char(WINDOW *wp, register LINESTRUCT *line, int c); /* zeichen einfÅgen */
int ins_line(WINDOW *wp);			/* neue zeile einfÅgen */
static int backspace(WINDOW *wp, register LINESTRUCT *line);
int del_char(WINDOW *wp, register LINESTRUCT *line);
int del_line(WINDOW *wp);
int adjust_best_position(WINDOW *wp); /* evtl. unten anpassen */

int hndl_umbruch(OBJECT *tree, WINDOW *wp, int start);

void blockformat(char *str, int diff); /* mit blanks expandieren */
int findlastspace(char *str, int umbr, int abs);

int isgerman(char key);

void editor(register WINDOW *wp, int state, int key, LINESTRUCT **begcut, LINESTRUCT **endcut); /* zeicheneingabe auswerten */
void hndl_chartable(WINDOW *wp, OBJECT *tree);

int altnum(int *state, int *key);

int special(WINDOW *wp, WINDOW **blkwp, int state, int key, LINESTRUCT **begcut, LINESTRUCT **endcut);

#endif /* EDITOR_H */