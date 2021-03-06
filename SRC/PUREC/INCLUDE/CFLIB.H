#ifndef _cflib_h_
#define _cflib_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __MINT__				/* MiNT/GEM-Lib */
#include <osbind.h>
#include <mintbind.h>
#include <gem.h>
#else								/* PureLIB/MT_AES */
#include <tos.h>
#include <aes.h>
#include <vdi.h>
#endif

/*******************************************************************************
 * Version
 *******************************************************************************/

#ifdef __MINT__
#define CFLIB_PATCHLEVEL	"12m"
#else
#define CFLIB_PATCHLEVEL	"12pgs"
#endif
extern char __Ident_cflib[];

/*******************************************************************************
 * spezielle Typen der CF-Lib
 *******************************************************************************/

typedef enum {null, Con, TCon, Datei, Terminal, 
				  Modem1, Modem2, Seriell1, Seriell2, Prn} DEBUGDEV;

typedef struct _popup
{
	OBJECT	*tree;		/* der Objektbaum */
	int		max_item;	/* maximal m�gliche Anzahl */
	int		akt_item;	/* aktuelle Anzahl */
	int		item_len;	/* L�nge eines Eintrages */
} POPUP;

typedef int	 (* KEY_CB)(OBJECT *tree, int edit_obj, int kstate, int *kreturn, int *next_obj);


typedef struct _mdial
{
	OBJECT			*tree;
	int				win_handle;
	char				*win_name;
	int				edit_obj,
						edit_idx,
						next_obj;
	int				save_frame, 
						delta_y;
	int				is_shaded;
	struct _mdial	*next;
} MDIAL;

typedef void (* MDIAL_WCB)(int *msg);


typedef struct _wdial
{
	struct _wdial	*next;

	OBJECT	*tree;						/* Objektbaum */
	OBJECT	*icon;						/* Icon f�r Iconify */
	int		mode;							/* aktueller Status */
	int		win_handle;					/* Fensterhandle */
	char		win_name[80];				/* Fenstertitel */
	int		win_kind;					/* Elemente */
	GRECT		work;							/* Fenstergr��e */
	int		title_obj;					/* Objektnummer des Titelobjektes */
	int		cancel_obj;					/*       "      des Abbruchbuttons */
	int		delta_y;						/* Offset bis zum Titelobjekt */
	int		edit_idx,					/* Objektnummern f�r die Editfelder */
				next_obj,
				edit_obj;

	void		(*open_cb) (struct _wdial *dial);	
	int		(*exit_cb) (struct _wdial *dial, int exit_obj);	
} WDIALOG;

typedef void 	(* WDIAL_OCB)(WDIALOG *dial);
typedef int		(* WDIAL_XCB)(WDIALOG *dial, int exit_obj);

#define WOCB_NULL	(WDIAL_OCB)NULL
#define WXCB_NULL	(WDIAL_XCB)NULL

typedef int (* FSEL_CB)(char *path, char *name);
#define FSCB_NULL	(FSEL_CB)NULL

/*******************************************************************************
 * globals.c
 *******************************************************************************/

extern int		gl_apid, gl_phys_handle;
extern GRECT	gl_desk;	

extern int		sys_big_id, sys_big_height, sys_big_pts,
					sys_sml_id, sys_sml_height, sys_sml_pts;
extern int		sys_wchar, sys_hchar, sys_wbox, sys_hbox;

extern int		gl_wchar, gl_hchar, gl_wbox, gl_hbox;

extern int		gl_gdos;
extern int		gl_planes;

extern int		gl_gem;
extern int		gl_mint;
extern int		gl_naes;
extern int		gl_xaaes;
extern int		gl_magx;
extern int		gl_nvdi;

extern char 	gl_appdir[];

/*******************************************************************************
 * alerts.c
 *******************************************************************************/

extern int do_alert	(int def, int undo, char *str);
extern int do_walert	(int def, int undo, char *str, char *win_title);

/*******************************************************************************
 * app.c
 *******************************************************************************/
 
extern void		init_app(char *rsc);
extern void		exit_gem(void);
extern void		exit_app(int ret);
extern void		hide_mouse(void);
extern int  	hide_mouse_if_needed(GRECT *rect);
extern void		show_mouse(void);
/*
extern int		appl_xgetinfo(int type, int *out1, int *out2, int *out3, int *out4);
*/
/*******************************************************************************
 * asciitab.c
 *******************************************************************************/

