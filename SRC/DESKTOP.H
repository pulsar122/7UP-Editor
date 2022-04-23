/*****************************************************************
	7UP
	Header: DESKTOP.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes AES.H wird geladen
*****************************************************************/

#ifndef DESKTOP_H
#define DESKTOP_H

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
	Prototypes
*****************************************************************/

void iconposition(int icon, int *x, int *y, int *w, int *h);
void Dredraw(OBJECT *tree, int x, int y, int w, int h);
void add_icon(OBJECT *tree, int icon);
void ren_icon(OBJECT *tree, int icon);
void del_icon(OBJECT *tree, int icon);
void drag_icon(OBJECT *tree, int icon);
void click_icon(OBJECT *tree, int icon);

int is_selected(OBJECT *tree, int from, int to);
void sel_icons(OBJECT *tree, int from, int to, int mode);
void desel_icons(OBJECT *tree, int from, int to, int mode);
int mark_icons(OBJECT *tree, int mx, int my);

int inst_trashcan_icon(OBJECT *tree, int old, int new, int mode);
int inst_clipboard_icon(OBJECT *tree, int full, int empty, int mode);

#endif /* DESKTOP_H */