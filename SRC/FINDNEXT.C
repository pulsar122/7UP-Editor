/*****************************************************************
	7UP
	Modul: FINDNEXT.C
	Copyright (c) Markus Kohm, 1997
	
	Nach der n„chsten Datei suchen, die auf ein bestimmtes
	Muster pat.
	
	Dieses Modul arbeitet nicht mit den TC-/PC-Libraries!
*****************************************************************/

#if !defined( __TURBOC__ ) || defined( __MINT__ )

#include "findnext.h"
#include <ctype.h>

/*
	Prft ob der angegebene Dateiname zur angegebenen Maske pat
	--> name     Name der Datei
	    pattern  Maske fr die Datei
	               *      beliebig viele beliebige Zeichen
	               ?      ein oder kein beliebiges Zeichen
	               [...]  eines der angegebene Zeichen, Bereiche
	                      sind mit <Zeichen>-<Zeichen> m”glich,
	                      mit []...] kann die Klammer aufgenommen
	                      werden.
	               [^...] keines der angegebenen Zeichen, Bereiche
	                      siehe oben.
	<-- 0      Datei und Muster passen aufeinander
	    sonst  Datei und Muster sind unvertr„glich
	ACHTUNG: Diese Funktion arbeitet derzeit rekursiv!
*/
int unxmatch( const char *name, const char *pattern ) {
	while ( *name && *pattern ) {
		switch( *pattern ) {
			case '*': while ( *pattern == '*' || *pattern == '?' )
			         		pattern++;
			         	while( *name && unxmatch( name, pattern ) )
			            name++;
			          return !*name && *pattern;
			case '?':	pattern++;
			         	if ( unxmatch( name, pattern ) )
			         		name++;
			         	else
			         		return 0;
			         	break;
			case '[': pattern++;
			         	if ( *pattern == '^' ) {
			         		pattern++;
			         		/* Nicht eines der Zeichen */
			         		if ( !*pattern )
			         			return !0;
			         		do {
			         			if ( pattern[1] == '-' &&
			         			     pattern[2] && pattern[2] != ']' )
			         				if ( *name >= *pattern &&
			         				     *name <= pattern[2] )
			         					return 0;
			         				else
			         					pattern += 3;
			         			else if ( *name == *pattern++ )
			         				return !0;
			         		} while ( *pattern && *pattern != ']' );
			         		if ( !*pattern )
			         			return !0;
			         		name++;
			         		pattern++;
			         	} else {
			         		/* eines der Zeichen */
			         		if ( !*pattern )
			         			return !0;
			         		for (;;) {
			         			if ( pattern[1] == '-' &&
			         			     pattern[2] && pattern[2] != ']' )
			         				if ( *name >= *pattern &&
			         				     *name <= pattern[2] )
			         					break;
			         				else
			         					pattern += 3;
			         			else if ( *name == *pattern++ )
			         				break;
			         			if ( *pattern == ']' )
			         				return !0;
			         		};
			         		name++;
			         		while ( *pattern && *pattern != ']' )
			         			pattern++;
			         		if ( *pattern )
			         			pattern++;
			         		else
			         			return !0;
			         	}
			         	break;
			default:	if ( *name++ != *pattern++ )
			        		return !0;
		}
	}
	while ( *pattern == '*' || *pattern == '?' )
		pattern++;
	return *name || *pattern;
}

int unximatch( const char *name, const char *pattern ) {
  int c;
	while ( *name && *pattern ) {
		switch( *pattern ) {
			case '*': while ( *pattern == '*' || *pattern == '?' )
			         		pattern++;
			         	while( *name && unximatch( name, pattern ) )
			            name++;
			          return !*name && *pattern;
			case '?':	pattern++;
			         	if ( unximatch( name, pattern ) )
			         		name++;
			         	else
			         		return 0;
			         	break;
			case '[': pattern++;
			         	if ( *pattern == '^' ) {
			         		pattern++;
			         		/* Nicht eines der Zeichen */
			         		if ( !*pattern )
			         			return !0;
			         		do {
			         			if ( pattern[1] == '-' &&
			         			     pattern[2] && pattern[2] != ']' )
			         				if ( ( c = tolower( *name ) ) >= tolower( *pattern ) &&
			         				     c <= tolower( pattern[2] ) )
			         					return 0;
			         				else
			         					pattern += 3;
			         			else if ( tolower( *name ) == tolower( *pattern++ ) )
			         				return !0;
			         		} while ( *pattern && *pattern != ']' );
			         		if ( !*pattern )
			         			return !0;
			         		name++;
			         		pattern++;
			         	} else {
			         		/* eines der Zeichen */
			         		if ( !*pattern )
			         			return !0;
			         		for (;;) {
			         			if ( pattern[1] == '-' &&
			         			     pattern[2] && pattern[2] != ']' )
			         				if ( ( c = tolower( *name ) ) >= tolower( *pattern ) &&
			         				     c <= tolower( pattern[2] ) )
			         					break;
			         				else
			         					pattern += 3;
			         			else if ( tolower( *name ) == tolower( *pattern++ ) )
			         				break;
			         			if ( *pattern == ']' )
			         				return !0;
			         		};
			         		name++;
			         		while ( *pattern && *pattern != ']' )
			         			pattern++;
			         		if ( *pattern )
			         			pattern++;
			         		else
			         			return !0;
			         	}
			         	break;
			default:	if ( tolower( *name++ ) != tolower( *pattern++ ) )
			        		return !0;
		}
	}
	while ( *pattern == '*' || *pattern == '?' )
		pattern++;
	return *name || *pattern;
}

/*
	Nach der n„chsten Datei im Verzeichnis suchen, die auf ein
	bestimmtes Muster pat.
	--> dir       Pointer auf das ge”ffnete Verzeichnis
	    pattern   Unixmuster, auf das die Datei passen mu
	<-- NULL   keine weitere Datei gefunden (oder Fehler)
	    sonst  dirent-Struktur der gefundenen Datei
*/
struct dirent *findnext( DIR *dir, const char *pattern ) {
	struct dirent *dent;
	while ( ( dent = readdir( dir ) ) != (void *)0 &&
	        unxmatch( dent->d_name, pattern ) );
	return dent;
}

struct dirent *findinext( DIR *dir, const char *pattern ) {
	struct dirent *dent;
	while ( ( dent = readdir( dir ) ) != (void *)0 &&
	        unximatch( dent->d_name, pattern ) );
	return dent;
}

#if 0
#include <stdio.h>
int main( int argc, const char *argv[] ) {
	int i;
	for ( i = 1; i < argc-1; i += 2 )
		printf( "%s %s --> %d\n", argv[i], argv[i+1], unxmatch( argv[i], argv[i+1] ) );
	return 0;
}
#endif

#endif