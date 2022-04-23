/*****************************************************************
	7UP
	Modul: USERDEF.C
	(c) by mt '90

	benutzerdefinierte AES Objekte
	
	1997-04-08 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-09 (MJK): MSDOS-Teile entfernt
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
	1997-04-16 (MJK): veraltetes FDB durch MFDB ersetzt
	2000-06-17 (GS) :	draw_altbutton, draw_checkbox und draw_radio erweitert,
										daû sie mit jedem AES-Font zurecht kommten (nach einer
										Idee von Ulrich Kaiser (UK)).
										BerÅcksichtigt nur Texte mit groûem Font (IBM)!

*****************************************************************/

#define RSC_CREATE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <gem.h>
#	include <gemx.h>
#endif
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#	include <ext.h>
#else
#	include <osbind.h>
#endif

#include "windows.h"
#include "7up.h"
#include "language.h"
#include "toolbar.h"
#include "7up3.h"
#include "forms.h"
#include "resource.h"
#include "editor.h"
#include "fontsel.h"
#include "fontaesi.h"

#include "userdef.h"

void  _exit( int status );

#ifndef SQUARED	/* error at gemfast? */
#	define SQUARED SQUARE
#endif

#ifndef DESK
#	define DESK		0
#endif

#ifndef MRKR_DOT
#	ifdef PM_DOT
#		define MRKR_DOT PM_DOT
#	endif
#endif

#ifndef __CDECL 
#	ifdef CDECL
#		define __CDECL CDECL
#	else
#		error __CDECL undefined, just define it
#	endif
#endif

#define DRBUTTON	 0x0001					 /* user defined objects */
#define DCHECKBOX  0x0002
#define DALTBUTTON 0x0003
#define DULINE	   0x0004
#define DEAR		   0x0005
#define DCIRCLE	   0x0006
#define DDLINE	   0x0007
#define DSELFONT   0x0008
#define D3DBOX     0x0009
#define DHEADER	   0x0020
#define DFONT	     0x0080
#define DTABBAR    0x00FF

#define FLAGS9  	 0x0200
#define FLAGS10 	 0x0400
#define FLAGS14    0x4000
#define FLAGS15    0x8000

int threedee; /* lange öberschriftenunterstreichung */
int dialbgcolor=WHITE;
int actbutcolor=WHITE;

static  USERBLK	  font_blk;		 /* used for 7UP fonts */
static  USERBLK	  ear_blk;
static  USERBLK	  circle_blk;
static  USERBLK	  dline_blk;
static  USERBLK	  selfont_blk;
static  USERBLK	  threeDbox_blk;

/*********************************************************************/

static int txtlen(int handle,char *str)
{
	int	pxy[8];
	
/*
	if (gl_nvdi >= 0x300)
		vqt_real_extent(handle, 0, 0, str, pxy);
	else
*/
		vqt_extent(handle, str, pxy);

	return pxy[2] - pxy[0];
}

static int get_shortcut(char *text, char *c)
{
	int pos;
	char *cp;
	
	pos = -1;
	if (c != NULL)
		*c = '\0';

  if((cp=strchr(text,'_'))!=NULL)
  {
  	pos = (int) (cp - text);
		if (c != NULL)
   		*c = *(cp +1);
	}

	return pos;
}

static void vdi_fix (MFDB *pfd, void *theaddr, int wb, int h)
{
  pfd->fd_addr    = theaddr;
  pfd->fd_w       = wb << 3;
  pfd->fd_h       = h;
  pfd->fd_wdwidth = wb >> 1;
  pfd->fd_nplanes = 1;
} /* vdi_fix */

static void vdi_trans (void *saddr, int swb, int *daddr, int dwb, int h)
{
  MFDB src, dst;

  vdi_fix(&src, saddr, swb, h);
  src.fd_stand = TRUE;

  vdi_fix(&dst, daddr, dwb, h);
  dst.fd_stand = FALSE;

  vr_trnfm (userhandle, &src, &dst);
} /* vdi_trans */

static void trans_gimage (OBJECT *tree, int obj)
{
  ICONBLK *piconblk;
  BITBLK  *pbitblk;
  void    *taddr;
  int	    wb, hl, type;

  type = (tree [obj].ob_type & 0xFF);
  if (type == G_ICON)
  {
	 piconblk = tree[obj].ob_spec.iconblk;
	 taddr	  = piconblk->ib_pmask;
	 wb		    = piconblk->ib_wicon;
	 wb		    = wb >> 3;
	 hl		    = piconblk->ib_hicon;
	 vdi_trans (taddr, wb, taddr, wb, hl);
	 taddr    = piconblk->ib_pdata;
  } /* if */
  else
  {
	 pbitblk = tree [obj].ob_spec.bitblk;
	 taddr	= pbitblk->bi_pdata;
	 wb		= pbitblk->bi_wb;
	 hl		= pbitblk->bi_hl;
  } /* else */
  vdi_trans (taddr, wb, taddr, wb, hl);
} /* trans_gimage */

/*****************************************************************************/
static void set_clip (int handle, int x, int y, int w, int h)
{
  int pxy[4];

  pxy [0] = x;
  pxy [1] = y;
  pxy [2] = x + w - 1;
  pxy [3] = y + h - 1;

  vs_clip (handle, TRUE, pxy);
} /* set_clip */

static void reset_clip (int handle, int x, int y, int w, int h)
{
  int pxy[4];

  pxy [0] = x;
  pxy [1] = y;
  pxy [2] = x + w - 1;
  pxy [3] = y + h - 1;

  vs_clip (handle, FALSE, pxy);
} /* set_clip */

/*****************************************************************************/
/* Zeichnet tastaturbedienbare Exitbuttons															 		 */
/*****************************************************************************/

#define odd(i) ((i)&1)

