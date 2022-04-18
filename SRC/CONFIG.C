/*****************************************************************
	7UP
	Modul: CONFIG.C
	(c) by TheoSoft '90

	Programmkonfiguration laden und speichern

	1997-03-26 (MJK): benîtigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	                  Speicherung der Iconpositionen verbessert
	1997-04-09 (MJK): MSDOS-Teile entfernt
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
	BEMERKUNG: Die Speicherung der Seitenparameter gehîrt geÑndert!
*****************************************************************/
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
#	include <vdi.h>
#else
#	include <aesbind.h>
#	include <vdibind.h>
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "alert.h"

#include "7UP.H"
#include "forms.h"
#include "windows.h"
#include "version.h"
#include "7UP3.h"
#include "resource.h"
#include "fileio.h"
#include "toolbar.h"
#include "fontsel.h"
#include "printer.h"
#include "desktop.h"
#include "objc_.h"
#include "di_fly.h"
#include "wind_.h"

#include "config.h"

#ifndef _AESnumapps
#	ifdef TCC_GEM
#		define _AESnumapps (_GemParBlk.global[1])
#	else
#		error First define _AESnumapps to global[1]
#	endif
#endif

#define MAXINT 0x7FFF
#define CRLF	1
#define LF	  2
#define CR	  3
#define EXOB_TYPE(x) (x>>8)
#define TABBAR 0xFF

static char pathname[PATH_MAX];

static WINDOW _dummy[]=
{
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L,
	-1,0,0,0,0,0,0,0,0,0L,0L,0,0,0,0,0,0,0L,0L,0L,0L,8,16,0,0,NULL,0L,10,1,3,0L,0L,0L,0L,0L,0L
};					  /* | | */

