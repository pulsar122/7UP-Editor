/*****************************************************************
	7UP
	Modul: fontaesi.C
	(c) by mt '90

	Ermittelt die Grîûe des Systemzeichensatz
	
*****************************************************************/

/*
    IDs und Grîûen der SystemzeichensÑtze erfragen

    Julian F. Reschke, 13. Januar 1995
    Verbesserung des Hacks fÅr TOS <4.0, zusÑtzlich
    BerÅcksichtigung von MagiC!, ?AGI, AFnt und SMAL
    Christoph G.A. Zwerschke, 4. Februar 1995
    AFnt-Abfrage in appl_xgetinfo() verlegt
    Hayo Schmidt, 18. April 1995
    Supexec() statt Super() verwendet
    Christoph G.A. Zwerschke, 1. November 1995
*/

#include <stdio.h>
#ifdef TCC_GEM
# include <aes.h>
# include <vdi.h>
#else
# include <gem.h>
#endif
#if defined( __PUREC__ ) && !defined( __MINT__ )
#	include <tos.h>
#else
#	include <osbind.h>
#endif

#ifndef _AESversion
#	ifdef __PUREC__
#		define _AESversion (_GemParBlk.global[0])
#	else
#		define _AESversion (aes_global[0])			/* 10.05.2000 GS */
#	endif
#endif


int AES_fontid_norm, AES_fontheight_norm;
int AES_fontid_icon, AES_fontheight_icon;

/* AES-Font Cookie-Struktur */

typedef struct
{
	long af_magic; 											/* AES-Font ID (AFnt) */
	int version; 												/* Version (BCD-Format) */
	   																	/* (Hi-Byte Cookie, Lo-Byte Programm) */
	int installed; 											/* Flag fÅr Fonts angemeldet */
	int cdecl (*afnt_getinfo) (int af_gtype, /* Aufruf */
	    int *af_gout1, int *af_gout2,
	    int *af_gout3, int *af_gout4);
}AFNT;

long get_cookiejar_aes (void)
{
    return *(long *)0x5a0;
}

int get_cookie_aes (long cookie, long *value)
{
    long *cookiejar;

    cookiejar = (long *) Supexec (get_cookiejar_aes);
    if (cookiejar == NULL) return 0;
    do
    {
        if (cookiejar[0] == cookie)
        {
            if (value) *value = cookiejar[1];
            return 1;
        }
        else
            cookiejar += 2;
    }   while (cookiejar[-2]);
    return 0;
}

static int load_font (int vdihandle, int fontid)
{
	int loaded = 0;
	
	if (fontid != vst_font (vdihandle, fontid) && vq_gdos ())
	{
		vst_load_fonts (vdihandle, 0);
	  loaded = 1;
	}
	
	return loaded;
}

void FontAESInfo (int vdihandle, int *pfontsloaded,
		int *pnormid, int *pnormheight,
    int *piconid, int *piconheight)
{
	static int done = 0;
	static int normid, normheight;
	static int iconid, iconheight;
	int fontsloaded = 0;
	
	if (!done)
	{
		int aeshandle, cellwidth, cellheight, dummy;
	
	  wind_update (BEG_UPDATE);
	
	  aeshandle = graf_handle (&cellwidth, &cellheight, &dummy, &dummy);
	
	  /* zunÑchst versuchen wir, die Fontids beim AES oder */
	  /* mit der Auskunftfunktion von AES-Font zu erfragen */
	
	  if (!appl_xgetinfo (0, &normheight, &normid, &dummy, &dummy) ||
	      !appl_xgetinfo (1, &iconheight, &iconid, &dummy, &dummy))
	  {
	      /* Hier fragen wir den aktuellen Font der AES-Workstation
	      ab. Dies ist ein Hack, aber es funktioniert mit den
	      unterschiedlichen Auto-Ordner-Tools und ist eben nur
	      bis AES 3.99 nîtig. Wir gehen dabei nicht davon aus,
	      daû fÅr beide Textgrîûen dieselbe Schrift verwendet wird */
	
	  	static TEDINFO dum_ted = {
	  					 " ", "", "", IBM  , 0,
	            TE_LEFT , 0, 0, 0, 2, 1 };
	
	    static OBJECT dum_ob = {
	            0, -1, -1, G_TEXT, LASTOB, NORMAL,
	            (long)&dum_ted, 0, 0, 0, 0 };
	
	    int attrib[10]; long small;
	
	    dum_ob.ob_width = cellwidth;
	    dum_ob.ob_height = cellheight;
	
	    dum_ted.te_font = IBM;
	    objc_draw (&dum_ob, 0, 1, 0, 0, 0, 0);
	    vqt_attributes (aeshandle, attrib);
	    normid = attrib[0]; normheight = attrib[7];
	
	    dum_ted.te_font = SMALL;
	    objc_draw (&dum_ob, 0, 1, 0, 0, 0, 0);
	    vqt_attributes (aeshandle, attrib);
	    iconid = attrib[0]; iconheight = attrib[7];
	
	    /* schlieûlich berÅcksichtigen wir noch den SMAL-Cookie */
			
	    if (get_cookie_aes('SMAL', &small))
	    {
	    	if ((dummy = (int) (small>>16)) != 0)
	      	normheight = dummy;
	      if ((dummy = (int) (small)) != 0)
	        iconheight = dummy;
	    }
	  }
	
	    /* Nun haben wir fÅr beide Fonts die Id und die Pixelgrîûe
	    (Parameter fÅr vst_height). Nun sorgen wir dafÅr, daû beide
	    Fonts auch wirklich auf der aktuellen virtuellen Workstation
	    geladen sind (wir gehen davon, daû sie generell verfÅgbar
	    sind, sonst hÑtte sie uns das AES ja nicht melden dÅrfen). */
	
	  fontsloaded |= load_font (vdihandle, normid);
	  fontsloaded |= load_font (vdihandle, iconid);
	
	  /* Systemfont in Standardgrîûe einstellen */
	
	  vst_font (vdihandle, normid);
	  vst_height (vdihandle, normheight, &dummy, &dummy, &dummy, &dummy);
	
	  wind_update (END_UPDATE);
	
	  done = 1;
	}
	
	/* RÅckgabewerte */
	
	if (pnormid) *pnormid = normid;
	if (pnormheight) *pnormheight = normheight;
	if (piconid) *piconid = iconid;
	if (piconheight) *piconheight = iconheight;
	if (pfontsloaded) *pfontsloaded = fontsloaded;
}
