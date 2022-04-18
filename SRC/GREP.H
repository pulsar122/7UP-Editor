/*****************************************************************
	7UP
	Header: GREP.H

	1997-03-27 (MJK):	Header begonnen
*****************************************************************/

#ifndef GREP_H
#define GREP_H

/*****************************************************************
	Prototypen
*****************************************************************/

int icompile(char *source);
int compile(char *source);

char *igrep(char *str, char *lp, int *len);
char *grep(char *str, char *lp, int *len);

#endif /* GREP_H */