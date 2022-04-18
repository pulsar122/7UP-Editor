/*****************************************************************
	7UP
	Header: NUMERIK.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”igtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef NUMERIK_H
#define NUMERIK_H

#include "windows.h"

/*****************************************************************
	Variablen
*****************************************************************/

extern int komma;

/*****************************************************************
	Prototypen
*****************************************************************/

int isnum(char c);
void rechnen(WINDOW * wp, OBJECT *tree, int operation, LINESTRUCT *begin, LINESTRUCT *end);
void hndl_nummenu(OBJECT *tree, int start, int mode);

#endif /* NUMERIK_H */