/*****************************************************************
	7UP
	Header: CONFIG.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-09 (MJK): Ben”tigtes WINDOWS.H wird geladen
*****************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include "WINDOWS.H"

/*****************************************************************
	Prototypen
*****************************************************************/

void hndl_diverses(OBJECT *tree, int start);

int _read_lin(WINDOW *wp, char *name); /* Zeilenlineal lesen */
void hndl_lineal(WINDOW *wp, int start);

char *find_7upinf(char *path, char *ext, int mode);
void saveconfig(int windstruct);
void restoreconfig(char *inffile);

int abs2rel(int abspos, int pixel);
int rel2abs(int relpos, int pixel);
void sicons(void);

int saveformat(OBJECT *tree, char *fname);
int loadformat(OBJECT *tree, char *fname);

int readnames(void);
void writenames(void);

#endif /* CONFIG_H */