/*****************************************************************
	7UP
	Header: FBINFO.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef FBINFO_H
#define FBINFO_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

void Wsetrcinfo(WINDOW *wp);
long Wline(WINDOW *wp, LINESTRUCT *line);

void textcompare(void);

void hndl_fileinfo(WINDOW *wp, WINDOW *blkwp);

#endif /* FBINFO_H */