extern int 	ascii_table(int id, int pts);
extern void set_asciitable_strings(char *title, char *button);

/*******************************************************************************
 * colorpop.c
 *******************************************************************************/

#define MAX_COLORPOP 10 /* Maximale Anzahl von Farb-Objekten in einem Programm */

extern int	init_colorpop(int maxplanes);
extern void	exit_colorpop(void);

extern void fix_colorpopobj(OBJECT *tree, int obj, int color);
extern void set_popobjcolor(OBJECT *tree, int obj, int color);
extern int  get_popobjcolor(OBJECT *tree, int obj);

extern int  do_colorpop(int x, int y, int item, int planes, int show_noncolor);
extern int  handle_colorpop(OBJECT *dial, int dial_obj, int mode, int planes, int show_noncolor);

/*******************************************************************************
 * comm.c
 *******************************************************************************/

extern void send_m_special(int mode, int app);
extern void send_vastart(int id, char *str);
extern void send_scchanged(void);

/*******************************************************************************
 * cookie.c
 *******************************************************************************/

extern int	getcookie(char *cookie, long *value);

/*******************************************************************************
 * debug.c
 *******************************************************************************/

extern int	gl_debug;

extern void	debug_init(char *prgName, DEBUGDEV dev, char *file);
extern void	debug_exit(void);
extern void	debug(char *FormatString, ...);

/*******************************************************************************
 * dragdrop.c
 *******************************************************************************/

#ifndef DD_OK
#define DD_OK			0
#define DD_NAK			1
#define DD_EXT			2
#define DD_LEN			3
#define DD_TIMEOUT	4000
#define DD_NUMEXTS	8
#define DD_EXTSIZE	32
#define DD_NAMEMAX	128
#define DD_HDRMAX		(8+DD_NAMEMAX)
#endif

extern int		dd_create(int apid, int winid, int msx, int msy, int kstate, char *exts);
extern int		dd_stry(int fd, char *ext, char *name, long size);
extern void		dd_close(int fd);
extern int		dd_open(int ddnam, char *ext);
extern int		dd_rtry(int fd, char *name, char *ext, long *size);
extern int		dd_reply(int fd, int ack);

/*******************************************************************************
 * file.c
 *******************************************************************************/

extern int	file_exists		(char *filename);
extern int	path_exists		(char *pathname);

extern int	get_path			(char *path, char drive);
extern int	set_path			(char *path);

extern void		split_filename	(char *fullname, char *path, char *name);
extern void 	split_extension(char *filename, char *name, char *extension);
extern void 	set_extension	(char *filename, char *new_ext);

extern int 		make_normalpath(char *path);
extern void 	make_shortpath	(char *path, char *shortpath, int maxlen);

#define NO_CASE	0		/* TOS-FS */
#define HALF_CASE	1		/* VFAT/Mac */
#define FULL_CASE	2		/* Minix */
extern int		fs_long_name	(char *filename);
extern int		fs_case_sens	(char *filename);

/*******************************************************************************
 * filesel.c
 *******************************************************************************/

extern int select_file(char *path, char *name, char *mask, char *title, FSEL_CB open_cb);

/*******************************************************************************
 * fontsel.c
 *******************************************************************************/

#define FS_M_XFSL		1		/* Cookie xFSL */
#define FS_M_FPROT	2		/* Fontprotokoll */
#define FS_M_MAGX		4		/* MagiC fnts_* */
#define FS_M_ALL		(FS_M_XFSL|FS_M_FPROT|FS_M_MAGX)

#define FS_F_MONO		8		/* nur monospaced */

extern int do_fontsel(int flags, char *title, int *id, int *pts);

/*******************************************************************************
 * form_do.c
 *******************************************************************************/

extern int	cf_form_do(OBJECT *tree, int *ed_start);
extern int 	simple_dial(OBJECT *tree, int start_edit);
extern KEY_CB set_formdo_keycb(KEY_CB keycb);

/*******************************************************************************
 * magx.c
 *******************************************************************************/
extern int	get_magx_version(void);

#define MX_NOTXOBJ		0		/* kein MagiC-Objekt */
#define MX_UNDERLINE		1		/* String als �berschrift */
#define MX_RADIO			2		/* Radio ohne Shortcut */
#define MX_SCRADIO		3		/* Radio mit Shortcut */
#define MX_CHECK			4		/* Check ohne Shortcut */
#define MX_SCCHECK		5		/* Check mit Shortcut */
#define MX_SCEXIT			6		/* EXIT mit Shortcut */
#define MX_SCSTRING		7		/* String mit Shortcut */
#define MX_GROUPBOX		8		/* Gruppenrahmen */
#define MX_EDIT3D			9		/* 3D-Editfeld */

