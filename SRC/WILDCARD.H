/*****************************************************************
	7UP
	Header: 7UP3.H

	1997-03-27 (MJK):	Header begonnen
*****************************************************************/

#ifndef WILDCARD_H
#define WILDCARD_H

/*****************************************************************
	Prototypen
*****************************************************************/

char *imatch(char *first, register char *string, register char *pattern, int *len, char all, char one);
char *match(char *first, register char *string, register char *pattern, int *len, char all, char one);

#endif /* WILDCARD_H */