static int __CDECL draw_altbutton (PARMBLK *pb)
{
  short   ob_x, ob_y, ob_width, ob_height;
  BOOLEAN selected, changed;
  int     pxy [10];
  int	    i,cx,cy,cw,ch,ret,l_width;
  int     bw,bh;  /* UK: boxwidth,boxheigt   */
	int     distances[5], effects[3]; /* GS: */
  char	 *cp,string[32], c;

  ob_x		= pb->pb_x+2;
  ob_y		= pb->pb_y+2;
  ob_width  = pb->pb_w-4;
  ob_height = pb->pb_h-4;

  selected  = pb->pb_currstate & SELECTED;
  changed	= (pb->pb_currstate ^ pb->pb_prevstate) & SELECTED;

  vsl_type (userhandle, SOLID);
  vsl_ends (userhandle, SQUARED, SQUARED);
  vsl_width (userhandle, l_width=1);
  vsl_color (userhandle, BLACK);
  vsf_interior(userhandle,FIS_SOLID); /* FÅllung */
  vst_alignment(userhandle,0,5,&ret,&ret); /* Ausrichtung */
  vswr_mode (userhandle, MD_REPLACE);

  if(pb->pb_tree[pb->pb_obj].ob_flags & EXIT)
  {
	  l_width=2;
  }
  if(pb->pb_tree[pb->pb_obj].ob_flags & DEFAULT)
  {
	  l_width=3;
  }

  set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  strcpy(string,(char *)((TEDINFO *)pb->pb_parm)->te_ptext);
  if((cp=strchr(string,'_'))!=NULL)
	  strcpy(cp,&cp[1]);

  if(threedee && !strcmp(KHILFE,string)) /* Hilfeknopf ein Pixel breit */
  	  l_width=1;
  	
  if (! changed) /* it was an objc_draw, so draw button */
  {
	 for(i=0; i<l_width; i++)
	 {
		 pxy [0] = ob_x -i;
		 pxy [1] = ob_y -i;
		 pxy [2] = ob_x + ob_width  - 1 +i;
		 pxy [3] = pxy [1];
		 pxy [4] = pxy [2];
		 pxy [5] = ob_y + ob_height - 1 +i;
		 pxy [6] = pxy [0];
		 pxy [7] = pxy [5];
		 pxy [8] = pxy [0];
		 pxy [9] = pxy [1];
		 v_pline (userhandle, 5, pxy);
	 }
	 pxy[0]=ob_x+1;
	 pxy[1]=ob_y+1;
	 pxy[2]=ob_x+ob_width-2;
	 pxy[3]=ob_y+ob_height-2;
	 if(selected)
	 {
		vsf_color(userhandle,BLACK);		  /* farbe  */
		vst_color(userhandle,WHITE);		  /* farbe  */
	 }
	 else
	 {
	 	if(threedee) /*3D*/
			vsf_color(userhandle,actbutcolor);		  /* farbe  */
		else
			vsf_color(userhandle,WHITE);		  /* farbe  */
		vst_color(userhandle,BLACK);		  /* farbe  */
	 }
	 vr_recfl(userhandle,pxy);	  /* weiûes rechteck in workspace */

   vst_color (userhandle, BLACK);
	 vswr_mode (userhandle, MD_TRANS); /*3D (XOR) */

	 if(((TEDINFO *)pb->pb_parm)->te_font==IBM)
	 {
			/* UK: removed: vst_point(userhandle,norm_point,&ret,&ret,&cw,&ch); */
			vst_font(userhandle,AES_fontid_norm); /* (UK) */
      vst_height(userhandle,AES_fontheight_norm,&cw,&ch,&bw,&bh); /* (UK) */

      /* UK: removed: cx=(int)(pb->pb_x+(pb->pb_w-strlen(string)*cw)/2); */
      /* UK: removed: cy=pb->pb_y+(pb->pb_h-ch)/2-1; */
           
      vqt_extent(userhandle,string,pxy);         /* (UK) */
      cx = pb->pb_x + (pb->pb_w - pxy[2]) / 2;   /* UK: zentrieren innherhalb pb_w */
      vqt_attributes(userhandle,pxy);            /* UK: pxy[9] enthÑlt die cell height
																										fÅr den oben gesetzten Font       */
      vqt_fontinfo(userhandle,&ret,&ret,distances,&ret,effects); /* UK: todo: geeignte Dekl. von distances und effects */
      /* UK: distances[4] ist der Abstand zwischen der Oberkante der "Zelle" des Zeichensatzes und der sog. Basislinie */

      cy = pb->pb_y + ((pb->pb_h - pxy[9]) / 2) /*+ distances[4]*/; /* (UK) */

		 if(boxh<=8)
		 	cy++;
		 if(threedee) /* 3D-Look */
		 {
	       if(selected)
	       {
	       	cx++;
	       	cy++;
	       }
       }
		 if(threedee) /* 3D-Look */
			 v_gtext(userhandle,cx,cy,string);
		 else
			 v_gtext(userhandle,cx,cy+1,string); /* ein Pixel tiefer */
		 if(cp)
		 {
       /* UK: removed:
			 pxy[0]=(int)(cx + (cp-string)*cw);
			 pxy[1]=cy + ch - 1;
			 pxy[2]=(int)(cx + (cp-string+1L)*cw - 1);
			 pxy[3]=cy + ch - 1;
			 vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]++;
				 pxy[3]++;
			 }
			 if(!threedee) /* kein 3D-Look */
			 {
				 pxy[1]++;
				 pxy[3]++;
			 }
			 v_pline(userhandle,2,pxy);
       :removed :UK */
			 
			 i = get_shortcut((char *)((TEDINFO *)pb->pb_parm)->te_ptext, &c);
   		 strcpy(string, (char *)((TEDINFO *)pb->pb_parm)->te_ptext);
			 string[i] = 0;
			 i = txtlen(userhandle, string);
			 string[0]=c;
			 string[1]=0;
       vst_effects(userhandle,TXT_UNDERLINED);			/* (UK) */
       v_gtext(userhandle,cx + i,cy,string);       	/* (UK) */
       vst_effects(userhandle,0);          					/* (UK) */
		 }
	 }
	 else
	 {
		 vst_height(userhandle,small_point,&ret,&ret,&cw,&ch);
		 cx=pb->pb_x+(3*cw);
		 cy=pb->pb_y+((boxh>8)?SMALL:SMALL/2-1)-1;
		 if(threedee) /* 3D-Look */
		 {
	       if(selected)
	       {
	       	cx++;
	       	cy++;
	       }
       }
		 v_gtext(userhandle,cx,cy,string);
		 if(cp)
		 {
			 pxy[0]=(int)(cx + (cp-string)*cw);
			 pxy[1]=cy + ch - 4;
			 pxy[2]=(int)(cx + (cp-string+1L)*cw - 1);
			 pxy[3]=cy + ch - 4;
			 vsl_color (userhandle, BLACK);
			 v_pline(userhandle,2,pxy);
		 }
	 }
	 vswr_mode (userhandle, MD_REPLACE);
	 if(threedee) /* 3D-Look */
	 {
		 vsl_color (userhandle, WHITE);/* oben, links weiû */
		 pxy[0]=ob_x+2-1;
		 pxy[1]=ob_y+ob_height-2-1;
		 pxy[2]=ob_x+2-1;
		 pxy[3]=ob_y+2-1;
		 pxy[4]=ob_x+ob_width-2-1;
		 pxy[5]=ob_y+2-1;
		 v_pline (userhandle, 3, pxy); 

		 vsl_color (userhandle, LBLACK); 
		 pxy[0]=ob_x+2;
		 pxy[1]=ob_y+ob_height-2;
		 pxy[2]=ob_x+ob_width-2;
		 pxy[3]=ob_y+ob_height-2;
		 pxy[4]=ob_x+ob_width-2;
		 pxy[5]=ob_y+2;
		 v_pline (userhandle, 3, pxy);/* unten, rechts schwarz */
	 }
  }
  else
  {
	 if(!threedee) /* kein 3D-Look */
	 {
		 pxy[0]=ob_x+1;
		 pxy[1]=ob_y+1;
		 pxy[2]=ob_x+ob_width-2;
		 pxy[3]=ob_y+ob_height-2;
		 if(selected)
		 {
			vsf_color(userhandle,BLACK);		  /* farbe  */
		 }
		 else
		 {
			vsf_color(userhandle,WHITE);		  /* farbe  */
		 }
		 vswr_mode(userhandle, MD_XOR);
		 vr_recfl(userhandle,pxy);	  /* weiûes rechteck in workspace */
	 }
	 else
	 {
		 pxy[0]=ob_x+1;
		 pxy[1]=ob_y+1;
		 pxy[2]=ob_x+ob_width-2;
		 pxy[3]=ob_y+ob_height-2;
		 vsf_color(userhandle,actbutcolor);		  /* farbe  */
		 vst_color(userhandle,BLACK);		  /* farbe  */
  		 vswr_mode (userhandle, MD_REPLACE); 
		 vr_recfl(userhandle,pxy);	  /* weiûes rechteck in workspace */
	
  		 vswr_mode (userhandle, MD_TRANS); 

		 if(((TEDINFO *)pb->pb_parm)->te_font==IBM)
		 {
			 /* UK: removed: vst_point(userhandle,norm_point,&ret,&ret,&cw,&ch); */
			 vst_font(userhandle,AES_fontid_norm); /* (UK) */
       vst_height(userhandle,AES_fontheight_norm,&cw,&ch,&bw,&bh); /* (UK) */

       /* UK: removed: cx=(int)(pb->pb_x+(pb->pb_w-strlen(string)*cw)/2); */
       /* UK: removed: cy=pb->pb_y+(pb->pb_h-ch)/2-1; */
           
       vqt_extent(userhandle,string,pxy);         /* (UK) */
       cx = pb->pb_x + (pb->pb_w - pxy[2]) / 2;   /* UK: zentrieren innherhalb pb_w */
       vqt_attributes(userhandle,pxy);            /* UK: pxy[9] enthÑlt die cell height
																										fÅr den oben gesetzten Font       */
       vqt_fontinfo(userhandle,&ret,&ret,distances,&ret,effects); /* UK: todo: geeignte Dekl. von distances und effects */
       /* UK: distances[4] ist der Abstand zwischen der Oberkante der "Zelle" des Zeichensatzes und der sog. Basislinie */

       cy = pb->pb_y + ((pb->pb_h - pxy[9]) / 2) /*+ distances[4]*/; /* (UK) */
 		   if(boxh<=8)
				cy++;
	     if(selected)
	     {
	      cx++;
	     	cy++;
	     }
			 v_gtext(userhandle,cx,cy,string);
			 if(cp)
			 {
    		 /* UK: removed:
				 pxy[0]=(int)(cx + (cp-string)*cw);
				 pxy[1]=cy + ch - 1;
				 pxy[2]=(int)(cx + (cp-string+1L)*cw - 1);
				 pxy[3]=cy + ch - 1;
				 vsl_color (userhandle, BLACK);
				 if(boxh<=8)
				 {
					 pxy[1]++;
					 pxy[3]++;
				 }
				 v_pline(userhandle,2,pxy);
         :removed :UK */

				 i = get_shortcut((char *)((TEDINFO *)pb->pb_parm)->te_ptext, &c);
	   		 strcpy(string, (char *)((TEDINFO *)pb->pb_parm)->te_ptext);
				 string[i] = 0;
				 i = txtlen(userhandle, string);
				 string[0]=c;
				 string[1]=0;
	       vst_effects(userhandle,TXT_UNDERLINED);			/* (UK) */
	       v_gtext(userhandle,cx + i,cy,string);       	/* (UK) */
	       vst_effects(userhandle,0);          					/* (UK) */
			 }
		 }
		 else
		 {
		 	 vst_height(userhandle,small_point,&ret,&ret,&cw,&ch);
			 cx=pb->pb_x+(3*cw);
			 cy=pb->pb_y+((boxh>8)?SMALL:SMALL/2-1)-1;
	       if(selected)
	       {
	       	cx++;
	       	cy++;
	       }
			 v_gtext(userhandle,cx,cy,string);
			 if(cp)
			 {
				 pxy[0]=(int)(cx + (cp-string)*cw);
				 pxy[1]=cy + ch - 4;
				 pxy[2]=(int)(cx + (cp-string+1L)*cw - 1);
				 pxy[3]=cy + ch - 4;
				 vsl_color (userhandle, BLACK);
				 v_pline(userhandle,2,pxy);
			 }
		 }
		 vswr_mode (userhandle, MD_REPLACE);
		 if(selected)
		 {
			 vsl_color (userhandle, BLACK);/* oben, links schwarz */
			 pxy[0]=ob_x+2-1;
			 pxy[1]=ob_y+ob_height-2-1;
			 pxy[2]=ob_x+2-1;
			 pxy[3]=ob_y+2-1;
			 pxy[4]=ob_x+ob_width-2-1;
			 pxy[5]=ob_y+2-1;
			 v_pline (userhandle, 3, pxy); 

			 vsl_color (userhandle, WHITE);
			 pxy[0]=ob_x+2;
			 pxy[1]=ob_y+ob_height-2;
			 pxy[2]=ob_x+ob_width-2;
			 pxy[3]=ob_y+ob_height-2;
			 pxy[4]=ob_x+ob_width-2;
			 pxy[5]=ob_y+2;
			 v_pline (userhandle, 3, pxy);/* unten, rechts weiû */ 
		 }
		 else
		 {
			 vsl_color (userhandle, WHITE);/* oben, links weiû */
			 pxy[0]=ob_x+2-1;
			 pxy[1]=ob_y+ob_height-2-1;
			 pxy[2]=ob_x+2-1;
			 pxy[3]=ob_y+2-1;
			 pxy[4]=ob_x+ob_width-2-1;
			 pxy[5]=ob_y+2-1;
			 v_pline (userhandle, 3, pxy); 

			 vsl_color (userhandle, LBLACK); /* dunkelgrau */
			 pxy[0]=ob_x+2;
			 pxy[1]=ob_y+ob_height-2;
			 pxy[2]=ob_x+ob_width-2;
			 pxy[3]=ob_y+ob_height-2;
			 pxy[4]=ob_x+ob_width-2;
			 pxy[5]=ob_y+2;
			 v_pline (userhandle, 3, pxy);/* unten, rechts dunkelgrau */
		 }
	 }
  }

  reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  return (pb->pb_currstate & ~ SELECTED);
} /* draw_altbutton */

