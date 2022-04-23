/*****************************************************************
	7UP
	Modul: SHELL.C
	(c) by TheoSoft '90

	Shell Funktionen
	
	1997-04-07 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <gem.h>
#endif
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#else
#	include <osbind.h>
	typedef struct {
		unsigned char length;
		char          command_tail[128];
	} COMMAND;
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "alert.h"
#include "falert.h"
#ifndef ENGLISH											/* (GS) */
	#include "7UP.h"
#else
	#include "7UP_eng.h"
#endif
#include "forms.h"
#include "windows.h"
#include "language.h"
#include "resource.h"
#include "7up3.h"
#include "fileio.h"
#include "graf_.h"

#include "shell.h"

int pexec=0; /* fr Fontbox, wg. "static" */

void get_path(char *path)
{
	 register int i;
	 i=(int)strlen(path)-1;
	 for(; i>=0; i--)
		 if(path[i]=='\\')
		 {
			 path[i+1]=0;
			 break;
		 }
}

int istos(char *pathname)
{
	char *cp;
	cp=&pathname[strlen(pathname)-3];		 /* auf Zeichen nach dem '.'	  */
	if(!strnicmp(cp,"TOS",3) ||			  /* ...und vergleichen, ob legal */
		!strnicmp(cp,"TTP",3))
		return(1);														 /* ok, er ist */
	return(0);
}

void hndl_shell(OBJECT *tree, int start)
{
	int exit_obj, olddrv, ret/*, area[4]*/;
	char oldpath[PATH_MAX],path[PATH_MAX],pathname[PATH_MAX],cmdstr[40];
	COMMAND cmd;
	/*static*/ char fpattern[FILENAME_MAX]="*.*";

	form_exopen(tree,start);
	do
	{
		exit_obj=form_exdo(tree,start);
		if(exit_obj==SHELHELP)
		{
			my_form_alert(1,Ashell[0]);
			objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
/*
			tree[exit_obj].ob_state&=~SELECTED;
			if(!windials)
				objc_update(tree,ROOT,MAX_DEPTH);
			else
				objc_update(tree,exit_obj,0);
*/
		}
	}
	while(exit_obj==SHELHELP);
	form_exclose(tree,exit_obj,0);

	if(exit_obj == SHELLOK)
	{
		if(tree[SHELLGEM].ob_state&SELECTED)
		{
			pathname[0]=0;
			if(getfilename(pathname,fpattern,"@",fselmsg[24]))
			{
				form_read(tree,SHELCOMM,cmd.command_tail);
				cmd.length=(unsigned char)strlen(cmd.command_tail);
				if(!cmd.length)
				{
					cmd.command_tail[0]=0;
					if(getfilename(cmd.command_tail,fpattern,"@",fselmsg[25]))
						cmd.length=(unsigned char)strlen(cmd.command_tail);
					else
						cmd.length=0;
				}
				if(istos(pathname))
				{
					graf_mouse_on(0);
#if MiNT
					wind_update(BEG_UPDATE);
#endif
					menu_bar(winmenu,0);
#if MiNT
					wind_update(END_UPDATE);
#endif
					v_enter_cur(aeshandle);
				}
				else
				{
					if(Sversion()>=0x1500)
						shel_write(1,1,1,pathname,(void *)&cmd);
					pexit();
					pexec=1;
				}
				olddrv=Dgetdrv();
				Dgetpath(oldpath,0);
				strcpy(path,pathname);
				get_path(path);
				Dsetdrv((int)(path[0]-'A'));
				Dsetpath(&path[2]);

				ret=(int)Pexec(0,pathname,&cmd,NULL); /* 1997-04-22 (MJK): Kopieren der Basepage implizit */

				Dsetdrv(olddrv);
				Dsetpath(oldpath);
				if(istos(pathname))
				{
					v_exit_cur(aeshandle);
#if MiNT
					wind_update(BEG_UPDATE);
#endif
					menu_bar(winmenu,1);
/*
					_wind_get(0, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
					while( area[2] && area[3] )
					{
						form_dial(FMD_FINISH,0,0,0,0,area[0],area[1],area[2],area[3]);
						_wind_get(0, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
					}
*/
#if MiNT
					wind_update(END_UPDATE);
#endif
					graf_mouse_on(1);
					form_dial(FMD_FINISH,0,0,0,0,xdesk,ydesk,wdesk,hdesk);
				}
				else
				{
					pinit(NULL);
#if OLDTOS
					wind_update(BEG_UPDATE);
#endif
					if(Sversion()>=0x1500)
						shel_write(0,1,1,"","");
				}
			}
		}
		else
		{
			if(system(NULL))
			{
#if MiNT
				wind_update(BEG_UPDATE);
#endif
				menu_bar(winmenu,0);
				wind_update(END_UPDATE);
				graf_mouse_on(0);
				v_enter_cur(aeshandle);
				graf_mouse_on(1);

				v_curtext(aeshandle,KOMMANDO);
				v_curtext(aeshandle,form_read(tree,SHELCOMM,cmdstr));
				v_curtext(aeshandle,"\r\n");
				ret=system(cmdstr);
				if(ret<0)
					form_error((~ret)-30);
				v_curtext(aeshandle,PRESSANYKEY);
				while(!Cconis()) /* 1997-04-22 (MJK): Cconis() statt kbhit() */
					;
				while(Cconis()) /* 1997-04-22 (MJK): Cconis() statt kbhit() */
					Cconin();

				graf_mouse_on(0);
				v_exit_cur(aeshandle);

				wind_update(BEG_UPDATE);
				menu_bar(winmenu,1);
/*
				wind_set(0,WF_NEWDESK,nodesktop?NULL:desktop,0,0);
				_wind_get(0, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
				while( area[2] && area[3] )
				{
					form_dial(FMD_FINISH,0,0,0,0,area[0],area[1],area[2],area[3]);
					_wind_get(0, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
				}
*/
#if MiNT
				wind_update(END_UPDATE);
#endif
				graf_mouse_on(1);
				graf_mouse(ARROW,NULL);
				form_dial(FMD_FINISH,0,0,0,0,xdesk,ydesk,wdesk,hdesk);
			}
			else
				my_form_alert(1,Ashell[1]);
		}
	}
}
