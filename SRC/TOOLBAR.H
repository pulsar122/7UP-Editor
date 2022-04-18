/*****************************************************************
	7UP
	Header: TOOLBAR
	
	1997-04-09 (MJK):	Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "windows.h"

/*****************************************************************
	Definitione
*****************************************************************/

#define EXOB_TYPE(x) (x>>8)
#define TABBAR 0xFF
#define TABSIGN '+' /*127*/
#define DEZTAB '#'

/*****************************************************************
	Variablen
*****************************************************************/

extern int tabbar;

/*****************************************************************
	Prototypes
*****************************************************************/

int toolbar_do(WINDOW *wp, OBJECT *tree, int x, int y, int bstate, int kstate);
int toolbar_tnormal(WINDOW *wp, OBJECT *tree, int menuitem, int mode);
void toolbar_adjust(WINDOW *wp);
int toolbar_inst(OBJECT *wtree, WINDOW * wp, OBJECT *ttree);

void tabbar_inst(WINDOW *wp, int selected);

void hndl_mm1(OBJECT *tree, int mx, int my);

int Wgetnexttab(WINDOW *wp);
int Wgetprevtab(WINDOW *wp);
char Wgettab(WINDOW *wp);

void Wtabbarupdate(WINDOW *wp);

#endif /* TOOLBAR_H */