/*****************************************************************************/
/* Zeichnet ankreuzbare Buttons									  													 */
/*****************************************************************************/

static int __CDECL draw_checkbox (PARMBLK *pb)
{
  short   ob_x, ob_y, ob_width, ob_height;
  BOOLEAN disabled, selected, changed;
  int     pxy[12];
  int			i;	/* GS */
  int	    cw,ch,ret,viele_Farben;
  char	 *cp,string[32],c;

  ob_x		= pb->pb_x+1;
  ob_y		= pb->pb_y+1;
  ob_width  = pb->pb_h-2; /* nicht pb_w!!! */  /* 3 */
  ob_height = pb->pb_h-2;                      /* 3 */
  selected  = pb->pb_currstate & SELECTED;
  disabled  = pb->pb_currstate & DISABLED;
  changed	= (pb->pb_currstate ^ pb->pb_prevstate) & SELECTED;
  viele_Farben = mindestens_16_Farben();

  set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  vsf_perimeter(userhandle,TRUE);
  vsl_type (userhandle, SOLID);
  vsl_ends (userhandle, SQUARED, SQUARED);
  vsl_width (userhandle, 1);
  vsl_color (userhandle, BLACK);

  if(threedee && disabled && viele_Farben)
     vst_color (userhandle, WHITE);
  else  
     vst_color (userhandle, BLACK);

  vsm_type(userhandle,MRKR_DOT);
  vswr_mode (userhandle, MD_TRANS);

  if (! changed) /* it was an objc_draw, so draw box */
  {
	 pxy [0] = ob_x;
	 pxy [1] = ob_y + ob_height - 1;
	 pxy [2] = ob_x;
	 pxy [3] = ob_y;
	 pxy [4] = ob_x + ob_width - 1;
	 pxy [5] = ob_y;
	 v_pline (userhandle, 3, pxy);

    if(threedee && viele_Farben)
	    vsl_color (userhandle, WHITE);
	 pxy [ 6] = ob_x + ob_width - 1;
	 pxy [ 7] = ob_y + 1;
	 pxy [ 8] = ob_x + ob_width - 1;
	 pxy [ 9] = ob_y + ob_height - 1;
	 pxy [10] = ob_x + 1;
	 pxy [11] = ob_y + ob_height - 1;
	 v_pline (userhandle, 3, &pxy[ 6]);

    vsl_color (userhandle, BLACK);

	 strcpy(string,(char *)((TEDINFO *)pb->pb_parm)->te_ptext);
	 if((cp=strchr(string,'_'))!=NULL)
		 strcpy(cp,&cp[1]);
	 vst_alignment(userhandle,0,5,&ret,&ret); /* Ausrichtung */

	 if(((TEDINFO *)pb->pb_parm)->te_font==IBM)
	 {
  	 vst_font(userhandle,AES_fontid_norm); /* (UK) */
     vst_height(userhandle,AES_fontheight_norm,&ret,&ret,&ret,&ret); /* (UK) */

     vqt_attributes(userhandle,pxy);            /* UK: pxy[9] enthÑlt die cell height
																										fÅr den oben gesetzten Font       */
		 if(boxh>8)
		 {
			 v_gtext(userhandle,pb->pb_x+(3*pxy[6]),pb->pb_y-1,string);
		 }
		 else
		 {
			 v_gtext(userhandle,pb->pb_x+(3*pxy[6]),pb->pb_y,string);
		 }
		 if(cp)
		 {
			 i = get_shortcut((char *)((TEDINFO *)pb->pb_parm)->te_ptext, &c);
   		 strcpy(string, (char *)((TEDINFO *)pb->pb_parm)->te_ptext);
			 string[i] = 0;
			 i = txtlen(userhandle, string);
			 string[0]=c;
			 string[1]=0;
       vst_effects(userhandle,TXT_UNDERLINED);			/* (UK) */
			 if(boxh>8)
			 {
				 v_gtext(userhandle,pb->pb_x+(3*pxy[6]) + i,pb->pb_y-1,string);
			 }
			 else
			 {
				 v_gtext(userhandle,pb->pb_x+(3*pxy[6]) + i,pb->pb_y,string);
			 }
       vst_effects(userhandle,0);          					/* (UK) */
		 }

		 /* GS: removed:
		 vst_point(userhandle,norm_point,&ret,&ret,&cw,&ch);
		 if(boxh>8)
		 {
			 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y-1,string);
		 }
		 else
		 {
			 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y,string);
		 }
		 if(cp)
		 {
			 pxy[0]=(int)(pb->pb_x + (cp-string)*cw+(3*cw));
			 pxy[1]=pb->pb_y + pb->pb_h - 2;
			 pxy[2]=(int)(pb->pb_x + (cp-string+1L)*cw+(3*cw) - 1);
			 pxy[3]=pb->pb_y + pb->pb_h - 2;
		    if(threedee && disabled && viele_Farben)
	   		vsl_color (userhandle, WHITE);
	   	 else
			 	vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]++;
				 pxy[3]++;
			 }
			 v_pline(userhandle,2,pxy);
		 }
		 :removed GS:*/
	 }
	 else
	 {
		 vst_height(userhandle,small_point,&ret,&ret,&cw,&ch);
		 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y+((boxh>8)?SMALL:SMALL/2-1),string);
		 if(cp)
		 {
			 pxy[0]=(int)(pb->pb_x + (cp-string)*cw+(3*cw));
			 pxy[1]=pb->pb_y + pb->pb_h - 4;
			 pxy[2]=(int)(pb->pb_x + (cp-string+1L)*cw+(3*cw) - 1);
			 pxy[3]=pb->pb_y + pb->pb_h - 4;
		    if(threedee && disabled && viele_Farben)
	   		vsl_color (userhandle, WHITE);
	   	 else
				vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]+=3;
				 pxy[3]+=3;
			 }
			 v_pline(userhandle,2,pxy);
		 }
	 }
  }

  if (!selected && !disabled) /* it was an objc_change */
	 vsl_color (userhandle, dialbgcolor);

  if (selected) /* it was an objc_change */
	 vsl_color (userhandle, BLACK);

  if (disabled) /* it was an objc_change */
	 vsl_color (userhandle, dialbgcolor);

  pxy [0] = ob_x + 1;
  pxy [1] = ob_y + 1;
  pxy [2] = ob_x + ob_width - 2;
  pxy [3] = ob_y + ob_height - 2;
  v_pline (userhandle, 2, pxy);

  pxy [0] = ob_x + ob_width - 2;
  pxy [1] = ob_y + 1;
  pxy [2] = ob_x + 1;
  pxy [3] = ob_y + ob_height - 2;
  v_pline (userhandle, 2, pxy);

  reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  if (threedee && viele_Farben)
	  return (pb->pb_currstate & ~ (SELECTED|DISABLED));
  else
	  return (pb->pb_currstate & ~ SELECTED);
} /* draw_checkbox */

