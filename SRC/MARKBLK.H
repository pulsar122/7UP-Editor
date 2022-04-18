/******************************************************************
	7UP
	Header: MARKBLK.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
******************************************************************/

#ifndef MARKBLK_H
#define MARKBLK_H

#include "windows.h"

/*****************************************************************
	Prototypen
*****************************************************************/

int Wdclickword(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, int mx, int my);
void mark_blk(WINDOW *wp,LINESTRUCT **begcut, LINESTRUCT **endcut);
int Wdclickbrace(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, int mx, int my);
int Wdclickline(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, int mx, int my);

/*
void swap(int *a, int *b);
*/

int _Wmarkblock(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, int e_mx, int e_my, int clicks, int key_state);
int _WKeyBlock(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut, int key_state, int key);

#endif /* MARKBLK_H */