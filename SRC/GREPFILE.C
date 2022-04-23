/*****************************************************************
	7UP
	Modul: GREPFILE.C
	(c) by TheoSoft '92

	Durchsucht Dateien nach Mustern
	
	1997-04-07 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
	PL02:
	1997-06-20 (MJK): auf caseinsensitiven Dateisystem Dateiname
	                  und Muster caseinsensitiv vergleichen
*****************************************************************/
#include <stdio.h>
#include <macros.h>
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	undef abs
#	include <tos.h>
#	include <ext.h>
#else
#	include <findnext.h>
#	include <osbind.h>
#	include <stat.h>
#	include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif

#include "fsel_inp.h"
#include "forms.h"
#include "windows.h"
#include "7up.h"
#include "version.h"
#include "alert.h"
#include "7up3.h"
#include "tabulat.h"
#include "fileio.h"
#include "objc_.h"
#include "grep.h"
#include "resource.h"

#include "grepfile.h"

#define ESC 0x01
#define WM_BACKDROPPED	31		/* Message vom Eventhandler */
#define FILEIOBUFF (32*1024L)

static char pattern[FILENAME_MAX],filename[PATH_MAX];
static int all,files,folders,abbruch=0,Xmode=0;
static long found;
static FILE *gp;
static int msgbuf[8];

static MEVENT mevent=
{
	MU_TIMER|MU_KEYBD|MU_MESAG,
	0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	msgbuf,
	0L,
	0,0,0,0,0,0,
/* nur der Vollst„ndigkeit halber die Variablen von XGEM */
	0,0,0,0,0,
	0,
	0L,
	0L,0L
};

static char *stradj(char *dest, char *src, int maxlen)
{
	register int len;
	if((len=(int)strlen(src))>maxlen)
	{
		strncpy(dest,src,maxlen/2);
		strncpy(&dest[maxlen/2],&src[len-maxlen/2],maxlen/2);
		dest[maxlen/2-1]='.';
		dest[maxlen/2+0]='.';
		dest[maxlen/2+1]='.';
		return(dest);
	}
	return(src);
}

static int grepfile(OBJECT *tree, char *fname)
{
	FILE *fp=NULL;
	int dummy,event,line=0,len,f=0;
	char *cp, string[29];
	WINDOW *wp;

	event=evnt_event(&mevent);
	wind_update(BEG_UPDATE);
	if(event & MU_MESAG)
	{
		if((msgbuf[0] != MN_SELECTED) && (msgbuf[0] < 50)) /* AP_TERM */
		{
			if(msgbuf[3]==dial_handle) /* Dialogfenster */
			{
				switch(msgbuf[0])
				{
					case WM_BACKDROPPED: /* nein, niemals */
						break;
					case WM_REDRAW:
						fwind_redraw(tree,msgbuf[3],&msgbuf[4]);
						break;
					case WM_MOVED:
						fwind_move(tree,msgbuf[3],&msgbuf[4]);
						break;
					case WM_TOPPED:
						wind_set(msgbuf[3],WF_TOP,0,0,0,0);
						break;
				}
			}
			else /* aber kein Schliežen oder Toppen */
				if(msgbuf[0]!=WM_CLOSED && msgbuf[0]!=WM_TOPPED)
					Wwindmsg(Wp(msgbuf[3]),msgbuf);
		}
	}
	if(event&MU_KEYBD)
	{
		if((mevent.e_kr>>8)==ESC)							 /* ESC gedrckt? */
			if(form_alert(2,Agrepfile[0])==2)
				abbruch=1;
	}
	wind_update(END_UPDATE);

	if(!(tree[GREPGREP].ob_state&SELECTED)) /* normal suchen */
	{
		form_read(tree,GREPPATT,alertstr);
		len=(int)strlen(alertstr);
	}

	if(!abbruch && (fp=fopen(fname,"r"))!=NULL)
	{
		setvbuf(fp,NULL,_IOFBF,min(filelength(fileno(fp)),FILEIOBUFF));
/*
      form_write(tree,GREPNAME,strlen(fname)>28?&fname[strlen(fname)-28]:fname,1);/* Namen einblenden */
*/
      form_write(tree,GREPNAME,stradj(string, fname, 28),1);/* Namen einblenden */
      
		while(fgets(iostr2,STRING_LENGTH+2,fp)!=NULL)
		{
			line++;
			stpexpan(iostring,iostr2,((wp=Wgettop())!=NULL)?wp->tab:3,STRING_LENGTH,&dummy);

         if(tree[GREPGREP].ob_state&SELECTED)
			   cp=grep(iostring,iostring,&len); /* regul„re Ausdrcke */
			else
			   cp=strstr(iostring,alertstr);         /* normal suchen */
/*
			cp=grep(iostring,iostring,&len);
*/
         if(cp)
			{
				if(!f)
				{
					fprintf(gp,"\nDatei %s\n",fname);
					files++;
				}
				found++;
				f++;
				fprintf(gp,"%-4d %-3d %-3d: %s",line,(int)(cp-iostring)+1,len,iostring);
				if(!all)
				{
					fclose(fp);
					return(f);
				}
			}
		}
		if(f)
			fprintf(gp,"%d Textstelle(n)\n",f);
		fclose(fp);
	}
	return(f);
}

