/*****************************************************************
	7UP
	Modul: LISTBOX.C
	(c) by TheoSoft '94
	
	1997-04-07 (MJK): ben�tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#endif
#include <stdio.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif
#include <string.h>

#include "forms.h"

#ifndef ENGLISH											/* (GS) */
	#include "7UP.h"
#else
	#include "7UP_eng.h"
#endif

#include "resource.h"
#include "objc_.h"

#include "listbox.h"

#define MAXENTRIES 5
#define notnull(x) ((x)?(x):1)
#define FLAGS15 0x8000

#define Objc_edit(a,b,c,d,e,f) objc_edit(a,b,c,f,e)

#define FMD_FORWARD  1
#define FMD_BACKWARD 2
#define FMD_DEFLT    0

#pragma warn -par
static int list_do(OBJECT *fm_tree, int fm_start_fld)
{
   int fm_next_obj,fm_which,fm_cont;
   int fm_mx,fm_my,fm_mb,fm_ks,fm_kr,fm_br;

   fm_cont=1;
   while(fm_cont)
   {
      fm_which=evnt_multi(MU_BUTTON,0x02,0x01,0x01,0,0,0,0,
                          0,0,0,0,0,0,NULL,
						              0,
#ifdef TCC_GEM
						              0,
#endif
                          &fm_mx, &fm_my, &fm_mb, &fm_ks, &fm_kr, &fm_br);
      if(fm_which&MU_BUTTON)
      {
         fm_next_obj=objc_find(fm_tree,ROOT,MAX_DEPTH,fm_mx,fm_my);
         if(fm_next_obj==-1)
         {
            fm_cont=0;
            fm_next_obj=0;
         }
         else
            fm_cont=form_button(fm_tree,fm_next_obj,fm_br,&fm_next_obj);
      }
   }
   return(fm_next_obj);
}
#pragma warn .par

void setsliderpos(OBJECT *tree, int item, int pos)
{
   long oldpos;
   oldpos=tree[item-1].ob_height-tree[item].ob_height;
   tree[item].ob_y=(int)(oldpos*(long)pos/1000L);
}

static void _objc_update(OBJECT *tree, int obj, int depth)
{
	int obx,oby;
	objc_offset(tree,obj,&obx,&oby);
	/* einen Pixel weniger, bei depth==0 */
	objc_draw(tree,obj,depth,obx,oby,
		tree[obj].ob_width-(depth?0:1),tree[obj].ob_height);
}

static void _form_write(OBJECT *tree, int item, char *string, int modus)
{
	strcpy(tree[item].ob_spec.tedinfo->te_ptext,string);
	if(modus)
		_objc_update(tree,item,0);
}

static void _listbox_fill(OBJECT *tree, SCROLLIST *liste, long firstentry)
{
	int i, slider;
	
   for(i=0; i<MAXENTRIES; i++)       
   {
      tree[i+ROOT+1].ob_state =
         liste->scrollist[firstentry+i].state;
      _form_write(tree,i+ROOT+1,liste->scrollist[firstentry+i].entry,1);
   }
   /* liste->count > MAXENTRIES. Keine Division durch null. */
   slider = (int)(firstentry*1000/max(1,liste->count-MAXENTRIES));
   setsliderpos(tree, SLBSL, slider);
   objc_update(tree, SLBGR, MAX_DEPTH);
}