#pragma warn -par
void hndl_diverses(OBJECT *tree, int start)
{
	int exit_obj,desk,area[4];
	int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,v,w,x,y,z;
	char *cp;
	/*static*/ char fpattern[FILENAME_MAX]="*.ACC";

	/* MT 19.11.94 abgelîst durch Pickliste */
	divmenu[DIVNAME].ob_flags |= HIDETREE;
	divmenu[DIVNAME].ob_state &=~SELECTED;

	r=tree[DIVTABEX].ob_state;
	a=tree[DIVWRET ].ob_state;
	b=tree[DIVKONV ].ob_state;
	c=tree[DIVCRLF ].ob_state;
	v=tree[DIVLF	].ob_state;
	w=tree[DIVCR	].ob_state;
	m=tree[DIVPAPER].ob_state;
	d=tree[DIVBACK ].ob_state;
	p=tree[DIVBUTIM].ob_state;
	e=tree[DIVINFO ].ob_state;
	f=tree[DIVSCROLL].ob_state;
	g=tree[DIVSTOOL].ob_state;
	h=tree[DIVDESK ].ob_state;
	i=tree[DIVZENT ].ob_state;
	j=tree[DIVFREE ].ob_state;
	k=tree[DIVMAUS ].ob_state;
	s=tree[DIVDWIN ].ob_state;
	l=tree[DIVSAVE ].ob_state;
	n=tree[DIVNAME ].ob_state;
	o=tree[DIVBLANK].ob_state;
	q=tree[DIVPATH ].ob_state;
	t=tree[DIVVAST ].ob_state;
	x=tree[DIVUMLAUT].ob_state;
	y=tree[DIVCLIP ].ob_state;
	z=tree[DIVTABBAR].ob_state;
	
	desk=nodesktop;
	sprintf(tree[DIVBACK2].ob_spec.tedinfo->te_ptext,"%02d",
			  (int)min(99,(int)(backuptime/60000L)));

	if(nodesktop) /* kein Desktop, kein Papierkorb */
		tree[DIVPAPER].ob_state|=DISABLED;
	else
		tree[DIVPAPER].ob_state&=~DISABLED;

	form_exopen(tree,0);
	do
	{
		exit_obj=form_exdo(tree,0);
		switch(exit_obj)
		{
			case DIVHELP:
				form_alert(1,Aconfig[0]);
				objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
				break;
			case DIVHDA:
				strcpy(alertstr,"@:\\");
		  		alertstr[0]=(char)(getbootdev()+'A');
		  		strcat(alertstr,fpattern);
/* 29.5.94 */
				if( ! (strrchr(alertstr,'\\') || strrchr(alertstr,'/')))
					*alertstr=0;

				if(getfilename(alertstr,fpattern,"@",fselmsg[1]))
				{
					if((cp=strrchr(alertstr,'.'))!=NULL)
						*(cp+1)=0;
					alertstr[strlen(alertstr)-1]=0;
/*
					form_write(tree,DIVHDA,(char *)split_fname(alertstr),1);
*/
					strcpy((char *)(tree[DIVHDA].ob_spec.index/*+16L*/),(char *)split_fname(alertstr));
				}
				tree[exit_obj].ob_state&=~SELECTED;
				if(!windials)
					objc_update(tree,ROOT,MAX_DEPTH);
				else
					objc_update(tree,exit_obj,0);
				break;
		}
	}
	while(exit_obj!=DIVOK && exit_obj!=DIVABBR);
	form_exclose(tree,exit_obj,0);
	if(exit_obj==DIVABBR)
	{
		tree[DIVTABEX].ob_state=r;
		tree[DIVWRET ].ob_state=a;
		tree[DIVKONV ].ob_state=b;
		tree[DIVCRLF ].ob_state=c;
		tree[DIVLF	].ob_state=v;
		tree[DIVCR	].ob_state=w;
		tree[DIVPAPER].ob_state=m;
		tree[DIVBACK ].ob_state=d;
		tree[DIVBUTIM].ob_state=p;
		tree[DIVINFO ].ob_state=e;
		tree[DIVSCROLL].ob_state=f;
		tree[DIVSTOOL].ob_state=g;
		tree[DIVDESK ].ob_state=h;
		tree[DIVZENT ].ob_state=i;
		tree[DIVFREE ].ob_state=j;
		tree[DIVMAUS ].ob_state=k;
		tree[DIVDWIN ].ob_state=s;
		tree[DIVSAVE ].ob_state=l;
		tree[DIVNAME ].ob_state=n;
		tree[DIVBLANK].ob_state=o;
		tree[DIVPATH ].ob_state=q;
		tree[DIVVAST ].ob_state=t;
		tree[DIVUMLAUT].ob_state=x;
		tree[DIVCLIP ].ob_state=y;
	   tree[DIVTABBAR].ob_state=z;
		return;
	}
	if(tree[DIVBUTIM].ob_state & SELECTED)
		backuptime=60000L*atol(form_read(tree,DIVBACK2,alertstr));
	else
		backuptime=0xFFFFFFFFL;
	if(backuptime==0L)
	{
		form_alert(1,Aconfig[1]);
		backuptime=0xFFFFFFFFL;
	}

	if(tree[DIVTABEX].ob_state & SELECTED)
		tabexp=1;
	else
		tabexp=0;

	if(tree[DIVKONV].ob_state & SELECTED)
		eszet=1;
	else
		eszet=0;

	if(tree[DIVBLANK].ob_state & SELECTED)
		bcancel=1;
	else
		bcancel=0;

	if(divmenu[DIVCRLF].ob_state & SELECTED)
		lineendsign=CRLF;
	if(divmenu[DIVLF].ob_state & SELECTED)
		lineendsign=LF;
	if(divmenu[DIVCR].ob_state & SELECTED)
		lineendsign=CR;

	if(divmenu[DIVWRET].ob_state & SELECTED)
		wret=1;
	else
		wret=0;

	if(divmenu[DIVDWIN].ob_state & SELECTED)
		windials=1;
	else
		windials=0;

	if(divmenu[DIVVAST].ob_state & SELECTED)
		vastart=1;
	else
		vastart=0;

	if(tree[DIVINFO].ob_state & SELECTED)
		WI_KIND|=INFO;
	else
		WI_KIND&=~INFO;
	for(a=1; a<MAXWINDOWS; a++)
		if(_wind[a].w_state & CREATED)
			Wattrchg(&_wind[a],WI_KIND);
	for(a=1; a<MAXWINDOWS; a++)
		if(_wind[a].w_state & ONTOP)
			Wtop(&_wind[a]);

	if(tree[DIVCLIP].ob_state & SELECTED)
		clipbrd=1;
	else
		clipbrd=0;

	if(tree[DIVUMLAUT].ob_state & SELECTED)
		umlautwandlung=1;
	else
		umlautwandlung=0;
	
	if(tree[DIVSTOOL].ob_state & SELECTED)
		toolbar_zeigen=1;
	else
		toolbar_zeigen=0;

	tabbar_inst(Wgettop(),tree[DIVTABBAR].ob_state & SELECTED);
	
	if(tree[DIVTABBAR].ob_state & SELECTED)
		tabbar=1;
	else
		tabbar=0;

	if(tree[DIVSCROLL].ob_state & SELECTED)
		scrollreal=1;
	else
		scrollreal=0;

	if(tree[DIVPAPER].ob_state!=m) /* nur wenn geÑndert, wg. Flackern */
		inst_trashcan_icon(desktop,DESKICN8,DESKICND,0);

	if(tree[DIVDESK].ob_state & SELECTED)
		nodesktop=0;
	else
		nodesktop=1;
	if(desk!=nodesktop)
		if(nodesktop)
		{
			winmenu[WINOPALL-1].ob_height=((WINDAT7-WINOPALL+1)*boxh);
			for(i=WINDAT1-1; i<=WINDAT7; i++)
				winmenu[i].ob_flags&=~HIDETREE;

			for(i=WINDAT7;i>=WINDAT1;i--)
				if(winmenu[i].ob_state&DISABLED)
				{
					winmenu[i].ob_flags|=HIDETREE;
					winmenu[WINOPALL-1].ob_height-=boxh;
				}
				else
					break;
			if(winmenu[WINOPALL-1].ob_height==(WINDAT1-WINOPALL)*boxh)
			{
				winmenu[WINDAT1-1].ob_flags|=HIDETREE;
				winmenu[WINOPALL-1].ob_height-=boxh;
			}
			wind_set(0,WF_NEWDESK,NULL,0,0);
			if(get_cookie('MiNT') && (_AESnumapps != 1))
			{
				form_dial(FMD_FINISH,0,0,0,0,xdesk,ydesk,wdesk,hdesk);
			}
			else
			{
				wind_update(BEG_UPDATE);
				_wind_get(0, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
				while( area[2] && area[3] )
				{
					form_dial(FMD_FINISH,0,0,0,0,area[0],area[1],area[2],area[3]);
					_wind_get(0, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
				}
				wind_update(END_UPDATE);
			}
		}
		else
		{
			winmenu[WINOPALL-1].ob_height=((WINFULL-WINOPALL+1)*boxh);
			for(i=WINDAT1-1; i<=WINDAT7; i++)
			  	winmenu[i].ob_flags|=HIDETREE;

			wind_set(0,WF_NEWDESK,desktop,0,0);
			inst_trashcan_icon(desktop,DESKICN8,DESKICND,0);
			wind_update(BEG_UPDATE);
			_wind_get(0, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
			while( area[2] && area[3] )
			{
				objc_draw(desktop,ROOT,MAX_DEPTH,area[0],area[1],area[2],area[3]);
				_wind_get(0, WF_NEXTXYWH, &area[0], &area[1], &area[2], &area[3]);
			}
			wind_update(END_UPDATE);
		}
}
#pragma warn .par

int _read_lin(WINDOW *wp, char *name) /* Zeilenlineal lesen */
{
	FILE *fp;
	unsigned int state;
	int id,size;
	OBJECT *ob;
	int obj;

	if((fp=fopen(name,"rb")) != NULL)
	{
		fread(alertstr,VERSIONSTRLEN,1,fp);
		if(!strcmp(VERSIONNAME,alertstr))
		{
			fread(&state,sizeof(int),1,fp);
			wp->w_state&=~(BLOCKSATZ+INSERT+INDENT+PROPFONT+COLUMN); /* alles lîschen */
			wp->w_state|=(state&(BLOCKSATZ+INSERT+INDENT+PROPFONT+COLUMN)); /* neu */
			fread(&wp->umbruch,sizeof(int),1,fp);
			fread(&wp->tab,sizeof(int),1,fp);
			fread(&id,sizeof(int),1,fp);
			fread(&size,sizeof(int),1,fp);
			if(id==1)		 /* wenn Standardfont */
				Wnewfont(wp,1,size); /* letzte Fontgrî·e */
			else						  /* sonst Fonts laden */
			{
				if(!(wp->w_state & GEMFONTS))
				{
					graf_mouse(BUSY_BEE,0L);
					vq_extnd(wp->vdihandle,0,work_out);
					if(vq_gdos())
					{
						additional=vst_load_fonts(wp->vdihandle,0)+work_out[10];
						wp->w_state|=GEMFONTS;
					}
					else
						additional=work_out[10]; /* Nur 6x6 system font */
					graf_mouse(ARROW,0L);
				}
				if(additional==work_out[10])	 /* Fehler, kein Fontladen mîglich */
					Wnewfont(wp,1,norm_point);  /* Standartfont bei Fehler */
				else
				{
					if(id==vst_font(wp->vdihandle,id)) /* falls in ASSIGN.SYS etwas geÑndert wurde */
						Wnewfont(wp,id,size); /* letzter Font und Grî·e */
					else
						Wnewfont(wp,1,norm_point);  /* Standartfont bei Fehler */
				}
			}
			if(wp->toolbar)
			{
				obj = 0;
			   do 
			   {
					ob	= &wp->toolbar[++obj];
					if((EXOB_TYPE(ob->ob_type)==TABBAR) ||
						          (ob->ob_type ==G_USERDEF))
					{
						if(ob->ob_type==G_USERDEF)
							fread((char *)((TEDINFO *)ob->ob_spec.userblk->ub_parm)->te_ptext,STRING_LENGTH+1,1,fp);
						else
							fread(ob->ob_spec.tedinfo->te_ptext,STRING_LENGTH+1,1,fp);
					}
				} 
				while (! (ob->ob_flags & LASTOB));
			}
			fclose(fp);
			return(1);
		}
		fclose(fp);
	}
	return(0);
}

static void _write_lin(WINDOW *wp, char *name) /* Zeilenlineal schreiben */
{
	FILE *fp;
	unsigned int state;
	OBJECT *ob;
	int obj;
	char *cp;
	
	if((fp=fopen(name,"wb")) != NULL)
	{
		state=wp->w_state&(BLOCKSATZ+INSERT+INDENT+PROPFONT+COLUMN);
		fwrite(VERSIONNAME,VERSIONSTRLEN,1,fp);
		fwrite(&state,sizeof(int),1,fp);
		fwrite(&wp->umbruch,sizeof(int),1,fp);
		fwrite(&wp->tab,sizeof(int),1,fp);
		fwrite(&wp->fontid,sizeof(int),1,fp);
		fwrite(&wp->fontsize,sizeof(int),1,fp);

		if(wp->toolbar)
		{
			obj = 0;
		   do 
		   {
				ob	= &wp->toolbar[++obj];
				if((EXOB_TYPE(ob->ob_type)==TABBAR) ||
					          (ob->ob_type ==G_USERDEF))
				{
					if(ob->ob_type==G_USERDEF)
						cp=(char *)((TEDINFO *)ob->ob_spec.userblk->ub_parm)->te_ptext;
					else
						cp=ob->ob_spec.tedinfo->te_ptext;
					fwrite(cp,STRING_LENGTH+1,1,fp);
				}
			} 
			while (! (ob->ob_flags & LASTOB));
		}
		fclose(fp);
	}
}

#pragma warn -par
void hndl_lineal(WINDOW *wp, int start)
{
/*
	/*static*/ char fpattern[FILENAME_MAX]="LINEAL.*";
*/
	/*static*/ char fpattern[FILENAME_MAX]="7up*.LIN";
	char filename[FILENAME_MAX];
	
	if(wp)
	{
		strcpy(pathname,(char *)Wname(wp));
		change_linealname(pathname,"LINEAL");
		sprintf(alertstr,Aconfig[3],
			split_fname((char *)Wname(wp)),split_fname(pathname));
		switch(form_alert(0,alertstr))
		{
		  case 1:
			  pathname[0]=0;
			  search_env(pathname,fpattern,0); /* READ */
/* 29.5.94 */
			  if( ! (strrchr(pathname,'\\') || strrchr(pathname,'/')))
				  *pathname=0;
			  if(getfilename(pathname,fpattern,"@",fselmsg[2]))
			  {
				  if(!_read_lin(wp,pathname))
				  {
					  sprintf(alertstr,Aconfig[4],
						  split_fname(pathname));
					  form_alert(1,alertstr);
				  }
			  }
			  break;
		  case 2:
			  strcpy(filename,(char *)split_fname((char *)Wname(wp)));
			  change_linealname(filename,"LINEAL");
			  search_env(pathname,filename,1); /* WRITE */
			  _write_lin(wp,pathname);
			  break;
		  case 3:
				break;
		}
	}
}
#pragma warn .par

char *find_7upinf(char *path, char *ext, int mode)
{
	char filename[FILENAME_MAX];

	strcpy(filename,"7UP.");
	strcat(filename,ext);
	return(search_env(path,filename,mode));
}

void saveconfig(int windstruct)
{
	FILE *fp;
	register int i;
	char *cp, fpattern[FILENAME_MAX];
	
	if(!windstruct)
	{
	   strcpy(fpattern,"*.INF");
		find_7upinf(pathname,"INF",1);
		if((cp=strrchr(pathname,'\\'))!=NULL || (cp=strrchr(pathname,'/'))!=NULL)
			strcpy(&cp[1],fpattern);
		else
			*pathname=0;
		if(getfilename(pathname,fpattern,"@",fselmsg[3]))
			fp=fopen(pathname,"wb");
		else
		   return;
	}
	else
		fp=fopen(find_7upinf(pathname,"INF",1),"wb");
	if(fp)
	{
		graf_mouse(BUSY_BEE,NULL);
		fwrite(VERSIONNAME,VERSIONSTRLEN,1,fp);

/* 30.9.94 jetzt in Diverses
		fwrite(&clipbrd,sizeof(int),1,fp);				 /* HÑkchen im PD-MenÅ */
*/
		fwrite(iconcoords,sizeof(ICNCOORDS),iconcoordcnt,fp);  /* Icons */

		/* ACHTUNG!!! DASS DAS FOLGENDE FUNKTIONIERT IST EHER ZUFALL!!! */
		fwrite(&zl,14*sizeof(int),1,fp);						/* Drucker */
		fwrite(&WI_KIND,sizeof(int),1,fp);					 /* Fensterdesign */
/*
		fwrite(&exitcode,sizeof(int),1,fp);					/* Exitcode */
*/
		fwrite(&layout[PRNKZL].ob_state,sizeof(int),1,fp);
		fwrite(&layout[PRNKZM].ob_state,sizeof(int),1,fp);
		fwrite(&layout[PRNKZR].ob_state,sizeof(int),1,fp);
		fwrite(&layout[PRNFZL].ob_state,sizeof(int),1,fp);
		fwrite(&layout[PRNFZM].ob_state,sizeof(int),1,fp);
		fwrite(&layout[PRNFZR].ob_state,sizeof(int),1,fp);
		fputs(form_read(layout,PRNKZSTR,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(layout,PRNFZSTR,pathname),fp);
		fputs("\r\n",fp);
		fwrite(&pinstall[PRNNUM].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNCUT].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNALT].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNKFBEG].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNFILE].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNBACK].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNDEL].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNALLP].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNPAIR].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNUNPA].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNFF].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNPAUSE].ob_state,sizeof(int),1,fp);
		fwrite(&pinstall[PRNNOFORM].ob_state,sizeof(int),1,fp);

		fwrite(&act_dev,  sizeof(int),1,fp);
		fwrite(&act_dist, sizeof(int),1,fp);
		fwrite(&act_paper,sizeof(int),1,fp);

		fwrite(&bracemenu[CBRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[SBRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[RBRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[PBRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[PKOMBRAC].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[CKOMBRAC].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[FREE1BRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[FREE2BRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[FREE3BRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[FREE4BRACE].ob_state,sizeof(int),1,fp);
		fwrite(&bracemenu[FREE5BRACE].ob_state,sizeof(int),1,fp);
		fputs(form_read(bracemenu,FREE1BEG,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE1END,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE2BEG,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE2END,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE3BEG,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE3END,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE4BEG,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE4END,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE5BEG,pathname),fp);
		fputs("\r\n",fp);
		fputs(form_read(bracemenu,FREE5END,pathname),fp);
		fputs("\r\n",fp);

		if(windstruct)
			fwrite(&_wind[1],(MAXWINDOWS-1)*sizeof(WINDOW),1,fp); /* Fenster */
		else
		{
			for(i=1; i<MAXWINDOWS; i++)
			{
				_dummy[i].work.g_x =_wind[i].work.g_x;
				_dummy[i].work.g_y =_wind[i].work.g_y;
				_dummy[i].work.g_w =_wind[i].work.g_w;
				_dummy[i].work.g_h =_wind[i].work.g_h;
				_dummy[i].w_state  =(_wind[i].w_state&(BLOCKSATZ+INSERT+INDENT+PROPFONT+COLUMN));
				_dummy[i].umbruch  =_wind[i].umbruch;
				_dummy[i].wscroll  =_wind[i].wscroll;
				_dummy[i].hscroll  =_wind[i].hscroll;
				_dummy[i].fontid   =_wind[i].fontid;
				_dummy[i].fontsize =_wind[i].fontsize;
				_dummy[i].tab      =_wind[i].tab;
			}
			fwrite(&_dummy[1],(MAXWINDOWS-1)*sizeof(WINDOW),1,fp); /* Fenster */
		}
/*
		fwrite(savename,sizeof(savename),1,fp);				/* Dateinamen */
*/
		/* Suchen / Ersetzen */
		fwrite(&findmenu[FINDNORM].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDGREP].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDMAT].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDSUCH].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDERS].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDALLQ].ob_state,sizeof(char),1,fp);
		fwrite(&findmenu[FINDSINQ].ob_state,sizeof(char),1,fp);
		fwrite(&findmenu[FINDBLK].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDANF].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDIGNO].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDWORD].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDASK].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDALL].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDFORW].ob_state,sizeof(int),1,fp);
		fwrite(&findmenu[FINDBACK].ob_state,sizeof(int),1,fp);

		fwrite(&divmenu[DIVTABEX].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVWRET ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVKONV ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVCRLF ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVLF	].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVCR	].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVPAPER].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVBACK ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVBUTIM].ob_state,sizeof(int),1,fp);
		fwrite(&backuptime,sizeof(long),1,fp);
		fwrite(&divmenu[DIVINFO ].ob_state,sizeof(int),1,fp);

		fwrite(&divmenu[DIVSTOOL].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVTABBAR].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVSCROLL].ob_state,sizeof(int),1,fp);

		fwrite(&divmenu[DIVUMLAUT].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVDESK ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVZENT ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVFREE ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVMAUS ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVDWIN ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVNAME ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVSAVE ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVBLANK].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVPATH ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVVAST ].ob_state,sizeof(int),1,fp);
		fwrite(&divmenu[DIVCLIP ].ob_state,sizeof(int),1,fp);

		fwrite(&sortmenu[SORTUP].ob_state,sizeof(int),1,fp);
		fwrite(&sortmenu[SORTDN].ob_state,sizeof(int),1,fp);
		fwrite(&sortmenu[SORTIGNO].ob_state,sizeof(int),1,fp);

		fputs(form_read(divmenu,DIVBACK2,pathname),fp);
		fputs("\r\n",fp);