static char lfilename[PATH_MAX];

static void walktree(OBJECT *tree,char *path)
{
#if defined ( __TURBOC__ ) && !defined( __MINT__ )
	struct ffblk l_dta;
#else
	DIR *dir;
	struct dirent *dent;
	struct stat finfo;
	int casesensitiv;
#endif
	char l_name[PATH_MAX];

#if defined ( __TURBOC__ ) && !defined( __MINT__ )
	sprintf(l_name,"%s%s",path,pattern);
	if(!findfirst(l_name,&l_dta,0))
	{
		sprintf(filename,"%s%s",path,l_dta.ff_name);
		/*31.12.95*/
		if(strcmp(filename, lfilename))/*Zieldatei nicht nochmal bewerten!*/
			grepfile(tree,filename);
		while(!findnext(&l_dta))
		{
			sprintf(filename,"%s%s",path,l_dta.ff_name);
			if(
#else
	if( (dir=opendir(path)) != NULL )
	{
	  casesensitiv = pathconf( path, 6 ) == 0; /* 1997-06-20 (MJK): auf caseinsensitive System so suchen */
		while ( (dent = casesensitiv ? findnext(dir,pattern) : findinext(dir,pattern)) != NULL )
		{
			sprintf(filename,"%s%s",path,dent->d_name);
			if ( !stat(filename,&finfo) && S_ISREG(finfo.st_mode) &&
#endif
			/*31.12.95*/
			     strcmp(filename, lfilename))/*Zieldatei nicht nochmal bewerten!*/
				grepfile(tree,filename);
		}
#if !defined ( __TURBOC__ ) || defined( __MINT__ )
		rewinddir(dir);
#endif		
	}
	if(!(grepmenu[GREPFOLD].ob_state & SELECTED))
		return;
#if defined ( __TURBOC__ ) && !defined( __MINT__ )
	sprintf(l_name,"%s*.*",path);
	if(!findfirst(l_name,&l_dta,0x10))
	{
		if(l_dta.ff_attrib & 0x10)
			if(strcmp(l_dta.ff_name,".") && strcmp(l_dta.ff_name,".."))
			{
				folders++;
				if(Xmode)
					sprintf(l_name,"%s%s/",path,l_dta.ff_name);
				else
					sprintf(l_name,"%s%s\\",path,l_dta.ff_name);
				walktree(tree,l_name);
			}
			while(!findnext(&l_dta))
			{
				if(l_dta.ff_attrib & 0x10)
					if(strcmp(l_dta.ff_name,".") && strcmp(l_dta.ff_name,".."))
#else
	if( dir )
	{
		while( (dent = readdir(dir)) != NULL )
			if ( *dent->d_name != '.' ||
			     dent->d_name[1] && 
			     ( dent->d_name[1] != '.' || dent->d_name[2] ) )
			{
				sprintf(l_name,"%s%s",path,dent->d_name);
				if ( !stat(l_name,&finfo) && S_ISDIR(finfo.st_mode) )
#endif
					{
						folders++;
						if(Xmode)
							sprintf(l_name,"%s%s/",path,
#if defined ( __TURBOC__ ) && !defined( __MINT__ )
							        l_dta.ff_name
#else
							        dent->d_name
#endif
							       );
						else
							sprintf(l_name,"%s%s\\",path,
#if defined ( __TURBOC__ ) && !defined( __MINT__ )
							        l_dta.ff_name
#else
							        dent->d_name
#endif
							       );
						walktree(tree,l_name);
					}
		  }
#if !defined ( __TURBOC__ ) || defined( __MINT__ )
		closedir(dir);
#endif		
	 }
}

char lpath[MAXPATHS][PATH_MAX]={"","","","","","",""};
char *lname[MAXPATHS]={lpath[0],lpath[1],lpath[2],lpath[3],lpath[4],lpath[5],lpath[6]};

void hndl_grepmenu(OBJECT *tree, int start)
{
	int a,b,c,i,exit_obj,thread=0;

	char *cp,pathname[PATH_MAX],openmodus[2];

	int done=0,ret,kstate;
	/*static*/ char fpattern[FILENAME_MAX]="*.*";
	/*static*/ char gpattern[FILENAME_MAX]="*.REG";
   
	found=all=files=folders=0;
	a=tree[GREPALL ].ob_state;
	b=tree[GREPFOLD].ob_state;
	c=tree[GREPMARK].ob_state;
	abbruch=0;
/*
	if(getenv("UNIXMODE")!=NULL)
		Xmode=1;
	else
*/
	   Xmode=0;
/*	
	local.tree=tree;
*/	
	if(begcut && endcut && begcut==endcut && endcut->begcol<endcut->used)
	{
		 strncpy(alertstr,&begcut->string[begcut->begcol],begcut->endcol-begcut->begcol);
		 alertstr[begcut->endcol-begcut->begcol]=0;
		 alertstr[tree[GREPPATT].ob_spec.tedinfo->te_txtlen-1]=0;
		 form_write(tree,GREPPATT,alertstr,0);
	}
	if( slct_check(0x0100) && tree[GREPMARK].ob_state == DISABLED)
		tree[GREPMARK].ob_state = NORMAL;
	if(!slct_check(0x0100))
		tree[GREPMARK].ob_state = DISABLED;
   form_write(tree,GREPNAME,"",0);/* letzten Namen l”schen */
	form_exopen(tree, 0);
	do
	{
		exit_obj=form_exdo(tree, start);
		switch(exit_obj)
		{
			case GREPHELP:
         	if(tree[GREPGREP].ob_state&SELECTED) /* bei reg. Exp. */
         	{
				   if(form_alert(2,Agrepfile[6])==2)
   				   if(form_alert(2,Agrepfile[7])==2)
	   				   form_alert(1,Agrepfile[8]);
				}
				else
				   form_alert(1,Agrepfile[1]);
				objc_change(tree,exit_obj,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
				break;
			case GREPOK:
				form_read(tree,GREPPATT,alertstr);
				if(!(*alertstr))
				{
					done=1;
					break;
				}
         	if(tree[GREPGREP].ob_state&SELECTED) /* regul„rer Ausdruck */
					if(!compile(alertstr)) /* immer kompilieren !!! */
					{
						done=1;
						break;
					}
				if(tree[GREPMARK].ob_state & SELECTED)
				{
					slct_morenames(0, MAXPATHS,lname); /* we want more */
					/*local.*/pathname[0]=0;
					getfilename(/*local.*/pathname,fpattern,"@",fselmsg[17]);
					if(!/*local.*/pathname[0]) /* Kunde will nicht */
					{
						done=1;
						break;
					}
				}
				else
				{
					for(i=0; i<MAXPATHS; i++)
					{
						sprintf(&alertstr[100],"%d. Startverzeichnis/Suchmuster",i+1);
						*lpath[i]=0;
						if(!getfilename(lpath[i],fpattern,"@",&alertstr[100]))
							break;
					}
					if(!*lpath[0]) /* Kunde will nicht */
					{
						done=1;
						break;
					}
				}
				/*local.*/lfilename[0]=0;
				if(getfilename(/*local.*/lfilename,gpattern,"7up.reg",fselmsg[18]))
				{
					if(!windials) /* Hintergrund restaurieren */
						objc_update(tree,ROOT,MAX_DEPTH);
					if(strstr(lfilename,".reg")==NULL || strstr(lfilename,".REG")==NULL)
					{
						form_alert(1,Agrepfile[3]);
						done=1;
						break;
					}
					graf_mkstate(&ret,&ret,&ret,&kstate);
					if(kstate & (K_LSHIFT|K_RSHIFT))		/* bei gedrckter Shifttaste... */
						strcpy(/*local.*/openmodus,"a");				 /* ans Regfile anh„ngen,...*/
					else
						strcpy(/*local.*/openmodus,"w");				 /* ...sonst neue Datei		 */
/*
					thread=tree[GREPTHREAD].ob_state&SELECTED;
					if(thread)
						tfork(_searchfiles, 0L);
					else
						_searchfiles(0L);
*/					
					if((gp=fopen(lfilename,openmodus))!=NULL)
					{
						setvbuf(gp,NULL,_IOFBF,FILEIOBUFF/8);
						fprintf(gp,"%s\n\n",VERSIONNAME);
						fprintf(gp,"Gesucht: \"%s\"\n",alertstr);
						if(tree[GREPMARK].ob_state & SELECTED)
						{
							folders=1;
							for(i=0; i<slct->out_count; i++)
							{  /* Pfadnamen zusammenbasteln */
								if((cp=strrchr(pathname,'\\'))!=NULL || 
									(cp=strrchr(pathname,'/'))!=NULL)
								{
									cp[1]=0;
									strcat(pathname,lpath[i]);
									all=(tree[GREPALL].ob_state & SELECTED);
									grepfile(tree,pathname);
								}
							}
						}
						else
						{
							for(i=0; i<MAXPATHS-2 && *lpath[i]; i++)
							{
								if((cp=strrchr(lpath[i],'\\'))!=NULL || 
									(cp=strrchr(lpath[i],'/'))!=NULL)
								{
									strcpy(pattern,cp+1L);
									cut_path(lpath[i]);
/*
									strcut(lpath[i],'\\');
*/
									all=(tree[GREPALL].ob_state & SELECTED);
									walktree(tree,lpath[i]);
								}
							}
						}
						fprintf(gp,"\n%ld Textstelle(n) in %d Datei(en) in %d Ordner(n).\n\n",found,files,folders+1);
						fclose(gp);
					}
					else
					{
						sprintf(alertstr,Agrepfile[5],(char *)split_fname(filename));
						form_alert(1,alertstr);
					}
				}
			case GREPABBR:
				done=1;
				break;
		}
	}
	while(!done);
	form_exclose(tree, exit_obj, 0);
	if(exit_obj==GREPABBR)
	{
		tree[GREPALL ].ob_state=a;
		tree[GREPFOLD].ob_state=b;
		tree[GREPMARK].ob_state=c;
		return;
	}
	if(files)
	{
		if(!thread)
			Wreadtempfile(/*local.*/lfilename,0);
	}
	else
	{
		unlink(/*local.*/lfilename);
	}
}

void prepare(OBJECT *tree, OBJECT *tree2, char *str)
{
	form_write(tree,FINDSTR ,str,0);
	form_write(tree,FINDREPL,"",0);

  	if(tree2[GREPGREP].ob_state & SELECTED) /* regul„rer Ausdruck */
   {
		tree[FINDNORM].ob_state=NORMAL;
		tree[FINDMAT ].ob_state=NORMAL;
		tree[FINDGREP].ob_state=SELECTED;
	}
	else
	{
		tree[FINDNORM].ob_state=SELECTED;
		tree[FINDMAT ].ob_state=NORMAL;
		tree[FINDGREP].ob_state=NORMAL;
	}
	tree[FINDSUCH].ob_state=SELECTED;
	tree[FINDERS ].ob_state=NORMAL;

	tree[FINDBLK ].ob_state=DISABLED;
	tree[FINDIGNO].ob_state=SELECTED;
	tree[FINDFORW].ob_state=SELECTED;
	tree[FINDASK ].ob_state=DISABLED;

	tree[FINDANF ].ob_state=NORMAL;
	tree[FINDWORD].ob_state=DISABLED;
	tree[FINDBACK].ob_state=DISABLED;
	tree[FINDALL ].ob_state=DISABLED;
}

