/*****************************************************************
	7UP
	Header: TEXTMARK.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef TEXTMARK_H
#define TEXTMARK_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

void textmarker(WINDOW *wp, OBJECT *tree, int item, int kstate, int key);

#endif /* TEXTMARK_H */