#define MX_GROUPBOX2		10		/* Gruppenrahmen, kleine Schrift */

#define MX_UNKNOWN		-1		/* Unbekanntes WHITEBACK-Obj */

extern int	get_magx_obj(OBJECT *tree, int obj);
extern int	get_magx_shortcut(OBJECT *tree, int obj, char *c);

/*******************************************************************************
 * mdial.c
 *******************************************************************************/

extern MDIAL	*open_mdial		(OBJECT *tree, int edit_start);
extern void	 	close_mdial		(MDIAL *mdial);
extern int		do_mdial			(MDIAL *mdial);

extern int 		simple_mdial	(OBJECT *tree, int edit_start);

extern void 	set_mdial_wincb(MDIAL_WCB cb);

extern void		redraw_mdobj	(MDIAL *mdial, int obj);
extern void 	change_mdedit	(MDIAL *mdial, int new);

/*******************************************************************************
 * menu.c
 *******************************************************************************/

extern int		create_menu(OBJECT *tree);
extern void 	delete_menu(void);

extern void 	disable_menu(void);
extern void 	enable_menu(void);

extern int	 	is_menu_key(int kreturn, int kstate, int *title, int *item);

/*******************************************************************************
 * misc.c
 *******************************************************************************/

extern long		ts2ol(short i1, short i2);
extern void 	ol2ts(long l, short *i1, short *i2);

extern void 	save_background(GRECT *box, MFDB *buffer);
extern void 	restore_background(GRECT *box, MFDB *buffer);

extern void 	*malloc_global(long size);

extern int		get_patchlev(const char *id_str, char *pl);
 
#ifndef _GEMLIB_H_
extern int 		*grect_to_array(GRECT *g, int *pxy);
extern void 	array_to_grect(int *pxy, GRECT *g);
#endif

/*******************************************************************************
 * nkcc.c
 *******************************************************************************/

   /* NKCC key code flags */
#define NKF_FUNC     0x8000         /* function          */
#define NKF_RESVD    0x4000         /* resvd, ignore it! */
#define NKF_NUM      0x2000         /* numeric pad       */
#define NKF_CAPS     0x1000         /* CapsLock          */
#define NKF_ALT      0x0800         /* Alternate         */
#define NKF_CTRL     0x0400         /* Control           */
#define NKF_SHIFT    0x0300         /* any Shift key     */
#define NKF_LSH      0x0200         /* left Shift key    */
#define NKF_RSH      0x0100         /* right Shift key   */

   /* special key codes for keys performing a function */
#define NK_INVALID   0x00           /* invalid key code  */
#define NK_UP        0x01           /* cursor up         */
#define NK_DOWN      0x02           /* cursor down       */
#define NK_RIGHT     0x03           /* cursor right      */
#define NK_LEFT      0x04           /* cursor left       */
#define NK_M_PGUP    0x05           /* Mac: page up      */
#define NK_M_PGDOWN  0x06           /* Mac: page down    */
#define NK_M_END     0x07           /* Mac: end          */
#define NK_BS        0x08           /* Backspace         */
#define NK_TAB       0x09           /* Tab               */
#define NK_ENTER     0x0a           /* Enter             */
#define NK_INS       0x0b           /* Insert            */
#define NK_CLRHOME   0x0c           /* Clr/Home          */
#define NK_RET       0x0d           /* Return            */
#define NK_HELP      0x0e           /* Help              */
#define NK_UNDO      0x0f           /* Undo              */
#define NK_F1        0x10           /* function key #1   */
#define NK_F2        0x11           /* function key #2   */
#define NK_F3        0x12           /* function key #3   */
#define NK_F4        0x13           /* function key #4   */
#define NK_F5        0x14           /* function key #5   */
#define NK_F6        0x15           /* function key #6   */
#define NK_F7        0x16           /* function key #7   */
#define NK_F8        0x17           /* function key #8   */
#define NK_F9        0x18           /* function key #9   */
#define NK_F10       0x19           /* function key #10  */
#define NK_M_F11     0x1a           /* Mac: func key #11 */
#define NK_ESC       0x1b           /* Esc               */
#define NK_M_F12     0x1c           /* Mac: func key #12 */
#define NK_M_F14     0x1d           /* Mac: func key #14 */
#define NK_RVD1E     0x1e           /* reserved!         */
#define NK_DEL       0x1f           /* Delete            */

