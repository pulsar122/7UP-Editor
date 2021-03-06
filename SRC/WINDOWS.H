/*****************************************************************
	7UP
	Header: WINDOWS.H
*****************************************************************/

#ifndef WINDOWS_H
#define WINDOWS_H

#ifndef _GEMFAST_H
#	ifndef __AES__
#		ifdef TCC_GEM
#			include <aes.h>
#		else
#			include <gem.h>
#		endif
#	endif
#endif

/*****************************************************************
	Makros
*****************************************************************/

#define MAXWINDOWS 8

#define CREATED   0x0001
#define OPENED    0x0002
#define FULLED    0x0004
#define ONTOP     0x0008
#define CURSOR    0x0010
#define CHANGED   0x0020
#define CURSON    0x0040
#define GEMFONTS  0x0080
#define CBLINK    0x0100
#define BLOCKSATZ 0x0200
#define INSERT    0x0400
#define INDENT    0x0800
#define TABCOMP   0x1000
#define PROPFONT  0x2000
#define COLUMN    0x4000
#define ICONIFIED 0x8000

#define STRING_LENGTH 512
#define NBLOCKS        31  /* 31+1=minimaler Block von TC */

#define MAXFONTS   16
#define MAXSIZES   16

/*****************************************************************
	Typen
*****************************************************************/

typedef struct
{
   char name[33];
   int id;
   int attr;
   int size[MAXSIZES+1];
}FONTINFO;

typedef struct line
{
   struct line *prev, *next;
   char *string;
   int used,len;
   int begcol, endcol;
   int attr;
   int effect; /* normal, fett, kursiv */
}LINESTRUCT;

typedef struct window             /* general window information */
{
   int wihandle;                     /* windowhandle     */
   int vdihandle;                    /* VDI Handle       */
   int icon;                         /* Iconnummer       */
   int mentry;                       /* Menueintrag      */
   int kind;                         /* WIKIND           */
   GRECT work;                       /* workspace *//* 1997-04-08 (MJK): GRECT statt x/y/w/hwork */
   char *name,*info;                 /* title and info   */
   unsigned int w_state;             /* window status    */
   int type;                         /* text or graphic  */
   int row,col,cspos;                /* Reihe,Spalte,Sollposition */
   int umbruch;                      /* Zeilenumbruch    */
   long wfirst,hfirst;               /* 1. x,y Position  */
   long wsize,hsize;/* L�nge und Breite dessen, was dargestellt werden soll */
   int  wscroll,hscroll;             /* hor. und vert. Scrollrate */
   int  slwpos,slhpos;               /* Sliderstellung 0-1000 */
   int (*draw)(struct window *wp, int dir, int clip[]);   /* (re)drawfunction */
   long outspec;                     /* anything else -> */
   int fontsize;                     /* fontsize         */
   int fontid;
   int tab;                          /* tabulator        */
   LINESTRUCT *cstr;                 /* Cursor string    */
   LINESTRUCT *wstr;                 /* Window string    */
   LINESTRUCT *fstr;                 /* First string     */
   OBJECT *toolbar;						 			 /* Windowtoolbar           */
   long toolbaraddress;					 		 /* Adresse f�r rsrc_free() */
	 OBJECT *tabbar;					 				 /* Windowtabbar            */
	 int tos_domain;									 /* Datei wurde in der TOS-Domain geladen 2000-10-21 (GS) */
}WINDOW;

/*****************************************************************
	Variablen
*****************************************************************/

extern WINDOW _wind[];

/*****************************************************************
	Prototypen
*****************************************************************/

int align(register int x, register int n);
#ifdef TCC_GEM
int rc_intersect(register GRECT *g1, register GRECT *g2);
#endif

WINDOW *Wcreate(int, int, int, int, int );
WINDOW *Wp(int wh);
WINDOW *Wgettop(void);
WINDOW *Wicon(int icon);
WINDOW *Wgetwp(char *filename);
WINDOW *Wmentry(int mentry);
WINDOW *Wreadtempfile(char *, int);
char *Wname(WINDOW *wp);
int  Wh(WINDOW *wp);
int  Wopen(WINDOW *wp);
void Wattrchg(WINDOW *wp, int newkind);
int  Whndlmsg(int *msgbuf);
int  Whndlkbd(WINDOW *wp,int ,int);
void Wredraw(WINDOW *wp, GRECT *);
void Wtop(WINDOW *wp);
void Wbottom(WINDOW *wp);
void Wfull(WINDOW *wp);
void Wadjust(WINDOW *wp);
void Wcycle(WINDOW *wp);
int  Wcount(int state);
void Wscroll(register WINDOW *wp, int dir, int delta, GRECT *rect);
void Wnewname(WINDOW *wp, const char *);
void Wnewinfo(WINDOW *wp, const char *);
int  Wnewfont(WINDOW *wp, int fontid, int fontsize);
int  Wloadfonts(WINDOW *wp, int maxfonts, int maxsizes, FONTINFO *fip);
void Wunloadfonts(WINDOW *wp);
int  Wfontsize(WINDOW *wp, int fontsize);
int  Wprintf(WINDOW *wp, const char *format, ... );
int  Wscanf(WINDOW *wp, const char *format, ... );
void Wclear(WINDOW *wp);
void Wclearall(WINDOW *wp);
void Wclearhome(WINDOW *wp);
void Wcblink(WINDOW *wp);
void Wcursor(WINDOW *wp);
int  Wmxycursor(WINDOW *wp, int, int);
void _Wcblink(WINDOW *wp, int mx, int my);
void Wcuron(WINDOW *wp);
void Wcuroff(WINDOW *wp);
int Wshiftpage(WINDOW *wp, int len, int used);
void Wcurup(WINDOW *wp);
void Wcurdown(WINDOW *wp);
void Wcurright(WINDOW *wp);
void Wcurleft(WINDOW *wp);
void Wmovesize(WINDOW *wp, GRECT *);
void Wcenter(WINDOW *wp);
void Wslide(WINDOW *wp, int, int );
void _Wscroll(WINDOW *wp, int dir, int delta, GRECT *rect);
void Wmoveblk(WINDOW *wp, int dir, int delta, GRECT *rect);
int  Warrow(WINDOW *wp, int );
void Wslsize(WINDOW *wp, int, int );
void Wslupdate(WINDOW *wp, int what);
void Wclose(WINDOW *wp);
void Wopenall(void);
void Wcloseall(void);
void Wfree(WINDOW *wp);
void Wdefattr(WINDOW *wp);
void Wdelete(WINDOW *wp);
void Wnew(void);
int  Wtxtsize(WINDOW *wp, long *lines, long *chars);
int  Wblksize(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end, long *lines, long *chars);
long Wchksum(WINDOW *wp);
int  Wlpage(WINDOW *wp, int col);
int  Wrpage(WINDOW *wp, int col);
int  Wsetscreen(WINDOW *wp);
int Wendline(WINDOW *wp);
int Wadjpage(WINDOW *wp);
void Warrange(int how);
void Wiconify(WINDOW *wp, int nxywh[]);
void Wuniconify(WINDOW *wp, int nxywh[]);
int Windex(WINDOW *wp);

#endif /* WINDOWS_H */