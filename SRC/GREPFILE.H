/*****************************************************************
	7UP
	Header: GREPFILE.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
*****************************************************************/

#ifndef GREPFILE_H
#define GREPFILE_H

#if !defined(MAXPATHS)
#	define MAXPATHS 7
#endif

#ifndef PATH_MAX
#	include <limits.h>
#endif

/*****************************************************************
	Variablen
*****************************************************************/

extern char lpath[MAXPATHS][PATH_MAX];
extern char *lname[];

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_grepmenu(OBJECT *tree, int start);
void prepare(OBJECT *tree, OBJECT *tree2, char *str);

#endif /* GREPFILE_H */