extern int					nkc_init		(void);
extern unsigned short	nkc_tos2n	(long toskey);
extern long					nkc_n2tos	(unsigned short nkcode);
extern unsigned char		nkc_toupper	(unsigned char chr);
extern unsigned char		nkc_tolower	(unsigned char chr);

extern unsigned short	gem_to_norm	(int ks, int kr);
extern void 				norm_to_gem	(unsigned int norm, int *ks, int *kr);

extern void 				str_toupper	(char *str);
extern void 				str_tolower	(char *str);

/*******************************************************************************
 * objc.c
 *******************************************************************************/

extern void 	set_obspec	(OBJECT *tree, int obj, long spec);
extern long 	get_obspec	(OBJECT *tree, int obj);
	
extern int 		get_obtype	(OBJECT *tree, int obj, short *ud);
extern void 	get_objframe(OBJECT *tree, int obj, GRECT *r);

extern void 	set_flag		(OBJECT *tree, int obj, int flag, int set);
extern int	 	get_flag		(OBJECT *tree, int obj, int flag);
extern int		find_flag	(OBJECT *tree, int flag);

extern void		set_state	(OBJECT *tree, int obj, int state, int set);
extern int		get_state	(OBJECT *tree, int obj, int state);
extern void		tree_state	(OBJECT *tree, int start_obj, int state, int set);

extern void		set_string	(OBJECT *tree, int obj, char *text);
extern void 	get_string	(OBJECT *tree, int obj, char *text);

extern void 	set_int		(OBJECT *tree, int obj, int i);
extern int  	get_int		(OBJECT *tree, int obj);

extern void 	set_long		(OBJECT *tree, int obj, long l);
extern void 	set_ulong	(OBJECT *tree, int obj, unsigned long l);
extern long 	get_long		(OBJECT *tree, int obj);

extern void 	redraw_obj	(OBJECT *tree, int obj);

/*******************************************************************************
 * popup.c
 *******************************************************************************/
 
extern int	create_popup(POPUP *p, int anz, int maxlen, char *item);
extern int	free_popup(POPUP *p);
extern int	append_popup(POPUP *p, char *item);

extern int	do_popup(POPUP *p, int button);
extern int	cf_menu_popup(MENU *m1, int x, int y, MENU *m2, int button, int offset);

#define POP_OPEN	1
#define POP_CYCLE	2
extern int	handle_popup(OBJECT *dial, int dial_obj, OBJECT *pop, int pop_obj, int mode);

/*******************************************************************************
 * scrap.c
 *******************************************************************************/

extern int	get_scrapdir(char *scrap);
extern void scrap_clear(void);
extern char *scrap_rtxt(char *buf, long *len, long maxlen);
extern void scrap_wtxt(char *buf);

/*******************************************************************************
 * userdef.c
 *******************************************************************************/

extern void fix_dial(OBJECT *tree);
extern void fix_menu(OBJECT *tree);
extern void fix_popup(OBJECT *tree, int thin_line);

/*******************************************************************************
 * vdi.c
 *******************************************************************************/

extern int	open_vwork(int *w_out);
extern void set_clipping(int handle, int x, int y, int w, int h, int on);
extern int 	height2pts(int handle, int f_id, int f_height);

/*******************************************************************************
 * wdial.c
 *******************************************************************************/

/* wd->mode */
#define WD_OPEN   1
#define WD_ICON   2
#define WD_SHADE  4

#define WD_CLOSER	0xFF

extern WDIALOG	*create_wdial(OBJECT *tree, OBJECT *icon, int edit_obj, 
										WDIAL_OCB open_cb, WDIAL_XCB exit_cb);
extern void	delete_wdial(WDIALOG *wd);
extern void	open_wdial(WDIALOG *wd, int pos_x, int pos_y);
extern void	close_wdial(WDIALOG *wd);

extern void	redraw_wdobj(WDIALOG *wd, int obj);
extern void	redraw_wdicon(WDIALOG *wd, int obj);
extern void change_wdedit(WDIALOG *wd, int new);

extern int	message_wdial(int *msg);
extern int	click_wdial(int clicks, int x, int y, int kshift, int mbutton);
extern int	key_wdial(int kreturn, int kstate);

#endif
