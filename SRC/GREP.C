/*****************************************************************
	7UP
	Modul: GREP.C
	(c) by DECUS '80 & TheoSoft '91

	Suchen mit regul�ren Ausdr�cken
	
	1997-04-07 (MJK): ben�tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	                  Reihenfolge einiger Funktionen vertauscht
	1997-04-11 (MJK): Eingeschr�nkt auf GEMDOS
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <ext.h>
#endif
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif

#include "alert.h"
#include "falert.h"
#include "findrep.h"

#include "grep.h"

#define PMAX	 128

#define CHAR	 1
#define BOL	  2
#define EOL	  3
#define ANY	  4
#define CLASS	5
#define NCLASS  6
#define STAR	 7
#define PLUS	 8
#define MINUS	9
#define ALPHA	10
#define DIGIT	11
#define NALPHA  12
#define PUNCT	13
#define RANGE	14
#define ENDPAT  15
#define INTERP  16
#define BOW	  17
#define EOW	  18

char	 *pp;

static char pbuf[PMAX];

/*******************************************************/
static void store(char op)
{
	if (pp >= &pbuf[PMAX])
	{
		 my_form_alert(1,Agrep[0]);
	}
	*pp++ = op;
}

/*******************************************************/

#pragma warn -par
static char *cclass(char *source, char *src)
/*
char		 *source;	/* Pattern start -- for error msg.		*/
char		 *src;		/* Class start			  */
*/
/*
 * Compile a class (within [])
 */
{
	register char	*s;		  /* Source pointer	 */
	register char	*cp;		 /* Pattern start	  */
	register int	 c;			/* Current character */
	int				 o;			/* Temp				  */

	s = src;
	o = CLASS;
	if (*s == '^') {
		++s;
		o = NCLASS;
	}
	store(o);
	cp = pp;
	store(0);								  /* Byte count		*/
	while (((c = *s++)!=0) && c!=']') {
		if (c == '\\') {					 /* Store quoted char	 */
			if ((c = *s++) == '\0')		/* Gotta get something  */
				return(NULL);
			else
				store(c);
		}
		else if (c == '-' &&
				(pp - cp) > 1 && *s != ']' && *s != '\0') {
			c = pp[-1];				 /* Range start	  */
			pp[-1] = RANGE;			/* Range signal	 */
			store(c);					/* Re-store start  */
			c = *s++;					/* Get end char and*/
			store(c);		/* Store it		  */
		}
		else {
			store(c);		/* Store normal char */
		}
	}
	if (c != ']')
		s=NULL;
	if ((c = (int)(pp - cp)) >= PMAX)
		s=NULL;
	if (c == 0)
		s=NULL;
	*cp = c;
	return(s);
}

/* keine Unterscheidung zwischen GRO�- und Kleinschreibweise */
static char *icclass(char *source, char *src)
/*
char		 *source;	/* Pattern start -- for error msg.		*/
char		 *src;		/* Class start			  */
*/
/*
 * Compile a class (within [])
 */
{
	register char	*s;		  /* Source pointer	 */
	register char	*cp;		 /* Pattern start	  */
	register int	 c;			/* Current character */
	int				 o;			/* Temp				  */

	s = src;
	o = CLASS;
	if (*s == '^') {
		++s;
		o = NCLASS;
	}
	store(o);
	cp = pp;
	store(0);								  /* Byte count		*/
	while (((c = *s++)!=0) && c!=']') {
		if (c == '\\') {					 /* Store quoted char	 */
			if ((c = *s++) == '\0')		/* Gotta get something  */
				return(NULL);
			else
				store(__tolower(c));
		}
		else if (c == '-' &&
				(pp - cp) > 1 && *s != ']' && *s != '\0') {
			c = pp[-1];				 /* Range start	  */
			pp[-1] = RANGE;			/* Range signal	 */
			store(c);					/* Re-store start  */
			c = *s++;					/* Get end char and*/
			store(__tolower(c));		/* Store it		  */
		}
		else {
			store(__tolower(c));		/* Store normal char */
		}
	}
	if (c != ']')
		s=NULL;
	if ((c = (int)(pp - cp)) >= PMAX)
		s=NULL;
	if (c == 0)
		s=NULL;
	*cp = c;
	return(s);
}
#pragma warn .par

/*******************************************************/

