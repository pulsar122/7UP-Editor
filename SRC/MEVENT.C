/*****************************************************************
	7UP
	Modul: mevent.c
	
	Neues evnt_multi() nach XGEM-Manier

	1997-04-07 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1998-03-06 (MJK): Die Makrorecordergeschichte, ist hier nun weg!
*****************************************************************/
#ifdef TCC_GEM
#include <stdio.h>
#include <aes.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#else
#	include <osbind.h>
#endif
#include <string.h>

#include "macro.h"

static AESPB aespb=
{
   _GemParBlk.contrl,
   _GemParBlk.global,
   _GemParBlk.intin,
   _GemParBlk.intout,
   (int *)_GemParBlk.addrin,
   (int *)_GemParBlk.addrout
};

int evnt_mevent(MEVENT *mevent)
{
   memcpy(_GemParBlk.intin,mevent,14*sizeof(int));   
   _GemParBlk.addrin[0]=mevent->e_mepbuf;
   
   _GemParBlk.intin[14]=(int)mevent->e_time & 0xffff;
   _GemParBlk.intin[15]=(int)mevent->e_time >> 16;

   _GemParBlk.contrl[0]=25;   
   _GemParBlk.contrl[1]=16;   
   _GemParBlk.contrl[2]=7;   
   _GemParBlk.contrl[3]=1;   
   _crystal(&aespb);

   memcpy(&mevent->e_mx,&_GemParBlk.intout[1],6*sizeof(int));

   return(_GemParBlk.intout[0]);
}
#endif /* TCC_GEM */