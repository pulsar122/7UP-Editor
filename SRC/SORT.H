/*****************************************************************
	7UP
	Header: SORT.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef SORT_H
#define SORT_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_sort(WINDOW *wp, OBJECT *tree, LINESTRUCT **begcut, LINESTRUCT **endcut);

#endif /* SORT_H */