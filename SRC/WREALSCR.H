/*****************************************************************
	7UP
	Header: WREALSCR.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef WREALSCR_H
#define WREALSCR_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

int isvertical(WINDOW *wp, int e_mx, int e_my);
int ishorizontal(WINDOW *wp, int e_mx, int e_my);

void Wrealscroll(WINDOW *wp, int e_mx, int e_my, int dir);

#endif /* WREALSCR_H */