/*
		fputs(form_read(divmenu,DIVHDA,pathname),fp);
*/
		fputs((char *)(divmenu[DIVHDA].ob_spec.index/*+16L*/),fp);
		fputs("\r\n",fp);

		fputs(form_read(findmenu,FINDSTR,pathname),fp);	  /* SUCHSTR */
		fputs("\r\n",fp);				  /* ^^^^^^ Suchstring */
		fputs(form_read(findmenu,FINDREPL,pathname),fp);	  /* ERSSTR */
		fputs("\r\n",fp);

		fwrite(&shellmenu[SHELLGEM].ob_state,sizeof(int),1,fp);/* SHELL */
		fputs(form_read(shellmenu,SHELCOMM,pathname),fp);		/* SHELL */
		fputs("\r\n",fp);

		fwrite(&grepmenu[GREPGREP].ob_state,sizeof(int),1,fp);/* GREP */
		fwrite(&grepmenu[GREPALL ].ob_state,sizeof(int),1,fp);/* GREP */
		fwrite(&grepmenu[GREPFOLD].ob_state,sizeof(int),1,fp);/* GREP */
		fwrite(&grepmenu[GREPMARK].ob_state,sizeof(int),1,fp);/* GREP */
		fputs(form_read(grepmenu,GREPPATT,pathname),fp);		/* GREP */
		fputs("\r\n",fp);
