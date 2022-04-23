/*****************************************************************
	7UP
	Modul: FKEYS.C
	(c) by TheoSoft '91

	Funktionstastenbelegung

	1997-03-25 (MJK):	static-Deklaration einiger nur lokal 
	                  ben”tigter Funktionen erg„nzt
	1997-03-27 (MJK): ben”tigte Headerfiles werden geladen,
	                  compilierbar mit max. Warnungen,
	1997-04-11 (MJK): Eingeschr„nkt auf GEMDOS
	2000-03-11 (GS) :	Beim Parameter %J in datetime Y2K behoben.
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <tos.h>
#else
#	include <osbind.h>
#	include <support.h>
#	define itoa(a,b,c) _itoa(a,b,c)
#endif
#ifndef PATH_MAX
#	include <limits.h>
#endif

#include "alert.h"
#include "windows.h"
#include "forms.h"
#include "7UP.h"
#include "language.h"
#include "undo.h"
#include "block.h"
#include "fileio.h"
#include "resource.h"
#include "findrep.h"
#include "editor.h"
#include "7up3.h"
#include "config.h"
#include "objc_.h"
#include "graf_.h"

#include "fkeys.h"

#define F1	 0x803BU
#define F10	 0x8044U
#define SF1	 0x8254U /*11.6.94 Bugfix*/
#define SF10 0x825DU
#define ESC	 27
#define TAB	 9
#define CR   0x0D

static char fkeystat[10]=
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

static char sfkeystat[10]=
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

static char *dayoftheweek[]=
{
	SONNTAG,
	MONTAG,
	DIENSTAG,
	MITTWOCH,
	DONNERSTAG,
	FREITAG,
	SONNABEND
};

static char *datetime(char what)
{
	long timer;
	struct tm *tp;
	static char str[16]="";

	time(&timer);
	tp=localtime(&timer);
	switch(what)
	{
		case 's':
			sprintf(str,"%02d",tp->tm_sec);
			break;
		case 'm':
			sprintf(str,"%02d",tp->tm_min);
			break;
		case 'h':
			sprintf(str,"%02d",tp->tm_hour);
			break;
		case 'T':
			sprintf(str,"%02d",tp->tm_mday);
			break;
		case 'M':
			sprintf(str,"%02d",tp->tm_mon+1);
			break;
		case 'J':
			sprintf(str,"%02d",tp->tm_year % 100);
			break;
		case 'W':
			sprintf(str,"%s",dayoftheweek[tp->tm_wday]);
			break;
		case 'D':
			sprintf(str,"%d",tp->tm_yday+1);
			break;
	}
	return(str);
}

char *expandfkey(WINDOW *wp, char *string, int page)
{
	register int i,k;
	char code, expanded[161];

	k=(int)strlen(string);
	strcpy(expanded,string);

	for(i=0; i<k; i++)
	{
		if(expanded[i]=='%')
		{
			switch(code=expanded[i+1])
			{
				case 's':
				case 'm':
				case 'h':
				case 'T':
				case 'M':
				case 'J':
				case 'W':
				case 'D':
					if(!expanded[i+2])
						strcpy(&expanded[i],datetime(code));
					else
					{
						strcpy(&expanded[i],&expanded[i+2]);
						strins(expanded,datetime(code),i);
						k+=(int)(strlen(datetime(code))-2);
					}
					break;
				case 'F':					  /* Pfadname */
					if(!expanded[i+2])
						strcpy(&expanded[i],(char *)Wname(wp));
					else
					{
						strcpy(&expanded[i],&expanded[i+2]);
						strins(expanded,(char *)Wname(wp),i);
						k+=(int)(strlen((char *)Wname(wp))-2);
					}
					break;
				case 'f':					 /* Name */
					strcpy(iostring,split_fname((char *)Wname(wp)));
					if(!expanded[i+2])
						strcpy(&expanded[i],iostring);
					else
					{
						strcpy(&expanded[i],&expanded[i+2]);
						strins(expanded,iostring,i);
						k+=(int)(strlen(iostring)-2);
					}
					break;
				case 'p':
					itoa(page,iostring,10);
					if(!expanded[i+2])
						strcpy(&expanded[i],iostring);
					else
					{
						strcpy(&expanded[i],&expanded[i+2]);
						strins(expanded,iostring,i);
						k+=(int)(strlen(iostring)-2);
					}
					break;
				case '%':
					strcpy(&expanded[i],&expanded[i+1]);
					k--;
					break;
				default:
					break;
			}
		}
	}
	strcpy(string, expanded);
	return(string);
}
/*
void _keyoff(void);
void _keyon(void);
*/
static int newfkey(char c)
{
	register int i=0;
	switch(c)
	{
		case '0':
			i++;
		case '9':
			i++;
		case '8':
			i++;
		case '7':
			i++;
		case '6':
			i++;
		case '5':
			i++;
		case '4':
			i++;
		case '3':
			i++;
		case '2':
			i++;
		case '1':
			return(i);
			/*break;*/
	}
	return(-1);
}

