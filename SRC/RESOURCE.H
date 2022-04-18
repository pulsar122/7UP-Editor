/*****************************************************************
	7UP
	Header: RESOURCE.H

	1997-03-25 (MJK):	Header begonnen
*****************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H

#include "WINDOWS.H"	/* Typ WINDOW (lÑdt AES.H fÅr Typ OBJECT) */

/*****************************************************************
	Varialen
*****************************************************************/

extern OBJECT *winmenu,*prtmenu,*popmenu,
              *desktop,*findmenu,*gotomenu,
		          *fontmenu,*tabmenu,*infomenu,
              *copyinfo,*shellmenu,*chartable,
              *shareware,*userimg,
              *shell2,*fkeymenu,*umbrmenu,
              *pinstall,*preview,*layout,
              *replmenu,*markmenu,
              *popmenu,*divmenu,*bracemenu,
              *sortmenu,*gdospop,*fselbox,
              *paperpop,*grepmenu,*distpop,
              *nummenu,*picklist,*menueditor,
              *registmenu,*formatmenu,*listbox;

/*****************************************************************
	Prototypen
*****************************************************************/

int rsrc_init(char *rscname, char *inffile);
void checkmenu(OBJECT *tree, WINDOW *wp);

#endif /* RESOURCE_H */