/*****************************************************************
	7UP
	Header: PRINTER.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tiges WINDOWS.H wird geladen
*****************************************************************/

#ifndef PRINTER_H
#define PRINTER_H

#include "windows.h"

/*****************************************************************
	Variablen
*****************************************************************/

extern int zl,           /* Zeilenl„nge */
           bl,           /* Blattl„nge */
           or,           /* oberer Rand */
           kz,           /* Kopfzeilen */
           fz,           /* Fužzeilen */
           ur,           /* unterer Rand */
           lr,           /* linker Rand */
           zz,           /* Zeilenzahl */
           act_dev,      /* Druckertyp */
           act_paper,    /* Papiertyp */
           act_dist;

/*****************************************************************
	Prototypen
*****************************************************************/
int is_busy(void);

void print_block(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end);
void spool(char *filename, int copies, int mode);
void spool_file(void);
int gdosprint(OBJECT *tree1, OBJECT *tree2, OBJECT *tree3, WINDOW *wp, int device, char *filename);

void hndl_layout(OBJECT *tree, int start);
void hndl_prtmenu(OBJECT *tree1, OBJECT *tree2, OBJECT *tree3, WINDOW *wp);

#endif PRINTER_H