/*****************************************************************************/
/* Zeichnet runde Radiobuttons															  */
/*****************************************************************************/
static int __CDECL draw_radio (PARMBLK *pb)
{
  short   ob_x, ob_y, ob_height;
  BOOLEAN disabled,selected,changed;
  MFDB    s, d;
  BITBLK *bitblk;
  int     robj; /* radio button object number */
  int 		i;
  int     pxy [10];
  int     index [2];
  int	    cw,ch,ret,viele_Farben;
  char	 *cp,string[32],c;

  ob_x         = pb->pb_x;
  ob_y         = pb->pb_y;
  ob_height    = pb->pb_h;
  disabled     = pb->pb_currstate & DISABLED;
  selected     = pb->pb_currstate & SELECTED;
  changed      = (pb->pb_currstate ^ pb->pb_prevstate) & SELECTED;
  viele_Farben = mindestens_16_Farben();

  if(threedee && disabled && viele_Farben)
     vst_color (userhandle, WHITE);
  else  
     vst_color (userhandle, BLACK);

  set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  if (selected) /* it was an objc_change */
  {
	  switch(norm_point)
	  {
	     case 9:
	        robj=RBLSEL;
	        break;
	     case 10:
	        if(threedee)
		        robj=RBHSEL3D;
		     else
		        robj=RBHSEL;
	        break;
	     case 20:
	        robj=RBBSEL;
	        break;
	     default:
	        if(threedee)
		        robj=RBHSEL3D;
		     else
		        robj=RBHSEL;
	        break;
	  }
  }
  else
  {
	  switch(norm_point)
	  {
	     case 9:
	        robj=RBLNORM;
	        break;
	     case 10:
	        if(threedee)
		        robj=RBHNORM3D;
		     else
		        robj=RBHNORM;
	        break;
	     case 20:
	        robj=RBBNORM;
	        break;
	     default:
	        if(threedee)
		        robj=RBHNORM3D;
		     else
		        robj=RBHNORM;
	        break;
	  }
  }
  
  if(threedee)
  {
	  bitblk = userimg [RBHBG3D].ob_spec.bitblk;
	
	  d.fd_addr    = NULL; /* screen */
	  s.fd_addr    = (void *)bitblk->bi_pdata;
	  s.fd_w       = bitblk->bi_wb << 3;
	  s.fd_h       = bitblk->bi_hl;
	  s.fd_wdwidth = s.fd_w/16;
	  s.fd_stand   = FALSE;
	  s.fd_nplanes = 1;
	
	  pxy [0] = 0;
	  pxy [1] = 0;
	  pxy [2] = s.fd_w - 1;
	  pxy [3] = s.fd_h - 1;
	  pxy [4] = ob_x + 1 + (ob_height-bitblk->bi_wb*8)/2-1;/* nicht ob_width! */
	  if(boxh<=8)
		 pxy [4]+=ob_height/2-1;
	  pxy [5] = ob_y + 1 + (ob_height-bitblk->bi_hl)/2-1;
	  pxy [6] = ob_x + pxy [2];
	  pxy [7] = ob_y + pxy [3];
	
	  index [0] = WHITE;
	  index [1] = dialbgcolor;
	  vrt_cpyfm (userhandle, MD_REPLACE, pxy, &s, &d, index);	 /* copy it */
  }
  
  bitblk = userimg [robj].ob_spec.bitblk;

  d.fd_addr    = NULL; /* screen */
  s.fd_addr    = (void *)bitblk->bi_pdata;
  s.fd_w       = bitblk->bi_wb << 3;
  s.fd_h       = bitblk->bi_hl;
  s.fd_wdwidth = s.fd_w/16;
  s.fd_stand   = FALSE;
  s.fd_nplanes = 1;

  pxy [0] = 0;
  pxy [1] = 0;
  pxy [2] = s.fd_w - 1;
  pxy [3] = s.fd_h - 1;
  pxy [4] = ob_x + 1 + (ob_height-bitblk->bi_wb*8)/2-1;/* nicht ob_width! */
  if(boxh<=8)
		pxy [4]+=ob_height/2-1;
  pxy [5] = ob_y + 1 + (ob_height-bitblk->bi_hl)/2-1;
  pxy [6] = ob_x + pxy [2];
  pxy [7] = ob_y + pxy [3];

  index [0] = BLACK;
  index [1] = dialbgcolor;

  vrt_cpyfm (userhandle, threedee?MD_TRANS:MD_REPLACE, pxy, &s, &d, index);	 /* copy it */

  vswr_mode (userhandle, MD_TRANS);

  if(!changed)
  {
	 strcpy(string,(char *)((TEDINFO *)pb->pb_parm)->te_ptext);
	 if((cp=strchr(string,'_'))!=NULL)
		 strcpy(cp,&cp[1]);
	 vst_alignment(userhandle,0,5,&ret,&ret); /* Ausrichtung */

	 if(((TEDINFO *)pb->pb_parm)->te_font==IBM)
	 {
  	 vst_font(userhandle,AES_fontid_norm); /* (UK) */
     vst_height(userhandle,AES_fontheight_norm,&ret,&ret,&ret,&ret); /* (UK) */

     vqt_attributes(userhandle,pxy);            /* UK: pxy[9] enthÑlt die cell height
																										fÅr den oben gesetzten Font       */
		 if(boxh>8)
		 {
			 v_gtext(userhandle,pb->pb_x+(3*pxy[6]),pb->pb_y-1,string);
		 }
		 else
		 {
			 v_gtext(userhandle,pb->pb_x+(3*pxy[6]),pb->pb_y,string);
		 }
		 if(cp)
		 {
			 i = get_shortcut((char *)((TEDINFO *)pb->pb_parm)->te_ptext, &c);
   		 strcpy(string, (char *)((TEDINFO *)pb->pb_parm)->te_ptext);
			 string[i] = 0;
			 i = txtlen(userhandle, string);
			 string[0]=c;
			 string[1]=0;
       vst_effects(userhandle,TXT_UNDERLINED);			/* (UK) */
			 if(boxh>8)
			 {
				 v_gtext(userhandle,pb->pb_x+(3*pxy[6]) + i,pb->pb_y-1,string);
			 }
			 else
			 {
				 v_gtext(userhandle,pb->pb_x+(3*pxy[6]) + i,pb->pb_y,string);
			 }
       vst_effects(userhandle,0);          					/* (UK) */
		 }

		 /* GS: removed:
		 vst_point(userhandle,norm_point,&ret,&ret,&cw,&ch);
		 if(boxh>8)
		 {
			 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y-1,string);
		 }
		 else
		 {
			 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y,string);
		 }
		 if(cp)
		 {
			 pxy[0]=(int)(pb->pb_x + (cp-string)*cw+(3*cw));
			 pxy[1]=pb->pb_y + pb->pb_h - 2;
			 pxy[2]=(int)(pb->pb_x + (cp-string+1L)*cw+(3*cw) - 1);
			 pxy[3]=pb->pb_y + pb->pb_h - 2;
		    if(threedee && disabled && viele_Farben)
	   		vsl_color (userhandle, WHITE);
	   	 else
				vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]++;
				 pxy[3]++;
			 }
			 v_pline(userhandle,2,pxy);
		 }
		 :removed GS:*/
	 }
	 else
	 {
		 vst_height(userhandle,small_point,&ret,&ret,&cw,&ch);
		 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y+((boxh>8)?SMALL:SMALL/2-1),string);
		 if(cp)
		 {
			 pxy[0]=(int)(pb->pb_x + (cp-string)*cw+(3*cw));
			 pxy[1]=pb->pb_y + pb->pb_h - 4;
			 pxy[2]=(int)(pb->pb_x + (cp-string+1L)*cw+(3*cw) - 1);
			 pxy[3]=pb->pb_y + pb->pb_h - 4;
		    if(threedee && disabled && viele_Farben)
	   		vsl_color (userhandle, WHITE);
	   	 else
				vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]+=3;
				 pxy[3]+=3;
			 }
			 v_pline(userhandle,2,pxy);
		 }
	 }
  }

  reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  if(threedee && viele_Farben)
     return (pb->pb_currstate & ~ (SELECTED|DISABLED));
  else
     return (pb->pb_currstate & ~ SELECTED);
} /* draw_radio */

