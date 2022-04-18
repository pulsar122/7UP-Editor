/*****************************************************************
	7UP
	Header: MACRO.H
	
	1997-03-25 (MJK):	Prototypen erg„nzt
	1997-04-09 (MJK): Mehrfachincludes verhindert
	1998-03-06 (MJK): MultiEvent mit Macro gibt es nur hier!
*****************************************************************/

#ifndef MACRO_H
#define MACRO_H

#include "version.h" /* Zwar liegt hier nicht wirklich eine
                      * Abh„ngigkeit vor, aber es wird der
                      * Wert von VERSIONSTRLEN unten ben”tigt! */

/*****************************************************************
	Konstantendefinitionen
*****************************************************************/

#define MAX_MACROS 256L /* Increment der Macroerweiterungen */

#define ALTF7 0x84100
#define ALTF8 0x84200
#define ALTF9 0x84300

/*****************************************************************
	Typen
*****************************************************************/

typedef struct
{
	int kstate, key;
}TMACROBUFF;

typedef struct
{
	char versionstr[31/*VERSIONSTRLEN*/];
	unsigned int version;
	int  headerlen;
	long size,
	     macroindex,
	     lastmacro,
	     rec,
	     play,
	     repindex,
	     lastrep;
	TMACROBUFF *mp;
}TMACRO;

/*****************************************************************
	Variablen
*****************************************************************/

extern TMACRO macro;

/*****************************************************************
	Prototypen
*****************************************************************/

int record_macro(int command, int kstate, int key);
void loadmacro(void);
void savemacro(void);
int mevnt_event(MEVENT *mevent);	/* MultiEvent mit Macrofunktion */

#endif /* MACRO_H */