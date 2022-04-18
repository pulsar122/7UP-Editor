/*****************************************************************
	7UP
	Header: LISTBOX.H

	1997-04-07 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes AES.H wird geladen
*****************************************************************/

#ifndef LISTBOX_H
#define LISTBOX_H

#ifndef _GEMFAST_H
#	ifndef __AES__
#		ifdef TCC_GEM
#			include <aes.h>
#		else
#			include <gemfast.h>
#		endif
#	endif
#endif

/*****************************************************************
	Typen
*****************************************************************/

typedef struct
{
	int flags;
	int state;
	int retcode;
	char entry[25];
	int dummy;
} SCROLLENTRY;

typedef struct
{
	long count;
	SCROLLENTRY scrollist[100];
} SCROLLIST;

/*****************************************************************
	Prototypen
*****************************************************************/

void setsliderpos(OBJECT *tree, int item, int pos);
int listbox_hndl(OBJECT *tree, int item, SCROLLIST *liste);

#endif /* LISTBOX_H */