/*
		for(i=FKEY1; i<=FKEY10; i++)
		{
			fputs(form_read(fkeymenu,i,pathname),fp);	  /* Fkttasten */
			fputs("\r\n",fp);
		}
		for(i=SFKEY1; i<=SFKEY10; i++)
		{
			fputs(form_read(fkeymenu,i,pathname),fp);
			fputs("\r\n",fp);
		}
*/
		fwrite(&nummenu[NUMNDT ].ob_state,sizeof(int),1,fp);
		fwrite(&nummenu[NUMNAMI].ob_state,sizeof(int),1,fp);
		fwrite(&nummenu[NUMTSEP].ob_state,sizeof(int),1,fp);
		fwrite(&nummenu[NUMNORM].ob_state,sizeof(int),1,fp);
		fwrite(&nummenu[NUMERM ].ob_state,sizeof(int),1,fp);
		fputs(form_read(nummenu,NUMMWSTN,pathname),fp); /* NUMMENU */
		fputs("\r\n",fp);
		fputs(form_read(nummenu,NUMMWSTE,pathname),fp); /* NUMMENU */
		fputs("\r\n",fp);
		fputs(form_read(nummenu,NUMKOMMA,pathname),fp);	/* NUMMENU */
		fputs("\r\n",fp);

		fwrite(&menueditor[MENUTAKTIV].ob_state,sizeof(int),1,fp);

		fclose(fp);
		graf_mouse(ARROW,NULL);
	}
}

