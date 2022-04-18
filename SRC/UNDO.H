/*****************************************************************
	7UP
	Header: UNDO.H
	(c) by TheoSoft '92
	
	1997-03-25 (MJK):	TAB2
	                  Prototypes ergÑnzt
	1997-04-08 (MJK): Benîtigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef UNDO_H
#define UNDO_H

#include "windows.h"

/*****************************************************************
	Definitionen
*****************************************************************/

#define LINEUNDO  (-1)
#define LINEPAST  (-2)
#define CUTLINE   (-3)
#define CUTPAST   (-4)
#define PASTCUT   (-5)
#define BACKSPACE (0x0008)
#define RETURN    (0x000D)

/*****************************************************************
	Typen
*****************************************************************/

typedef struct
{
	int menu,item;                /* FÅr PASTE */
	LINESTRUCT *blkbeg, *blkend;  
	
	long wline, begline, endline; /* FÅr CUT   */
	int begcol, endcol; 
	int blktype;                  /* Blocktyp */
	
	char string[STRING_LENGTH+2]; /* Lineundo */
	char *cp;
	int row,col;
	int flag;
} UNDO;

/*****************************************************************
	Variablen
*****************************************************************/

extern UNDO undo;

/*****************************************************************
	Prototypes
*****************************************************************/

void free_undoblk(WINDOW *wp, LINESTRUCT *line);
void do_undo(WINDOW *wp); /* Undo ausfÅhren */
void store_undo(WINDOW *wp, UNDO *undo, LINESTRUCT *beg, LINESTRUCT *end, int menu, int item);

#endif /* UNDO_H */