/*****************************************************************
	7UP
	Header: FKEYS.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef FKEYS_H
#define FKEYS_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

char *expandfkey(WINDOW *wp, char *string, int page);
int fkeys(WINDOW *wp, int ks, unsigned int kr, LINESTRUCT **begcut, LINESTRUCT **endcut);
void loadsoftkeys(char *filename);
void hndl_fkeymenu(OBJECT *tree, int start);

#endif /* FKEYS_H */