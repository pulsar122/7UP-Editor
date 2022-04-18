/*****************************************************************
	7UP
	Header: WFINDBLK.H

	1997-04-08 (MJK):	Header begonnen
*****************************************************************/

#ifndef WFINDBLK_H
#define WFINDBLK_H

#include "undo.h"	/* GANZ SCHLECHT, WEIL KREISABHŽNGIG! */
#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

WINDOW *Wfindblk(WINDOW *wp, WINDOW **blkwp, LINESTRUCT **beg, LINESTRUCT **end);
int Wrestblk(WINDOW *wp, UNDO *undo, LINESTRUCT **beg, LINESTRUCT **end);

#endif /* WFINDBLK_H */