/*
/*****************************************************************************/
/* Zeichnet runde Radiobuttons mit VDI	            NICHT BENUTZT!!!		     */
/*****************************************************************************/
static int __CDECL draw_radio (PARMBLK *pb)
{
  short   ob_x, ob_y, ob_height;
  BOOLEAN selected,changed;
  int	    cw,ch,ret,pxy[4];
  char	 *cp,string[32];

  ob_x		= pb->pb_x;
  ob_y		= pb->pb_y;
  ob_height = pb->pb_h;
  selected  = pb->pb_currstate & SELECTED;
  changed	= (pb->pb_currstate ^ pb->pb_prevstate) & SELECTED;

  set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  vst_color (userhandle, BLACK);
  vswr_mode (userhandle, MD_TRANS);
  vsf_color(userhandle,BLACK);
  vsf_interior(userhandle,FIS_HOLLOW);
  v_ellipse(userhandle,ob_x+ob_height/2-1,ob_y+ob_height/2-1,
                       ob_height/2-1,   ob_height/2-1);
  if(selected)
  {
	  vsf_interior(userhandle,FIS_SOLID);
	  v_ellipse(userhandle,ob_x+ob_height/2-1,ob_y+ob_height/2-1,
	                       ob_height/4,   ob_height/4);
  }

  if(!changed)
  {
	 strcpy(string,(char *)((TEDINFO *)pb->pb_parm)->te_ptext);
	 if((cp=strchr(string,'_'))!=NULL)
		 strcpy(cp,&cp[1]);
	 vst_alignment(userhandle,0,5,&ret,&ret); /* Ausrichtung */

	 if(((TEDINFO *)pb->pb_parm)->te_font==IBM)
	 {
		 vst_point(userhandle,norm_point,&ret,&ret,&cw,&ch);
		 if(boxh>8)
		 {
			 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y-1,string);
		 }
		 else
		 {
			 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y,string);
		 }
		 if(cp)
		 {
			 pxy[0]=pb->pb_x + (cp-string)*cw+(3*cw);
			 pxy[1]=pb->pb_y + pb->pb_h - 2;
			 pxy[2]=pb->pb_x + (cp-string+1L)*cw+(3*cw) - 1;
			 pxy[3]=pb->pb_y + pb->pb_h - 2;
			 vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]++;
				 pxy[3]++;
			 }
			 v_pline(userhandle,2,pxy);
		 }
	 }
	 else
	 {
		 vst_height(userhandle,small_point,&ret,&ret,&cw,&ch);
		 v_gtext(userhandle,pb->pb_x+(3*cw),pb->pb_y+((boxh>8)?SMALL:SMALL/2-1),string);
		 if(cp)
		 {
			 pxy[0]=pb->pb_x + (cp-string)*cw+(3*cw);
			 pxy[1]=pb->pb_y + pb->pb_h - 4;
			 pxy[2]=pb->pb_x + (cp-string+1L)*cw+(3*cw) - 1;
			 pxy[3]=pb->pb_y + pb->pb_h - 4;
			 vsl_color (userhandle, BLACK);
			 if(boxh<=8)
			 {
				 pxy[1]+=3;
				 pxy[3]+=3;
			 }
			 v_pline(userhandle,2,pxy);
		 }
	 }
  }

  reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  return (pb->pb_currstate & ~ SELECTED);
} /* draw_radio */
*/

/*****************************************************************************/
/* Zeichnet Buttons mit Kreis																*/
/*****************************************************************************/
static int __CDECL draw_circle (PARMBLK *pb)
{
  short	  ob_x, ob_y, ob_width, ob_height, cobj;
  BOOLEAN changed;
  MFDB	  s, d;
  BITBLK *bitblk;
  int 	  pxy[10];
  int 	  index [2];

  ob_x		= pb->pb_x;
  ob_y		= pb->pb_y;
  ob_width  = pb->pb_w;
  ob_height = pb->pb_h;
  changed	= (pb->pb_currstate ^ pb->pb_prevstate) & SELECTED;

  set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  if (! changed) /* it was an objc_draw, so draw box */
  {
	 vsf_interior(userhandle,FIS_SOLID);
	 vsf_perimeter (userhandle, TRUE);
    vswr_mode (userhandle, MD_TRANS);

	 pxy [0] = ob_x+2;
	 pxy [1] = ob_y+2;
	 pxy [2] = ob_x+2 + ob_width - 1;
	 pxy [3] = ob_y+2 + ob_height - 1;
	 vsf_color (userhandle, BLACK);
	 v_bar (userhandle, pxy);

	 pxy [0] = ob_x;
	 pxy [1] = ob_y;
	 pxy [2] = ob_x + ob_width  - 1;
	 pxy [3] = ob_y + ob_height - 1;
	 vsf_color (userhandle, WHITE);
	 v_bar (userhandle, pxy);

	 vsl_type (userhandle, SOLID);
	 vsl_ends (userhandle, SQUARED, SQUARED);
	 vsl_width (userhandle, 1);
	 vsl_color (userhandle, BLACK);
/*
    vswr_mode (userhandle, MD_REPLACE);
*/
	 pxy [0] = ob_x;
	 pxy [1] = ob_y;
	 pxy [2] = ob_x + ob_width - 1;
	 pxy [3] = ob_y;
	 pxy [4] = pxy [2];
	 pxy [5] = ob_y + ob_height - 1;
	 pxy [6] = ob_x;
	 pxy [7] = pxy [5];
	 pxy [8] = ob_x;
	 pxy [9] = ob_y;
	 v_pline (userhandle, 5, pxy);

  } /* if */
  switch(norm_point)
  {
     case 9:
        cobj=CIRCLEL;
        break;
     case 10:
     		if(pb->pb_currstate & DISABLED)
        		cobj=CIRCLEHDIS;
       	else
        		cobj=CIRCLEH;
        break;
     case 20:
        cobj=CIRCLEB;
        break;
     default:
     		if(pb->pb_currstate & DISABLED)
        		cobj=CIRCLEHDIS;
       	else
        		cobj=CIRCLEH;
        break;
  }
  bitblk = userimg [cobj].ob_spec.bitblk;

  d.fd_addr    = NULL; /* screen */
  s.fd_addr    = (void *)bitblk->bi_pdata;
  s.fd_w       = bitblk->bi_wb << 3;
  s.fd_h       = bitblk->bi_hl;
  s.fd_wdwidth = s.fd_w/16;
  s.fd_stand   = FALSE;
  s.fd_nplanes = 1;

  pxy [0] = 0;
  pxy [1] = 0;
  pxy [2] = s.fd_w - 1;
  pxy [3] = s.fd_h - 1;
  pxy [4] = ob_x + 1 + (ob_width-bitblk->bi_wb*8)/2-1;
  pxy [5] = ob_y + 1 + (ob_height-bitblk->bi_hl)/2-1;
  pxy [6] = ob_x + pxy [2];
  pxy [7] = ob_y + pxy [3];

  index [0] = BLACK;
  index [1] = WHITE;

  vrt_cpyfm (userhandle, MD_TRANS, pxy, &s, &d, index);	 /* copy it */

  reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

  return ((pb->pb_currstate & ~ SELECTED) & ~ DISABLED);
} /* draw_circel */

