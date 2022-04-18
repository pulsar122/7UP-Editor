/*****************************************************************
	7UP
	Header: DEADKEY.H

	1997-03-25 (MJK):	Header begonnen
******************************************************************/

#ifndef DEADKEY_H
#define DEADKEY_H

/*****************************************************************
	Variablen
*****************************************************************/

extern int  scaktiv;
extern char shortcutfile[];

/*****************************************************************
	Prototypen
*****************************************************************/

int deadkey(int dead, int key, char **cp); /* Zeichen wandeln	*/
int isdeadkey(int dead); /* geh”rt das Zeichen zu den Deadkeys */

void loadshortcuts(char *filename);

void hndl_shortcuts(void);

#endif /* DEADKEY_H */