/*****************************************************************************
	7UP
	Header: 7UP3.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-08 (MJK): Benîtigtes WINDOWS.H wird geladen
	1997-04-09 (MJK): MSDOS-Teile entfernt
*****************************************************************************/

#ifndef SEVENUP3_H
#define SEVENUP3_H

#include "windows.h"

/*****************************************************************
	Typen
*****************************************************************/

typedef struct { /* FÅr die Speicherung der Icon-Koordinaten 
                  * auf dem Desktop */
	int x,y;
} ICNCOORDS;

/*****************************************************************
	Variablen
*****************************************************************/

extern LINESTRUCT   *begcut, *endcut;
extern int           cut,
                     nodesktop,
                     vastart,
                     clipbrd,
                     umlautwandlung,
                     toolbar_zeigen,
                     scrollreal,
                     terminate_at_once,
                     topwin,
                     charw,charh,
                     boxw,boxh,
                     norm_point,small_point,
                     xdesk,ydesk,wdesk,hdesk,
                     aeshandle,
                     vdihandle,
                     userhandle,
                     exitcode,
                     gl_apid;
extern int           work_in[],work_out[];

extern unsigned long backuptime; /* timer fÅr autom. Backup */

extern char          alertstr[];

extern ICNCOORDS     iconcoords[];
extern unsigned int  iconcoordcnt; /* Anzahl Iconkoordinaten */

extern WINDOW *blkwp;

/*****************************************************************
	Funktionsmakros
*****************************************************************/

#define grect2array( grect ) ((int *)grect)
#define array2grect( array ) ((GRECT *)array)

/*****************************************************************
	Prototypen
*****************************************************************/

int open_work(int device);
void close_work(int handle, int device);

int Wtxtdraw(register WINDOW *wp, int dir, int clip[]);
void Wdefattr(WINDOW *wp);
void Wwindmsg(WINDOW *wp, int *msgbuf);

int delete(WINDOW *wp, OBJECT *tree, int desk_obj);

void hndl_mesag(int mbuf[]);
void hndl_menu(int menu, int item);
void hndl_keybd(int kstate, int key);
void hndl_icons(OBJECT *tree, int mx, int my, int key, int click);
void hndl_button(void);
void hndl_timer(void);

void pinit(char *inffile);
void pexit(void);

#endif SEVENUP3_H