int compile(char *source)
/*
char		 *source;	/* Pattern to compile			*/
*/
/*
 * Compile the pattern into global pbuf[]
 */
{
	register char  *s;     /* Source string pointer	  */
	register char  *lp;    /* Last pattern pointer		*/
	register int	c;       /* Current character			*/
	int				o;           /* Temp							 */
	char			  *spp;      /* Save beginning of pattern */

	s = source;
	lp = pp = pbuf; /* lp inserted for TC ************************/
	while ((c = *s++)!=0) {
		/*
		 * STAR, PLUS and MINUS are special.
		 */
		if (c == '*' || c == '+' || c == '-') {
			if (pp == pbuf ||
				  (o=pp[-1]) == BOL ||
				  o == EOL ||
				  o == STAR ||
				  o == PLUS ||
				  o == MINUS)
				return(0);

			store(ENDPAT);
			store(ENDPAT);
			spp = pp;					/* Save pattern end	  */
			while (--pp > lp)		 /* Move pattern down	 */
				*pp = pp[-1];		  /* one byte				 */
			*pp =	(c == '*') ? STAR :
				(c == '-') ? MINUS : PLUS;
			pp = spp;					/* Restore pattern end  */
			continue;
		}
		/*
		 * All the rest.
		 */
		lp = pp;			/* Remember start		 */
		switch(c) {

		case '^':
			store(BOL);
			break;

		case '$':
			store(EOL);
			break;

		case '.':
			store(ANY);
			break;
/*
		case '<':
			store(BOW);
			break;

		case '>':
			store(EOW);
			break;
*/
		case '[':
			s = cclass(source, s);
			if(s==NULL)
				return(0);
			break;

		case ':':
			if (*s) {
				c = *s++;
				switch(__tolower(c)) {

				case 'a':
				case 'A':
					store(ALPHA);
					break;

				case 'd':
				case 'D':
					store(DIGIT);
					break;

				case 'n':
				case 'N':
					store(NALPHA);
					break;

				case ' ':
					store(PUNCT);
					break;
/* NEU */
				case '.':
					store(INTERP);
					break;
/*	  */
				default:
					return(0);
				}
				break;
			}
			else
				return(0);
		case '\\':
			if (*s)
				c = *s++;

		default:
			store(CHAR);
			store(c);
		}
	}
	store(ENDPAT);
	store(0);					 /* Terminate string	  */
	return(1);
}

/* keine Unterscheidung zwischen GRO�- und Kleinschreibweise */
int icompile(char *source)
/*
char		 *source;	/* Pattern to compile			*/
*/
/*
 * Compile the pattern into global pbuf[]
 */
{
	register char  *s;			/* Source string pointer	  */
	register char  *lp;		  /* Last pattern pointer		*/
	register int	c;			 /* Current character			*/
	int				o;			 /* Temp							 */
	char			  *spp;		 /* Save beginning of pattern */

	s = source;
	lp = pp = pbuf; /* lp inserted for TC ************************/
	while ((c = *s++)!=0) {
		/*
		 * STAR, PLUS and MINUS are special.
		 */
		if (c == '*' || c == '+' || c == '-') {
			if (pp == pbuf ||
				  (o=pp[-1]) == BOL ||
				  o == EOL ||
				  o == STAR ||
				  o == PLUS ||
				  o == MINUS)
				return(0);

			store(ENDPAT);
			store(ENDPAT);
			spp = pp;					/* Save pattern end	  */
			while (--pp > lp)		 /* Move pattern down	 */
				*pp = pp[-1];		  /* one byte				 */
			*pp =	(c == '*') ? STAR :
				(c == '-') ? MINUS : PLUS;
			pp = spp;					/* Restore pattern end  */
			continue;
		}
		/*
		 * All the rest.
		 */
		lp = pp;			/* Remember start		 */
		switch(c) {

		case '^':
			store(BOL);
			break;

		case '$':
			store(EOL);
			break;

		case '.':
			store(ANY);
			break;
/*
		case '<':
			store(BOW);
			break;

		case '>':
			store(EOW);
			break;
*/
		case '[':
			s = icclass(source, s);
			if(s==NULL)
				return(0);
			break;

		case ':':
			if (*s) {
				c = *s++;
				switch(__tolower(c)) {

				case 'a':
				case 'A':
					store(ALPHA);
					break;

				case 'd':
				case 'D':
					store(DIGIT);
					break;

				case 'n':
				case 'N':
					store(NALPHA);
					break;

				case ' ':
					store(PUNCT);
					break;
/* NEU */
				case '.':
					store(INTERP);
					break;
/*	  */
				default:
					return(0);
				}
				break;
			}
			else
				return(0);
		case '\\':
			if (*s)
				c = *s++;

		default:
			store(CHAR);
			store(__tolower(c));
		}
	}
	store(ENDPAT);
	store(0);					 /* Terminate string	  */
	return(1);
}

