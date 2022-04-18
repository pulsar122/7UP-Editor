/*****************************************************************
	7UP
	Header: TABULAT.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-08 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef TABULAT_H
#define TABULAT_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

char *stpexpan(register char *ptarget, char *psource, register int incr, register int tarsize, register int *linelen);
char *stptabfy(char *psource, int incr);
int hndl_tab(OBJECT *tree,WINDOW *wp);

#endif /* TABULAT_H */