static char *stradj2(char *dest, char *src, int maxlen)
{
	register int len;
	dest[0]=' ';
	if((len=(int)strlen(src))>maxlen)
	{
		strncpy(dest,src,maxlen/2);
		strncpy(&dest[maxlen/2],&src[len-maxlen/2],maxlen/2);
		dest[maxlen/2-1+1]='.';
		dest[maxlen/2+0+1]='.';
		dest[maxlen/2+1+1]='.';
		dest[maxlen]      = 0;
	}
	else
	{
		strcpy(dest,src);
	}
	return(dest);
}

void restoreconfig(char *inffile) /* es kann 7UP.INF als Parameter Åbergeben werden */
{
	FILE *fp;
/*	register int i; */
	char fpattern[FILENAME_MAX];
	
	fp=fopen(inffile?inffile:find_7upinf(pathname,"INF",0),"rb");
	if(!fp)
		fp=fopen("7UP.INF","rb");
	
	if(!fp)
	{
		sprintf(alertstr,Aconfig[8],stradj2(fpattern,pathname,40));
		form_alert(1,alertstr);
		
	   pathname[0]=0;
	   strcpy(fpattern,"*.INF");
		if(getfilename(pathname,fpattern,"@",fselmsg[4]))
			fp=fopen(pathname,"rb");
	}
	if(fp)
	{
		fread(alertstr,VERSIONSTRLEN,1,fp);
		if(!strcmp(VERSIONNAME,alertstr))
		{
/* 30.9.94 jetzt in Diverses
			fread(&clipbrd,sizeof(int),1,fp);
*/
			fread(iconcoords,sizeof(ICNCOORDS),iconcoordcnt,fp);

			fread(&zl,14*sizeof(int),1,fp);
			fread(&WI_KIND,sizeof(int),1,fp);					/* Fensterdesign */
/*
			fread(&exitcode,sizeof(int),1,fp);					/* Exitcode */
*/
			fread(&layout[PRNKZL].ob_state,sizeof(int),1,fp);
			fread(&layout[PRNKZM].ob_state,sizeof(int),1,fp);
			fread(&layout[PRNKZR].ob_state,sizeof(int),1,fp);
			fread(&layout[PRNFZL].ob_state,sizeof(int),1,fp);
			fread(&layout[PRNFZM].ob_state,sizeof(int),1,fp);
			fread(&layout[PRNFZR].ob_state,sizeof(int),1,fp);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(layout,PRNKZSTR,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(layout,PRNFZSTR,pathname,0);
			fread(&pinstall[PRNNUM].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNCUT].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNALT].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNKFBEG].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNFILE].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNBACK].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNDEL].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNALLP].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNPAIR].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNUNPA].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNFF].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNPAUSE].ob_state,sizeof(int),1,fp);
			fread(&pinstall[PRNNOFORM].ob_state,sizeof(int),1,fp);

			fread(&act_dev,  sizeof(int),1,fp);
			fread(&act_dist, sizeof(int),1,fp);
			fread(&act_paper,sizeof(int),1,fp);

			fread(&bracemenu[CBRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[SBRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[RBRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[PBRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[PKOMBRAC].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[CKOMBRAC].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[FREE1BRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[FREE2BRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[FREE3BRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[FREE4BRACE].ob_state,sizeof(int),1,fp);
			fread(&bracemenu[FREE5BRACE].ob_state,sizeof(int),1,fp);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE1BEG,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE1END,pathname,0);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE2BEG,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE2END,pathname,0);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE3BEG,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE3END,pathname,0);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE4BEG,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE4END,pathname,0);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE5BEG,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(bracemenu,FREE5END,pathname,0);

			fread(&_wind[1],(MAXWINDOWS-1)*sizeof(WINDOW),1,fp);
/*
			fread(savename,sizeof(savename),1,fp);
*/
			fread(&findmenu[FINDNORM].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDGREP].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDMAT].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDSUCH].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDERS].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDALLQ].ob_state,sizeof(char),1,fp);
			fread(&findmenu[FINDSINQ].ob_state,sizeof(char),1,fp);
			fread(&findmenu[FINDBLK].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDANF].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDIGNO].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDWORD].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDASK].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDALL].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDFORW].ob_state,sizeof(int),1,fp);
			fread(&findmenu[FINDBACK].ob_state,sizeof(int),1,fp);

			fread(&divmenu[DIVTABEX].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVWRET].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVKONV].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVCRLF ].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVLF	].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVCR	].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVPAPER].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVBACK].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVBUTIM].ob_state,sizeof(int),1,fp);
			fread(&backuptime,sizeof(long),1,fp);
			fread(&divmenu[DIVINFO].ob_state,sizeof(int),1,fp);

			fread(&divmenu[DIVSTOOL].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVTABBAR].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVSCROLL].ob_state,sizeof(int),1,fp);

			fread(&divmenu[DIVUMLAUT].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVDESK].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVZENT].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVFREE].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVMAUS].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVDWIN].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVNAME].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVSAVE].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVBLANK].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVPATH].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVVAST].ob_state,sizeof(int),1,fp);
			fread(&divmenu[DIVCLIP].ob_state,sizeof(int),1,fp);

			fread(&sortmenu[SORTUP].ob_state,sizeof(int),1,fp);
			fread(&sortmenu[SORTDN].ob_state,sizeof(int),1,fp);
			fread(&sortmenu[SORTIGNO].ob_state,sizeof(int),1,fp);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(divmenu,DIVBACK2,pathname,0);

			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