static void newkey(int c1, int c2, int *state, int *key)
{
	switch(c1)
	{
		case 'A':
		case 'a':
			*state=K_ALT;
			*key=c2;
			break;
		case 'C':
		case 'c':
			*state=K_CTRL;
			*key=c2;
			break;
	}
}

#pragma warn -par
int fkeys(WINDOW *wp, int ks, unsigned int kr, LINESTRUCT **begcut, LINESTRUCT **endcut)
{
	char *str,fstr[161];
	register int key,i,k,m,ret=0;
	int nstate,nkey,fkey=0;
	
	if(wp && (kr & 0x8000))/* Scancodebit muž gesetzt sein */
	{
		if(kr>=F1 && kr<=F10)
		{
			key=(kr)-(F1);
			fkeystat[key]=1;
			if((str=expandfkey(wp,form_read(fkeymenu,FKEY1+key,fstr),0)) != NULL)
			{
				k=(int)strlen(str);
				if(k)
				{
					for(i=0; i<k; i++)
					{
						switch(str[i])
						{
							case '\\':
								switch(str[i+1])
								{
									case 'C': /* Cntrl ? */
									case 'c': /* Cntrl ? */
									case 'A': /* Alt ? */
									case 'a': /* Alt ? */
										newkey(__toupper(str[i+1]),__toupper(str[i+2]),&nstate,&nkey);
										hndl_keybd(nstate,nkey);
										break;
									case 'f':
										if((fkey=newfkey(str[i+2]))>=0)
										{
											if(!fkeystat[fkey])
											{
												fkeystat[fkey]=1;
												fkeys(wp,0,((F1)+fkey),begcut,endcut);
												fkeystat[fkey]=0;
											}
											i+=2;
										}
										else
											editor(wp,0,(int)str[i],begcut,endcut);
										break;
									case 'F':
										if((fkey=newfkey(str[i+2]))>=0)
										{
											if(!sfkeystat[fkey])
											{
												sfkeystat[fkey]=1;
												fkeys(wp,K_LSHIFT,((SF1)+fkey),begcut,endcut);
												sfkeystat[fkey]=0;
											}
											i+=2;
										}
										else
											editor(wp,0,(int)str[i],begcut,endcut);
										break;
									case 't':/* Text hinzuladen */
/* MT 18.6.95 */
										if(!*begcut && !*endcut)
										{
											switch(_read_blk(wp, &str[i+2], begcut, endcut))
											{
												case -1:	/* kein ram frei */
													sprintf(alertstr,Afkeys[1],split_fname(&str[i+2]));
													form_alert(1,alertstr); /* kein break, es geht weiter */
												case 1: /* ok */
													store_undo(wp, &undo, *begcut, *endcut, WINEDIT, EDITCUT);
													wp->w_state|=CHANGED;
													m=(*endcut)->used; /* merken, wird ben”tigt, geht aber verloren */
													graf_mouse_on(0);
													Wcursor(wp);
													if((wp->w_state&COLUMN))
														paste_col(wp,*begcut,*endcut);
													else
														paste_blk(wp,*begcut,*endcut);
													Wcursor(wp);
													graf_mouse_on(1);
													(*endcut)->endcol=m;
													if(!(wp->w_state&COLUMN))
														hndl_blkfind(wp,*begcut,*endcut,SEAREND);
													else
														free_blk(wp,*begcut); /* Spaltenblock freigeben */
													graf_mouse_on(0);
													Wcursor(wp);
													Wredraw(wp,&wp->work);
													Wcursor(wp);
													graf_mouse_on(1);
													break;
												case 0:
													sprintf(alertstr,Afkeys[2],(char *)split_fname(&str[i+2]));
													form_alert(1,alertstr);
													undo.item=0;	/* EDITCUT */
													break;
											}
											*begcut=*endcut=NULL;
										}
										return(1);/* raus, sonst wird auch der Dateiname eingefgt */
										/*break;*/
									default:
										editor(wp,0,(int)str[i],begcut,endcut);
										break;
								}
								break;
							case TAB:
								editor(wp,0,TAB,begcut,endcut);
								break;
							case ESC:
								editor(wp,0,ESC,begcut,endcut);
								break;
							case '\r': /* ^M == CR bercksichtigen */
								editor(wp,0,CR,begcut,endcut);
								break;
							default:
								editor(wp,0,(int)str[i],begcut,endcut);
								break;
						}
					}
				}
				else
				{
					fkeymenu[FKNORM ].ob_flags&=~HIDETREE;
					fkeymenu[FKSHIFT].ob_flags|=HIDETREE;
					if((*begcut) && (*endcut) && (*begcut)==(*endcut) && (*endcut)->begcol<(*endcut)->used)
					{
						strncpy(alertstr,&(*begcut)->string[(*begcut)->begcol],(*begcut)->endcol-(*begcut)->begcol);
						alertstr[(*begcut)->endcol-(*begcut)->begcol]=0;
						alertstr[fkeymenu[FKEY1+key].ob_spec.tedinfo->te_txtlen-1]=0;
						form_write(fkeymenu,FKEY1+key,alertstr,0);
					}
					hndl_fkeymenu(fkeymenu,FKEY1+key);
				}
			}
			fkeystat[key]=0;
			ret=1;
		}
		if(kr>=SF1 && kr<=SF10)
		{
			key=(kr)-(SF1);
			sfkeystat[key]=1;
			if((str=expandfkey(wp,form_read(fkeymenu,SFKEY1+key,fstr),0)) != NULL)
			{
				k=(int)strlen(str);
				if(k)
				{
					for(i=0; i<k; i++)
					{
						switch(str[i])
						{
							case '\\':
								switch(str[i+1])
								{
									case 'C': /* Cntrl ? */
									case 'c': /* Cntrl ? */
									case 'A': /* Alt ? */
									case 'a': /* Alt ? */
										newkey(__toupper(str[i+1]),__toupper(str[i+2]),&nstate,&nkey);
										hndl_keybd(nstate,nkey);
										break;
									case 'f':
										if((fkey=newfkey(str[i+2]))>=0)
										{
											if(!fkeystat[fkey])
											{
												fkeystat[fkey]=1;
												fkeys(wp,0,((F1)+fkey),begcut,endcut);
												fkeystat[fkey]=0;
											}
											i+=2;
										}
										else
											editor(wp,0,(int)str[i],begcut,endcut);
										break;
									case 'F':
										if((fkey=newfkey(str[i+2]))>=0)
										{
											if(!sfkeystat[fkey])
											{
												sfkeystat[fkey]=1;
												fkeys(wp,K_LSHIFT,((SF1)+fkey),begcut,endcut);
												sfkeystat[fkey]=0;
											}
											i+=2;
										}
										else
											editor(wp,0,(int)str[i],begcut,endcut);
										break;
									case 't':/* Text hinzuladen */
/* MT 18.6.95 */
										if(!*begcut && !*endcut)
										{
											switch(_read_blk(wp, &str[i+2], begcut, endcut))
											{
												case -1:	/* kein ram frei */
													sprintf(alertstr,Afkeys[1],split_fname(&str[i+2]));
													form_alert(1,alertstr); /* kein break, es geht weiter */
												case 1: /* ok */
													store_undo(wp, &undo, *begcut, *endcut, WINEDIT, EDITCUT);
													wp->w_state|=CHANGED;
													m=(*endcut)->used; /* merken, wird ben”tigt, geht aber verloren */
													graf_mouse_on(0);
													Wcursor(wp);
													if((wp->w_state&COLUMN))
														paste_col(wp,*begcut,*endcut);
													else
														paste_blk(wp,*begcut,*endcut);
													Wcursor(wp);
													graf_mouse_on(1);
													(*endcut)->endcol=m;
													if(!(wp->w_state&COLUMN))
														hndl_blkfind(wp,*begcut,*endcut,SEAREND);
													else
														free_blk(wp,*begcut); /* Spaltenblock freigeben */
													graf_mouse_on(0);
													Wcursor(wp);
													Wredraw(wp,&wp->work);
													Wcursor(wp);
													graf_mouse_on(1);
													break;
												case 0:
													sprintf(alertstr,Afkeys[2],(char *)split_fname(&str[i+2]));
													form_alert(1,alertstr);
													undo.item=0;	/* EDITCUT */
													break;
											}
											*begcut=*endcut=NULL;
										}
										return(1);/* raus, sonst wird auch der Dateiname eingefgt */
										/*break;*/
									default:
										editor(wp,0,(int)str[i],begcut,endcut);
										break;
								}
								break;
							case TAB:
								editor(wp,0,TAB,begcut,endcut);
								break;
							case ESC:
								editor(wp,0,ESC,begcut,endcut);
								break;
							case '\r': /* ^M == CR bercksichtigen */
								editor(wp,0,CR,begcut,endcut);
								break;
							default:
								editor(wp,0,(int)str[i],begcut,endcut);
								break;
						}
					}
				}
				else
				{
					fkeymenu[FKNORM ].ob_flags|= HIDETREE;
					fkeymenu[FKSHIFT].ob_flags&=~HIDETREE;
					if((*begcut) && (*endcut) && (*begcut)==(*endcut) && (*endcut)->begcol<(*endcut)->used)
					{
						strncpy(alertstr,&(*begcut)->string[(*begcut)->begcol],(*begcut)->endcol-(*begcut)->begcol);
						alertstr[(*begcut)->endcol-(*begcut)->begcol]=0;
						alertstr[fkeymenu[SFKEY1+key].ob_spec.tedinfo->te_txtlen-1]=0;
						form_write(fkeymenu,SFKEY1+key,alertstr,0);
					}
					hndl_fkeymenu(fkeymenu,SFKEY1+key);
				}
			}
			sfkeystat[key]=0;
			ret=1;
		}
	}
	return(ret);
}
#pragma warn .par

