/*****************************************************************
	7UP
	Header: OBJC_.H
	(c) by TheoSoft '90
	
	1997-04-09 (MJK): Funktionen in dieses neue Modul ausgelagert
*****************************************************************/

#ifndef OBJC__H
#define OBJC__H

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

void objc_update(OBJECT *tree, int obj, int depth);
int objc_sysvar(int ob_smode, int ob_swhich, 
                int ob_sival1, int ob_sival2,
                int *ob_soval1, int *ob_soval2 );

#endif /* OBJC__H */