/*
			form_write(divmenu,DIVHDA,pathname,0);
*/
		   strcpy((char *)(divmenu[DIVHDA].ob_spec.index/*+16L*/),pathname);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(findmenu,FINDSTR,pathname,0);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(findmenu,FINDREPL,pathname,0);

			fread(&shellmenu[SHELLGEM].ob_state,sizeof(int),1,fp);/* SHELL */
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(shellmenu,SHELCOMM,pathname,0);

			fread(&grepmenu[GREPGREP].ob_state,sizeof(int),1,fp);/* GREP */
			fread(&grepmenu[GREPALL ].ob_state,sizeof(int),1,fp);/* GREP */
			fread(&grepmenu[GREPFOLD].ob_state,sizeof(int),1,fp);/* GREP */
			fread(&grepmenu[GREPMARK].ob_state,sizeof(int),1,fp);/* GREP */
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(grepmenu,GREPPATT,pathname,0);
/*
			for(i=FKEY1; i<=FKEY10; i++)
			{
				fgets(pathname,64,fp);
				pathname[strlen(pathname)-2]=0;
				form_write(fkeymenu,i,pathname,0);
			}
			for(i=SFKEY1; i<=SFKEY10; i++)
			{
				fgets(pathname,64,fp);
				pathname[strlen(pathname)-2]=0;
				form_write(fkeymenu,i,pathname,0);
			}
*/
			fread(&nummenu[NUMNDT].ob_state,sizeof(int),1,fp);
			fread(&nummenu[NUMNAMI].ob_state,sizeof(int),1,fp);
			fread(&nummenu[NUMTSEP].ob_state,sizeof(int),1,fp);
			fread(&nummenu[NUMNORM].ob_state,sizeof(int),1,fp);
			fread(&nummenu[NUMERM ].ob_state,sizeof(int),1,fp);
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(nummenu,NUMMWSTN,pathname,0);/* NUMMENU */
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(nummenu,NUMMWSTE,pathname,0);/* NUMMENU */
			fgets(pathname,64,fp);
			pathname[strlen(pathname)-2]=0;
			form_write(nummenu,NUMKOMMA,pathname,0);/* NUMMENU */

			fread(&menueditor[MENUTAKTIV].ob_state,sizeof(int),1,fp);

		}
		fclose(fp);
	}
}

