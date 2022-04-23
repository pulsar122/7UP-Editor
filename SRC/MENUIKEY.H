/*****************************************************************
	7UP
	Header: MENUIKEY.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes AES.H wird geladen
*****************************************************************/

#ifndef MENUIKEY_H
#define MENUIKEY_H

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

int menu_ikey(OBJECT *m_tree, int kstate, int key, int *menu, int *item);

void _loadmenu(char *filename);
void _savemenu(char *filename);
void loadmenu(char *filename);

int MapKey(int *kstate, int *key);

#endif MENUIKEY_H