/*******************************************************/

static char *pmatch(char *lbuf, char *line, char *pattern)
/*
char					*line;	  /* (partial) line to match		*/
char					*pattern;  /* (partial) pattern to match	*/
*/
{
	register char	*l;		  /* Current line pointer			*/
	register char	*p;		  /* Current pattern pointer		*/
	register char	c;			/* Current character				*/
	char				*e;		  /* End for STAR and PLUS match  */
	int				 op;		  /* Pattern operation				*/
	int				 n;			/* Class counter					 */
	char				*are;		/* Start of STAR match			 */

	l = line;
	p = pattern;
	while ((op = *p++) != ENDPAT) {
		switch(op) {

		case CHAR:
			if (*l++ != *p++)
				return(NULL);
			break;

		case BOL:
			if (l != lbuf)
				return(NULL);
			break;

		case EOL:
			if (*l != '\0')
				return(NULL);
			break;

		case ANY:
			if (*l++ == '\0')
				return(NULL);
			break;
/*
		case BOW:
			c = *l++;
			if(!(c == ' ' && *l != ' '))
				return(NULL);
			break;

		case EOW:
			c = *l++;
			if(!(c == ' ' && *(l-2L) != ' '))
				return(NULL);
			break;
*/
		case DIGIT:
			if ((c = *l++) < '0' || (c > '9'))
				return(NULL);
			break;

		case ALPHA:
			c = __tolower(*l++); /* Ausnahme */
			if (c < 'a' || c > 'z')
				return(NULL);
			break;

		case NALPHA:
			c = __tolower(*l++); /* Ausnahme */
			if (c >= 'a' && c <= 'z')
				break;
			else if (c < '0' || c > '9')
				return(NULL);
			break;

		case PUNCT:
			c = *l++;
			if (c == 0 || c > ' ')
				return(NULL);
			break;
/* NEU */
		case INTERP:
			c = *l++;
			if(!ispunct(c))
				return(NULL);
			break;
/*	  */
		case CLASS:
		case NCLASS:
			c = *l++;
			n = *p++ & 0377;
			do {
				if (*p == RANGE) {
					p += 3;
					n -= 2;
					if (c >= p[-2] && c <= p[-1])
						break;
				}
				else if (c == *p++)
					break;
			} while (--n > 1);
			if ((op == CLASS) == (n <= 1))
				return(NULL);
			if (op == CLASS)
				p += n - 2;
			break;

		case MINUS:
			e = pmatch(lbuf, l, p); /* Look for a match	 */
			while (*p++ != ENDPAT); /* Skip over pattern	*/
			if (e)						/* Got a match?		  */
				l = e;					/* Yes, update string  */
			break;						/* Always succeeds	  */

		case PLUS:					  /* One or more ...	  */
			if ((l = pmatch(lbuf, l, p)) == NULL)
				return(NULL);		  /* Gotta have a match  */
		case STAR:					  /* Zero or more ...	 */
			are = l;					 /* Remember line start */
			while (*l && ((e = pmatch(lbuf, l, p))!=NULL))
				l = e;					/* Get longest match	*/
			while (*p++ != ENDPAT); /* Skip over pattern	*/
			while (l >= are) {		/* Try to match rest	*/
				if ((e = pmatch(lbuf, l, p))!=NULL)
					return(e);
				--l;					  /* Nope, try earlier	*/
			}
			return(NULL);			  /* Nothing else worked */
/*
		default:
			error("Cannot happen -- match");
			exit(-1);
*/
		}
	}
	return(l);
}