/*****************************************************************************/
/* Zeichnet aktuellen Font																	*/
/*****************************************************************************/
static int __CDECL draw_font (PARMBLK *pb)
{
	int     ret;
	BOOLEAN selected, changed;
	int     pxyarray[4];
	char    string[]="X";
	static  int w=0;

	selected  = pb->pb_currstate & SELECTED;
	changed	= (pb->pb_currstate ^ pb->pb_prevstate) & SELECTED;

   set_clip (twp->vdihandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	if(!changed) /* ordinary objc_draw */
	{
		vswr_mode(twp->vdihandle,MD_TRANS);
		if(pb->pb_obj==FCHAR)
			if(vst_point(twp->vdihandle,5,&ret,&ret,&w,&ret)!=5)
			{
				if(boxh>8)
					vst_point(twp->vdihandle,norm_point,&ret,&ret,&w,&ret);
				else
					vst_point(twp->vdihandle,5,&ret,&ret,&w,&ret);
			}
		*string=pb->pb_obj-FCHAR;
		v_gtext(twp->vdihandle,pb->pb_x+w,pb->pb_y,string);
	}
	else		  /* objc_change */
	{
      pxyarray[0]=pb->pb_x;
      pxyarray[1]=pb->pb_y;
      pxyarray[2]=pb->pb_x+pb->pb_w-1;
      pxyarray[3]=pb->pb_y+pb->pb_h-1;
		vswr_mode(twp->vdihandle,MD_XOR);
		if(selected)
		{
			vsf_color(twp->vdihandle,BLACK);
			vst_color(twp->vdihandle,WHITE);
			vr_recfl(twp->vdihandle,pxyarray);		/* markieren */
		}
		else
		{
			vsf_color(twp->vdihandle,WHITE);
			vst_color(twp->vdihandle,BLACK);
			vr_recfl(twp->vdihandle,pxyarray);		/* markieren */
		}
	}

   reset_clip (twp->vdihandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate & ~ SELECTED);

} /* draw_font */

/*****************************************************************************/
/* Zeichnet öberschriftenunterstreichung												 						 */
/*****************************************************************************/
static int __CDECL draw_uline (PARMBLK *pb)
{
	int x, cw, ch, width, ret, pxy[4];

  set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	vst_color (userhandle, BLACK);
	vswr_mode (userhandle, MD_TRANS);
	vst_alignment(userhandle,0,5,&ret,&ret); /* Ausrichtung */
	vst_point(userhandle,norm_point,&ret,&ret,&cw,&ch);
	switch(((TEDINFO *)pb->pb_parm)->te_just)
	{
		case TE_LEFT:
			x=pb->pb_x;
			break;
		case TE_RIGHT:
			x=(int)(pb->pb_x+pb->pb_w-strlen((char *)((TEDINFO *)pb->pb_parm)->te_ptext)*cw);
			break;
		case TE_CNTR:
			x=(int)(pb->pb_x+(pb->pb_w-strlen((char *)((TEDINFO *)pb->pb_parm)->te_ptext)*cw)/2);
			break;
	}

	v_gtext(userhandle,x,pb->pb_y,(char *)((TEDINFO *)pb->pb_parm)->te_ptext);
	width=pb->pb_w+2;

	pxy[0]=pb->pb_x - 1;
	pxy[1]=pb->pb_y + pb->pb_h;
	pxy[2]=pb->pb_x - 1 + width - 1;
	pxy[3]=pxy[1];
	vswr_mode (userhandle, MD_REPLACE);
	vsl_color (userhandle, BLACK);
	v_pline(userhandle,2,pxy);
	pxy[1]++;
	pxy[3]++;
	vsl_color (userhandle, WHITE);
	v_pline(userhandle,2,pxy);

  reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate);

} /* draw_uline */

/*****************************************************************************/
/* Zeichnet Eselsohren																											 */
/*****************************************************************************/
static int __CDECL draw_ear (PARMBLK *pb)
{
	int pxy[8];

   set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	vswr_mode (userhandle, MD_REPLACE);
	vsl_color (userhandle, BLACK);
	vsf_color(userhandle, dialbgcolor);
	vsf_perimeter(userhandle, TRUE);
	vsf_style(userhandle, 1);
	vsf_interior(userhandle, FIS_SOLID);

	pxy[0]=pb->pb_x;
	pxy[1]=pb->pb_y;
	pxy[2]=pb->pb_x + pb->pb_w - 1;
	pxy[3]=pb->pb_y + pb->pb_h - 1;
	pxy[4]=pb->pb_x;
	pxy[5]=pb->pb_y + pb->pb_h - 1;
	pxy[6]=pb->pb_x;
	pxy[7]=pb->pb_y;
	v_fillarea(userhandle,4,pxy);
	v_pline(userhandle,4,pxy);

	pxy[0]=pb->pb_x+3;
	pxy[1]=pb->pb_y+3;
	pxy[2]=pb->pb_x+3;
	pxy[3]=pb->pb_y + pb->pb_h - 1 - 3;
	pxy[4]=pb->pb_x + pb->pb_w - 1 - 3;
	pxy[5]=pb->pb_y + pb->pb_h - 1 - 3;
	v_pline(userhandle,3,pxy);

   reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate);

} /* draw_ear */

