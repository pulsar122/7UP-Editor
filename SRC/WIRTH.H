/*****************************************************************
	7UP
	Header: WIRTH.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef WIRTH_H
#define WIRTH_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

int Wirth(WINDOW *wp, LINESTRUCT *begcut ,LINESTRUCT *endcut ,char *begstr, char *endstr, long *lc, int *col);
int Wirth2(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, char *begstr, char *endstr);

int isfreepattern(OBJECT *tree, char *str);

#endif /* WIRTH_H */