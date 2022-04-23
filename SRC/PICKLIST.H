/*****************************************************************
	7UP
	Header: PICKLIST.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes AES.H wird geladen
*****************************************************************/

#ifndef PICKLIST_H
#define PICKLIST_H

#ifndef _GEMFAST_H
#	ifndef __AES__
#		ifdef TCC_GEM
#			include <aes.h>
#		else
#			include <gem.h>
#		endif
#	endif
#endif

/*****************************************************************
	Prototypen
*****************************************************************/

void append_picklist(OBJECT * tree, char *pathname, long line);
void hndl_picklist(OBJECT *tree);

#endif /* PICKLIST_H */