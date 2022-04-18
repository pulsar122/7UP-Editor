/*****************************************************************
	Modul: fsel_inp.c
	(c) 1992 by Oliver Scheel
	
	Ein universeller fsel_(ex)input() Call und Routinen fÅr den
	Selectric Support.
	
	1997-04-09 (MJK): Alle Umschaltungen per Super() durch Aufrufe
	                  von Funktionen per Supexec() ersetzt,
	                  Test, ob GEM Åber fsel_exinput verfÅgt, von
	                  der GEM-Version statt der TOS-Version
	                  abhÑngig gemacht.
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#else
#	include <osbind.h>
#endif
#ifdef TCC_GEM
#	include <vdi.h>
#else
#	include <vdibind.h>
#endif

#include "fsel_inp.h"

/*****************************************************************
	Globale Variablen
*****************************************************************/

SLCT_STR	*slct = NULL;
long		  *fsel = NULL;

/*****************************************************************
	lokal Typdefnitionen
*****************************************************************/

typedef struct /* Cookie Jar */
{
	long	id,
	     *ptr;
} COOKJAR;

/*****************************************************************
	Funktionen
*****************************************************************/

/*
	--> cookie:	Name des gesuchten Cookies
	<-- Wert des Cookies, wenn gefunden, sonst 0-Pointer
	Bemerkung: Das ist ziemlich schlecht, falls ein Cookie den
	           Wert 0L hat!
*/
static long get_cookiejar( void ) /* NUR PER SUPEXEC AUFRUFEN!!! */
{
	return *((long *)0x05a0l);
}

long *get_cookie( long cookie )
{
	COOKJAR	*cookiejar;
	int	i = 0;

	cookiejar = (COOKJAR *)Supexec(get_cookiejar);

	if(cookiejar)
	{
		while(cookiejar[i].id)
		{
			if(cookiejar[i].id == cookie)
				return cookiejar[i].ptr;
			i++;
		}
	}

	return(0l);
}

/*
	PrÅft nach, ob ein FSEL-Cookie vorhanden ist.
	--> keine
	<-- 1:  FSEL-Cookie vorhanden.
	    0: FSEL-Cookie nicht vorhanden.
*/
int fsel_check(void)
{
	if(!fsel)
		fsel = get_cookie('FSEL');
	return(fsel ? 1 : 0);
}

/*
	Checkt, ob Selectric installiert ist und ob es die
	Mindestversionsnummer besitzt.
	--> version: EnhÑlt die zu prÅfende Versionsnummer
               (es wird ein '>='-Test gemacht!!)
	<-- 1:  Selectric ist installiert und die
	           Versionsnummer ist ok.
	    0: Entweder nicht installiert oder zu
	           niedrige Versionsnummer.
*/
int slct_check(unsigned int version)
{
	if(fsel_check())
	{
		slct = (SLCT_STR *)fsel;
		if(slct->id != 'SLCT')
			slct = 0L;
	}
	if(slct && (slct->version >= version))
		return(1);
	else
		return(0);
}

#if 0
/*
	Ruft den FileSelector in komfortabler Art und Weise
	auf. Dabei kann man alle Parts (Filename, Pathname,
	etc.) einzeln Åbergeben. Man kann aber auch Pathname
	und den kompletten Namen in `pfname' Åbergeben. Diese
	Routine sucht sich schon das, was ihr fehlt in
	bestimmten Grenzen selbst heraus.
	Diese Funktion unterstÅtzt den FSEL-Cookie und lÑuft
	auch ohne Selectric.
	--> *pfname: EnthÑlt abschlieûend den fertigen Pfad, den
	             man sofort in ein `open' einsetzen kann.
	    *pname:  Der Startpfad (ohne Wildcards!).
	    *fname:  Ein voreingestellte Filenamen.
	    *ext:    Eine Extension.
	    *title:  Einen Boxtitel. Dabei wird sowohl die 
	             GEM-Version als auch der FSEL-Cookie ÅberprÅft.
	<-- Der Button mit dem der Selector verlassen wurde.
	Bemerkung: Beim Aufruf aus Accessories nicht vergessen ein
             BEG/END_UPDATE um diesen Aufruf zu legen!!!!!!!!!!
	           Die meisten File-Selector Clones (incl. Selectric)
	           machen das eh, nicht aber das Original ...
*/
int file_select(char *pfname, char *pname, char *fname,
                const char *ext, char *title)
{
	int	but;
	char	*p;

	if(!fname[0])
	{
		p = strrchr(pfname, '\\');
		if(!p)
			p = strrchr(pfname, '/')
		if(p)
			strcpy(fname, p+1);
		else
			strcpy(fname, pfname);
	}
	if(!pname[0])
	{
		p = strrchr(pfname, '\\');
		if(!p)
			p = strrchr(pfname, '/')
		if(p)
		{
			p[1] = '\0';
			strcpy(pname, pfname);
		}
	}
	else
		complete_path(pname); /* '/' oder  '\' */
/*
	else if(pname[strlen(pname)-1] != '\\')
		strcat(pname, "\\");
*/
	strcat(pname, ext);

	if(fsel_check() || 
	   ((_GemParBlk.global[0] >= 0x0104) && 
	    (_GemParBlk.global[0] < 0x0200) ||
	    (_GemParBlk.global[0] >= 0x0300)))
		fsel_exinput(pname, fname, &but, title);
	else
		fsel_input(pname, fname, &but);

	p = strrchr(pname, '\\');
	if(!p)
		p = strrchr(pname, '/');
	if(p)
		*p = '\0';
	strcpy(pfname, pname);
/*
	if(getenv("UNIXMODE")!=NULL)
		strcat(pfname, "/");
	else
*/
		strcat(pfname, "\\");
	strcat(pfname, fname);
	return(but);
}

/*
	Setzt benutzerdefinierte Extensions und Pfade, welche
	dann von Selectric benutzt werden. Die Extensions und
	Pfade mÅssen vor jedem Selectric-Aufruf gesetzt werden!
	--> ext_num:  Anzahl der Extensions
	    *ext[]:   Die Extensions
	    path_num: Anzahl Pfade
	    *paths[]: Die Pfade
	<-- 1:  Selectric ist installiert
	    0: Selectric ist nicht installiert
*/
int slct_extpath(int ext_num, char *(*ext)[], int path_num, char *(*paths)[])
{
	if(slct_check(0x0100))
	{
		slct->num_ext = ext_num;
		slct->ext = ext;
		slct->num_paths = path_num;
		slct->paths = paths;
		return(1);
	}
	else
		return(0);
}
#endif /* 0 */

/*
	Initialisiert Selectric so, daû es weiû, daû mehr
	als ein Name zurÅckgegeben werden kann.
	-->	mode: Gibt den Modus an. Z. Zt sind folgende Modi
	          vorhanden:
	            0: Files in Pointerlist zurÅckgeben.
	            1: Files in einem einzigen String -"-.
	    num:  Anzahl der Namen die maximal zurÅckgegeben
	          werden sollen.
	    *ptr: Der Zeiger auf die entsprechende Struktur.
	<-- 1:  Selectric ist installiert
	    0: Selectric ist nicht installiert
*/
int slct_morenames(int mode, int num, void *ptr)
{
	if(slct_check(0x0100))
	{
		slct->comm |= CMD_FILES_OUT;
		if(mode)
			slct->comm |= CFG_ONESTRING;
		slct->out_count = num;
		slct->out_ptr = ptr;
		return(1);
	}
	else
		return(0);
}