/* keine Unterscheidung zwischen GRO�- und Kleinschreibweise */
static char *ipmatch(char *lbuf, char *line, char *pattern)
/*
char					*line;	  /* (partial) line to match		*/
char					*pattern;  /* (partial) pattern to match	*/
*/
{
	register char	*l;		  /* Current line pointer			*/
	register char	*p;		  /* Current pattern pointer		*/
	register char	c;			/* Current character				*/
	char				*e;		  /* End for STAR and PLUS match  */
	int				 op;		  /* Pattern operation				*/
	int				 n;			/* Class counter					 */
	char				*are;		/* Start of STAR match			 */

	l = line;
	p = pattern;
	while ((op = *p++) != ENDPAT) {
		switch(op) {

		case CHAR:
			if (__tolower(*l++) != *p++)
				return(NULL);
			break;

		case BOL:
			if (l != lbuf)
				return(NULL);
			break;

		case EOL:
			if (*l != '\0')
				return(NULL);
			break;

		case ANY:
			if (*l++ == '\0')
				return(NULL);
			break;
/*
		case BOW:
			c = *l++;
			if(!(c == ' ' && *l != ' '))
				return(NULL);
			break;

		case EOW:
			c = *l++;
			if(!(c == ' ' && *(l-2L) != ' '))
				return(NULL);
			break;
*/
		case DIGIT:
			if ((c = *l++) < '0' || (c > '9'))
				return(NULL);
			break;

		case ALPHA:
			c = __tolower(*l++);
			if (c < 'a' || c > 'z')
				return(NULL);
			break;

		case NALPHA:
			c = __tolower(*l++);
			if (c >= 'a' && c <= 'z')
				break;
			else if (c < '0' || c > '9')
				return(NULL);
			break;

		case PUNCT:
			c = *l++;
			if (c == 0 || c > ' ')
				return(NULL);
			break;
/* NEU */
		case INTERP:
			c = *l++;
			if(!ispunct(c))
				return(NULL);
			break;
/*	  */
		case CLASS:
		case NCLASS:
			c = __tolower(*l++);
			n = *p++ & 0377;
			do {
				if (*p == RANGE) {
					p += 3;
					n -= 2;
					if (c >= p[-2] && c <= p[-1])
						break;
				}
				else if (c == *p++)
					break;
			} while (--n > 1);
			if ((op == CLASS) == (n <= 1))
				return(NULL);
			if (op == CLASS)
				p += n - 2;
			break;

		case MINUS:
			e = ipmatch(lbuf, l, p); /* Look for a match	 */
			while (*p++ != ENDPAT); /* Skip over pattern	*/
			if (e)						/* Got a match?		  */
				l = e;					/* Yes, update string  */
			break;						/* Always succeeds	  */

		case PLUS:					  /* One or more ...	  */
			if ((l = ipmatch(lbuf, l, p)) == NULL)
				return(NULL);		  /* Gotta have a match  */
		case STAR:					  /* Zero or more ...	 */
			are = l;					 /* Remember line start */
			while (*l && ((e = ipmatch(lbuf, l, p))!=NULL))
				l = e;					/* Get longest match	*/
			while (*p++ != ENDPAT); /* Skip over pattern	*/
			while (l >= are) {		/* Try to match rest	*/
				if ((e = ipmatch(lbuf, l, p))!=NULL)
					return(e);
				--l;					  /* Nope, try earlier	*/
			}
			return(NULL);			  /* Nothing else worked */
/*
		default:
			error("Cannot happen -- match");
			exit(-1);
*/
		}
	}
	return(l);
}

/*******************************************************/

char *grep(char *str, char *lp, int *len)
/*
 * Match the current line (in lbuf[]), return 1 if it does.
 */
{
	char *ep;
	for (lp = str; *lp; lp++) {
		if ((ep=pmatch(str, lp, pbuf)) != NULL)
		{
			*len=(int)(ep-lp);
			return(lp);
		}
	}
	*len=0;
	return(NULL);
}

/* keine Unterscheidung zwischen GRO�- und Kleinschreibweise */
char *igrep(char *str, char *lp, int *len)
/*
 * Match the current line (in lbuf[]), return 1 if it does.
 */
{
	char *ep;
	for (lp = str; *lp; lp++) {
		if ((ep=ipmatch(str, lp, pbuf)) != NULL)
		{
			*len=(int)(ep-lp);
			return(lp);
		}
	}
	*len=0;
	return(NULL);
}
