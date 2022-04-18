/*****************************************************************
	7UP
	Header: SHELL.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes AES.H wird geladen
*****************************************************************/

#ifndef SHELL_H
#define SHELL_H

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
	Variablen
*****************************************************************/

extern int pexec;

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_shell(OBJECT *tree, int start);

#endif /* SHELL_H */