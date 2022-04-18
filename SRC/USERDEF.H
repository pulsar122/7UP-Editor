/*****************************************************************
	7UP
	Header: USERDEF.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-08 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef USERDEF_H
#define USERDEF_H

#include "windows.h"

/*****************************************************************
	Typen
*****************************************************************/

#ifndef BOOLEAN
#	define BOOLEAN int             /* 2 valued (true/false) */
#	define FALSE   (BOOLEAN)0      /* Function FALSE value */
#	define TRUE    (BOOLEAN)1      /* Function TRUE  value */
#endif

/*****************************************************************
	Variablen
*****************************************************************/

extern int threedee,     /* lange šberschriftenunterstreichung */
           dialbgcolor,  /* Hintergrundfarbe */
           actbutcolor;  /* Farbe aktvierter Buttons */

/*****************************************************************
	Prototypen
*****************************************************************/

void tabbar_fix(WINDOW *wp);
void form_fix(OBJECT *tree, BOOLEAN is_dialog);

#endif /* USERDEF_H */