static int _listbox_hndl(OBJECT *tree, SCROLLIST *liste)
{
   int  ret, boxw, boxh, i, x, y, mx, my, exit_obj, slider, actentry=0, done=0;
   long firstentry=0;

   if(!tree)
      return(-1);

   if(!liste)
      return(-2);

   if(liste->count <= 0)
      return(-3);

   if(liste->count > MAXENTRIES) /* bei mehr Eintr�gen Selektion positionieren */
      for(i=0; i<liste->count; i++)
         if(liste->scrollist[i].state & SELECTED)
         {
            firstentry = i-2; /* Eintrag positionieren */
            if(firstentry > liste->count-MAXENTRIES)
               firstentry = liste->count-MAXENTRIES;
            if(firstentry < 0)
               firstentry = 0;
            break;
         }

   for(i=0; i<MAXENTRIES; i++)       
   {
      tree[i+ROOT+1].ob_state =
         liste->scrollist[firstentry+i].state;
      form_write(tree,i+ROOT+1,liste->scrollist[firstentry+i].entry,0);
   }

   for(i=0; i<MAXENTRIES; i++) /* alles verstecken, was nicht da ist */
      if(!*liste->scrollist[i].entry)
         tree[i+ROOT+1].ob_flags |= HIDETREE;

   graf_handle(&boxw,&boxh,&ret,&ret);
   tree[SLBSL].ob_height=(int)max(boxh,MAXENTRIES*tree[SLBGR].ob_height/max(MAXENTRIES,liste->count));
   setsliderpos(tree, SLBSL,0);

   i=(int)((firstentry*1000L)/max(1,liste->count-MAXENTRIES));
   i=min(i,1000);
   i=max(0,i);
   setsliderpos(tree, SLBSL, i);

   form_open(tree,0);
   do
   {
      exit_obj=list_do(tree,0);
      if(exit_obj & 0x8000) /* Doppelklick */
      {
         switch(exit_obj &= 0x7FFF) /* MSB weg */
         {
            case SLBUP:
               if(firstentry>0)
               {
                  firstentry=0;
									_listbox_fill(tree, liste, firstentry);
               }
               break;
            case SLBDN:
               if(firstentry < liste->count-MAXENTRIES)
               {
	               firstentry = liste->count-MAXENTRIES;
									_listbox_fill(tree, liste, firstentry);
               }
					break;
/*
            case ROOT+1:
            case ROOT+2:
            case ROOT+3:
            case ROOT+4:
            case ROOT+5:
               actentry = firstentry + exit_obj - 1;
               done = 1;
               break;
*/
         }
      }
      else
      {
         switch(exit_obj)
         {
            case ROOT:
               done = 1;
               break;
            case SLBUP:
               if(firstentry>0)
               {
                  firstentry--;
									_listbox_fill(tree, liste, firstentry);
               }
               break;
            case SLBDN:
               if(firstentry < liste->count-MAXENTRIES)
               {
                  firstentry++;
									_listbox_fill(tree, liste, firstentry);
               }
               break;
            case SLBGR:
               objc_offset(tree, SLBSL, &x, &y);
               graf_mkstate(&mx,&my,&i,&i);
               if(my < y)
               {
                  if((firstentry -= MAXENTRIES) < 0)
                     firstentry = 0;
               }
               if(my > y)
               {
                  if((firstentry += MAXENTRIES) > (liste->count-MAXENTRIES))
                     firstentry = liste->count-MAXENTRIES;
               }
							 _listbox_fill(tree, liste, firstentry);
               break;
            case SLBSL:
               graf_mouse(FLAT_HAND, NULL);
               slider=graf_slidebox(tree, exit_obj-1, exit_obj, 1);
               graf_mouse(ARROW, NULL);
               firstentry = (liste->count-MAXENTRIES) * slider/1000;
		  				 _listbox_fill(tree, liste, firstentry);
               break;
            case ROOT+1:
            case ROOT+2:
            case ROOT+3:
            case ROOT+4:
            case ROOT+5:
               actentry = (int)(firstentry + exit_obj - 1);
               done = 1;
               break;
         }
      }
   }
   while(!done);
   form_close(tree,exit_obj,0);

   for(i=0; i<MAXENTRIES; i++)
   {
      tree[i+ROOT+1].ob_flags &= ~HIDETREE;
      tree[i+ROOT+1].ob_state &= ~SELECTED;
   }

	if(exit_obj>ROOT)
	{
	   for(i=0; i<liste->count; i++)
   	   liste->scrollist[i].state &= ~SELECTED;
   	liste->scrollist[actentry].state |= SELECTED;
	}
   
   return(exit_obj==ROOT?-1:liste->scrollist[actentry].retcode);
}

int listbox_hndl(OBJECT *tree, int item, SCROLLIST *liste)
{
	int exit_obj, x, y, ret, boxw, boxh;
	
   graf_handle(&boxw,&boxh,&ret,&ret);
   objc_offset(tree,item,&x,&y);
   listbox->ob_x=x;
   listbox->ob_y=y+boxh+1;
   listbox->ob_type|=(113<<8); /*LTMF ausschalten*/
   if(liste->count<=MAXENTRIES)
   {
	   listbox->ob_width-=(2*boxw+1);
	   listbox[SLBUP].ob_flags|=HIDETREE;
	   listbox[SLBDN].ob_flags|=HIDETREE;
	   listbox[SLBGR].ob_flags|=HIDETREE;
	}
/*
	listbox->ob_flags|=FLAGS15; /* kein Windial!!!, weil kein Handle mehr frei*/
*/
   exit_obj=_listbox_hndl(listbox,liste);
   if(liste->count<=MAXENTRIES)
   {
	   listbox->ob_width+=(2*boxw+1);
	   listbox[SLBUP].ob_flags&=~HIDETREE;
	   listbox[SLBDN].ob_flags&=~HIDETREE;
	   listbox[SLBGR].ob_flags&=~HIDETREE;
	}
	if(!windials)
		objc_draw(tree,ROOT,MAX_DEPTH,listbox->ob_x-1,listbox->ob_y-1,
   		listbox->ob_width+4,listbox->ob_height+4);
   return(exit_obj);
}
