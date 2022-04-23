/*****************************************************************
	7UP
	Modul: GRAF_.C
	
	Ersatzfunktion. Macht die Maus nicht um jeden Preis sichtbar.
*****************************************************************/

#include <stdio.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif

static int mhidden=0;

#ifdef TCC_GEM
static AESPB aespb=
{
   _GemParBlk.contrl,
   _GemParBlk.global,
   _GemParBlk.intin,
   _GemParBlk.intout,
   (int *)_GemParBlk.addrin,
   (int *)_GemParBlk.addrout
};

int graf_mouse( int gr_monumber, MFORM *gr_mofaddr )
{
   _GemParBlk.intin [0] = gr_monumber;
   _GemParBlk.addrin[0] = gr_mofaddr;
   _GemParBlk.contrl[0] = 78;   
   _GemParBlk.contrl[1] = 1;   
   _GemParBlk.contrl[2] = 1;   
   _GemParBlk.contrl[3] = 1;   

   switch(gr_monumber)
   {
      case M_ON:
         if(mhidden==1)
         {
            _GemParBlk.intin [0] = M_ON;
            _crystal(&aespb);
            mhidden=0;
         }
         break;
      case M_OFF:
         if(mhidden==0)
         {
            _crystal(&aespb);
            return(mhidden=1);
         }
         else
            return(!mhidden);
         /*break;*/
      default:
         _crystal(&aespb);
         break;
   }
   return(_GemParBlk.intout[0]);
}

int graf_mouse_on( int on ) {
	graf_mouse( on ? M_ON : M_OFF, NULL );
}
#else
int graf_mouse_on( int on ) {
	if ( on && mhidden ) {
		mhidden=0;
		return graf_mouse( M_ON, NULL );
	} else if ( !on && !mhidden ) {
		mhidden=1;
		return graf_mouse( M_OFF, NULL );
	} else {
		/* Hier stimmt eigentlich etwas nicht! */
		return 0;
	}
}
#endif