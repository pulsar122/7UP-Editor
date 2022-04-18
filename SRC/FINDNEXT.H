/*****************************************************************
	7UP
	Header: FINDNEXT.H
	Copyright (c) Markus Kohm, 1997
	
	Nach der n„chsten Datei suchen, die auf ein bestimmtes
	Muster pat.
	
	Dieses Modul arbeitet nicht mit den TC-/PC-Libraries!
*****************************************************************/

#if !defined( __TURBOC__ ) || defined( __MINT__ )
#ifndef FINDNEXT_H
#define FINDNEXT_H

#include <dirent.h>

/*****************************************************************
	Prototypen
*****************************************************************/

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
int unxmatch( const char *name, const char *pattern );
int unximatch( const char *name, const char *pattern );

/*
	Nach der n„chsten Datei im Verzeichnis suchen, die auf ein
	bestimmtes Muster pat.
	--> dir       Pointer auf das ge”ffnete Verzeichnis
	    pattern   Unixmuster, auf das die Datei passen mu
	<-- NULL   keine weitere Datei gefunden (oder Fehler)
	    sonst  dirent-Struktur der gefundenen Datei
*/
struct dirent *findnext( DIR *dir, const char *pattern );
struct dirent *findinext( DIR *dir, const char *pattern );

#endif /* FINDNEXT_H */
#endif /* if !defined ... */