int abs2rel(int abspos, int pixel)
{
	long hilf;
	hilf=(long)abspos*10000/pixel;
	return((int)hilf);
}

int rel2abs(int relpos, int pixel)
{
	long hilf;
	hilf=(long)relpos*pixel/10000;
	return((int)hilf);
}

int saveformat(OBJECT *tree, char *fname)
{
	FILE *fp;
	if((fp=fopen(fname,"wb"))!=NULL)
	{
		fwrite(VERSIONNAME,VERSIONSTRLEN,1,fp);
		fwrite(&zl,8*sizeof(int),1,fp);					  /* Drucker */
		fwrite(&tree[PRNKZL].ob_state,sizeof(int),1,fp);
		fwrite(&tree[PRNKZM].ob_state,sizeof(int),1,fp);
		fwrite(&tree[PRNKZR].ob_state,sizeof(int),1,fp);
		fwrite(&tree[PRNFZL].ob_state,sizeof(int),1,fp);
		fwrite(&tree[PRNFZM].ob_state,sizeof(int),1,fp);
		fwrite(&tree[PRNFZR].ob_state,sizeof(int),1,fp);
		fputs(form_read(tree,PRNKZSTR,alertstr),fp);
		fputs("\r\n",fp);
		fputs(form_read(tree,PRNFZSTR,alertstr),fp);
		fputs("\r\n",fp);
		fclose(fp);
		return(1);
	}
	else
		form_alert(1,Aconfig[5]);
	return(0);
}