/*****************************************************************************/
/* Zeichnet 3Dbox																		  		  */
/*****************************************************************************/
static int __CDECL draw_3Dbox (PARMBLK *pb)
{
	int pxy[12];
	int ob_x,ob_y,ob_width,ob_height;
	
	ob_x      = pb->pb_x;
	ob_y      = pb->pb_y;
	ob_width  = pb->pb_w;
	ob_height = pb->pb_h;
	
   set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	vswr_mode(userhandle,MD_REPLACE);
	vsl_type (userhandle, SOLID);
	vsl_ends (userhandle, SQUARED, SQUARED);
	vsl_width (userhandle, 1);
	vsl_color (userhandle, BLACK);
   vsf_interior(userhandle,FIS_SOLID); /* FÅllung */

	pxy [0] = ob_x;
	pxy [1] = ob_y;
	pxy [2] = ob_x + ob_width - 1;
	pxy [3] = ob_y;
	pxy [4] = pxy [2];
	pxy [5] = ob_y + ob_height - 1;
	pxy [6] = ob_x;
	pxy [7] = pxy [5];
	pxy [8] = ob_x;
	pxy [9] = ob_y;
	v_pline (userhandle, 5, pxy);

	pxy[0]=ob_x+1;
	pxy[1]=ob_y+1;
	pxy[2]=ob_x+ob_width-3;
	pxy[3]=ob_y+ob_height-3;
	if(threedee) /*3D*/
		vsf_color(userhandle,dialbgcolor);		  /* farbe  */
	else
		vsf_color(userhandle,WHITE);		  /* farbe  */
	vr_recfl(userhandle,pxy);	  /* weiûes rechteck in workspace */

	if(threedee)
	{
		vsl_color (userhandle, WHITE);
		pxy [0] = ob_x + 1;
		pxy [1] = ob_y + ob_height - 2;
		pxy [2] = ob_x + 1;
		pxy [3] = ob_y + 1;
		pxy [4] = ob_x + ob_width - 2;
		pxy [5] = ob_y + 1;
		v_pline (userhandle, 3, pxy);

		vsl_color (userhandle, BLACK);
		pxy [0] = ob_x + ob_width - 2;
		pxy [1] = ob_y + 2;
		pxy [2] = ob_x + ob_width - 2;
		pxy [3] = ob_y + ob_height - 2;
		pxy [4] = ob_x + 2;
		pxy [5] = ob_y + ob_height - 2;
		v_pline (userhandle, 3, pxy);

		vsl_color (userhandle, WHITE);
		pxy [0] = ob_x + ob_width - 1;
		pxy [1] = ob_y + 1;
		pxy [2] = ob_x + ob_width - 1;
		pxy [3] = ob_y + ob_height - 1;
		pxy [4] = ob_x + 1;
		pxy [5] = ob_y + ob_height - 1;
		v_pline (userhandle, 3, pxy);

	}
   reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate);
}
/*

static int __CDECL draw_3Dbox (PARMBLK *pb)
{
	int pxy[12];
	int ob_x,ob_y,ob_width,ob_height;
	
	ob_x      = pb->pb_x;
	ob_y      = pb->pb_y;
	ob_width  = pb->pb_w;
	ob_height = pb->pb_h;
	
   set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	vswr_mode(userhandle,MD_REPLACE);
	vsl_type (userhandle, SOLID);
	vsl_ends (userhandle, SQUARED, SQUARED);
	vsl_width (userhandle, 1);
	vsl_color (userhandle, BLACK);
   vsf_interior(userhandle,FIS_SOLID); /* FÅllung */

	pxy [0] = ob_x;
	pxy [1] = ob_y;
	pxy [2] = ob_x + ob_width - 1;
	pxy [3] = ob_y;
	pxy [4] = pxy [2];
	pxy [5] = ob_y + ob_height - 1;
	pxy [6] = ob_x;
	pxy [7] = pxy [5];
	pxy [8] = ob_x;
	pxy [9] = ob_y;
	v_pline (userhandle, 5, pxy);

	pxy[0]=ob_x+1;
	pxy[1]=ob_y+1;
	pxy[2]=ob_x+ob_width-3;
	pxy[3]=ob_y+ob_height-3;
	if(threedee) /*3D*/
		vsf_color(userhandle,dialbgcolor);		  /* farbe  */
	else
		vsf_color(userhandle,WHITE);		  /* farbe  */
	vr_recfl(userhandle,pxy);	  /* weiûes rechteck in workspace */

	if(threedee)
	{
		vsl_color (userhandle, WHITE);
		pxy [0] = ob_x + ob_width - 2;
		pxy [1] = ob_y + 1;
		pxy [2] = ob_x + ob_width - 2;
		pxy [3] = ob_y + ob_height - 2;
		pxy [4] = ob_x + 1;
		pxy [5] = ob_y + ob_height - 2;
		v_pline (userhandle, 3, pxy);

		vsl_color (userhandle, LBLACK);
		pxy [0] = ob_x + 1;
		pxy [1] = ob_y + ob_height - 2;
		pxy [2] = ob_x + 1;
		pxy [3] = ob_y + 1;
		pxy [4] = ob_x + ob_width - 2;
		pxy [5] = ob_y + 1;
		v_pline (userhandle, 3, pxy);
	}
   reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate);
}
*/
/*
static int __CDECL draw_3Dbox (PARMBLK *pb)
{
	int pxy[12];
	int ob_x,ob_y,ob_width,ob_height,viele_Farben;
	
	ob_x      = pb->pb_x;
	ob_y      = pb->pb_y;
	ob_width  = pb->pb_w;
	ob_height = pb->pb_h;
	
   set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	vswr_mode(userhandle,MD_REPLACE);
	vsl_type (userhandle, SOLID);
	vsl_ends (userhandle, SQUARED, SQUARED);
	vsl_width (userhandle, 1);
	vsl_color (userhandle, BLACK);
   vsf_interior(userhandle,FIS_SOLID); /* FÅllung */
   viele_Farben = mindestens_16_Farben();

   pxy [0] = ob_x;
	pxy [1] = ob_y + ob_height - 1;
	pxy [2] = ob_x;
	pxy [3] = ob_y;
	pxy [4] = ob_x + ob_width - 1;
	pxy [5] = ob_y;
	v_pline (userhandle, 3, pxy);

	if(threedee && viele_Farben)
		vsl_color (userhandle, WHITE);

	pxy [0] = ob_x + ob_width - 1;
	pxy [1] = ob_y + 1;
	pxy [2] = pxy [0];
	pxy [3] = ob_y + ob_height - 1;
	pxy [4] = ob_x + 1;
	pxy [5] = pxy [3];
	v_pline (userhandle, 3, pxy);

	pxy[0]=ob_x+1;
	pxy[1]=ob_y+1;
	pxy[2]=ob_x+ob_width-3;
	pxy[3]=ob_y+ob_height-3;
	if(threedee && viele_Farben) /*3D*/
		vsf_color(userhandle,dialbgcolor);		  /* farbe  */
	else
		vsf_color(userhandle,WHITE);		  /* farbe  */
	vr_recfl(userhandle,pxy);	  /* weiûes rechteck in workspace */

   reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate);
}
*/
/*****************************************************************************/
/* Zeichnet selektierten Font in der Fontbox						   				  */
/*****************************************************************************/
static int __CDECL draw_selfont (PARMBLK *pb)
{
	int oby,h,ret;
	int pxyarray[14];
	
	pxyarray[10]=pb->pb_x;
	pxyarray[11]=pb->pb_y;
	pxyarray[12]=pb->pb_w;
	pxyarray[13]=pb->pb_h;
	
	if(rc_intersect(array2grect(&pb->pb_xc),array2grect(&pxyarray[10])))
	{
		set_clip (twp->vdihandle,pxyarray[10],pxyarray[11],pxyarray[12],pxyarray[13]);
		
		draw_3Dbox(pb);
		
		vst_font(twp->vdihandle,tid);
		if(tattr && (vq_vgdos()==0x5F46534DL)) /* Vektor-GDOS */
			vst_arbpt(twp->vdihandle,tsize,&ret,&ret,&ret,&h);
		else
			vst_point(twp->vdihandle,tsize,&ret,&ret,&ret,&h);
		oby =  pb->pb_y;
		oby += (pb->pb_h - h)/2;
		vswr_mode(twp->vdihandle, MD_TRANS);
		v_gtext(twp->vdihandle,pb->pb_x+1,oby,"The quick brown fox jumps over the lazy dog.");
		vst_font(twp->vdihandle,twp->fontid);
		vst_point(twp->vdihandle,twp->fontsize,&ret,&ret,&ret,&ret);
		vswr_mode(twp->vdihandle, MD_REPLACE); /* zurÅckstellen */
		
		reset_clip (twp->vdihandle,pxyarray[10],pxyarray[11],pxyarray[12],pxyarray[13]);
	}
	return (pb->pb_currstate);
}

static int __CDECL draw_tabbar (PARMBLK *pb)
{
	int i, ret, viele_Farben;
	char *cp;
	int attrib[10], pxy [4];
	
	set_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	vqt_attributes(userhandle,attrib);
	vst_font(userhandle,twp->fontid);
	vst_point(userhandle,twp->fontsize,&ret,&ret,&ret,&ret);
	vst_alignment(userhandle,0,5,&ret,&ret); /* Zellenoberkante */
	vst_color(userhandle,BLACK);
	
	vsl_type (userhandle, SOLID);
	vsl_ends (userhandle, SQUARED, SQUARED);
	vsl_width (userhandle, 1);
	vsl_color (userhandle, BLACK);
	vsf_interior(userhandle,FIS_SOLID); /* FÅllung */
   viele_Farben = mindestens_16_Farben();
	
	pxy[0]=pb->pb_x;
	pxy[1]=pb->pb_y;
	pxy[2]=pb->pb_x+pb->pb_w-1;
	pxy[3]=pb->pb_y+pb->pb_h-1;

	if(threedee && viele_Farben) /*3D*/
		vsf_color(userhandle,dialbgcolor);		  /* farbe  */
	else
		vsf_color(userhandle,WHITE);		  /* farbe  */
	vswr_mode(userhandle,MD_REPLACE);
	vr_recfl(userhandle,pxy);	  /* rechteck in workspace */
	
	pxy[0]=pb->pb_x-twp->wscroll/2;      /* Trennlinie ziehen */
	pxy[1]=pb->pb_y-1;
	pxy[2]=pb->pb_x+pb->pb_w-1;
	pxy[3]=pb->pb_y-1;
/*
	vsl_color (userhandle, WHITE);
*/
	v_pline(userhandle,2,pxy);
/*
	pxy[1]++;
	pxy[3]++;
	vsl_color (userhandle, BLACK);
	v_pline(userhandle,2,pxy);
*/
	vswr_mode (userhandle, /*MD_REPLACE*/MD_TRANS); 
	cp=(char *)((TEDINFO *)pb->pb_parm)->te_ptext;
	if(cp[twp->umbruch-2] != ']')
	{
		for(i=1;i<STRING_LENGTH;i++)
			if(cp[i]==']')
			{
				if( ! (i%twp->tab))
					cp[i] = TABSIGN; /*HÅtchen im Zeichensatz*/
				else
					cp[i] = '.';
				break;
			}
		cp[twp->umbruch-2] = ']';
	}
	v_gtext(userhandle,pb->pb_x,pb->pb_y+(pb->pb_h-twp->hscroll)/2,&cp[twp->wfirst/twp->wscroll]);
	vst_font(userhandle,attrib[0]);
	vst_height(userhandle,attrib[7],&ret,&ret,&ret,&ret);
	
	reset_clip (userhandle, pb->pb_xc,pb->pb_yc,pb->pb_wc,pb->pb_hc);

	return (pb->pb_currstate);
}

