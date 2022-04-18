/*****************************************************************
	7UP
	Header: FORMAT.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef FORMAT_H
#define FORMAT_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_textformat(WINDOW *wp, OBJECT *tree, LINESTRUCT **begcut, LINESTRUCT **endcut);

#endif /* FORMAT_H */