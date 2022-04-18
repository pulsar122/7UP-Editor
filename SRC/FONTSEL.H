/*****************************************************************
	7UP
	Header: FONTSEL.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-07 (MJK): ben”tigtes WINDOW.H wird geladen
*****************************************************************/

#ifndef FONTSEL_H
#define FONTSEL_H

#include "windows.h"

/*****************************************************************
	Variablen
*****************************************************************/

extern int additional,
           tid,tsize,tattr; /* topId, topSize, topAttr */

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_font(WINDOW *wp, OBJECT *tree); /* geraete direkt ansprechen */

#endif /* FONTSEL_H */