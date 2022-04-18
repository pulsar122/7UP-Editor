/*****************************************************************
	7UP
	Header: PREVIEW.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef PREVIEW_H
#define PREVIEW_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_preview(WINDOW *wp, OBJECT *tree, LINESTRUCT *begcut, LINESTRUCT *endcut);

#endif /* PREVIEW_H */