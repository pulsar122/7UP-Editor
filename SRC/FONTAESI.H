/*
 * Header file fÅr das ermitteln des Systemzeichensatz
 *
 * 2000-05-18 (GS): new
 */

extern int AES_fontid_norm, AES_fontheight_norm;
extern int AES_fontid_icon, AES_fontheight_icon;


void FontAESInfo (int vdihandle, int *pfontsloaded,
		int *pnormid, int *pnormheight,
    int *piconid, int *piconheight);