int loadformat(OBJECT *tree, char *fname)
{
	FILE *fp;
	if((fp=fopen(fname,"rb"))!=NULL)
	{
		fread(alertstr,VERSIONSTRLEN,1,fp);
		if(!strcmp(VERSIONNAME,alertstr))
		{
			fread(&zl,8*sizeof(int),1,fp);
			sprintf(tree[PRNZL].ob_spec.tedinfo->te_ptext,"%3d",zl);
			sprintf(tree[PRNBL].ob_spec.tedinfo->te_ptext,"%3d",bl);
			sprintf(tree[PRNOR].ob_spec.tedinfo->te_ptext,"%02d",or);
			sprintf(tree[PRNKZ].ob_spec.tedinfo->te_ptext,"%02d",kz);
			sprintf(tree[PRNFZ].ob_spec.tedinfo->te_ptext,"%02d",fz);
			sprintf(tree[PRNUR].ob_spec.tedinfo->te_ptext,"%02d",ur);
			sprintf(tree[PRNLR].ob_spec.tedinfo->te_ptext,"%02d",lr);
			sprintf(tree[PRNZZ].ob_spec.tedinfo->te_ptext,"%3d",zz);
			fread(&tree[PRNKZL].ob_state,sizeof(int),1,fp);
			fread(&tree[PRNKZM].ob_state,sizeof(int),1,fp);
			fread(&tree[PRNKZR].ob_state,sizeof(int),1,fp);
			fread(&tree[PRNFZL].ob_state,sizeof(int),1,fp);
			fread(&tree[PRNFZM].ob_state,sizeof(int),1,fp);
			fread(&tree[PRNFZR].ob_state,sizeof(int),1,fp);
			fgets(alertstr,64,fp);
			alertstr[strlen(alertstr)-2]=0;
			form_write(tree,PRNKZSTR,alertstr,0);
			fgets(alertstr,64,fp);
			alertstr[strlen(alertstr)-2]=0;
			form_write(tree,PRNFZSTR,alertstr,0);
			fclose(fp);
			return(1);
		}
		fclose(fp);
		form_alert(1,Aconfig[6]);
	}
	else
		form_alert(1,Aconfig[7]);
	return(0);
}

void sicons(void)
{
	register int i;
	for(i=DESKICN1;i<=DESKICNA;i++)				  /* koordinaten sichern */
	{
		iconcoords[i-DESKICN1].x = abs2rel(desktop[i].ob_x, xdesk+wdesk);
		iconcoords[i-DESKICN1].y = abs2rel(desktop[i].ob_y, ydesk+hdesk);
	}
	if(desktop[DESKICN8].ob_flags&HIDETREE)
	{
		iconcoords[i-DESKICN1].x = abs2rel(desktop[DESKICND].ob_x, xdesk+wdesk);
		iconcoords[i-DESKICN1].y = abs2rel(desktop[DESKICND].ob_y, ydesk+hdesk);
	}
	else
	{
		iconcoords[i-DESKICN1].x = abs2rel(desktop[DESKICN8].ob_x, xdesk+wdesk);
		iconcoords[i-DESKICN1].y = abs2rel(desktop[DESKICN8].ob_y, ydesk+hdesk);
	}
	if(desktop[DESKICNB].ob_flags&HIDETREE)
	{
		iconcoords[i-DESKICN1].x = abs2rel(desktop[DESKICNC].ob_x, xdesk+wdesk);
		iconcoords[i-DESKICN1].y = abs2rel(desktop[DESKICNC].ob_y, ydesk+hdesk);
	}
	else
	{
		iconcoords[i-DESKICN1].x = abs2rel(desktop[DESKICNB].ob_x, xdesk+wdesk);
		iconcoords[i-DESKICN1].y = abs2rel(desktop[DESKICNB].ob_y, ydesk+hdesk);
	}
}
/*
int readnames(void) /* letzte Dateinamen sichern */
{
	FILE *fp;
	char filename[PATH_MAX];

	if(divmenu[DIVNAME].ob_state & SELECTED)
	{
		if((fp=fopen(find_7upinf(pathname,"NMS",0),"r"))!=NULL)
		{
			fgets(filename,PATH_MAX,fp);
			filename[strlen(filename)-1]=0;
			if(!strcmp(VERSIONNAME,filename))
			{
				while(fgets(filename,PATH_MAX,fp))
				{
					filename[strlen(filename)-1]=0;
					Wreadtempfile(filename,1);
				}
			}
			fclose(fp);
			unlink(pathname);
			return(1);
		}
	}
	return(0);
}

void writenames(void) /* letzte Dateien laden */
{
	int i;
	FILE *fp;

	if(divmenu[DIVNAME].ob_state & SELECTED)
	{
		if((Wcount(CREATED)>0) && ((fp=fopen(find_7upinf(pathname,"NMS",1),"w"))!=NULL))
		{
			fputs(VERSIONNAME,fp);
			fputs("\n",fp);
			for(i=1; i<MAXWINDOWS; i++)
			{
				if(_wind[i].w_state & CREATED)
				{
					fputs((char *)Wname(&_wind[i]),fp);
					fputs("\n",fp);
				}
				else
					_wind[i].col=_wind[i].row=
					_wind[i].wfirst=_wind[i].hfirst=0;
			}
			fclose(fp);
		}
	}
}
*/