/*****************************************************************************/
#define MAXUSERBLK (219+3+1) /* 3 Buttons in form_alert() + 1 Reserve */

static int userdefobjs=0;

static USERBLK rs_userblk[MAXUSERBLK+1];

void tabbar_fix(WINDOW *wp)
{
	if(wp && wp->toolbar)
	{
		rs_userblk[userdefobjs].ub_code = draw_tabbar;
		rs_userblk[userdefobjs].ub_parm = wp->tabbar->ob_spec.index;
		wp->tabbar->ob_type = G_USERDEF;
		wp->tabbar->ob_spec.index = (long)&rs_userblk[userdefobjs++];
/*
printf("\33H%003d ",userdefobjs);
*/
	}
}

#pragma warn -par
void form_fix(OBJECT *tree, BOOLEAN is_dialog)
{
  int      obj;
  OBJECT  *ob;
  ICONBLK *ib;
  TEDINFO *ti;
  unsigned int type, xtype;

  if (tree != NULL)
  {
#if GEM & (GEM2 | GEM3 | XGEM)
	 if (is_dialog)
	 {
		tree->ob_state&=~SHADOWED; /* Atari-like */
		tree->ob_state|=OUTLINED;
	 } /* if */
#endif

	 obj = 0;

	 do
	 {
		ob = &tree [++obj];
		type  = ob->ob_type & 0xFF;
		xtype = ob->ob_type >> 8;

		if (threedee && (ob->ob_flags & EDITABLE) && mindestens_16_Farben())
		{  /* vertiefte Eingabefelder */
/*
			ob->ob_y-=2;
			ob->ob_height+=4;
*/
			ob->ob_y--;
			ob->ob_height+=2;
			ob->ob_type  =G_FBOXTEXT;
			ob->ob_state|=SELECTED;
			ob->ob_flags|=FLAGS9;
			ob->ob_flags|=FLAGS10;
			ti = ob->ob_spec.tedinfo;

			ti->te_thickness=0;

			ti->te_just=TE_CNTR;
			ti->te_color=0x11F0;
		}

		if (type == G_ICON)
		{
		  ib = ob->ob_spec.iconblk;
		  ob->ob_height = ib->ib_ytext + ib->ib_htext; /* Objekthîhe = Iconhîhe */
		  trans_gimage (tree, obj);		  /* Icons an Bildschirm anpassen */
		} /* if */

		if (type == G_IMAGE)
		{
		  trans_gimage (tree, obj);		  /* Bit Images an Bildschirm anpassen */
		} /* if */

		switch (xtype)
		{
		  case DCHECKBOX  :
			  rs_userblk[userdefobjs].ub_code	  = draw_checkbox;
			  rs_userblk[userdefobjs].ub_parm	  = ob->ob_spec.index;
			  ob->ob_type	      = G_USERDEF;
			  ob->ob_spec.index	= (long)&rs_userblk[userdefobjs++];
			  break;
		  case DRBUTTON	:
			  rs_userblk[userdefobjs].ub_code	  = draw_radio;
			  rs_userblk[userdefobjs].ub_parm	  = ob->ob_spec.index;
			  ob->ob_type	      = G_USERDEF;
			  ob->ob_spec.index	= (long)&rs_userblk[userdefobjs++];
			  break;
		  case DALTBUTTON :
			  rs_userblk[userdefobjs].ub_code	  = draw_altbutton;
			  rs_userblk[userdefobjs].ub_parm	  = ob->ob_spec.index;
			  ob->ob_type	  		= G_USERDEF;
			  ob->ob_spec.index	= (long)&rs_userblk[userdefobjs++];
				ob->ob_x      	  -= 4;
				ob->ob_y      	  -= 5; /* 4 */
				ob->ob_width  	  += 8;
				ob->ob_height 	  += 10; /* 8 */
				ob->ob_flags      |= FLAGS14; /* wg. '*' im Dialog */
			  if(!threedee)
			  {
					ob->ob_y++;
					ob->ob_height-=2;
			  }
			  break;
		  case DULINE	  :
			  rs_userblk[userdefobjs].ub_code	  = draw_uline;
			  rs_userblk[userdefobjs].ub_parm	  = ob->ob_spec.index;
			  ob->ob_type	  		= G_USERDEF;
			  ob->ob_spec.index = (long)&rs_userblk[userdefobjs++];
			  break;
		  case DHEADER	   :
				ob->ob_y			 	-= boxh / 2;
			  break;
		  case DFONT		:
			  font_blk.ub_code  	 = draw_font;
			  font_blk.ub_parm  	 = ob->ob_spec.index;
			  ob->ob_type		  	   = G_USERDEF;
			  ob->ob_spec.index  	 = (long)&font_blk;
			  break;
		  case DEAR		 :
			  ear_blk.ub_code	    = draw_ear;
			  ear_blk.ub_parm	  	= ob->ob_spec.index;
			  ob->ob_x			 	   -= 3;
			  ob->ob_y			 	   -= 3;
			  ob->ob_width			  = ob->ob_height;
				if(boxh<=8)
					ob->ob_width=2*ob->ob_height;
			  ob->ob_width		 	 += 6;
			  ob->ob_height	 	   += 6;
			  ob->ob_type		  	  = G_USERDEF;
			  ob->ob_state		 	 &= ~OUTLINED;
			  ob->ob_flags		 	 |= TOUCHEXIT;
			  ob->ob_flags		 	 &= ~SELECTABLE;
			  ob->ob_flags		 	 &= ~EXIT;
			  ob->ob_spec.index	  = (long)&ear_blk;
			  break;
		  case DCIRCLE	 :
			  circle_blk.ub_code  = draw_circle;
			  circle_blk.ub_parm  = ob->ob_spec.index;
			  ob->ob_x				   -= 1;
			  ob->ob_y			 	   -= 1;
			  ob->ob_width		 	 += 2;
			  ob->ob_height	 	   += 2;
			  ob->ob_type		      = G_USERDEF;
			  ob->ob_flags		   |= TOUCHEXIT;
			  ob->ob_spec.index   = (long)&circle_blk;
			  break;
		  case DDLINE	  :
			  ob->ob_type		     &= 0x00FF;
			  break;
		  case DSELFONT	:
			  selfont_blk.ub_code = draw_selfont;
			  selfont_blk.ub_parm = ob->ob_spec.index;
			  ob->ob_type		      = G_USERDEF;
			  ob->ob_spec.index   = (long)&selfont_blk;
				ob->ob_x   		     -= 1;
				ob->ob_y    		   -= 1;
				ob->ob_width       += 2;
				ob->ob_height      += 2;
			  break;
		  case D3DBOX     :
			  threeDbox_blk.ub_code = draw_3Dbox;
			  threeDbox_blk.ub_parm = ob->ob_spec.index;
			  ob->ob_type		        = G_USERDEF;
			  ob->ob_spec.index	    = (long)&threeDbox_blk;
				ob->ob_x   		       -= 1;
				ob->ob_y    		     -= 1;
				ob->ob_width  	     += 2;
				ob->ob_height 	     += 2;
				ob->ob_flags         |= FLAGS15; /* wg. draw_altbutton() */
		     break;
		} /* switch */
/*
printf("\33H%003d ",userdefobjs);
*/
		if(userdefobjs>MAXUSERBLK)
		{
			Bconout(2,7);
			Cconws("\rNot enough memory to support USERDEFs!\r\nPress any key to abort...");
			while(!Cconis()) /* 1997-04-22 (MJK): Cconis() statt kbhit() */
				;
			_exit(-1);
		}
	 } while (! (ob->ob_flags & LASTOB));
  } /* if */
} /* fix_objs */
#pragma warn .par
