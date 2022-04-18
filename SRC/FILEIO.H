/*****************************************************************
	7UP
	Header: FILEIO.H

	1997-03-25 (MJK):	Header begonnen
	1997-04-11 (MJK): EingeschrÑnkt auf GEMDOS
*****************************************************************/

#ifndef FILEIO_H
#define FILEIO_H

/*****************************************************************
	Globale Variablen
*****************************************************************/

extern char iostring[],iostr2[];
extern char errorstr[];
extern int  tabexp,
            eszet,
            countdeletedfiles,
            bcancel,
            lineendsign,
            wret,
            WI_KIND;

/*****************************************************************
	Prototypes
*****************************************************************/

#if defined( __TURBOC__ ) && !defined( __MINT__ )
int access(const char *filename, int fattr);
#else
long filelength(int handle);
#endif
void Wfree(WINDOW *wp);
WINDOW *Wnewfile(char *name);
WINDOW *Wreadfile(char *name, int automatic);
WINDOW *Wreadtempfile(char *filename, int mode);

void complete_path(char *path);
void cut_path(char *path);

char *search_env(char *path, char *filename, int write);

int gettrashname(char *pathname, const char *meldung);
int samefile(const char *filename1, const char *filename2);
int getfilename(char *pathname, char *pattern, char *fname, const char *meldung);
char *split_fname(char *pathname);

char *change_ext(char *name, char *ext);
char *change_name(char *name, char *newname);
char *change_linealname(char *name, char *newname);

long strcchg(register char *str, register char old, register char new);

void _write_file(WINDOW *wp, char *filename, char *openmodus, int newname, int ascformat, int signal);
void write_file(WINDOW *wp, int newname);
void write_block(WINDOW *wp, LINESTRUCT *beg, LINESTRUCT *end);
int _read_blk(WINDOW *wp, char *filename, LINESTRUCT **begcut, LINESTRUCT **endcut);

int getbootdev(void);

int create_clip(void);
int scrp_clear(void); /* Clipbrd lîschen, fÅr Atari */
void write_clip(WINDOW *wp, LINESTRUCT *begcut, LINESTRUCT *endcut);
int read_clip(WINDOW *wp, LINESTRUCT **begcut, LINESTRUCT **endcut);

void Gsave(WINDOW *wp); /* GEMINI: nach $TRASHDIR */
void delete_file(void);

int isnumeric(char *num);
char *isinffile(int argc, char *argv[]);

void file_input(int argc, char *argv[]);

void loadfiles(char *TV_path, char *TV_pattern); /* evtl. Treeviewpfade */

#endif /* FILEIO_H */