static void set_3D_flags(OBJECT *tree)
{
	int i;
	
	tree[FKSHIFT].ob_flags |= 0x0400;
	tree[FKNORM ].ob_flags |= 0x0400;
	for(i=0; i<10; i++)
		tree[i+FKEY1].ob_flags |= 0x0400;
	for(i=10; i<20; i++)
		tree[i-10+SFKEY1].ob_flags |= 0x0400;
}

static void _savesoftkeys(char *pathname)
{
	FILE *fp;
	int i;
	
	if((fp=fopen(pathname,"wb"))!=NULL)
	{
      graf_mouse(BUSY_BEE,NULL);
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
		fclose(fp);
      graf_mouse(ARROW,NULL);
	}
}

static void _loadsoftkeys(char *pathname)
{
	FILE *fp;
	int i;
		
	if((fp=fopen(pathname,"rb"))!=NULL)
	{
      graf_mouse(BUSY_BEE,NULL);
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
		fclose(fp);
      graf_mouse(ARROW,NULL);
	}
}

void loadsoftkeys(char *filename)
{
	char pathname[PATH_MAX];
	
   search_env(pathname,filename,0); /* READ */
	_loadsoftkeys(pathname);
}

void hndl_fkeymenu(OBJECT *tree, int start)
{
	int x,y,exit_obj,i,done=0;
	char *cp,keys[20][33];

	char filename[PATH_MAX]="";
	/*static*/ char fpattern[FILENAME_MAX]="*.*";


	if(tree[FKSHIFT].ob_flags & HIDETREE)
	{
		form_write(tree,FKEYSHFT,MIT_SHIFT,0);
		if(!start)
			start=FKEY1;
	}
	else
	{
		form_write(tree,FKEYSHFT,OHNE_SHIFT,0);
		if(!start)
			start=SFKEY1;
	}
	for(i=0; i<10; i++)
		form_read(tree,i+FKEY1,keys[i]);
	for(i=10; i<20; i++)
		form_read(tree,i-10+SFKEY1,keys[i]);
   
   set_3D_flags(tree);
  	
  	form_exopen(tree,0);
	do
	{
		exit_obj=form_exdo(tree,start);
		objc_offset(tree,ROOT,&x,&y);
		switch(exit_obj)
		{
			case FKEYLOAD:
				strcpy(fpattern,"*.SFK");
				find_7upinf(filename,"SFK",1);
				if((cp=strrchr(filename,'\\'))!=NULL || (cp=strrchr(filename,'/'))!=NULL)
					strcpy(&cp[1],fpattern);
				else
					*filename=0;
				if(getfilename(filename,fpattern,"@",fselmsg[9]))
					_loadsoftkeys(filename);
				tree[exit_obj].ob_state&=~SELECTED;
				objc_update(tree,ROOT,MAX_DEPTH); /* Alles zeichnen! */
				break;
			case FKEYSAVE:
				strcpy(fpattern,"*.SFK");
				find_7upinf(filename,"SFK",1);
				if((cp=strrchr(filename,'\\'))!=NULL || (cp=strrchr(filename,'/'))!=NULL)
					strcpy(&cp[1],fpattern);
				else
					*filename=0;
				if(getfilename(filename,fpattern,"@",fselmsg[8]))
					_savesoftkeys(filename);
				tree[exit_obj].ob_state&=~SELECTED;
				if(!windials)
					objc_update(tree,ROOT,MAX_DEPTH);
				else
					objc_update(tree,exit_obj,0);
				break;
			case FKEYSHFT:
				tree[FKSHIFT].ob_flags^=HIDETREE;
				tree[FKNORM ].ob_flags^=HIDETREE;
				tree[exit_obj].ob_state&=~SELECTED;
				if(tree[FKSHIFT].ob_flags & HIDETREE)
				{
					for(i=FKEY1; i<=FKEY10; i++)
						tree[i].ob_flags |= EDITABLE;
					for(i=SFKEY1; i<=SFKEY10; i++)
						tree[i].ob_flags &= ~EDITABLE;
					form_write(tree,FKEYSHFT,MIT_SHIFT,1);
					start=FKEY1;
					objc_update(tree,ROOT/*FKNORM*/,MAX_DEPTH);
				}
				else
				{
					for(i=FKEY1; i<=FKEY10; i++)
						tree[i].ob_flags &= ~EDITABLE;
					for(i=SFKEY1; i<=SFKEY10; i++)
						tree[i].ob_flags |=  EDITABLE;
					form_write(tree,FKEYSHFT,OHNE_SHIFT,1);
					start=SFKEY1;
					objc_update(tree,ROOT/*FKSHIFT*/,MAX_DEPTH);
				}
				break;
			case FKEYHELP:
				form_alert(1,Afkeys[0]);
				objc_change(tree,exit_obj,0,x,y,tree->ob_width,tree->ob_height,tree[exit_obj].ob_state&~SELECTED,1);
				break;
			case FKEYOK:
			case FKEYABBR:
				done=1;
				break;
		}
	}
	while(!done);
	form_exclose(tree,exit_obj,0);
	if(exit_obj==FKEYABBR)
	{
		for(i=0; i<10; i++)
			form_write(tree,i+FKEY1,keys[i],0);
		for(i=10; i<20; i++)
			form_write(tree,i-10+SFKEY1,keys[i],0);
	}
}

