/* GEM Resource C Source */

#include <portab.h>
#include <aes.h>
#include "7UP_ENG.H"

#if !defined(WHITEBAK)
#define WHITEBAK    0x0040
#endif
#if !defined(DRAW3D)
#define DRAW3D      0x0080
#endif

#define FLAGS9  0x0200
#define FLAGS10 0x0400
#define FLAGS11 0x0800
#define FLAGS12 0x1000
#define FLAGS13 0x2000
#define FLAGS14 0x4000
#define FLAGS15 0x8000
#define STATE8  0x0100
#define STATE9  0x0200
#define STATE10 0x0400
#define STATE11 0x0800
#define STATE12 0x1000
#define STATE13 0x2000
#define STATE14 0x4000
#define STATE15 0x8000

static TEDINFO rs_tedinfo[] =
{ "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Find/Replace",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "SEARCH FOR:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 12, 1,
  "___________________________________________________________",
  "___________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 60, 60,
  "REPLACE WITH:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 14, 1,
  "___________________________________________________________",
  "___________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 60, 60,
  " Options ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_normal  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Match  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "_Grep  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  " Modus ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "_Search  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_in block  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "From _top  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "_Check case",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "_Whole word  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "RICHTUNG:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Forward  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "_Backward",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Replace  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "_Ask  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "Replace a_ll  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "*",
  "_",
  "x",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 2,
  "?",
  "_",
  "x",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 2,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Goto line",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_____",
  "_____",
  "99999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 6,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Tab width",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "__",
  "__",
  "99",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 3, 3,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "About 7UP",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Info",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "TOS shell",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "COMMAND:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 9, 1,
  "___________________________________________________________",
  "___________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 60, 60,
  "_Start program",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Character set",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 36, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "7UP is since version 2.33 freeware.",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 36, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Font",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 5, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 1, 1,
  "name",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x11A1, 0, -1, 5, 1,
  "Size",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x11A1, 0, -1, 5, 1,
  "__________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "__________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "__________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "__________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "__________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "_____",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "_____",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "_____",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "_____",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "_____",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "___",
  "___ pt",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 7,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Dateiausgabe",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 41, 1,
  "__",
  "Copie: __",
  "XX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 3, 10,
  "____",
  "Page: ____",
  "XXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 5, 11,
  "ESC",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 1,
  " \232berschrift ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "_Radiobutton  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "_Checkbox  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "_OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Under",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "Circle",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Programm starten",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "NONAME.EXE                                      ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 49, 1,
  "Parameter                                       ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 49, 1,
  " Program ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_GEM  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "_DOS  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "_belegt ganzen Speicher  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1,
  " Parameter ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "_keine Parameter  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "_aktuelle Datei  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  " Options ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "vorher Arbeit _sichern  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 25, 1,
  "_zur\201ck nach 7UP  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Softkeys",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "________________________________",
  "\001F1:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F2:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F3:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F4:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F5:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F6:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F7:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F8:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F9:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "________________________________",
  "\001F10:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 38,
  "________________________________",
  "F1:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F2:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F3:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F4:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F5:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F6:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F7:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F8:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F9:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 36,
  "________________________________",
  "F10:________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 37,
  "_load...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "_with \001",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "_save as...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Line lenght",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "___",
  "___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 4,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Page layout",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "rechte Druckseite",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 18, 1,
  "NOLAYOUT.LAY",
  "\0",
  "\0",
  IBM  , 0, TE_RIGHT, 0x1180, 0, -1, 13, 1,
  "HEADER:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 8, 1,
  "______________________________",
  "______________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 31, 31,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "FOOTER:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 8, 1,
  "L   M   R",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "______________________________",
  "______________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 31, 31,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "PAPER SIZE:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 12, 1,
  "DISTANCE:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 10, 1,
  "PRINTER:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 9, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 1, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 1, 1,
  " Page format ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "080",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 1,
  "00",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "072",
  "\0",
  "\0",
  IBM  , 0, TE_RIGHT, 0x1180, 0, -1, 4, 1,
  "04",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "00",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "00",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "04",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "064",
  "\0",
  "\0",
  IBM  , 0, TE_RIGHT, 0x1180, 0, -1, 4, 1,
  "_load...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "_save...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Print",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  " Numbering ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "0000",
  "____",
  "9999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 5, 5,
  "0001",
  "____",
  "9999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 5, 5,
  "9999",
  "____",
  "9999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 5, 5,
  "001",
  "___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 4,
  "512",
  "___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 4,
  "Page number offset",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 19, 1,
  "First page",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 11, 1,
  "Last Page",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 10, 1,
  "First column",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 13, 1,
  "Last column",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 12, 1,
  "Print _linenumbers",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  " Pages ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "_Header and footer on first page",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 33, 1,
  "Alter_nate pages",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "_Cut too long lines",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "Print _all pages",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "Print _pair pages",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  "Print _unpair pages",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "Add formfeed",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "Pause after _each page",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 23, 1,
  "Print _without page layout",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 27, 1,
  " Options ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "Print in _file...  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "Print in _background",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 21, 1,
  "_Delete spoolfile",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  "Copies",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 7, 1,
  "01",
  "__",
  "99",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 3, 3,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Text preview",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "__________________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 51, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Replace",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "REPLACE WITH:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 14, 1,
  "___________________________________________________________",
  "___________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 60, 60,
  "_No",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "File information",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "___________",
  "________.___",
  "fffffffffff",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 13,
  "____________________",
  "____________________",
  "XXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 21, 21,
  "99.99.99",
  "________",
  "XXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 9, 9,
  "99:99:99",
  "________",
  "XXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 9, 9,
  "________",
  "________",
  "XXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 9,
  "_____",
  "_____",
  "XXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 6,
  "_____",
  "_____",
  "XXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 6,
  "_____/___",
  "_________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 10,
  "_",
  "_",
  "X",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 2, 2,
  "________",
  "________",
  "XXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 9,
  "________",
  "________",
  "XXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 9,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Mark set/goto",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "_1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "__________________",
  "__________________",
  "XXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 19, 19,
  "_2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "__________________",
  "__________________",
  "XXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 19, 19,
  "_3",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "__________________",
  "__________________",
  "XXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 19, 19,
  "_4",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "__________________",
  "__________________",
  "XXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 19, 19,
  "_5",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "__________________",
  "__________________",
  "XXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 19, 19,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Miscellaneous",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  " Window ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "Act. path in fileselector",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1,
  "Show _infoline",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "Show _toolbar",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "Show tabulator _bar",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "Scrollbar in _realtime",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 23, 1,
  " Dialogs ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Centered",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Free  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "Under _mouse  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "In _window  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  " File ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "_Expand tabs",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "Atari \236-conversion",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "Convert _Umlaut",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "Smooth CRs  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "_Strip spaces",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "CR-LF   ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "LF   ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "CR   ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "Create backup _file",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "Backup time",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "__",
  "__",
  "99",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 3, 3,
  " Miscellaneous ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "Install _desktop  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "Gemini trashcan",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "_GEM-Clipboard  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "Send VASTART  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "Save filenames",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "Save preferences",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "TOS domain shell",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Check brackets",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  " Typ ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "{ }  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "[ ]  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "( )  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "< >  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "(* *)  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "/* */  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 1, 1,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "__________",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Sort lines",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  " Area ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "____",
  "____",
  "9999",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 5, 5,
  "____",
  "____",
  "9999",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 5, 5,
  "___",
  "___",
  "999",
  IBM  , 0, TE_LEFT , 0x1100, 0, -1, 4, 4,
  " Direction ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "_Increasing  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "_Decreasing  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  " Method ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "_Line  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "_Group  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "_Criterion  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "_Extra help",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  " Options ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "Check _case  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Search in files",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "SEARCH FOR:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 12, 1,
  "___________________________________________________________",
  "___________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 60, 60,
  " Options ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Grep  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "List _all occurrences  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 24, 1,
  "Incl. _folders  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "in _selected files  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 21, 1,
  "SEARCHING IN:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 14, 1,
  "C:\134ORDNER\134ORDNER\134DATNAME.EXT",
  "____________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 29, 29,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Setup",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  " Notation ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "_German  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_English  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "_Separators  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  " Tax ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "_Normal  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "15,00",
  "_____",
  "XXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 6,
  "_Reduced  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  " 7,00",
  "_____",
  "XXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 6,
  "2",
  "_",
  "9",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 2,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Picklist",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "__________________________________________",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 43, 1,
  "________________________________________",
  "________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 41, 41,
  "_active  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Clean list  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "_Delete",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Shortcuts",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  " Menue ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "___________________________",
  "___________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 28, 28,
  "___",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "KEINNAME.MNU",
  "\0",
  "\0",
  IBM  , 0, TE_RIGHT, 0x1180, 0, -1, 13, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "_save...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "_load...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  " Keyboard ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "a_ctive  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "KEINNAME.KBD",
  "\0",
  "\0",
  IBM  , 0, TE_RIGHT, 0x1100, 0, -1, 13, 1,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "_save...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "_load...",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Register 7UP",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "_____________________________",
  "_____________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 30, 30,
  "__________",
  "__________",
  "9999999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 11,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "\0",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 2, 1, 1,
  "Format",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  " Justify ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_Left  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "_Centered  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "_Right",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "_Block  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  " Bereich ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "_From cursor  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "_Paragraph  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "_Selected block  ",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  "___",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "Help",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Cancel",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "OK",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "1234567890123456789012345",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1,
  "1234567890123456789012345",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1,
  "1234567890123456789012345",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1,
  "1234567890123456789012345",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1,
  "1234567890123456789012345",
  "\0",
  "\0",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 26, 1
};

static WORD RSIB0MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB0DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB1MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB1DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB2MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB2DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB3MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB3DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB4MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB4DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB5MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB5DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB6MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFDC, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0, 
  0x1FFF, 0xFFC0, 0x1FFF, 0xFFC0
};

static WORD RSIB6DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x1FFF, 0xFFC0, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x13DE, 0x7A40, 0x1000, 0x0040, 
  0x12FB, 0x9E40, 0x1000, 0x0040, 
  0x13DD, 0xE640, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1000, 0xFC40, 
  0x13D9, 0x0240, 0x1002, 0x0140, 
  0x1334, 0xC0C0, 0x1007, 0x2040, 
  0x1379, 0xFF3C, 0x1008, 0xC01C, 
  0x1004, 0x001C, 0x1002, 0x001C, 
  0x1001, 0x801C, 0x1000, 0x7F9C, 
  0x1000, 0x007C, 0x1000, 0x005C, 
  0x1000, 0x0040, 0x1000, 0x0040, 
  0x1000, 0x0040, 0x1FFF, 0xFFC0
};

static WORD RSIB7MASK[] =
{ 0x0003, 0xE000, 0x007E, 0x3F00, 
  0x01FF, 0xFFC0, 0x03FF, 0xFFE0, 
  0x03FF, 0xFFE0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x01FF, 0xFFC0, 0x01FF, 0xFFC0, 
  0x00FF, 0xFF80, 0x003F, 0xFE00, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSIB7DATA[] =
{ 0x0003, 0xE000, 0x007E, 0x3F00, 
  0x01C6, 0x31C0, 0x0200, 0x0020, 
  0x03C0, 0x01E0, 0x017F, 0xFF40, 
  0x0100, 0x0040, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0144, 0x8A40, 0x0144, 0xDA40, 
  0x0144, 0x7240, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0144, 0x2240, 0x0144, 0x2240, 
  0x0164, 0x2640, 0x0186, 0x60C0, 
  0x00E0, 0x0380, 0x003F, 0xFE00, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSIB8MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0001, 0xFFFF, 
  0xFC00, 0x0003, 0xFFFF, 0xFC00, 
  0x0007, 0xFFFF, 0xFC00, 0x000F, 
  0xFFFF, 0xFC00, 0x001F, 0xFFFF, 
  0xFC00, 0x003F, 0xFFFF, 0xFC00, 
  0x003F, 0xFFFF, 0xFC00, 0x003F, 
  0xFFFF, 0xFC00, 0x003F, 0xFFFF, 
  0xFC00, 0x003F, 0xFFFF, 0xFC00, 
  0x003F, 0xFFFF, 0xFC00, 0x003F, 
  0xFFFF, 0xFC00, 0x003F, 0xFFFF, 
  0xFC00, 0x003F, 0xFFFF, 0xFC00, 
  0x003F, 0xFFFF, 0xFC00, 0x03FF, 
  0xFFFF, 0xFFC0, 0x07FF, 0xFFFF, 
  0xFFE0, 0x1FFF, 0xFFFF, 0xFFF8, 
  0x1FFF, 0xFFFF, 0xFFF8, 0x3FFF, 
  0xFFFF, 0xFFFC, 0x3FFF, 0xFFFF, 
  0xFFFC, 0x3FFF, 0xFFFF, 0xFFFC, 
  0x3FFF, 0xFFFF, 0xFFFC, 0x3FFF, 
  0xFFFF, 0xFFFC, 0x3FFF, 0xFFFF, 
  0xFFFC, 0x3FFF, 0xFFFF, 0xFFF8, 
  0x1FFF, 0xFFFF, 0xFFF8, 0x0780, 
  0x0000, 0x01E0
};

static WORD RSIB8DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0001, 0xFFFF, 
  0xFC00, 0x0003, 0x0000, 0x1400, 
  0x0005, 0xEBB7, 0x0400, 0x0009, 
  0x0000, 0x1400, 0x0011, 0xBB7A, 
  0x0400, 0x003F, 0x0000, 0x1400, 
  0x0023, 0x5E80, 0x0400, 0x0028, 
  0x0000, 0x1400, 0x0023, 0x95A0, 
  0x0400, 0x0028, 0x0000, 0x1400, 
  0x0022, 0xE000, 0x0400, 0x0028, 
  0x0000, 0x1400, 0x0023, 0x75E0, 
  0x0400, 0x0028, 0x0000, 0x1400, 
  0x0020, 0x0000, 0x0400, 0x03FF, 
  0xFFFF, 0xFFC0, 0x0400, 0x0000, 
  0x0020, 0x1FFF, 0xFFFF, 0xFFF8, 
  0x1AAA, 0xAAAA, 0xAAA8, 0x3FFF, 
  0xFD55, 0x5554, 0x2800, 0x0AAA, 
  0xAAAC, 0x39D5, 0xCD55, 0x5554, 
  0x29DD, 0xCAAA, 0xAAAC, 0x3800, 
  0x0D55, 0x5554, 0x2FFF, 0xFAAA, 
  0xAAAC, 0x3555, 0x5555, 0x5558, 
  0x1FFF, 0xFFFF, 0xFFF8, 0x0780, 
  0x0000, 0x01E0
};

static WORD RSIB9MASK[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x1FFF, 0xFFF8, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x3FFF, 0xFFFC, 
  0x3FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x0FFF, 0xFFFC, 0x07FF, 0xFFF8
};

static WORD RSIB9DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0FFF, 0xFFF0, 
  0x1FFF, 0xFFF8, 0x1C00, 0x0018, 
  0x1C00, 0x0018, 0x1C00, 0x0018, 
  0x1CFF, 0xFF98, 0x1C00, 0x0018, 
  0x1C00, 0x0018, 0x1C00, 0x0018, 
  0x1CFF, 0xFF98, 0x1C00, 0x0018, 
  0x1C00, 0x0018, 0x1C00, 0x0018, 
  0x1CFF, 0xFF98, 0x1C00, 0x0018, 
  0x1C00, 0x0018, 0x1E00, 0x0038, 
  0x1FFF, 0xFFF8, 0x1FE0, 0x07F8, 
  0x1FC6, 0x03F8, 0x1FC6, 0x03F8, 
  0x1FC6, 0x03F8, 0x1FC6, 0x03F8, 
  0x0FC6, 0x03F8, 0x07C6, 0x03F8, 
  0x03C0, 0x03F0, 0x0000, 0x0000
};

static WORD RSIB10MASK[] =
{ 0x0007, 0xE000, 0x000F, 0xF000, 
  0x03FF, 0xFFC0, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

static WORD RSIB10DATA[] =
{ 0x0007, 0xE000, 0x000C, 0x3000, 
  0x03F9, 0x9FC0, 0xFE00, 0x007F, 
  0x8600, 0x0061, 0xBC00, 0x003D, 
  0xAFFF, 0xFFF5, 0xA000, 0x0005, 
  0xAFBE, 0xC005, 0xA000, 0x0005, 
  0xADD7, 0x0005, 0xA000, 0x0005, 
  0xAF00, 0x0005, 0xA000, 0x0005, 
  0xAC00, 0x0005, 0xA000, 0x0005, 
  0xAE80, 0x0005, 0xA000, 0x0005, 
  0xA000, 0x0005, 0xA000, 0x0005, 
  0xA000, 0x0005, 0xA000, 0x0005, 
  0xA000, 0x0005, 0xA000, 0x007D, 
  0xA000, 0x0049, 0xA000, 0x0051, 
  0xA000, 0x0061, 0xBFFF, 0xFFC1, 
  0x8000, 0x0001, 0xFFFF, 0xFFFF
};

static WORD RSIB11MASK[] =
{ 0x0007, 0xE000, 0x000F, 0xF000, 
  0x03FF, 0xFFC0, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

static WORD RSIB11DATA[] =
{ 0x0007, 0xE000, 0x000C, 0x3000, 
  0x03F9, 0x9FC0, 0xFE00, 0x007F, 
  0x8600, 0x0061, 0x8C00, 0x0031, 
  0x8FFF, 0xFFF1, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0x8000, 0x0001, 
  0x8000, 0x0001, 0xFFFF, 0xFFFF
};

static WORD RSIB12MASK[] =
{ 0x0000, 0x0000, 0x0007, 0xF000, 
  0x00FF, 0xFF80, 0x01FF, 0xFFC0, 
  0x03FF, 0xFFE0, 0x03FF, 0xFFE0, 
  0x03FF, 0xFFE0, 0x01FF, 0xFFC0, 
  0x03FF, 0xFFE0, 0x03FF, 0xFFE0, 
  0x07FF, 0xFFF0, 0x07FF, 0xFFF0, 
  0x0FFF, 0xFFF8, 0x0FFF, 0xFFF8, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x1FFF, 0xFFFC, 
  0x1FFF, 0xFFFC, 0x0FFF, 0xFFF8, 
  0x0FFF, 0xFFF8, 0x07FF, 0xFFF0, 
  0x07FF, 0xFFF0, 0x03FF, 0xFFF0, 
  0x03FF, 0xFFE0, 0x01FF, 0xFFC0, 
  0x00FF, 0xFF80, 0x003F, 0xFE00, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSIB12DATA[] =
{ 0x0000, 0x0000, 0x0007, 0xF000, 
  0x00FE, 0xBF80, 0x01C6, 0x31C0, 
  0x0200, 0x0020, 0x03C0, 0x01E0, 
  0x037F, 0xFF60, 0x0100, 0x0040, 
  0x0344, 0x2260, 0x0244, 0x2220, 
  0x0644, 0x2230, 0x0484, 0x2110, 
  0x0C88, 0x2118, 0x0888, 0x8908, 
  0x1988, 0xD98C, 0x1108, 0x7084, 
  0x1108, 0x2084, 0x1108, 0x2084, 
  0x1108, 0x2084, 0x1108, 0x2084, 
  0x1988, 0x208C, 0x0888, 0x2188, 
  0x0C88, 0x2118, 0x0484, 0x2310, 
  0x0644, 0x2210, 0x0244, 0x2230, 
  0x0324, 0x2660, 0x0186, 0x60C0, 
  0x00E0, 0x0380, 0x003F, 0xFE00, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static ICONBLK rs_iconblk[] =
{ RSIB0MASK, RSIB0DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB1MASK, RSIB1DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB2MASK, RSIB2DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB3MASK, RSIB3DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB4MASK, RSIB4DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB5MASK, RSIB5DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB6MASK, RSIB6DATA, "FILENAME.DCA", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  32,  72,   8,
  RSIB7MASK, RSIB7DATA, "Trash\000\000\000\000\000\000\000", 0x1000|'\0',
    0,   0,  20,   0,  32,  32,   0,  32,  72,   8,
  RSIB8MASK, RSIB8DATA, "Printer\000\000\000\000\000", 0x1000|'\0',
    0,   0,  12,   0,  48,  30,   0,  30,  72,   8,
  RSIB9MASK, RSIB9DATA, "Disk\000\000\000\000\000\000\000\000", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  30,  72,   8,
  RSIB10MASK, RSIB10DATA, "Clipboard\000\000\000", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  30,  72,   8,
  RSIB11MASK, RSIB11DATA, "Clipboard\000\000\000", 0x1000|'\0',
    0,   0,  20,   0,  32,  30,   0,  30,  72,   8,
  RSIB12MASK, RSIB12DATA, "Trash\000\000\000\000\000\000\000", 0x1000|'\0',
    8,  13,  20,   0,  32,  32,   0,  32,  72,   8
};

static WORD RSBB0DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x03FF, 0xE000, 0x0000, 0x0003, 
  0xE000, 0x03E0, 0x03FF, 0xC000, 
  0x0000, 0x0007, 0xF000, 0x07F0, 
  0x07FF, 0xC000, 0x0000, 0x0018, 
  0x7800, 0x0878, 0x0401, 0x8000, 
  0x0000, 0x0010, 0x3C00, 0x1038, 
  0x0801, 0x0000, 0x0000, 0x0020, 
  0x1C00, 0x0038, 0x0003, 0x0000, 
  0x0000, 0x0000, 0x1C00, 0x0030, 
  0x0006, 0x0000, 0x0000, 0x0000, 
  0x1C00, 0x0070, 0x0006, 0x0E06, 
  0x0678, 0x0000, 0x1800, 0x00C0, 
  0x000C, 0x7E0E, 0x3EFC, 0x0000, 
  0x3800, 0x0300, 0x0008, 0x1C0C, 
  0x1D1E, 0x0000, 0x3000, 0x1F80, 
  0x0018, 0x0C0C, 0x0E0E, 0x0000, 
  0x6000, 0x03C0, 0x0030, 0x1C1C, 
  0x1C0E, 0x0000, 0xC000, 0x01E0, 
  0x0030, 0x1C1C, 0x1C0E, 0x0001, 
  0x8000, 0x01E0, 0x0060, 0x1838, 
  0x180E, 0x0003, 0x0000, 0x00E0, 
  0x00C0, 0x3838, 0x381C, 0x0004, 
  0x0000, 0x00E0, 0x00C0, 0x3858, 
  0x381C, 0x0008, 0x0000, 0x00C0, 
  0x0180, 0x30B8, 0x3038, 0x0010, 
  0x0800, 0x00C0, 0x0180, 0x7132, 
  0x3030, 0x0020, 0x1080, 0x0180, 
  0x0300, 0x7232, 0x7060, 0x007F, 
  0xF1C1, 0x8180, 0x0600, 0x7C3C, 
  0x71C0, 0x00FF, 0xF1C1, 0xE200, 
  0x0600, 0x3838, 0x7F00, 0x01FF, 
  0xE080, 0xFC00, 0x0000, 0x0000, 
  0x6000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0xE000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0xC000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0xC000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0xC000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0001, 0xC000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0007, 
  0xF000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSBB1DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x7FFF, 
  0xFFFF, 0xFFFE, 0x7FFF, 0xFFFF, 
  0xFFFE, 0x7FFF, 0xFFFF, 0xFFFE, 
  0x7FFF, 0xFFFF, 0xFFFE, 0x0078, 
  0x0000, 0x0000, 0x0078, 0x0000, 
  0x0000, 0x0078, 0x0000, 0x0000, 
  0x0078, 0x0000, 0x0000, 0x0078, 
  0x7FFF, 0xFFFE, 0x0078, 0x7FFF, 
  0xFFFE, 0x0078, 0x7FFF, 0xFFFE, 
  0x0078, 0x7FFF, 0xFFFE, 0x7F78, 
  0x7800, 0x0000, 0x4178, 0x7800, 
  0x0000, 0x4178, 0x7800, 0x0000, 
  0x7F78, 0x7800, 0x0000, 0x0078, 
  0x7FFF, 0xFFFE, 0x7F78, 0x7FFF, 
  0xFFFE, 0x4878, 0x7FFF, 0xFFFE, 
  0x4878, 0x7FFF, 0xFFFE, 0x7878, 
  0x0000, 0x001E, 0x0078, 0x0000, 
  0x001E, 0x7F78, 0x0000, 0x001E, 
  0x4878, 0x0000, 0x001E, 0x4878, 
  0x7FFF, 0xFFFE, 0x7878, 0x7FFF, 
  0xFFFE, 0x0078, 0x7FFF, 0xFFFE, 
  0x7F78, 0x7FFF, 0xFFFE, 0x0000, 
  0x0000, 0x0000
};

static WORD RSBB2DATA[] =
{ 0x0FF0, 0x381C, 0x6006, 0x4002, 
  0x6006, 0x381C, 0x0FF0, 0x0000
};

static WORD RSBB3DATA[] =
{ 0x0FF0, 0x381C, 0x63C6, 0x4FF2, 
  0x63C6, 0x381C, 0x0FF0, 0x0000
};

static WORD RSBB4DATA[] =
{ 0x0000, 0x07E0, 0x1818, 0x2004, 
  0x2004, 0x4002, 0x4002, 0x4002, 
  0x4002, 0x4002, 0x4002, 0x2004, 
  0x2004, 0x1818, 0x07E0, 0x0000
};

static WORD RSBB5DATA[] =
{ 0x0000, 0x07E0, 0x1818, 0x2004, 
  0x23C4, 0x47E2, 0x4FF2, 0x4FF2, 
  0x4FF2, 0x4FF2, 0x47E2, 0x23C4, 
  0x2004, 0x1818, 0x07E0, 0x0000
};

static WORD RSBB6DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x001F, 0xF800, 0x0060, 0x0600, 
  0x0180, 0x0180, 0x0200, 0x0040, 
  0x0400, 0x0020, 0x0800, 0x0010, 
  0x0800, 0x0010, 0x1000, 0x0008, 
  0x1000, 0x0008, 0x2000, 0x0004, 
  0x2000, 0x0004, 0x2000, 0x0004, 
  0x2000, 0x0004, 0x2000, 0x0004, 
  0x2000, 0x0004, 0x2000, 0x0004, 
  0x2000, 0x0004, 0x2000, 0x0004, 
  0x2000, 0x0004, 0x1000, 0x0008, 
  0x1000, 0x0008, 0x0800, 0x0010, 
  0x0800, 0x0010, 0x0400, 0x0020, 
  0x0200, 0x0040, 0x0180, 0x0180, 
  0x0060, 0x0600, 0x001F, 0xF800, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSBB7DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x001F, 0xF800, 0x0060, 0x0600, 
  0x0180, 0x0180, 0x0200, 0x0040, 
  0x0400, 0x0020, 0x0800, 0x0010, 
  0x0807, 0xE010, 0x101F, 0xF808, 
  0x103F, 0xFC08, 0x207F, 0xFE04, 
  0x207F, 0xFE04, 0x20FF, 0xFF04, 
  0x20FF, 0xFF04, 0x20FF, 0xFF04, 
  0x20FF, 0xFF04, 0x20FF, 0xFF04, 
  0x20FF, 0xFF04, 0x207F, 0xFE04, 
  0x207F, 0xFE04, 0x103F, 0xFC08, 
  0x101F, 0xF808, 0x0807, 0xE010, 
  0x0800, 0x0010, 0x0400, 0x0020, 
  0x0200, 0x0040, 0x0180, 0x0180, 
  0x0060, 0x0600, 0x001F, 0xF800, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSBB8DATA[] =
{ 0x47F0, 0x7C1E, 0x7807, 0x7C00, 
  0x003E, 0xE01E, 0x783E, 0x0FE2
};

static WORD RSBB9DATA[] =
{ 0x0000, 0x03C0, 0x2FF0, 0x3C38, 
  0x380C, 0x3C0C, 0x0006, 0x6006, 
  0x6006, 0x6000, 0x303C, 0x301C, 
  0x1C3C, 0x0FF4, 0x03C0, 0x0000
};

static WORD RSBB10DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x001F, 0xF800, 0x007F, 0xFE00, 
  0x01E0, 0x0780, 0x0380, 0x01C0, 
  0x0600, 0x0060, 0x0C00, 0x0030, 
  0x2C00, 0x0030, 0x3800, 0x0018, 
  0x3800, 0x0018, 0x3C00, 0x000C, 
  0x3E00, 0x000C, 0x3F00, 0x000C, 
  0x0000, 0x000C, 0x0000, 0x000C, 
  0x3000, 0x0000, 0x3000, 0x0000, 
  0x3000, 0x00FC, 0x3000, 0x007C, 
  0x3000, 0x003C, 0x1800, 0x001C, 
  0x1800, 0x001C, 0x0C00, 0x0034, 
  0x0C00, 0x0030, 0x0600, 0x0060, 
  0x0380, 0x01C0, 0x01E0, 0x0780, 
  0x007F, 0xFE00, 0x001F, 0xF800, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSBB11DATA[] =
{ 0xFFFF, 0x8001, 0x8001, 0x8001, 
  0xBFE1, 0xA021, 0xA555, 0xA021, 
  0xBFE5, 0x8001, 0x8555, 0x8001, 
  0x8001, 0x8001, 0x8001, 0xFFFF
};

static WORD RSBB12DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0003, 0xFFFE, 
  0x0000, 0x0000, 0x0000, 0x0003, 
  0xFFFC, 0x0000, 0x0000, 0x0000, 
  0x0007, 0xFFFC, 0x0000, 0x0000, 
  0x0000, 0x0007, 0xFFF8, 0x0000, 
  0x0000, 0x0000, 0x000C, 0x0030, 
  0x0000, 0x0000, 0x0000, 0x0008, 
  0x0030, 0x0000, 0x0000, 0x0000, 
  0x0010, 0x0060, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x00E0, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x00C0, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x01C0, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0180, 0xF00E, 0x0187, 
  0x8000, 0x0000, 0x0307, 0xF01E, 
  0x1F9F, 0xE000, 0x0000, 0x0701, 
  0xE01C, 0x07A1, 0xE000, 0x0000, 
  0x0600, 0xE01C, 0x0740, 0xF000, 
  0x0000, 0x0E01, 0xE03C, 0x0780, 
  0xF000, 0x0000, 0x0C01, 0xE03C, 
  0x0700, 0xF000, 0x0000, 0x1801, 
  0xC038, 0x0700, 0xF000, 0x0000, 
  0x3803, 0xC078, 0x0F00, 0xF000, 
  0x0000, 0x3003, 0xC078, 0x0E01, 
  0xE000, 0x0000, 0x7003, 0x80F0, 
  0x0E01, 0xE000, 0x0000, 0x6007, 
  0x81F0, 0x1E01, 0xC000, 0x0000, 
  0xE007, 0x8170, 0x1C03, 0xC000, 
  0x0001, 0xC007, 0x82F0, 0x1C03, 
  0x8000, 0x0001, 0x800F, 0x04E1, 
  0x1C07, 0x0000, 0x0003, 0x800F, 
  0x08E2, 0x3C0E, 0x0000, 0x0003, 
  0x000F, 0x10E4, 0x3C1C, 0x0000, 
  0x0007, 0x000F, 0xE0F8, 0x3C38, 
  0x0000, 0x000E, 0x0007, 0x80F0, 
  0x3FE0, 0x0000, 0x000E, 0x0000, 
  0x0000, 0x7800, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x7000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x7000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x7000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0xF000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0xE000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0xE000, 
  0x0000, 0x0000, 0x0000, 0x0003, 
  0xF000, 0x0000, 0x0000, 0x0000, 
  0x000F, 0xFC00, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static WORD RSBB13DATA[] =
{ 0x0000, 0x07E0, 0x1818, 0x200C, 
  0x2004, 0x4006, 0x4006, 0x4006, 
  0x4006, 0x4006, 0x4006, 0x200C, 
  0x301C, 0x1FF8, 0x07E0, 0x0000
};

static WORD RSBB14DATA[] =
{ 0x0000, 0x07E0, 0x1FF8, 0x380C, 
  0x3004, 0x63C2, 0x67E2, 0x67E2, 
  0x67E2, 0x67E2, 0x63C2, 0x2004, 
  0x3004, 0x1818, 0x07E0, 0x0000
};

static WORD RSBB15DATA[] =
{ 0x0000, 0x0140, 0x2AA0, 0x1410, 
  0x2808, 0x1404, 0x0002, 0x4004, 
  0x2002, 0x4000, 0x2028, 0x1014, 
  0x0828, 0x0554, 0x0280, 0x0000
};

static WORD RSBB16DATA[] =
{ 0x0000, 0x07E0, 0x1FF8, 0x381C, 
  0x300C, 0x6006, 0x6006, 0x6006, 
  0x6006, 0x6006, 0x6006, 0x300C, 
  0x381C, 0x1FF8, 0x07E0, 0x0000
};

static WORD RSBB17DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0FFF, 0xF800, 0x0000, 0x0000, 
  0x0FFF, 0xF000, 0x0000, 0x0000, 
  0x1FFF, 0xF000, 0x0000, 0x0000, 
  0x1FFF, 0xE000, 0x0000, 0x0000, 
  0x3000, 0xC000, 0x0000, 0x0000, 
  0x2000, 0xC000, 0x0000, 0x0000, 
  0x4001, 0x8000, 0x0000, 0x0000, 
  0x0003, 0x8000, 0x0000, 0x0000, 
  0x0003, 0x0000, 0x0000, 0x0000, 
  0x0007, 0x0000, 0x0000, 0x0000, 
  0x0006, 0x03C0, 0x3806, 0x1E00, 
  0x000C, 0x1FC0, 0x787E, 0x7F80, 
  0x001C, 0x0780, 0x701E, 0x8780, 
  0x0018, 0x0380, 0x701D, 0x03C0, 
  0x0038, 0x0780, 0xF01E, 0x03C0, 
  0x0030, 0x0780, 0xF01C, 0x03C0, 
  0x0060, 0x0700, 0xE01C, 0x03C0, 
  0x00E0, 0x0F01, 0xE03C, 0x03C0, 
  0x00C0, 0x0F01, 0xE038, 0x0780, 
  0x01C0, 0x0E03, 0xC038, 0x0780, 
  0x0180, 0x1E07, 0xC078, 0x0700, 
  0x0380, 0x1E05, 0xC070, 0x0F00, 
  0x0700, 0x1E0B, 0xC070, 0x0E00, 
  0x0600, 0x3C13, 0x8470, 0x1C00, 
  0x0E00, 0x3C23, 0x88F0, 0x3800, 
  0x0C00, 0x3C43, 0x90F0, 0x7000, 
  0x1C00, 0x3F83, 0xE0F0, 0xE000, 
  0x3800, 0x1E03, 0xC0FF, 0x8000, 
  0x3800, 0x0000, 0x01E0, 0x0000, 
  0x0000, 0x0000, 0x01C0, 0x0000, 
  0x0000, 0x0000, 0x01C0, 0x0000, 
  0x0000, 0x0000, 0x01C0, 0x0000, 
  0x0000, 0x0000, 0x03C0, 0x0000, 
  0x0000, 0x0000, 0x0380, 0x0000, 
  0x0000, 0x0000, 0x0380, 0x0000, 
  0x0000, 0x0000, 0x0FC0, 0x0000, 
  0x0000, 0x0000, 0x3FF0, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

static BITBLK rs_bitblk[] =
{ RSBB0DATA,  12,  48,   0,   0, 0x0001,
  RSBB1DATA,   6,  30,   0,   0, 0x0001,
  RSBB2DATA,   2,   8,   0,   0, 0x0001,
  RSBB3DATA,   2,   8,   0,   0, 0x0001,
  RSBB4DATA,   2,  16,   0,   0, 0x0001,
  RSBB5DATA,   2,  16,   0,   0, 0x0001,
  RSBB6DATA,   4,  32,   0,   0, 0x0001,
  RSBB7DATA,   4,  32,   0,   0, 0x0001,
  RSBB8DATA,   2,   8,   0,   0, 0x0001,
  RSBB9DATA,   2,  16,   0,   0, 0x0001,
  RSBB10DATA,   4,  32,   0,   0, 0x0001,
  RSBB11DATA,   2,  16,   0,   0, 0x0001,
  RSBB12DATA,  10,  72,   0,   0, 0x0001,
  RSBB13DATA,   2,  16,   0,   0, 0x0001,
  RSBB14DATA,   2,  16,   0,   0, 0x0001,
  RSBB15DATA,   2,  16,   0,   0, 0x0001,
  RSBB16DATA,   2,  16,   0,   0, 0x0000,
  RSBB17DATA,   8,  42,   0,   0, 0x0001
};

static OBJECT rs_object[] =
{ 
  /******** Tree 0 WINMENU ****************************************************/
        -1,        1,       12, G_IBOX            ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0000, 0x0050, 0x0019,
        12,        2,        2, G_BOX             ,   /* Object 1  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0000, 0x0000, 0x0050, 0x0201,
         1,        3, WINOPT          , G_IBOX            ,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0002, 0x0000, 0x003D, 0x0301,
  WINFILE         ,       -1,       -1, G_TITLE           ,   /* Object 3  */
  NONE, NORMAL, (LONG)" 7UP",
  0x0000, 0x0000, 0x0005, 0x0301,
  WINEDIT         ,       -1,       -1, G_TITLE           ,   /* Object 4 WINFILE */
  NONE, NORMAL, (LONG)" File",
  0x0005, 0x0000, 0x0006, 0x0301,
  WINSEAR         ,       -1,       -1, G_TITLE           ,   /* Object 5 WINEDIT */
  NONE, NORMAL, (LONG)" Edit",
  0x000B, 0x0000, 0x0006, 0x0301,
  WINBLK          ,       -1,       -1, G_TITLE           ,   /* Object 6 WINSEAR */
  NONE, NORMAL, (LONG)" Search",
  0x0011, 0x0000, 0x0008, 0x0301,
  WINFORM         ,       -1,       -1, G_TITLE           ,   /* Object 7 WINBLK */
  NONE, NORMAL, (LONG)" Calc",
  0x0019, 0x0000, 0x0006, 0x0301,
  WINMAC          ,       -1,       -1, G_TITLE           ,   /* Object 8 WINFORM */
  NONE, NORMAL, (LONG)" Mode",
  0x001F, 0x0000, 0x0006, 0x0301,
  WINWIN          ,       -1,       -1, G_TITLE           ,   /* Object 9 WINMAC */
  NONE, NORMAL, (LONG)" Macro",
  0x0025, 0x0000, 0x0007, 0x0301,
  WINOPT          ,       -1,       -1, G_TITLE           ,   /* Object 10 WINWIN */
  NONE, NORMAL, (LONG)" Window",
  0x002C, 0x0000, 0x0008, 0x0301,
         2,       -1,       -1, G_TITLE           ,   /* Object 11 WINOPT */
  NONE, NORMAL, (LONG)" Options",
  0x0034, 0x0000, 0x0009, 0x0301,
         0,       13,      117, G_IBOX            ,   /* Object 12  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0301, 0x0050, 0x0017,
        22, WINABOUT        ,       21, G_BOX             ,   /* Object 13  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0002, 0x0000, 0x0014, 0x0008,
        15,       -1,       -1, G_STRING          ,   /* Object 14 WINABOUT */
  NONE, NORMAL, (LONG)"  About 7UP...   \007A ",
  0x0000, 0x0000, 0x0014, 0x0001,
        16,       -1,       -1, G_STRING          |0x0700,   /* Object 15  */
  NONE, DISABLED, (LONG)"--------------------",
  0x0000, 0x0001, 0x0014, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"  Desk Accessory 1  ",
  0x0000, 0x0002, 0x0014, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"  Desk Accessory 2  ",
  0x0000, 0x0003, 0x0014, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"  Desk Accessory 3  ",
  0x0000, 0x0004, 0x0014, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 19  */
  NONE, NORMAL, (LONG)"  Desk Accessory 4  ",
  0x0000, 0x0005, 0x0014, 0x0001,
        21,       -1,       -1, G_STRING          ,   /* Object 20  */
  NONE, NORMAL, (LONG)"  Desk Accessory 5  ",
  0x0000, 0x0006, 0x0014, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 21  */
  NONE, NORMAL, (LONG)"  Desk Accessory 6  ",
  0x0000, 0x0007, 0x0014, 0x0001,
        40, WINNEW          , WINQUIT         , G_BOX             ,   /* Object 22  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0007, 0x0000, 0x0014, 0x0011,
  WINOPEN         ,       -1,       -1, G_STRING          ,   /* Object 23 WINNEW */
  NONE, NORMAL, (LONG)"  New            ^N ",
  0x0000, 0x0000, 0x0014, 0x0001,
  WINPICK         ,       -1,       -1, G_STRING          ,   /* Object 24 WINOPEN */
  NONE, NORMAL, (LONG)"  Open...        ^O ",
  0x0000, 0x0001, 0x0014, 0x0001,
  WINREAD         ,       -1,       -1, G_STRING          ,   /* Object 25 WINPICK */
  NONE, NORMAL, (LONG)"  Picklist...       ",
  0x0000, 0x0002, 0x0014, 0x0001,
  WININFO         ,       -1,       -1, G_STRING          ,   /* Object 26 WINREAD */
  NONE, NORMAL, (LONG)"  Insert...      ^R ",
  0x0000, 0x0003, 0x0014, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27 WININFO */
  NONE, NORMAL, (LONG)"  Information... ^I ",
  0x0000, 0x0004, 0x0014, 0x0001,
  WINCLOSE        ,       -1,       -1, G_STRING          |0x0700,   /* Object 28  */
  NONE, DISABLED, (LONG)"--------------------",
  0x0000, 0x0005, 0x0014, 0x0001,
  WINSAVE         ,       -1,       -1, G_STRING          ,   /* Object 29 WINCLOSE */
  NONE, NORMAL, (LONG)"  Close          ^U ",
  0x0000, 0x0006, 0x0014, 0x0001,
  WINSAVAS        ,       -1,       -1, G_STRING          ,   /* Object 30 WINSAVE */
  FLAGS15, NORMAL, (LONG)"  Save           ^S ",
  0x0000, 0x0007, 0x0014, 0x0001,
  WINABORT        ,       -1,       -1, G_STRING          ,   /* Object 31 WINSAVAS */
  FLAGS15, NORMAL, (LONG)"  Save as...     ^M ",
  0x0000, 0x0008, 0x0014, 0x0001,
        33,       -1,       -1, G_STRING          ,   /* Object 32 WINABORT */
  FLAGS15, NORMAL, (LONG)"  Abort          ^D ",
  0x0000, 0x0009, 0x0014, 0x0001,
  WINSHELL        ,       -1,       -1, G_STRING          |0x0700,   /* Object 33  */
  NONE, DISABLED, (LONG)"--------------------",
  0x0000, 0x000A, 0x0014, 0x0001,
        35,       -1,       -1, G_STRING          ,   /* Object 34 WINSHELL */
  NONE, NORMAL, (LONG)"  TOS shell...   ^Z ",
  0x0000, 0x000B, 0x0014, 0x0001,
  WINLAY          ,       -1,       -1, G_STRING          |0x0700,   /* Object 35  */
  NONE, DISABLED, (LONG)"--------------------",
  0x0000, 0x000C, 0x0014, 0x0001,
  WINPRINT        ,       -1,       -1, G_STRING          ,   /* Object 36 WINLAY */
  NONE, NORMAL, (LONG)"  Page layout... \007L ",
  0x0000, 0x000D, 0x0014, 0x0001,
        38,       -1,       -1, G_STRING          ,   /* Object 37 WINPRINT */
  FLAGS15, NORMAL, (LONG)"  Print...       ^P ",
  0x0000, 0x000E, 0x0014, 0x0001,
  WINQUIT         ,       -1,       -1, G_STRING          |0x0700,   /* Object 38  */
  NONE, DISABLED, (LONG)"--------------------",
  0x0000, 0x000F, 0x0014, 0x0001,
        22,       -1,       -1, G_STRING          ,   /* Object 39 WINQUIT */
  FLAGS15, NORMAL, (LONG)"  Quit           ^Q ",
  0x0000, 0x0010, 0x0014, 0x0001,
        57, EDITUNDO        , EDITCAPS        , G_BOX             ,   /* Object 40  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x000D, 0x0000, 0x0012, 0x0010,
        42,       -1,       -1, G_STRING          ,   /* Object 41 EDITUNDO */
  NONE, NORMAL, (LONG)"  Undo       Undo ",
  0x0000, 0x0000, 0x0012, 0x0001,
  EDITCUT         ,       -1,       -1, G_STRING          |0x0700,   /* Object 42  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0001, 0x0012, 0x0001,
  EDITCOPY        ,       -1,       -1, G_STRING          ,   /* Object 43 EDITCUT */
  FLAGS15, NORMAL, (LONG)"  Cut          ^X ",
  0x0000, 0x0002, 0x0012, 0x0001,
  EDITPAST        ,       -1,       -1, G_STRING          ,   /* Object 44 EDITCOPY */
  FLAGS15, NORMAL, (LONG)"  Copy         ^C ",
  0x0000, 0x0003, 0x0012, 0x0001,
  EDITALL         ,       -1,       -1, G_STRING          ,   /* Object 45 EDITPAST */
  NONE, NORMAL, (LONG)"  Insert       ^V ",
  0x0000, 0x0004, 0x0012, 0x0001,
        47,       -1,       -1, G_STRING          ,   /* Object 46 EDITALL */
  NONE, NORMAL, (LONG)"  Select all   ^A ",
  0x0000, 0x0005, 0x0012, 0x0001,
  EDITSHLF        ,       -1,       -1, G_STRING          |0x0700,   /* Object 47  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0006, 0x0012, 0x0001,
  EDITSHRT        ,       -1,       -1, G_STRING          ,   /* Object 48 EDITSHLF */
  FLAGS15, NORMAL, (LONG)"  Shift left   ^J ",
  0x0000, 0x0007, 0x0012, 0x0001,
        50,       -1,       -1, G_STRING          ,   /* Object 49 EDITSHRT */
  FLAGS15, NORMAL, (LONG)"  Shift right  ^K ",
  0x0000, 0x0008, 0x0012, 0x0001,
  EDITFORM        ,       -1,       -1, G_STRING          |0x0700,   /* Object 50  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0009, 0x0012, 0x0001,
  EDITSORT        ,       -1,       -1, G_STRING          ,   /* Object 51 EDITFORM */
  NONE, NORMAL, (LONG)"  Format...    \007F ",
  0x0000, 0x000A, 0x0012, 0x0001,
  EDITTOGL        ,       -1,       -1, G_STRING          ,   /* Object 52 EDITSORT */
  NONE, NORMAL, (LONG)"  Sort line... \007O ",
  0x0000, 0x000B, 0x0012, 0x0001,
  EDITBIG         ,       -1,       -1, G_STRING          ,   /* Object 53 EDITTOGL */
  NONE, NORMAL, (LONG)"  Toggle       ^~ ",
  0x0000, 0x000C, 0x0012, 0x0001,
  EDITSMAL        ,       -1,       -1, G_STRING          ,   /* Object 54 EDITBIG */
  NONE, NORMAL, (LONG)"  All upper    ^+ ",
  0x0000, 0x000D, 0x0012, 0x0001,
  EDITCAPS        ,       -1,       -1, G_STRING          ,   /* Object 55 EDITSMAL */
  NONE, NORMAL, (LONG)"  All lower    ^- ",
  0x0000, 0x000E, 0x0012, 0x0001,
        40,       -1,       -1, G_STRING          ,   /* Object 56 EDITCAPS */
  NONE, NORMAL, (LONG)"  Capitalize   ^\047 ",
  0x0000, 0x000F, 0x0012, 0x0001,
        72, SEARFIND        , SEARFILE        , G_BOX             ,   /* Object 57  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0013, 0x0000, 0x0019, 0x000E,
  SEARNEXT        ,       -1,       -1, G_STRING          ,   /* Object 58 SEARFIND */
  NONE, NORMAL, (LONG)"  Find/Replace...     ^F ",
  0x0000, 0x0000, 0x0019, 0x0001,
  SEARSEL         ,       -1,       -1, G_STRING          ,   /* Object 59 SEARNEXT */
  FLAGS15, NORMAL, (LONG)"  Repeat              ^G ",
  0x0000, 0x0001, 0x0019, 0x0001,
        61,       -1,       -1, G_STRING          ,   /* Object 60 SEARSEL */
  NONE, NORMAL, (LONG)"  Find selection         ",
  0x0000, 0x0002, 0x0019, 0x0001,
  SEARBEG         ,       -1,       -1, G_STRING          |0x0700,   /* Object 61  */
  NONE, DISABLED, (LONG)"-------------------------",
  0x0000, 0x0003, 0x0019, 0x0001,
  SEAREND         ,       -1,       -1, G_STRING          ,   /* Object 62 SEARBEG */
  NONE, NORMAL, (LONG)"  Blockstart         \001^B ",
  0x0000, 0x0004, 0x0019, 0x0001,
        64,       -1,       -1, G_STRING          ,   /* Object 63 SEAREND */
  NONE, NORMAL, (LONG)"  Blockend           \001^E ",
  0x0000, 0x0005, 0x0019, 0x0001,
  SEARSMRK        ,       -1,       -1, G_STRING          |0x0700,   /* Object 64  */
  NONE, DISABLED, (LONG)"-------------------------",
  0x0000, 0x0006, 0x0019, 0x0001,
  SEARGMRK        ,       -1,       -1, G_STRING          ,   /* Object 65 SEARSMRK */
  NONE, NORMAL, (LONG)"  Set mark...         \007M ",
  0x0000, 0x0007, 0x0019, 0x0001,
        67,       -1,       -1, G_STRING          ,   /* Object 66 SEARGMRK */
  NONE, NORMAL, (LONG)"  Goto mark...       \001\007M ",
  0x0000, 0x0008, 0x0019, 0x0001,
  SEARPAGE        ,       -1,       -1, G_STRING          |0x0700,   /* Object 67  */
  NONE, DISABLED, (LONG)"-------------------------",
  0x0000, 0x0009, 0x0019, 0x0001,
  SEARGOTO        ,       -1,       -1, G_STRING          ,   /* Object 68 SEARPAGE */
  NONE, NORMAL, (LONG)"  Goto page...       \001^L ",
  0x0000, 0x000A, 0x0019, 0x0001,
        70,       -1,       -1, G_STRING          ,   /* Object 69 SEARGOTO */
  NONE, NORMAL, (LONG)"  Goto line...        ^L ",
  0x0000, 0x000B, 0x0019, 0x0001,
  SEARFILE        ,       -1,       -1, G_STRING          ,   /* Object 70  */
  NONE, DISABLED, (LONG)"-------------------------",
  0x0000, 0x000C, 0x0019, 0x0001,
        57,       -1,       -1, G_STRING          ,   /* Object 71 SEARFILE */
  NONE, NORMAL, (LONG)"  Search in files... \001^F ",
  0x0000, 0x000D, 0x0019, 0x0001,
        83, BLKCNT          , BLKKONF         , G_BOX             ,   /* Object 72  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x001B, 0x0000, 0x0015, 0x000A,
  BLKSUM          ,       -1,       -1, G_STRING          ,   /* Object 73 BLKCNT */
  NONE, NORMAL, (LONG)"  Quantity        ^1 ",
  0x0000, 0x0000, 0x0015, 0x0001,
  BLKMEAN         ,       -1,       -1, G_STRING          ,   /* Object 74 BLKSUM */
  NONE, NORMAL, (LONG)"  Sum             ^2 ",
  0x0000, 0x0001, 0x0015, 0x0001,
  BLKSDEV         ,       -1,       -1, G_STRING          ,   /* Object 75 BLKMEAN */
  NONE, NORMAL, (LONG)"  Mean            ^3 ",
  0x0000, 0x0002, 0x0015, 0x0001,
  BLKMWST         ,       -1,       -1, G_STRING          ,   /* Object 76 BLKSDEV */
  NONE, NORMAL, (LONG)"  Std.-deviation  ^4 ",
  0x0000, 0x0003, 0x0015, 0x0001,
  BLKINTER        ,       -1,       -1, G_STRING          ,   /* Object 77 BLKMWST */
  NONE, NORMAL, (LONG)"  Tax             ^5 ",
  0x0000, 0x0004, 0x0015, 0x0001,
        79,       -1,       -1, G_STRING          ,   /* Object 78 BLKINTER */
  NONE, NORMAL, (LONG)"  Slove           ^6 ",
  0x0000, 0x0005, 0x0015, 0x0001,
  BLKALL          ,       -1,       -1, G_STRING          |0x0700,   /* Object 79  */
  NONE, DISABLED, (LONG)"---------------------",
  0x0000, 0x0006, 0x0015, 0x0001,
        81,       -1,       -1, G_STRING          ,   /* Object 80 BLKALL */
  NONE, NORMAL, (LONG)"  Calc all        ^0 ",
  0x0000, 0x0007, 0x0015, 0x0001,
  BLKKONF         ,       -1,       -1, G_STRING          ,   /* Object 81  */
  NONE, DISABLED, (LONG)"---------------------",
  0x0000, 0x0008, 0x0015, 0x0001,
        72,       -1,       -1, G_STRING          ,   /* Object 82 BLKKONF */
  NONE, NORMAL, (LONG)"  Setup...        \007E ",
  0x0000, 0x0009, 0x0015, 0x0001,
        90, FORMTAB         , FORMIND         , G_BOX             ,   /* Object 83  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0021, 0x0000, 0x0016, 0x0006,
  FORMUMBR        ,       -1,       -1, G_STRING          ,   /* Object 84 FORMTAB */
  NONE, NORMAL, (LONG)"  Tab width...     \007T ",
  0x0000, 0x0000, 0x0016, 0x0001,
  FORMBLK         ,       -1,       -1, G_STRING          ,   /* Object 85 FORMUMBR */
  NONE, NORMAL, (LONG)"  Line length...   \007U ",
  0x0000, 0x0001, 0x0016, 0x0001,
  BLKCOL          ,       -1,       -1, G_STRING          ,   /* Object 86 FORMBLK */
  NONE, NORMAL, (LONG)"  Block mode       \007B ",
  0x0000, 0x0002, 0x0016, 0x0001,
  FORMINS         ,       -1,       -1, G_STRING          ,   /* Object 87 BLKCOL */
  NONE, NORMAL, (LONG)"  Colum block      \007C ",
  0x0000, 0x0003, 0x0016, 0x0001,
  FORMIND         ,       -1,       -1, G_STRING          ,   /* Object 88 FORMINS */
  NONE, NORMAL, (LONG)"  Insert mode     Ins ",
  0x0000, 0x0004, 0x0016, 0x0001,
        83,       -1,       -1, G_STRING          ,   /* Object 89 FORMIND */
  NONE, NORMAL, (LONG)"  Auto indent      \007I ",
  0x0000, 0x0005, 0x0016, 0x0001,
        99, MACOPEN         , MACSCT          , G_BOX             ,   /* Object 90  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0027, 0x0000, 0x0012, 0x0008,
  MACSAVE         ,       -1,       -1, G_STRING          ,   /* Object 91 MACOPEN */
  NONE, NORMAL, (LONG)"  Open...         ",
  0x0000, 0x0000, 0x0012, 0x0001,
        93,       -1,       -1, G_STRING          ,   /* Object 92 MACSAVE */
  NONE, NORMAL, (LONG)"  Save...         ",
  0x0000, 0x0001, 0x0012, 0x0001,
  MACREC          ,       -1,       -1, G_STRING          ,   /* Object 93  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0002, 0x0012, 0x0001,
  MACSTOP         ,       -1,       -1, G_STRING          ,   /* Object 94 MACREC */
  NONE, NORMAL, (LONG)"  Record          ",
  0x0000, 0x0003, 0x0012, 0x0001,
  MACPLAY         ,       -1,       -1, G_STRING          ,   /* Object 95 MACSTOP */
  NONE, NORMAL, (LONG)"  Stop            ",
  0x0000, 0x0004, 0x0012, 0x0001,
        97,       -1,       -1, G_STRING          ,   /* Object 96 MACPLAY */
  NONE, NORMAL, (LONG)"  Play            ",
  0x0000, 0x0005, 0x0012, 0x0001,
  MACSCT          ,       -1,       -1, G_STRING          ,   /* Object 97  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0006, 0x0012, 0x0001,
        90,       -1,       -1, G_STRING          ,   /* Object 98 MACSCT */
  NONE, NORMAL, (LONG)"  Shortcuts... \007S ",
  0x0000, 0x0007, 0x0012, 0x0001,
       117, WINOPALL        , WINDAT7         , G_BOX             ,   /* Object 99  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x072C, 0x0000, 0x0018, 0x0011,
  WINCLALL        ,       -1,       -1, G_STRING          ,   /* Object 100 WINOPALL */
  NONE, NORMAL, (LONG)"  Open all          \001^O ",
  0x0000, 0x0000, 0x0018, 0x0001,
  WINNEXT2        ,       -1,       -1, G_STRING          ,   /* Object 101 WINCLALL */
  NONE, NORMAL, (LONG)"  Close all         \001^U ",
  0x0000, 0x0001, 0x0018, 0x0001,
       103,       -1,       -1, G_STRING          ,   /* Object 102 WINNEXT2 */
  NONE, NORMAL, (LONG)"  Next window        ^W ",
  0x0000, 0x0002, 0x0018, 0x0001,
  WINARR1         ,       -1,       -1, G_STRING          |0x0700,   /* Object 103  */
  NONE, DISABLED, (LONG)"------------------------",
  0x0000, 0x0003, 0x0018, 0x0001,
  WINARR2         ,       -1,       -1, G_STRING          ,   /* Object 104 WINARR1 */
  NONE, NORMAL, (LONG)"  Tile              \007F1 ",
  0x0000, 0x0004, 0x0018, 0x0001,
  WINARR3         ,       -1,       -1, G_STRING          ,   /* Object 105 WINARR2 */
  NONE, NORMAL, (LONG)"  One upon another  \007F2 ",
  0x0000, 0x0005, 0x0018, 0x0001,
       107,       -1,       -1, G_STRING          ,   /* Object 106 WINARR3 */
  NONE, NORMAL, (LONG)"  Stagger           \007F3 ",
  0x0000, 0x0006, 0x0018, 0x0001,
  WINFULL         ,       -1,       -1, G_STRING          ,   /* Object 107  */
  NONE, DISABLED, (LONG)"------------------------",
  0x0000, 0x0007, 0x0018, 0x0001,
       109,       -1,       -1, G_STRING          ,   /* Object 108 WINFULL */
  FLAGS15, NORMAL, (LONG)"  Full Size        \007F10 ",
  0x0000, 0x0008, 0x0018, 0x0001,
  WINDAT1         ,       -1,       -1, G_STRING          |0x0700,   /* Object 109  */
  NONE, DISABLED, (LONG)"------------------------",
  0x0000, 0x0009, 0x0018, 0x0001,
  WINDAT2         ,       -1,       -1, G_STRING          ,   /* Object 110 WINDAT1 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x000A, 0x0018, 0x0001,
  WINDAT3         ,       -1,       -1, G_STRING          ,   /* Object 111 WINDAT2 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x000B, 0x0018, 0x0001,
  WINDAT4         ,       -1,       -1, G_STRING          ,   /* Object 112 WINDAT3 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x000C, 0x0018, 0x0001,
  WINDAT5         ,       -1,       -1, G_STRING          ,   /* Object 113 WINDAT4 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x000D, 0x0018, 0x0001,
  WINDAT6         ,       -1,       -1, G_STRING          ,   /* Object 114 WINDAT5 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x000E, 0x0018, 0x0001,
  WINDAT7         ,       -1,       -1, G_STRING          ,   /* Object 115 WINDAT6 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x000F, 0x0018, 0x0001,
        99,       -1,       -1, G_STRING          ,   /* Object 116 WINDAT7 */
  NONE, DISABLED, (LONG)"  ORDNER\134DATEINAME.EXT  ",
  0x0000, 0x0010, 0x0018, 0x0001,
        12, OPTHELP         , OPTSAVE         , G_BOX             ,   /* Object 117  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x072A, 0x0000, 0x0017, 0x000D,
       119,       -1,       -1, G_STRING          ,   /* Object 118 OPTHELP */
  NONE, NORMAL, (LONG)"  Help...         Help ",
  0x0000, 0x0000, 0x0017, 0x0001,
  OPTFONT         ,       -1,       -1, G_STRING          |0x0700,   /* Object 119  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x0001, 0x0017, 0x0001,
  OPTCHARS        ,       -1,       -1, G_STRING          ,   /* Object 120 OPTFONT */
  NONE, NORMAL, (LONG)"  Font...              ",
  0x0000, 0x0002, 0x0017, 0x0001,
       122,       -1,       -1, G_STRING          ,   /* Object 121 OPTCHARS */
  NONE, NORMAL, (LONG)"  Character set...  \007Z ",
  0x0000, 0x0003, 0x0017, 0x0001,
  OPTFKT          ,       -1,       -1, G_STRING          |0x0700,   /* Object 122  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x0004, 0x0017, 0x0001,
  OPTVIEW         ,       -1,       -1, G_STRING          ,   /* Object 123 OPTFKT */
  NONE, NORMAL, (LONG)"  Soft keys...         ",
  0x0000, 0x0005, 0x0017, 0x0001,
  OPTLIN          ,       -1,       -1, G_STRING          ,   /* Object 124 OPTVIEW */
  NONE, NORMAL, (LONG)"  Text preview...   \007P ",
  0x0000, 0x0006, 0x0017, 0x0001,
  OPTBRACE        ,       -1,       -1, G_STRING          ,   /* Object 125 OPTLIN */
  NONE, NORMAL, (LONG)"  Ruler...         \001\007L ",
  0x0000, 0x0007, 0x0017, 0x0001,
  OPTCOMP         ,       -1,       -1, G_STRING          ,   /* Object 126 OPTBRACE */
  NONE, NORMAL, (LONG)"  Check brackets... \007K ",
  0x0000, 0x0008, 0x0017, 0x0001,
       128,       -1,       -1, G_STRING          ,   /* Object 127 OPTCOMP */
  FLAGS15, NORMAL, (LONG)"  Compare texts     \007V ",
  0x0000, 0x0009, 0x0017, 0x0001,
  OPTDIV          ,       -1,       -1, G_STRING          |0x0700,   /* Object 128  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x000A, 0x0017, 0x0001,
  OPTSAVE         ,       -1,       -1, G_STRING          ,   /* Object 129 OPTDIV */
  NONE, NORMAL, (LONG)"  Miscellaneous...  \007D ",
  0x0000, 0x000B, 0x0017, 0x0001,
       117,       -1,       -1, G_STRING          ,   /* Object 130 OPTSAVE */
  LASTOB, NORMAL, (LONG)"  Save preferences...  ",
  0x0000, 0x000C, 0x0017, 0x0001,
  
  /******** Tree 1 DESKTOP ****************************************************/
        -1, DESKICN1        , DESKICND        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00FF0144L,
  0x0000, 0x0000, 0x0050, 0x0018,
  DESKICN2        ,       -1,       -1, G_ICON            ,   /* Object 1 DESKICN1 */
  NONE, NORMAL, (LONG)&rs_iconblk[0],
  0x0001, 0x0015, 0x0009, 0x0802,
  DESKICN3        ,       -1,       -1, G_ICON            ,   /* Object 2 DESKICN2 */
  NONE, NORMAL, (LONG)&rs_iconblk[1],
  0x000B, 0x0015, 0x0009, 0x0802,
  DESKICN4        ,       -1,       -1, G_ICON            ,   /* Object 3 DESKICN3 */
  NONE, NORMAL, (LONG)&rs_iconblk[2],
  0x0015, 0x0015, 0x0009, 0x0802,
  DESKICN5        ,       -1,       -1, G_ICON            ,   /* Object 4 DESKICN4 */
  NONE, NORMAL, (LONG)&rs_iconblk[3],
  0x001F, 0x0015, 0x0009, 0x0802,
  DESKICN6        ,       -1,       -1, G_ICON            ,   /* Object 5 DESKICN5 */
  NONE, NORMAL, (LONG)&rs_iconblk[4],
  0x0029, 0x0015, 0x0009, 0x0802,
  DESKICN7        ,       -1,       -1, G_ICON            ,   /* Object 6 DESKICN6 */
  NONE, NORMAL, (LONG)&rs_iconblk[5],
  0x0033, 0x0015, 0x0009, 0x0802,
  DESKICN8        ,       -1,       -1, G_ICON            ,   /* Object 7 DESKICN7 */
  NONE, NORMAL, (LONG)&rs_iconblk[6],
  0x003D, 0x0015, 0x0009, 0x0802,
  DESKICN9        ,       -1,       -1, G_ICON            ,   /* Object 8 DESKICN8 */
  NONE, NORMAL, (LONG)&rs_iconblk[7],
  0x0047, 0x0015, 0x0009, 0x0802,
  DESKICNA        ,       -1,       -1, G_ICON            ,   /* Object 9 DESKICN9 */
  NONE, NORMAL, (LONG)&rs_iconblk[8],
  0x0047, 0x0010, 0x0009, 0x0602,
  DESKICNB        ,       -1,       -1, G_ICON            ,   /* Object 10 DESKICNA */
  NONE, NORMAL, (LONG)&rs_iconblk[9],
  0x0047, 0x000D, 0x0009, 0x0602,
  DESKICNC        ,       -1,       -1, G_ICON            ,   /* Object 11 DESKICNB */
  NONE, NORMAL, (LONG)&rs_iconblk[10],
  0x0047, 0x000A, 0x0009, 0x0602,
  DESKICND        ,       -1,       -1, G_ICON            ,   /* Object 12 DESKICNC */
  NONE, NORMAL, (LONG)&rs_iconblk[11],
  0x0047, 0x0007, 0x0009, 0x0602,
         0,       -1,       -1, G_ICON            ,   /* Object 13 DESKICND */
  LASTOB, NORMAL, (LONG)&rs_iconblk[12],
  0x0047, 0x0004, 0x0009, 0x0802,
  
  /******** Tree 2 FINDMENU ****************************************************/
        -1,        1, FINDOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0041, 0x0011,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[0],
  0x003F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[1],
  0x0003, 0x0001, 0x000C, 0x0001,
  FINDSTR         ,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[2],
  0x0003, 0x0003, 0x0208, 0x0001,
         5,       -1,       -1, G_FTEXT           ,   /* Object 4 FINDSTR */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[3],
  0x0003, 0x0004, 0x003B, 0x0001,
  FINDREPL        ,       -1,       -1, G_TEXT            ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_tedinfo[4],
  0x0003, 0x0005, 0x0609, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 FINDREPL */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[5],
  0x0003, 0x0006, 0x003B, 0x0001,
        12,        8, FINDGREP        , G_BOX             |0x0900,   /* Object 7  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0008, 0x000D, 0x0006,
  FINDNORM        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 8  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[6],
  0x0001, 0x0000, 0x0009, 0x0001,
  FINDMAT         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 9 FINDNORM */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[7],
  0x0002, 0x0001, 0x0009, 0x0001,
  FINDGREP        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 10 FINDMAT */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[8],
  0x0002, 0x0002, 0x0008, 0x0001,
         7,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 11 FINDGREP */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[9],
  0x0002, 0x0003, 0x0007, 0x0001,
        26,       13, FINDALL         , G_BOX             |0x0900,   /* Object 12  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0012, 0x0008, 0x002C, 0x0006,
  FINDSUCH        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 13  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[10],
  0x0001, 0x0000, 0x0007, 0x0001,
  FINDBLK         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 14 FINDSUCH */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[11],
  0x0002, 0x0001, 0x0009, 0x0001,
  FINDANF         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 15 FINDBLK */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[12],
  0x000E, 0x0001, 0x000B, 0x0001,
  FINDIGNO        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 16 FINDANF */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[13],
  0x001C, 0x0001, 0x000D, 0x0001,
  FINDWORD        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 17 FINDIGNO */
  SELECTABLE|EXIT, SELECTED, (LONG)&rs_tedinfo[14],
  0x000E, 0x0002, 0x000D, 0x0001,
        19,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 18 FINDWORD */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[15],
  0x001C, 0x0002, 0x000E, 0x0001,
        20,       -1,       -1, G_TEXT            ,   /* Object 19  */
  NONE, NORMAL, (LONG)&rs_tedinfo[16],
  0x0006, 0x0003, 0x0606, 0x0001,
  FINDERS         , FINDFORW        , FINDBACK        , G_IBOX            ,   /* Object 20  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x000E, 0x0003, 0x001D, 0x0001,
  FINDBACK        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 21 FINDFORW */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[17],
  0x0000, 0x0000, 0x000B, 0x0001,
        20,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 22 FINDBACK */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[18],
  0x000E, 0x0000, 0x000C, 0x0001,
  FINDASK         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 23 FINDERS */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[19],
  0x0002, 0x0004, 0x000B, 0x0001,
  FINDALL         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 24 FINDASK */
  SELECTABLE|EXIT, DISABLED, (LONG)&rs_tedinfo[20],
  0x000E, 0x0004, 0x000D, 0x0001,
        12,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 25 FINDALL */
  SELECTABLE|EXIT, DISABLED, (LONG)&rs_tedinfo[21],
  0x001C, 0x0004, 0x000E, 0x0001,
  FINDALLQ        ,       -1,       -1, G_STRING          ,   /* Object 26  */
  NONE, NORMAL, (LONG)"Wildcards:",
  0x0003, 0x000F, 0x000A, 0x0001,
        28,       -1,       -1, G_FTEXT           ,   /* Object 27 FINDALLQ */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[22],
  0x000E, 0x000F, 0x0002, 0x0001,
  FINDSINQ        ,       -1,       -1, G_STRING          ,   /* Object 28  */
  NONE, NORMAL, (LONG)"and",
  0x0011, 0x000F, 0x0003, 0x0001,
  FINDHELP        ,       -1,       -1, G_FTEXT           ,   /* Object 29 FINDSINQ */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[23],
  0x0015, 0x000F, 0x0002, 0x0001,
  FINDABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 30 FINDHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[24],
  0x0022, 0x000F, 0x0008, 0x0001,
  FINDOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 31 FINDABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[25],
  0x002C, 0x000F, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 32 FINDOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[26],
  0x0036, 0x000F, 0x0008, 0x0001,
  
  /******** Tree 3 GOTOMENU ****************************************************/
        -1,        1, GOTOOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0018, 0x0007,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[27],
  0x0016, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[28],
  0x0003, 0x0001, 0x0409, 0x0001,
  GOTOLINE        ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Line:",
  0x0003, 0x0003, 0x0005, 0x0001,
  GOTOABBR        ,       -1,       -1, G_FTEXT           ,   /* Object 4 GOTOLINE */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[29],
  0x000A, 0x0003, 0x0005, 0x0001,
  GOTOOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 5 GOTOABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[30],
  0x0003, 0x0005, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 6 GOTOOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[31],
  0x000D, 0x0005, 0x0008, 0x0001,
  
  /******** Tree 4 TABMENU ****************************************************/
        -1,        1, TABOK           , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0018, 0x0007,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[32],
  0x0016, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[33],
  0x0003, 0x0001, 0x0009, 0x0001,
  TABULAT         ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Width:",
  0x0003, 0x0003, 0x0006, 0x0001,
  TABABBR         ,       -1,       -1, G_FTEXT           ,   /* Object 4 TABULAT */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[34],
  0x000A, 0x0003, 0x0002, 0x0001,
  TABOK           ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 5 TABABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[35],
  0x0003, 0x0005, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 6 TABOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[36],
  0x000D, 0x0005, 0x0008, 0x0001,
  
  /******** Tree 5 COPYINFO ****************************************************/
        -1,        1, CCF             , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0023, 0x0016,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[37],
  0x0021, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[38],
  0x0003, 0x0001, 0x0009, 0x0001,
         4,       -1,       -1, G_IMAGE           ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_bitblk[0],
  0x0003, 0x0802, 0x000C, 0x0003,
  CDATE           ,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"GEM-Editor for Atari Computer",
  0x0003, 0x0805, 0x001D, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5 CDATE */
  NONE, NORMAL, (LONG)"Version 2.33-MJK  MAR 12 1997",
  0x0003, 0x0806, 0x001D, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"Copyright \275 1994 bei TheoSoft",
  0x0003, 0x000B, 0x001D, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"Michael Th\204nitz",
  0x000A, 0x000C, 0x000F, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"Patchs and development",
  0x0003, 0x000D, 0x0016, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"as open project:",
  0x0003, 0x000E, 0x0010, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"Markus Kohm (MJK)",
  0x0009, 0x000F, 0x0011, 0x0001,
  THEOLOGO        ,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"International version",
  0x0406, 0x0811, 0x0015, 0x0001,
  CINFO           ,       -1,       -1, G_IMAGE           ,   /* Object 12 THEOLOGO */
  NONE, NORMAL, (LONG)&rs_bitblk[1],
  0x0003, 0x0812, 0x0006, 0x0E01,
  COPYOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 13 CINFO */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[39],
  0x000E, 0x0813, 0x0008, 0x0001,
        15,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 14 COPYOK */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)&rs_tedinfo[40],
  0x0018, 0x0813, 0x0008, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"Gerhard Stoll (GS)",
  0x0009, 0x0010, 0x0012, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"MiNT-Lib:",
  0x0003, 0x0807, 0x0009, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"GEM-Lib :",
  0x0003, 0x0808, 0x0009, 0x0001,
  CMINT           ,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"CF-Lib  :",
  0x0003, 0x0809, 0x0009, 0x0001,
  CGEM            ,       -1,       -1, G_STRING          ,   /* Object 19 CMINT */
  NONE, NORMAL, (LONG)"not used",
  0x040D, 0x0807, 0x0008, 0x0001,
  CCF             ,       -1,       -1, G_STRING          ,   /* Object 20 CGEM */
  NONE, NORMAL, (LONG)"not used",
  0x040D, 0x0808, 0x0008, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 21 CCF */
  LASTOB, NORMAL, (LONG)"not used",
  0x040D, 0x0809, 0x0008, 0x0001,
  
  /******** Tree 6 SHELMENU ****************************************************/
        -1,        1, SHELLOK         , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0041, 0x0008,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[41],
  0x003F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[42],
  0x0003, 0x0001, 0x0009, 0x0001,
  SHELCOMM        ,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[43],
  0x0003, 0x0003, 0x0006, 0x0001,
  SHELLGEM        ,       -1,       -1, G_FTEXT           ,   /* Object 4 SHELCOMM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[44],
  0x0003, 0x0004, 0x003B, 0x0001,
  SHELHELP        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 5 SHELLGEM */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[45],
  0x0003, 0x0006, 0x0013, 0x0001,
  SHELABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 6 SHELHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[46],
  0x0022, 0x0006, 0x0008, 0x0001,
  SHELLOK         ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 7 SHELABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[47],
  0x002C, 0x0006, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 8 SHELLOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[48],
  0x0036, 0x0006, 0x0008, 0x0001,
  
  /******** Tree 7 CHARTBL ****************************************************/
        -1,        1, CHAROK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0034, 0x0016,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[49],
  0x0032, 0x0000, 0x0002, 0x0001,
  FCHAR           ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[50],
  0x0003, 0x0001, 0x000D, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3 FCHAR */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0003, 0x0003, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0003, 0x0003, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0003, 0x0003, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0003, 0x0003, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0003, 0x0003, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0003, 0x0003, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0003, 0x0003, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0003, 0x0003, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0003, 0x0003, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0003, 0x0003, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0003, 0x0003, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0003, 0x0003, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0003, 0x0003, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0003, 0x0003, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0003, 0x0003, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0003, 0x0003, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 19  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0004, 0x0003, 0x0001,
        21,       -1,       -1, G_STRING          ,   /* Object 20  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0004, 0x0003, 0x0001,
        22,       -1,       -1, G_STRING          ,   /* Object 21  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0004, 0x0003, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0004, 0x0003, 0x0001,
        24,       -1,       -1, G_STRING          ,   /* Object 23  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0004, 0x0003, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0004, 0x0003, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0004, 0x0003, 0x0001,
        27,       -1,       -1, G_STRING          ,   /* Object 26  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0004, 0x0003, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0004, 0x0003, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0004, 0x0003, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0004, 0x0003, 0x0001,
        31,       -1,       -1, G_STRING          ,   /* Object 30  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0004, 0x0003, 0x0001,
        32,       -1,       -1, G_STRING          ,   /* Object 31  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0004, 0x0003, 0x0001,
        33,       -1,       -1, G_STRING          ,   /* Object 32  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0004, 0x0003, 0x0001,
        34,       -1,       -1, G_STRING          ,   /* Object 33  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0004, 0x0003, 0x0001,
        35,       -1,       -1, G_STRING          ,   /* Object 34  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0004, 0x0003, 0x0001,
        36,       -1,       -1, G_STRING          ,   /* Object 35  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0005, 0x0003, 0x0001,
        37,       -1,       -1, G_STRING          ,   /* Object 36  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0005, 0x0003, 0x0001,
        38,       -1,       -1, G_STRING          ,   /* Object 37  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0005, 0x0003, 0x0001,
        39,       -1,       -1, G_STRING          ,   /* Object 38  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0005, 0x0003, 0x0001,
        40,       -1,       -1, G_STRING          ,   /* Object 39  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0005, 0x0003, 0x0001,
        41,       -1,       -1, G_STRING          ,   /* Object 40  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0005, 0x0003, 0x0001,
        42,       -1,       -1, G_STRING          ,   /* Object 41  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0005, 0x0003, 0x0001,
        43,       -1,       -1, G_STRING          ,   /* Object 42  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0005, 0x0003, 0x0001,
        44,       -1,       -1, G_STRING          ,   /* Object 43  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0005, 0x0003, 0x0001,
        45,       -1,       -1, G_STRING          ,   /* Object 44  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0005, 0x0003, 0x0001,
        46,       -1,       -1, G_STRING          ,   /* Object 45  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0005, 0x0003, 0x0001,
        47,       -1,       -1, G_STRING          ,   /* Object 46  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0005, 0x0003, 0x0001,
        48,       -1,       -1, G_STRING          ,   /* Object 47  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0005, 0x0003, 0x0001,
        49,       -1,       -1, G_STRING          ,   /* Object 48  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0005, 0x0003, 0x0001,
        50,       -1,       -1, G_STRING          ,   /* Object 49  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0005, 0x0003, 0x0001,
        51,       -1,       -1, G_STRING          ,   /* Object 50  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0005, 0x0003, 0x0001,
        52,       -1,       -1, G_STRING          ,   /* Object 51  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0006, 0x0003, 0x0001,
        53,       -1,       -1, G_STRING          ,   /* Object 52  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0006, 0x0003, 0x0001,
        54,       -1,       -1, G_STRING          ,   /* Object 53  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0006, 0x0003, 0x0001,
        55,       -1,       -1, G_STRING          ,   /* Object 54  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0006, 0x0003, 0x0001,
        56,       -1,       -1, G_STRING          ,   /* Object 55  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0006, 0x0003, 0x0001,
        57,       -1,       -1, G_STRING          ,   /* Object 56  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0006, 0x0003, 0x0001,
        58,       -1,       -1, G_STRING          ,   /* Object 57  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0006, 0x0003, 0x0001,
        59,       -1,       -1, G_STRING          ,   /* Object 58  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0006, 0x0003, 0x0001,
        60,       -1,       -1, G_STRING          ,   /* Object 59  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0006, 0x0003, 0x0001,
        61,       -1,       -1, G_STRING          ,   /* Object 60  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0006, 0x0003, 0x0001,
        62,       -1,       -1, G_STRING          ,   /* Object 61  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0006, 0x0003, 0x0001,
        63,       -1,       -1, G_STRING          ,   /* Object 62  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0006, 0x0003, 0x0001,
        64,       -1,       -1, G_STRING          ,   /* Object 63  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0006, 0x0003, 0x0001,
        65,       -1,       -1, G_STRING          ,   /* Object 64  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0006, 0x0003, 0x0001,
        66,       -1,       -1, G_STRING          ,   /* Object 65  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0006, 0x0003, 0x0001,
        67,       -1,       -1, G_STRING          ,   /* Object 66  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0006, 0x0003, 0x0001,
        68,       -1,       -1, G_STRING          ,   /* Object 67  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0007, 0x0003, 0x0001,
        69,       -1,       -1, G_STRING          ,   /* Object 68  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0007, 0x0003, 0x0001,
        70,       -1,       -1, G_STRING          ,   /* Object 69  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0007, 0x0003, 0x0001,
        71,       -1,       -1, G_STRING          ,   /* Object 70  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0007, 0x0003, 0x0001,
        72,       -1,       -1, G_STRING          ,   /* Object 71  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0007, 0x0003, 0x0001,
        73,       -1,       -1, G_STRING          ,   /* Object 72  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0007, 0x0003, 0x0001,
        74,       -1,       -1, G_STRING          ,   /* Object 73  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0007, 0x0003, 0x0001,
        75,       -1,       -1, G_STRING          ,   /* Object 74  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0007, 0x0003, 0x0001,
        76,       -1,       -1, G_STRING          ,   /* Object 75  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0007, 0x0003, 0x0001,
        77,       -1,       -1, G_STRING          ,   /* Object 76  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0007, 0x0003, 0x0001,
        78,       -1,       -1, G_STRING          ,   /* Object 77  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0007, 0x0003, 0x0001,
        79,       -1,       -1, G_STRING          ,   /* Object 78  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0007, 0x0003, 0x0001,
        80,       -1,       -1, G_STRING          ,   /* Object 79  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0007, 0x0003, 0x0001,
        81,       -1,       -1, G_STRING          ,   /* Object 80  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0007, 0x0003, 0x0001,
        82,       -1,       -1, G_STRING          ,   /* Object 81  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0007, 0x0003, 0x0001,
        83,       -1,       -1, G_STRING          ,   /* Object 82  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0007, 0x0003, 0x0001,
        84,       -1,       -1, G_STRING          ,   /* Object 83  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0008, 0x0003, 0x0001,
        85,       -1,       -1, G_STRING          ,   /* Object 84  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0008, 0x0003, 0x0001,
        86,       -1,       -1, G_STRING          ,   /* Object 85  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0008, 0x0003, 0x0001,
        87,       -1,       -1, G_STRING          ,   /* Object 86  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0008, 0x0003, 0x0001,
        88,       -1,       -1, G_STRING          ,   /* Object 87  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0008, 0x0003, 0x0001,
        89,       -1,       -1, G_STRING          ,   /* Object 88  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0008, 0x0003, 0x0001,
        90,       -1,       -1, G_STRING          ,   /* Object 89  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0008, 0x0003, 0x0001,
        91,       -1,       -1, G_STRING          ,   /* Object 90  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0008, 0x0003, 0x0001,
        92,       -1,       -1, G_STRING          ,   /* Object 91  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0008, 0x0003, 0x0001,
        93,       -1,       -1, G_STRING          ,   /* Object 92  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0008, 0x0003, 0x0001,
        94,       -1,       -1, G_STRING          ,   /* Object 93  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0008, 0x0003, 0x0001,
        95,       -1,       -1, G_STRING          ,   /* Object 94  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0008, 0x0003, 0x0001,
        96,       -1,       -1, G_STRING          ,   /* Object 95  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0008, 0x0003, 0x0001,
        97,       -1,       -1, G_STRING          ,   /* Object 96  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0008, 0x0003, 0x0001,
        98,       -1,       -1, G_STRING          ,   /* Object 97  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0008, 0x0003, 0x0001,
        99,       -1,       -1, G_STRING          ,   /* Object 98  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0008, 0x0003, 0x0001,
       100,       -1,       -1, G_STRING          ,   /* Object 99  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0009, 0x0003, 0x0001,
       101,       -1,       -1, G_STRING          ,   /* Object 100  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0009, 0x0003, 0x0001,
       102,       -1,       -1, G_STRING          ,   /* Object 101  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0009, 0x0003, 0x0001,
       103,       -1,       -1, G_STRING          ,   /* Object 102  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0009, 0x0003, 0x0001,
       104,       -1,       -1, G_STRING          ,   /* Object 103  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0009, 0x0003, 0x0001,
       105,       -1,       -1, G_STRING          ,   /* Object 104  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0009, 0x0003, 0x0001,
       106,       -1,       -1, G_STRING          ,   /* Object 105  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0009, 0x0003, 0x0001,
       107,       -1,       -1, G_STRING          ,   /* Object 106  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0009, 0x0003, 0x0001,
       108,       -1,       -1, G_STRING          ,   /* Object 107  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0009, 0x0003, 0x0001,
       109,       -1,       -1, G_STRING          ,   /* Object 108  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0009, 0x0003, 0x0001,
       110,       -1,       -1, G_STRING          ,   /* Object 109  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0009, 0x0003, 0x0001,
       111,       -1,       -1, G_STRING          ,   /* Object 110  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0009, 0x0003, 0x0001,
       112,       -1,       -1, G_STRING          ,   /* Object 111  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0009, 0x0003, 0x0001,
       113,       -1,       -1, G_STRING          ,   /* Object 112  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0009, 0x0003, 0x0001,
       114,       -1,       -1, G_STRING          ,   /* Object 113  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0009, 0x0003, 0x0001,
       115,       -1,       -1, G_STRING          ,   /* Object 114  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0009, 0x0003, 0x0001,
       116,       -1,       -1, G_STRING          ,   /* Object 115  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x000A, 0x0003, 0x0001,
       117,       -1,       -1, G_STRING          ,   /* Object 116  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x000A, 0x0003, 0x0001,
       118,       -1,       -1, G_STRING          ,   /* Object 117  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x000A, 0x0003, 0x0001,
       119,       -1,       -1, G_STRING          ,   /* Object 118  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x000A, 0x0003, 0x0001,
       120,       -1,       -1, G_STRING          ,   /* Object 119  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x000A, 0x0003, 0x0001,
       121,       -1,       -1, G_STRING          ,   /* Object 120  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x000A, 0x0003, 0x0001,
       122,       -1,       -1, G_STRING          ,   /* Object 121  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x000A, 0x0003, 0x0001,
       123,       -1,       -1, G_STRING          ,   /* Object 122  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x000A, 0x0003, 0x0001,
       124,       -1,       -1, G_STRING          ,   /* Object 123  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x000A, 0x0003, 0x0001,
       125,       -1,       -1, G_STRING          ,   /* Object 124  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x000A, 0x0003, 0x0001,
       126,       -1,       -1, G_STRING          ,   /* Object 125  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x000A, 0x0003, 0x0001,
       127,       -1,       -1, G_STRING          ,   /* Object 126  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x000A, 0x0003, 0x0001,
       128,       -1,       -1, G_STRING          ,   /* Object 127  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x000A, 0x0003, 0x0001,
       129,       -1,       -1, G_STRING          ,   /* Object 128  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x000A, 0x0003, 0x0001,
       130,       -1,       -1, G_STRING          ,   /* Object 129  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x000A, 0x0003, 0x0001,
       131,       -1,       -1, G_STRING          ,   /* Object 130  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x000A, 0x0003, 0x0001,
       132,       -1,       -1, G_STRING          ,   /* Object 131  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x000B, 0x0003, 0x0001,
       133,       -1,       -1, G_STRING          ,   /* Object 132  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x000B, 0x0003, 0x0001,
       134,       -1,       -1, G_STRING          ,   /* Object 133  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x000B, 0x0003, 0x0001,
       135,       -1,       -1, G_STRING          ,   /* Object 134  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x000B, 0x0003, 0x0001,
       136,       -1,       -1, G_STRING          ,   /* Object 135  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x000B, 0x0003, 0x0001,
       137,       -1,       -1, G_STRING          ,   /* Object 136  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x000B, 0x0003, 0x0001,
       138,       -1,       -1, G_STRING          ,   /* Object 137  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x000B, 0x0003, 0x0001,
       139,       -1,       -1, G_STRING          ,   /* Object 138  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x000B, 0x0003, 0x0001,
       140,       -1,       -1, G_STRING          ,   /* Object 139  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x000B, 0x0003, 0x0001,
       141,       -1,       -1, G_STRING          ,   /* Object 140  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x000B, 0x0003, 0x0001,
       142,       -1,       -1, G_STRING          ,   /* Object 141  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x000B, 0x0003, 0x0001,
       143,       -1,       -1, G_STRING          ,   /* Object 142  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x000B, 0x0003, 0x0001,
       144,       -1,       -1, G_STRING          ,   /* Object 143  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x000B, 0x0003, 0x0001,
       145,       -1,       -1, G_STRING          ,   /* Object 144  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x000B, 0x0003, 0x0001,
       146,       -1,       -1, G_STRING          ,   /* Object 145  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x000B, 0x0003, 0x0001,
       147,       -1,       -1, G_STRING          ,   /* Object 146  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x000B, 0x0003, 0x0001,
       148,       -1,       -1, G_STRING          ,   /* Object 147  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x000C, 0x0003, 0x0001,
       149,       -1,       -1, G_STRING          ,   /* Object 148  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x000C, 0x0003, 0x0001,
       150,       -1,       -1, G_STRING          ,   /* Object 149  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x000C, 0x0003, 0x0001,
       151,       -1,       -1, G_STRING          ,   /* Object 150  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x000C, 0x0003, 0x0001,
       152,       -1,       -1, G_STRING          ,   /* Object 151  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x000C, 0x0003, 0x0001,
       153,       -1,       -1, G_STRING          ,   /* Object 152  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x000C, 0x0003, 0x0001,
       154,       -1,       -1, G_STRING          ,   /* Object 153  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x000C, 0x0003, 0x0001,
       155,       -1,       -1, G_STRING          ,   /* Object 154  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x000C, 0x0003, 0x0001,
       156,       -1,       -1, G_STRING          ,   /* Object 155  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x000C, 0x0003, 0x0001,
       157,       -1,       -1, G_STRING          ,   /* Object 156  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x000C, 0x0003, 0x0001,
       158,       -1,       -1, G_STRING          ,   /* Object 157  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x000C, 0x0003, 0x0001,
       159,       -1,       -1, G_STRING          ,   /* Object 158  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x000C, 0x0003, 0x0001,
       160,       -1,       -1, G_STRING          ,   /* Object 159  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x000C, 0x0003, 0x0001,
       161,       -1,       -1, G_STRING          ,   /* Object 160  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x000C, 0x0003, 0x0001,
       162,       -1,       -1, G_STRING          ,   /* Object 161  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x000C, 0x0003, 0x0001,
       163,       -1,       -1, G_STRING          ,   /* Object 162  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x000C, 0x0003, 0x0001,
       164,       -1,       -1, G_STRING          ,   /* Object 163  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x000D, 0x0003, 0x0001,
       165,       -1,       -1, G_STRING          ,   /* Object 164  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x000D, 0x0003, 0x0001,
       166,       -1,       -1, G_STRING          ,   /* Object 165  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x000D, 0x0003, 0x0001,
       167,       -1,       -1, G_STRING          ,   /* Object 166  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x000D, 0x0003, 0x0001,
       168,       -1,       -1, G_STRING          ,   /* Object 167  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x000D, 0x0003, 0x0001,
       169,       -1,       -1, G_STRING          ,   /* Object 168  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x000D, 0x0003, 0x0001,
       170,       -1,       -1, G_STRING          ,   /* Object 169  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x000D, 0x0003, 0x0001,
       171,       -1,       -1, G_STRING          ,   /* Object 170  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x000D, 0x0003, 0x0001,
       172,       -1,       -1, G_STRING          ,   /* Object 171  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x000D, 0x0003, 0x0001,
       173,       -1,       -1, G_STRING          ,   /* Object 172  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x000D, 0x0003, 0x0001,
       174,       -1,       -1, G_STRING          ,   /* Object 173  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x000D, 0x0003, 0x0001,
       175,       -1,       -1, G_STRING          ,   /* Object 174  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x000D, 0x0003, 0x0001,
       176,       -1,       -1, G_STRING          ,   /* Object 175  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x000D, 0x0003, 0x0001,
       177,       -1,       -1, G_STRING          ,   /* Object 176  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x000D, 0x0003, 0x0001,
       178,       -1,       -1, G_STRING          ,   /* Object 177  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x000D, 0x0003, 0x0001,
       179,       -1,       -1, G_STRING          ,   /* Object 178  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x000D, 0x0003, 0x0001,
       180,       -1,       -1, G_STRING          ,   /* Object 179  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x000E, 0x0003, 0x0001,
       181,       -1,       -1, G_STRING          ,   /* Object 180  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x000E, 0x0003, 0x0001,
       182,       -1,       -1, G_STRING          ,   /* Object 181  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x000E, 0x0003, 0x0001,
       183,       -1,       -1, G_STRING          ,   /* Object 182  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x000E, 0x0003, 0x0001,
       184,       -1,       -1, G_STRING          ,   /* Object 183  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x000E, 0x0003, 0x0001,
       185,       -1,       -1, G_STRING          ,   /* Object 184  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x000E, 0x0003, 0x0001,
       186,       -1,       -1, G_STRING          ,   /* Object 185  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x000E, 0x0003, 0x0001,
       187,       -1,       -1, G_STRING          ,   /* Object 186  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x000E, 0x0003, 0x0001,
       188,       -1,       -1, G_STRING          ,   /* Object 187  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x000E, 0x0003, 0x0001,
       189,       -1,       -1, G_STRING          ,   /* Object 188  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x000E, 0x0003, 0x0001,
       190,       -1,       -1, G_STRING          ,   /* Object 189  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x000E, 0x0003, 0x0001,
       191,       -1,       -1, G_STRING          ,   /* Object 190  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x000E, 0x0003, 0x0001,
       192,       -1,       -1, G_STRING          ,   /* Object 191  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x000E, 0x0003, 0x0001,
       193,       -1,       -1, G_STRING          ,   /* Object 192  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x000E, 0x0003, 0x0001,
       194,       -1,       -1, G_STRING          ,   /* Object 193  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x000E, 0x0003, 0x0001,
       195,       -1,       -1, G_STRING          ,   /* Object 194  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x000E, 0x0003, 0x0001,
       196,       -1,       -1, G_STRING          ,   /* Object 195  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x000F, 0x0003, 0x0001,
       197,       -1,       -1, G_STRING          ,   /* Object 196  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x000F, 0x0003, 0x0001,
       198,       -1,       -1, G_STRING          ,   /* Object 197  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x000F, 0x0003, 0x0001,
       199,       -1,       -1, G_STRING          ,   /* Object 198  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x000F, 0x0003, 0x0001,
       200,       -1,       -1, G_STRING          ,   /* Object 199  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x000F, 0x0003, 0x0001,
       201,       -1,       -1, G_STRING          ,   /* Object 200  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x000F, 0x0003, 0x0001,
       202,       -1,       -1, G_STRING          ,   /* Object 201  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x000F, 0x0003, 0x0001,
       203,       -1,       -1, G_STRING          ,   /* Object 202  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x000F, 0x0003, 0x0001,
       204,       -1,       -1, G_STRING          ,   /* Object 203  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x000F, 0x0003, 0x0001,
       205,       -1,       -1, G_STRING          ,   /* Object 204  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x000F, 0x0003, 0x0001,
       206,       -1,       -1, G_STRING          ,   /* Object 205  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x000F, 0x0003, 0x0001,
       207,       -1,       -1, G_STRING          ,   /* Object 206  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x000F, 0x0003, 0x0001,
       208,       -1,       -1, G_STRING          ,   /* Object 207  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x000F, 0x0003, 0x0001,
       209,       -1,       -1, G_STRING          ,   /* Object 208  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x000F, 0x0003, 0x0001,
       210,       -1,       -1, G_STRING          ,   /* Object 209  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x000F, 0x0003, 0x0001,
       211,       -1,       -1, G_STRING          ,   /* Object 210  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x000F, 0x0003, 0x0001,
       212,       -1,       -1, G_STRING          ,   /* Object 211  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0010, 0x0003, 0x0001,
       213,       -1,       -1, G_STRING          ,   /* Object 212  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0010, 0x0003, 0x0001,
       214,       -1,       -1, G_STRING          ,   /* Object 213  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0010, 0x0003, 0x0001,
       215,       -1,       -1, G_STRING          ,   /* Object 214  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0010, 0x0003, 0x0001,
       216,       -1,       -1, G_STRING          ,   /* Object 215  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0010, 0x0003, 0x0001,
       217,       -1,       -1, G_STRING          ,   /* Object 216  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0010, 0x0003, 0x0001,
       218,       -1,       -1, G_STRING          ,   /* Object 217  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0010, 0x0003, 0x0001,
       219,       -1,       -1, G_STRING          ,   /* Object 218  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0010, 0x0003, 0x0001,
       220,       -1,       -1, G_STRING          ,   /* Object 219  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0010, 0x0003, 0x0001,
       221,       -1,       -1, G_STRING          ,   /* Object 220  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0010, 0x0003, 0x0001,
       222,       -1,       -1, G_STRING          ,   /* Object 221  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0010, 0x0003, 0x0001,
       223,       -1,       -1, G_STRING          ,   /* Object 222  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0010, 0x0003, 0x0001,
       224,       -1,       -1, G_STRING          ,   /* Object 223  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0010, 0x0003, 0x0001,
       225,       -1,       -1, G_STRING          ,   /* Object 224  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0010, 0x0003, 0x0001,
       226,       -1,       -1, G_STRING          ,   /* Object 225  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0010, 0x0003, 0x0001,
       227,       -1,       -1, G_STRING          ,   /* Object 226  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0010, 0x0003, 0x0001,
       228,       -1,       -1, G_STRING          ,   /* Object 227  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0011, 0x0003, 0x0001,
       229,       -1,       -1, G_STRING          ,   /* Object 228  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0011, 0x0003, 0x0001,
       230,       -1,       -1, G_STRING          ,   /* Object 229  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0011, 0x0003, 0x0001,
       231,       -1,       -1, G_STRING          ,   /* Object 230  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0011, 0x0003, 0x0001,
       232,       -1,       -1, G_STRING          ,   /* Object 231  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0011, 0x0003, 0x0001,
       233,       -1,       -1, G_STRING          ,   /* Object 232  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0011, 0x0003, 0x0001,
       234,       -1,       -1, G_STRING          ,   /* Object 233  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0011, 0x0003, 0x0001,
       235,       -1,       -1, G_STRING          ,   /* Object 234  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0011, 0x0003, 0x0001,
       236,       -1,       -1, G_STRING          ,   /* Object 235  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0011, 0x0003, 0x0001,
       237,       -1,       -1, G_STRING          ,   /* Object 236  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0011, 0x0003, 0x0001,
       238,       -1,       -1, G_STRING          ,   /* Object 237  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0011, 0x0003, 0x0001,
       239,       -1,       -1, G_STRING          ,   /* Object 238  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0011, 0x0003, 0x0001,
       240,       -1,       -1, G_STRING          ,   /* Object 239  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0011, 0x0003, 0x0001,
       241,       -1,       -1, G_STRING          ,   /* Object 240  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0011, 0x0003, 0x0001,
       242,       -1,       -1, G_STRING          ,   /* Object 241  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0011, 0x0003, 0x0001,
       243,       -1,       -1, G_STRING          ,   /* Object 242  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0011, 0x0003, 0x0001,
       244,       -1,       -1, G_STRING          ,   /* Object 243  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0002, 0x0012, 0x0003, 0x0001,
       245,       -1,       -1, G_STRING          ,   /* Object 244  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0005, 0x0012, 0x0003, 0x0001,
       246,       -1,       -1, G_STRING          ,   /* Object 245  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0008, 0x0012, 0x0003, 0x0001,
       247,       -1,       -1, G_STRING          ,   /* Object 246  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000B, 0x0012, 0x0003, 0x0001,
       248,       -1,       -1, G_STRING          ,   /* Object 247  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x000E, 0x0012, 0x0003, 0x0001,
       249,       -1,       -1, G_STRING          ,   /* Object 248  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0011, 0x0012, 0x0003, 0x0001,
       250,       -1,       -1, G_STRING          ,   /* Object 249  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0014, 0x0012, 0x0003, 0x0001,
       251,       -1,       -1, G_STRING          ,   /* Object 250  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0017, 0x0012, 0x0003, 0x0001,
       252,       -1,       -1, G_STRING          ,   /* Object 251  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001A, 0x0012, 0x0003, 0x0001,
       253,       -1,       -1, G_STRING          ,   /* Object 252  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x001D, 0x0012, 0x0003, 0x0001,
       254,       -1,       -1, G_STRING          ,   /* Object 253  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0020, 0x0012, 0x0003, 0x0001,
       255,       -1,       -1, G_STRING          ,   /* Object 254  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0023, 0x0012, 0x0003, 0x0001,
       256,       -1,       -1, G_STRING          ,   /* Object 255  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0026, 0x0012, 0x0003, 0x0001,
       257,       -1,       -1, G_STRING          ,   /* Object 256  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x0029, 0x0012, 0x0003, 0x0001,
       258,       -1,       -1, G_STRING          ,   /* Object 257  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002C, 0x0012, 0x0003, 0x0001,
  CHARHELP        ,       -1,       -1, G_STRING          ,   /* Object 258  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)" M ",
  0x002F, 0x0012, 0x0003, 0x0001,
  CHAROK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 259 CHARHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[51],
  0x001F, 0x0014, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 260 CHAROK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[52],
  0x0029, 0x0014, 0x0008, 0x0001,
  
  /******** Tree 8 FSELBOX ****************************************************/
        -1, FSELTEXT        , FSELTEXT        , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0028, 0x0202,
         0,       -1,       -1, G_TEXT            ,   /* Object 1 FSELTEXT */
  LASTOB|FLAGS10, NORMAL, (LONG)&rs_tedinfo[53],
  0x0002, 0x0500, 0x0024, 0x0001,
  
  /******** Tree 9 SHARWARE ****************************************************/
        -1,        1, SHAREOK         , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0033, 0x0011,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[54],
  0x0031, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[55],
  0x0008, 0x0001, 0x0023, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Freeware means you may copy 7UP for",
  0x0003, 0x0003, 0x0023, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"non-commercial use.",
  0x0003, 0x0004, 0x0013, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"7UP ist  dar\201ber  hinaus ein offenes Projekt,",
  0x0003, 0x0006, 0x002D, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"Sie d\201rfen also selbst \216nderungen am Programm",
  0x0003, 0x0007, 0x002D, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"vornehmen.  Zwecks  Koordination sollten  Sie",
  0x0003, 0x0008, 0x002D, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"diese  \216nderungen aber  dokumentieren  und an",
  0x0003, 0x0009, 0x002D, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"folgende Adresse schicken:",
  0x0003, 0x000A, 0x001A, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"eMail: gerhard_stoll@b.maus.de",
  0x0008, 0x000C, 0x001E, 0x0001,
  SHAREOK         ,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"Thanks.",
  0x0004, 0x080E, 0x0007, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 12 SHAREOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[56],
  0x0029, 0x080E, 0x0008, 0x0001,
  
  /******** Tree 10 FONTSEL ****************************************************/
        -1,        1, FSOK            , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0028, 0x0014,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[57],
  0x0026, 0x0000, 0x0002, 0x0001,
  FSSHOW          ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[58],
  0x0003, 0x0001, 0x0004, 0x0001,
  FNTITLE         ,       -1,       -1, G_BOXTEXT         |0x0800,   /* Object 3 FSSHOW */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[59],
  0x0003, 0x0003, 0x0022, 0x0003,
  FSTITLE         ,       -1,       -1, G_BOXTEXT         ,   /* Object 4 FNTITLE */
  NONE, NORMAL, (LONG)&rs_tedinfo[60],
  0x0003, 0x0007, 0x0016, 0x0001,
         6,       -1,       -1, G_BOXTEXT         ,   /* Object 5 FSTITLE */
  NONE, NORMAL, (LONG)&rs_tedinfo[61],
  0x001C, 0x0007, 0x0009, 0x0001,
        12, FTYPE1          , FTYPE5          , G_BOX             ,   /* Object 6  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0008, 0x0014, 0x0007,
         8,       -1,       -1, G_TEXT            ,   /* Object 7 FTYPE1 */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[62],
  0x0001, 0x0001, 0x0012, 0x0001,
         9,       -1,       -1, G_TEXT            ,   /* Object 8  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[63],
  0x0001, 0x0002, 0x0012, 0x0001,
        10,       -1,       -1, G_TEXT            ,   /* Object 9  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[64],
  0x0001, 0x0003, 0x0012, 0x0001,
  FTYPE5          ,       -1,       -1, G_TEXT            ,   /* Object 10  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[65],
  0x0001, 0x0004, 0x0012, 0x0001,
         6,       -1,       -1, G_TEXT            ,   /* Object 11 FTYPE5 */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[66],
  0x0001, 0x0005, 0x0012, 0x0001,
        17, FTUP            , FTBOX           , G_BOX             ,   /* Object 12  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0017, 0x0008, 0x0002, 0x0007,
  FTDN            ,       -1,       -1, G_BOXCHAR         ,   /* Object 13 FTUP */
  TOUCHEXIT|FLAGS12, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  FTBOX           ,       -1,       -1, G_BOXCHAR         ,   /* Object 14 FTDN */
  TOUCHEXIT|FLAGS13, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x0000, 0x0006, 0x0002, 0x0001,
        12, FTSLIDE         , FTSLIDE         , G_BOX             ,   /* Object 15 FTBOX */
  TOUCHEXIT|FLAGS14, NORMAL, (LONG)0x00FF1111L,
  0x0000, 0x0001, 0x0002, 0x0005,
  FTBOX           ,       -1,       -1, G_BOX             ,   /* Object 16 FTSLIDE */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0003,
        23, FSIZE1          , FSIZE5          , G_BOX             ,   /* Object 17  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x001C, 0x0008, 0x0007, 0x0007,
        19,       -1,       -1, G_TEXT            ,   /* Object 18 FSIZE1 */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[67],
  0x0001, 0x0001, 0x0005, 0x0001,
        20,       -1,       -1, G_TEXT            ,   /* Object 19  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[68],
  0x0001, 0x0002, 0x0005, 0x0001,
        21,       -1,       -1, G_TEXT            ,   /* Object 20  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[69],
  0x0001, 0x0003, 0x0005, 0x0001,
  FSIZE5          ,       -1,       -1, G_TEXT            ,   /* Object 21  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[70],
  0x0001, 0x0004, 0x0005, 0x0001,
        17,       -1,       -1, G_TEXT            ,   /* Object 22 FSIZE5 */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[71],
  0x0001, 0x0005, 0x0005, 0x0001,
        28, FSUP            , FSBOX           , G_BOX             ,   /* Object 23  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0023, 0x0008, 0x0002, 0x0007,
  FSDN            ,       -1,       -1, G_BOXCHAR         ,   /* Object 24 FSUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  FSBOX           ,       -1,       -1, G_BOXCHAR         ,   /* Object 25 FSDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x0000, 0x0006, 0x0002, 0x0001,
        23, FSSLIDE         , FSSLIDE         , G_BOX             ,   /* Object 26 FSBOX */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1111L,
  0x0000, 0x0001, 0x0002, 0x0005,
  FSBOX           ,       -1,       -1, G_BOX             ,   /* Object 27 FSSLIDE */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0003,
        33, FTLF            , FTHBOX          , G_BOX             ,   /* Object 28  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x000F, 0x0014, 0x0001,
  FTRT            ,       -1,       -1, G_BOXCHAR         ,   /* Object 29 FTLF */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  FTHBOX          ,       -1,       -1, G_BOXCHAR         ,   /* Object 30 FTRT */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0012, 0x0000, 0x0002, 0x0001,
        28, FTHSLD          , FTHSLD          , G_BOX             ,   /* Object 31 FTHBOX */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1111L,
  0x0002, 0x0000, 0x0010, 0x0001,
  FTHBOX          ,       -1,       -1, G_BOX             ,   /* Object 32 FTHSLD */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0006, 0x0001,
  FSARBPT         ,       -1,       -1, G_BOXCHAR         ,   /* Object 33  */
  NONE, NORMAL, (LONG) ((LONG)'\0' << 24)|0x00FF1100L,
  0x0017, 0x000F, 0x0002, 0x0001,
  FSHELP          ,       -1,       -1, G_FBOXTEXT        ,   /* Object 34 FSARBPT */
  EDITABLE|TOUCHEXIT|FLAGS10, NORMAL, (LONG)&rs_tedinfo[72],
  0x001C, 0x080F, 0x0009, 0x0001,
  FSABBR          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 35 FSHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[73],
  0x0009, 0x0012, 0x0008, 0x0001,
  FSOK            ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 36 FSABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[74],
  0x0013, 0x0012, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 37 FSOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[75],
  0x001D, 0x0012, 0x0008, 0x0001,
  
  /******** Tree 11 PRTMENU ****************************************************/
        -1,        1, PRTESC          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002E, 0x0009,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[76],
  0x002C, 0x0000, 0x0002, 0x0001,
  PRTNAME         ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[77],
  0x0003, 0x0001, 0x000C, 0x0001,
  PRTCOPY         ,       -1,       -1, G_TEXT            ,   /* Object 3 PRTNAME */
  NONE, NORMAL, (LONG)&rs_tedinfo[78],
  0x0003, 0x0003, 0x0028, 0x0001,
  PRTPAGE         ,       -1,       -1, G_FTEXT           ,   /* Object 4 PRTCOPY */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[79],
  0x0003, 0x0005, 0x0009, 0x0001,
  PRTESC          ,       -1,       -1, G_FTEXT           ,   /* Object 5 PRTPAGE */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[80],
  0x0020, 0x0005, 0x000A, 0x0001,
         0,       -1,       -1, G_BOXTEXT         ,   /* Object 6 PRTESC */
  LASTOB, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[81],
  0x0003, 0x0007, 0x0005, 0x0001,
  
  /******** Tree 12 USERIMG ****************************************************/
        -1, RBLNORM         , WICON_KLEIN     , G_BOX             ,   /* Object 0  */
  FLAGS9, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0028, 0x000D,
  RBLSEL          ,       -1,       -1, G_IMAGE           ,   /* Object 1 RBLNORM */
  NONE, NORMAL, (LONG)&rs_bitblk[2],
  0x0002, 0x0001, 0x0002, 0x0800,
  RBHNORM         ,       -1,       -1, G_IMAGE           ,   /* Object 2 RBLSEL */
  NONE, NORMAL, (LONG)&rs_bitblk[3],
  0x0005, 0x0001, 0x0002, 0x0800,
  RBHSEL          ,       -1,       -1, G_IMAGE           ,   /* Object 3 RBHNORM */
  NONE, NORMAL, (LONG)&rs_bitblk[4],
  0x0009, 0x0001, 0x0002, 0x0001,
  RBBNORM         ,       -1,       -1, G_IMAGE           ,   /* Object 4 RBHSEL */
  NONE, NORMAL, (LONG)&rs_bitblk[5],
  0x000C, 0x0001, 0x0002, 0x0001,
  RBBSEL          ,       -1,       -1, G_IMAGE           ,   /* Object 5 RBBNORM */
  NONE, NORMAL, (LONG)&rs_bitblk[6],
  0x0011, 0x0001, 0x0004, 0x0002,
  OBJ0            ,       -1,       -1, G_IMAGE           ,   /* Object 6 RBBSEL */
  NONE, NORMAL, (LONG)&rs_bitblk[7],
  0x0015, 0x0001, 0x0004, 0x0002,
  RADIOBUT        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 7 OBJ0 */
  NONE, NORMAL, (LONG)&rs_tedinfo[82],
  0x0002, 0x0003, 0x000E, 0x0001,
  CHECKBUT        ,       -1,       -1, G_BOX             |0x6000,   /* Object 8 RADIOBUT */
  SELECTABLE|RBUTTON, NORMAL, (LONG)0x00011101L,
  0x0012, 0x0003, 0x0002, 0x0001,
        10,       -1,       -1, G_BOX             |0x4000,   /* Object 9 CHECKBUT */
  SELECTABLE, NORMAL, (LONG)0x00011101L,
  0x0016, 0x0003, 0x0002, 0x0001,
  CIRCLEL         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 10  */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[83],
  0x0002, 0x0005, 0x000E, 0x0001,
  CIRCLEH         ,       -1,       -1, G_IMAGE           ,   /* Object 11 CIRCLEL */
  NONE, NORMAL, (LONG)&rs_bitblk[8],
  0x0012, 0x0005, 0x0002, 0x0800,
  CIRCLEB         ,       -1,       -1, G_IMAGE           ,   /* Object 12 CIRCLEH */
  NONE, NORMAL, (LONG)&rs_bitblk[9],
  0x0016, 0x0005, 0x0002, 0x0001,
        14,       -1,       -1, G_IMAGE           ,   /* Object 13 CIRCLEB */
  NONE, NORMAL, (LONG)&rs_bitblk[10],
  0x0012, 0x0006, 0x0004, 0x0002,
        15,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 14  */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[84],
  0x0002, 0x0007, 0x000B, 0x0001,
        16,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 15  */
  NONE, NORMAL, (LONG)&rs_tedinfo[85],
  0x0002, 0x0009, 0x0008, 0x0001,
        17,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 16  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[86],
  0x000E, 0x0009, 0x0002, 0x0001,
        18,       -1,       -1, G_IMAGE           ,   /* Object 17  */
  NONE, NORMAL, (LONG)&rs_bitblk[11],
  0x0014, 0x0009, 0x0002, 0x0001,
        19,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 18  */
  NONE, NORMAL, (LONG)&rs_tedinfo[87],
  0x0002, 0x000B, 0x0008, 0x0001,
  WICON           ,       -1,       -1, G_BOXTEXT         |0x0600,   /* Object 19  */
  NONE, NORMAL, (LONG)&rs_tedinfo[88],
  0x000E, 0x000B, 0x0008, 0x0001,
  RBHNORM3D       ,       -1,       -1, G_IMAGE           ,   /* Object 20 WICON */
  NONE, NORMAL, (LONG)&rs_bitblk[12],
  0x001D, 0x0800, 0x000A, 0x0804,
  RBHSEL3D        ,       -1,       -1, G_IMAGE           ,   /* Object 21 RBHNORM3D */
  NONE, NORMAL, (LONG)&rs_bitblk[13],
  0x0009, 0x0002, 0x0002, 0x0001,
  CIRCLEHDIS      ,       -1,       -1, G_IMAGE           ,   /* Object 22 RBHSEL3D */
  NONE, NORMAL, (LONG)&rs_bitblk[14],
  0x000C, 0x0002, 0x0002, 0x0001,
  RBHBG3D         ,       -1,       -1, G_IMAGE           ,   /* Object 23 CIRCLEHDIS */
  NONE, NORMAL, (LONG)&rs_bitblk[15],
  0x001A, 0x0005, 0x0002, 0x0001,
  WICON_KLEIN     ,       -1,       -1, G_IMAGE           ,   /* Object 24 RBHBG3D */
  NONE, NORMAL, (LONG)&rs_bitblk[16],
  0x000F, 0x0002, 0x0002, 0x0001,
         0,       -1,       -1, G_IMAGE           ,   /* Object 25 WICON_KLEIN */
  LASTOB, NORMAL, (LONG)&rs_bitblk[17],
  0x041E, 0x0008, 0x0008, 0x0A02,
  
  /******** Tree 13 SHELL2 ****************************************************/
        -1,        1, SH2ABBR         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0041, 0x0012,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[89],
  0x003F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[90],
  0x0003, 0x0001, 0x0010, 0x0001,
  SH2PRG          ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Program:",
  0x0003, 0x0003, 0x0008, 0x0001,
         5,       -1,       -1, G_BOXTEXT         ,   /* Object 4 SH2PRG */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[91],
  0x000E, 0x0003, 0x0030, 0x0001,
  SH2PARAM        ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Parameter:",
  0x0003, 0x0005, 0x000A, 0x0001,
         7,       -1,       -1, G_BOXTEXT         ,   /* Object 6 SH2PARAM */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[92],
  0x000E, 0x0005, 0x0030, 0x0001,
        12,        8, SH2ALLR         , G_BOX             ,   /* Object 7  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0007, 0x001D, 0x0005,
  SH2GEM          ,       -1,       -1, G_TEXT            |0x2000,   /* Object 8  */
  NONE, NORMAL, (LONG)&rs_tedinfo[93],
  0x0001, 0x0000, 0x0009, 0x0001,
  SH2DOS          ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 9 SH2GEM */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[94],
  0x0002, 0x0001, 0x0006, 0x0001,
  SH2ALLR         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 10 SH2DOS */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[95],
  0x0002, 0x0002, 0x0006, 0x0001,
         7,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 11 SH2ALLR */
  SELECTABLE|EXIT, DISABLED, (LONG)&rs_tedinfo[96],
  0x0002, 0x0003, 0x0019, 0x0001,
        16,       13, SH2AKTP         , G_BOX             ,   /* Object 12  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0021, 0x0007, 0x001D, 0x0005,
  SH2NOP          ,       -1,       -1, G_TEXT            |0x2000,   /* Object 13  */
  NONE, NORMAL, (LONG)&rs_tedinfo[97],
  0x0001, 0x0000, 0x000E, 0x0001,
  SH2AKTP         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 14 SH2NOP */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[98],
  0x0002, 0x0001, 0x0012, 0x0001,
        12,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 15 SH2AKTP */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[99],
  0x0002, 0x0002, 0x0012, 0x0001,
  SH2OK           ,       17, SH2BACK         , G_BOX             ,   /* Object 16  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x000D, 0x001D, 0x0004,
  SH2SAVE         ,       -1,       -1, G_TEXT            |0x2000,   /* Object 17  */
  NONE, NORMAL, (LONG)&rs_tedinfo[100],
  0x0001, 0x0000, 0x0009, 0x0001,
  SH2BACK         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 18 SH2SAVE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[101],
  0x0002, 0x0001, 0x0018, 0x0001,
        16,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 19 SH2BACK */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[102],
  0x0002, 0x0002, 0x0012, 0x0001,
  SH2ABBR         ,       -1,       -1, G_BUTTON          ,   /* Object 20 SH2OK */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"OK",
  0x0036, 0x000E, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 21 SH2ABBR */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)"Cancel",
  0x0036, 0x0010, 0x0008, 0x0001,
  
  /******** Tree 14 FKEYMENU ****************************************************/
        -1,        1, FKEYOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002C, 0x0017,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[103],
  0x002A, 0x0000, 0x0002, 0x0001,
  FKSHIFT         ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[104],
  0x0003, 0x0001, 0x0008, 0x0001,
  FKNORM          , SFKEY1          , SFKEY10         , G_BOX             ,   /* Object 3 FKSHIFT */
  FLAGS10, NORMAL, (LONG)0x00001100L,
  0x0003, 0x0003, 0x0025, 0x000F,
         5,       -1,       -1, G_FTEXT           ,   /* Object 4 SFKEY1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[105],
  0x0001, 0x0000, 0x0024, 0x0001,
         6,       -1,       -1, G_FTEXT           ,   /* Object 5  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[106],
  0x0001, 0x0801, 0x0024, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[107],
  0x0001, 0x0003, 0x0024, 0x0001,
         8,       -1,       -1, G_FTEXT           ,   /* Object 7  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[108],
  0x0001, 0x0804, 0x0024, 0x0001,
         9,       -1,       -1, G_FTEXT           ,   /* Object 8  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[109],
  0x0001, 0x0006, 0x0024, 0x0001,
        10,       -1,       -1, G_FTEXT           ,   /* Object 9  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[110],
  0x0001, 0x0807, 0x0024, 0x0001,
        11,       -1,       -1, G_FTEXT           ,   /* Object 10  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[111],
  0x0001, 0x0009, 0x0024, 0x0001,
        12,       -1,       -1, G_FTEXT           ,   /* Object 11  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[112],
  0x0001, 0x080A, 0x0024, 0x0001,
  SFKEY10         ,       -1,       -1, G_FTEXT           ,   /* Object 12  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[113],
  0x0001, 0x000C, 0x0024, 0x0001,
  FKSHIFT         ,       -1,       -1, G_FTEXT           ,   /* Object 13 SFKEY10 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[114],
  0x0000, 0x080D, 0x0025, 0x0001,
  FKEYLOAD        , FKEY1           , FKEY10          , G_BOX             ,   /* Object 14 FKNORM */
  FLAGS10, NORMAL, (LONG)0x00001100L,
  0x0003, 0x0003, 0x0026, 0x000F,
        16,       -1,       -1, G_FTEXT           ,   /* Object 15 FKEY1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[115],
  0x0002, 0x0000, 0x0023, 0x0001,
        17,       -1,       -1, G_FTEXT           ,   /* Object 16  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[116],
  0x0002, 0x0801, 0x0023, 0x0001,
        18,       -1,       -1, G_FTEXT           ,   /* Object 17  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[117],
  0x0002, 0x0003, 0x0023, 0x0001,
        19,       -1,       -1, G_FTEXT           ,   /* Object 18  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[118],
  0x0002, 0x0804, 0x0023, 0x0001,
        20,       -1,       -1, G_FTEXT           ,   /* Object 19  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[119],
  0x0002, 0x0006, 0x0023, 0x0001,
        21,       -1,       -1, G_FTEXT           ,   /* Object 20  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[120],
  0x0002, 0x0807, 0x0023, 0x0001,
        22,       -1,       -1, G_FTEXT           ,   /* Object 21  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[121],
  0x0002, 0x0009, 0x0023, 0x0001,
        23,       -1,       -1, G_FTEXT           ,   /* Object 22  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[122],
  0x0002, 0x080A, 0x0023, 0x0001,
  FKEY10          ,       -1,       -1, G_FTEXT           ,   /* Object 23  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[123],
  0x0002, 0x000C, 0x0023, 0x0001,
  FKNORM          ,       -1,       -1, G_FTEXT           ,   /* Object 24 FKEY10 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[124],
  0x0001, 0x080D, 0x0024, 0x0001,
  FKEYSHFT        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 25 FKEYLOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[125],
  0x0003, 0x0013, 0x000C, 0x0001,
  FKEYHELP        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 26 FKEYSHFT */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[126],
  0x0017, 0x0013, 0x0008, 0x0001,
  FKEYSAVE        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 27 FKEYHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[127],
  0x0021, 0x0013, 0x0008, 0x0001,
  FKEYABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 28 FKEYSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[128],
  0x0003, 0x0015, 0x000C, 0x0001,
  FKEYOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 29 FKEYABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[129],
  0x0017, 0x0015, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 30 FKEYOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[130],
  0x0021, 0x0015, 0x0008, 0x0001,
  
  /******** Tree 15 UMBRMENU ****************************************************/
        -1,        1, UMBROK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0018, 0x0007,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[131],
  0x0016, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[132],
  0x0003, 0x0001, 0x000B, 0x0001,
  UMBRUCH         ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Lenght:",
  0x0003, 0x0003, 0x0007, 0x0001,
  UMBRABBR        ,       -1,       -1, G_FTEXT           ,   /* Object 4 UMBRUCH */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[133],
  0x000A, 0x0003, 0x0003, 0x0001,
  UMBROK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 5 UMBRABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[134],
  0x0003, 0x0005, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 6 UMBROK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[135],
  0x000D, 0x0005, 0x0008, 0x0001,
  
  /******** Tree 16 PRNINST ****************************************************/
        -1,        1, PRNOK           , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002F, 0x0019,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[136],
  0x002D, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[137],
  0x0003, 0x0001, 0x000B, 0x0001,
  PRNLAY          ,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[138],
  0x001F, 0x0001, 0x000D, 0x0001,
         5,       -1,       -1, G_TEXT            ,   /* Object 4 PRNLAY */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[139],
  0x0020, 0x0002, 0x000C, 0x0001,
  PRNKZSTR        ,       -1,       -1, G_TEXT            ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_tedinfo[140],
  0x0003, 0x0003, 0x0205, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 PRNKZSTR */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[141],
  0x0003, 0x0004, 0x001E, 0x0001,
        11, PRNKZL          , PRNKZR          , G_BOX             ,   /* Object 7  */
  FLAGS10, NORMAL, (LONG)0x00001100L,
  0x0024, 0x0004, 0x0008, 0x0001,
  PRNKZM          ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 8 PRNKZL */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[142],
  0x0000, 0x0000, 0x0002, 0x0001,
  PRNKZR          ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 9 PRNKZM */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[143],
  0x0003, 0x0000, 0x0002, 0x0001,
         7,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 10 PRNKZR */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[144],
  0x0006, 0x0000, 0x0002, 0x0001,
        12,       -1,       -1, G_TEXT            ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[145],
  0x0003, 0x0005, 0x0205, 0x0001,
  PRNFZSTR        ,       -1,       -1, G_TEXT            ,   /* Object 12  */
  NONE, NORMAL, (LONG)&rs_tedinfo[146],
  0x0024, 0x0005, 0x0008, 0x0001,
        14,       -1,       -1, G_FTEXT           ,   /* Object 13 PRNFZSTR */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[147],
  0x0003, 0x0006, 0x001E, 0x0001,
        18, PRNFZL          , PRNFZR          , G_BOX             ,   /* Object 14  */
  FLAGS10, NORMAL, (LONG)0x00001100L,
  0x0024, 0x0006, 0x0008, 0x0001,
  PRNFZM          ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 15 PRNFZL */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[148],
  0x0000, 0x0000, 0x0002, 0x0001,
  PRNFZR          ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 16 PRNFZM */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[149],
  0x0003, 0x0000, 0x0002, 0x0001,
        14,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 17 PRNFZR */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[150],
  0x0006, 0x0000, 0x0002, 0x0001,
        19,       -1,       -1, G_TEXT            ,   /* Object 18  */
  NONE, NORMAL, (LONG)&rs_tedinfo[151],
  0x0003, 0x0008, 0x0208, 0x0001,
        20,       -1,       -1, G_TEXT            ,   /* Object 19  */
  NONE, NORMAL, (LONG)&rs_tedinfo[152],
  0x0013, 0x0008, 0x0606, 0x0001,
  PRNPAPER        ,       -1,       -1, G_TEXT            ,   /* Object 20  */
  NONE, NORMAL, (LONG)&rs_tedinfo[153],
  0x0003, 0x000B, 0x0006, 0x0001,
  PRNCIRC1        ,       -1,       -1, G_BUTTON          ,   /* Object 21 PRNPAPER */
  TOUCHEXIT, SHADOWED, (LONG)"  Default   ",
  0x0003, 0x0009, 0x000C, 0x0001,
  PRNDIST         ,       -1,       -1, G_BOXTEXT         |0x0600,   /* Object 22 PRNCIRC1 */
  NONE, SHADOWED, (LONG)&rs_tedinfo[154],
  0x000F, 0x0009, 0x0002, 0x0001,
  PRNCIRC3        ,       -1,       -1, G_BUTTON          ,   /* Object 23 PRNDIST */
  TOUCHEXIT, SHADOWED, (LONG)"  1  ",
  0x0013, 0x0009, 0x0005, 0x0001,
  PRNDRUCK        ,       -1,       -1, G_BOXTEXT         |0x0600,   /* Object 24 PRNCIRC3 */
  NONE, SHADOWED, (LONG)&rs_tedinfo[155],
  0x0018, 0x0009, 0x0002, 0x0001,
  PRNCIRC2        ,       -1,       -1, G_BUTTON          ,   /* Object 25 PRNDRUCK */
  TOUCHEXIT, SHADOWED, (LONG)"  undefiniert            ",
  0x0003, 0x000C, 0x0019, 0x0001,
        27,       -1,       -1, G_BOXCHAR         ,   /* Object 26 PRNCIRC2 */
  SELECTABLE|EXIT, SHADOWED, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x001C, 0x000C, 0x0002, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  NONE, NORMAL, (LONG)"Layout",
  0x0023, 0x000D, 0x0006, 0x0001,
  PRNLOAD         ,       29,       60, G_BOX             |0x0900,   /* Object 28  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x000E, 0x001A, 0x000A,
        30,       -1,       -1, G_TEXT            |0x2000,   /* Object 29  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[156],
  0x0001, 0x0000, 0x000D, 0x0001,
        53, PRNZLDN         , PRNZZ           , G_BOX             ,   /* Object 30  */
  FLAGS10, NORMAL, (LONG)0x00001100L,
  0x0001, 0x0001, 0x000A, 0x0008,
  PRNZL           ,       -1,       -1, G_BOXCHAR         ,   /* Object 31 PRNZLDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0000, 0x0002, 0x0001,
  PRNZLUP         ,       -1,       -1, G_TEXT            ,   /* Object 32 PRNZL */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[157],
  0x0004, 0x0000, 0x0003, 0x0001,
  PRNLRDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 33 PRNZLUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0000, 0x0002, 0x0001,
  PRNLR           ,       -1,       -1, G_BOXCHAR         ,   /* Object 34 PRNLRDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0001, 0x0002, 0x0001,
  PRNLRUP         ,       -1,       -1, G_TEXT            ,   /* Object 35 PRNLR */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[158],
  0x0005, 0x0001, 0x0002, 0x0001,
  PRNBLDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 36 PRNLRUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0001, 0x0002, 0x0001,
  PRNBL           ,       -1,       -1, G_BOXCHAR         ,   /* Object 37 PRNBLDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0002, 0x0002, 0x0001,
  PRNBLUP         ,       -1,       -1, G_TEXT            ,   /* Object 38 PRNBL */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[159],
  0x0004, 0x0002, 0x0003, 0x0001,
  PRNORDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 39 PRNBLUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0002, 0x0002, 0x0001,
  PRNOR           ,       -1,       -1, G_BOXCHAR         ,   /* Object 40 PRNORDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0003, 0x0002, 0x0001,
  PRNORUP         ,       -1,       -1, G_TEXT            ,   /* Object 41 PRNOR */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[160],
  0x0005, 0x0003, 0x0002, 0x0001,
  PRNKZDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 42 PRNORUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0003, 0x0002, 0x0001,
  PRNKZ           ,       -1,       -1, G_BOXCHAR         ,   /* Object 43 PRNKZDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0004, 0x0002, 0x0001,
  PRNKZUP         ,       -1,       -1, G_TEXT            ,   /* Object 44 PRNKZ */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[161],
  0x0005, 0x0004, 0x0002, 0x0001,
  PRNFZDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 45 PRNKZUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0004, 0x0002, 0x0001,
  PRNFZ           ,       -1,       -1, G_BOXCHAR         ,   /* Object 46 PRNFZDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0005, 0x0002, 0x0001,
  PRNFZUP         ,       -1,       -1, G_TEXT            ,   /* Object 47 PRNFZ */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[162],
  0x0005, 0x0005, 0x0002, 0x0001,
  PRNURDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 48 PRNFZUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0005, 0x0002, 0x0001,
  PRNUR           ,       -1,       -1, G_BOXCHAR         ,   /* Object 49 PRNURDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0001, 0x0006, 0x0002, 0x0001,
  PRNURUP         ,       -1,       -1, G_TEXT            ,   /* Object 50 PRNUR */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[163],
  0x0005, 0x0006, 0x0002, 0x0001,
  PRNZZ           ,       -1,       -1, G_BOXCHAR         ,   /* Object 51 PRNURUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0008, 0x0006, 0x0002, 0x0001,
        30,       -1,       -1, G_TEXT            ,   /* Object 52 PRNZZ */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[164],
  0x0004, 0x0007, 0x0003, 0x0001,
        54,       -1,       -1, G_STRING          ,   /* Object 53  */
  NONE, NORMAL, (LONG)"Chars/line",
  0x000C, 0x0001, 0x000A, 0x0001,
        55,       -1,       -1, G_STRING          ,   /* Object 54  */
  NONE, NORMAL, (LONG)"Left border",
  0x000C, 0x0002, 0x000B, 0x0001,
        56,       -1,       -1, G_STRING          ,   /* Object 55  */
  NONE, NORMAL, (LONG)"Lines/page",
  0x000C, 0x0003, 0x000A, 0x0001,
        57,       -1,       -1, G_STRING          ,   /* Object 56  */
  NONE, NORMAL, (LONG)"Upper border",
  0x000C, 0x0004, 0x000C, 0x0001,
        58,       -1,       -1, G_STRING          ,   /* Object 57  */
  NONE, NORMAL, (LONG)"Header",
  0x000C, 0x0005, 0x0006, 0x0001,
        59,       -1,       -1, G_STRING          ,   /* Object 58  */
  NONE, NORMAL, (LONG)"Footer",
  0x000C, 0x0006, 0x0006, 0x0001,
        60,       -1,       -1, G_STRING          ,   /* Object 59  */
  NONE, NORMAL, (LONG)"Lower border",
  0x000C, 0x0007, 0x000C, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 60  */
  NONE, NORMAL, (LONG)"Lines/page",
  0x000C, 0x0008, 0x000A, 0x0001,
  PRNSAVE         ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 61 PRNLOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[165],
  0x0021, 0x000F, 0x000B, 0x0001,
  PRNHELP         ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 62 PRNSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[166],
  0x0021, 0x0011, 0x000B, 0x0001,
  PRNABBR         ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 63 PRNHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[167],
  0x0024, 0x0013, 0x0008, 0x0001,
  PRNOK           ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 64 PRNABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[168],
  0x0024, 0x0015, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 65 PRNOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[169],
  0x0024, 0x0017, 0x0008, 0x0001,
  
  /******** Tree 17 PRNINST2 ****************************************************/
        -1,        1, PRN2OK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004A, 0x0016,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[170],
  0x0048, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[171],
  0x0003, 0x0001, 0x0005, 0x0001,
        17,        4, PRNNUM          , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x001D, 0x000B,
         5,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[172],
  0x0001, 0x0000, 0x000B, 0x0001,
        11, PRNPGOFF        , PRNLCOL         , G_BOX             ,   /* Object 5  */
  FLAGS10, NORMAL, (LONG)0x00001100L,
  0x0001, 0x0001, 0x0006, 0x0807,
  PRNFROM         ,       -1,       -1, G_FTEXT           ,   /* Object 6 PRNPGOFF */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[173],
  0x0001, 0x0000, 0x0004, 0x0001,
  PRNTO           ,       -1,       -1, G_FTEXT           ,   /* Object 7 PRNFROM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[174],
  0x0001, 0x0801, 0x0004, 0x0001,
  PRNFCOL         ,       -1,       -1, G_FTEXT           ,   /* Object 8 PRNTO */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[175],
  0x0001, 0x0003, 0x0004, 0x0001,
  PRNLCOL         ,       -1,       -1, G_FTEXT           ,   /* Object 9 PRNFCOL */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[176],
  0x0002, 0x0804, 0x0003, 0x0001,
         5,       -1,       -1, G_FTEXT           ,   /* Object 10 PRNLCOL */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[177],
  0x0002, 0x0006, 0x0003, 0x0001,
        12,       -1,       -1, G_TEXT            ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[178],
  0x0007, 0x0001, 0x0012, 0x0001,
        13,       -1,       -1, G_TEXT            ,   /* Object 12  */
  NONE, NORMAL, (LONG)&rs_tedinfo[179],
  0x0007, 0x0802, 0x000A, 0x0001,
        14,       -1,       -1, G_TEXT            ,   /* Object 13  */
  NONE, NORMAL, (LONG)&rs_tedinfo[180],
  0x0007, 0x0004, 0x0009, 0x0001,
        15,       -1,       -1, G_TEXT            ,   /* Object 14  */
  NONE, NORMAL, (LONG)&rs_tedinfo[181],
  0x0007, 0x0805, 0x000C, 0x0001,
  PRNNUM          ,       -1,       -1, G_TEXT            ,   /* Object 15  */
  NONE, NORMAL, (LONG)&rs_tedinfo[182],
  0x0007, 0x0007, 0x000B, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 16 PRNNUM */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[183],
  0x0004, 0x0808, 0x0012, 0x0001,
        28,       18, PRNNOFORM       , G_BOX             |0x0900,   /* Object 17  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0022, 0x0003, 0x0025, 0x000B,
  PRNKFBEG        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 18  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[184],
  0x0001, 0x0000, 0x0007, 0x0001,
  PRNALT          ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 19 PRNKFBEG */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[185],
  0x0002, 0x0001, 0x0022, 0x0001,
  PRNCUT          ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 20 PRNALT */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[186],
  0x0002, 0x0002, 0x001B, 0x0001,
  PRNALLP         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 21 PRNCUT */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[187],
  0x0002, 0x0003, 0x0019, 0x0001,
  PRNPAIR         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 22 PRNALLP */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[188],
  0x0002, 0x0004, 0x0016, 0x0001,
  PRNUNPA         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 23 PRNPAIR */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[189],
  0x0002, 0x0005, 0x001C, 0x0001,
  PRNFF           ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 24 PRNUNPA */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[190],
  0x0002, 0x0006, 0x001E, 0x0001,
  PRNPAUSE        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 25 PRNFF */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[191],
  0x0002, 0x0007, 0x001D, 0x0001,
  PRNNOFORM       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 26 PRNPAUSE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[192],
  0x0002, 0x0008, 0x0019, 0x0001,
        17,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 27 PRNNOFORM */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[193],
  0x0002, 0x0009, 0x001C, 0x0001,
  PRN2HELP        ,       29, PRNCOPY         , G_BOX             |0x0900,   /* Object 28  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x000F, 0x001D, 0x0006,
  PRNFILE         ,       -1,       -1, G_TEXT            |0x2000,   /* Object 29  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[194],
  0x0001, 0x0000, 0x0009, 0x0001,
  PRNBACK         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 30 PRNFILE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[195],
  0x0002, 0x0001, 0x0016, 0x0001,
  PRNDEL          ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 31 PRNBACK */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[196],
  0x0002, 0x0002, 0x0019, 0x0001,
        33,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 32 PRNDEL */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[197],
  0x0002, 0x0003, 0x0015, 0x0001,
  PRNCOPY         ,       -1,       -1, G_TEXT            ,   /* Object 33  */
  NONE, NORMAL, (LONG)&rs_tedinfo[198],
  0x0005, 0x0804, 0x0006, 0x0001,
        28,       -1,       -1, G_FTEXT           ,   /* Object 34 PRNCOPY */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[199],
  0x0002, 0x0804, 0x0002, 0x0001,
  PRN2ABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 35 PRN2HELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[200],
  0x002B, 0x0014, 0x0008, 0x0001,
  PRN2OK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 36 PRN2ABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[201],
  0x0035, 0x0014, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 37 PRN2OK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[202],
  0x003F, 0x0014, 0x0008, 0x0001,
  
  /******** Tree 18 PREVIEW ****************************************************/
        -1,        1, PREVOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0046, 0x0017,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[203],
  0x0044, 0x0000, 0x0002, 0x0001,
  PREVNAME        ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[204],
  0x0003, 0x0001, 0x000C, 0x0001,
  PREVPAGE        ,       -1,       -1, G_TEXT            ,   /* Object 3 PREVNAME */
  NONE, NORMAL, (LONG)&rs_tedinfo[205],
  0x0011, 0x0001, 0x0032, 0x0001,
  PREVHELP        ,       -1,       -1, G_BOX             ,   /* Object 4 PREVPAGE */
  TOUCHEXIT|FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0002, 0x0037, 0x0014,
  PREVABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 5 PREVHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[206],
  0x003B, 0x0011, 0x0008, 0x0001,
  PREVOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 6 PREVABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[207],
  0x003B, 0x0013, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 7 PREVOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[208],
  0x003B, 0x0015, 0x0008, 0x0001,
  
  /******** Tree 19 REPLMENU ****************************************************/
        -1,        1, REPLJA          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0041, 0x0008,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[209],
  0x003F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[210],
  0x0003, 0x0001, 0x0008, 0x0001,
  REPLSTR         ,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[211],
  0x0003, 0x0003, 0x0609, 0x0001,
  REPLNEIN        ,       -1,       -1, G_FTEXT           ,   /* Object 4 REPLSTR */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[212],
  0x0003, 0x0004, 0x003B, 0x0001,
  REPLABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 5 REPLNEIN */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[213],
  0x0022, 0x0006, 0x0008, 0x0001,
  REPLJA          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 6 REPLABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[214],
  0x002C, 0x0006, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 7 REPLJA */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[215],
  0x0036, 0x0006, 0x0008, 0x0001,
  
  /******** Tree 20 INFOMENU ****************************************************/
        -1,        1, INFOOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002F, 0x0014,
  INFOTITL        ,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[216],
  0x002D, 0x0000, 0x0002, 0x0001,
  INFOEDIT        ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2 INFOTITL */
  NONE, NORMAL, (LONG)&rs_tedinfo[217],
  0x0003, 0x0001, 0x0010, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3 INFOEDIT */
  NONE, NORMAL, (LONG)"_",
  0x000A, 0x0003, 0x0001, 0x0001,
  INFOZUR         ,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"Name:",
  0x000C, 0x0003, 0x0005, 0x0001,
  INFOTNAME       ,       -1,       -1, G_BOXCHAR         ,   /* Object 5 INFOZUR */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0012, 0x0003, 0x0002, 0x0001,
  INFOMNAME       ,       -1,       -1, G_FTEXT           ,   /* Object 6 INFOTNAME */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[218],
  0x0015, 0x0003, 0x000C, 0x0001,
  INFOVOR         ,       -1,       -1, G_FTEXT           ,   /* Object 7 INFOMNAME */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[219],
  0x0015, 0x0003, 0x0014, 0x0001,
         9,       -1,       -1, G_BOXCHAR         ,   /* Object 8 INFOVOR */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x002A, 0x0003, 0x0002, 0x0001,
  INFODATE        ,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"Date:",
  0x000C, 0x0005, 0x0005, 0x0001,
        11,       -1,       -1, G_FTEXT           ,   /* Object 10 INFODATE */
  NONE, NORMAL, (LONG)&rs_tedinfo[220],
  0x0015, 0x0005, 0x0008, 0x0001,
  INFOTIME        ,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"Time:",
  0x000C, 0x0006, 0x0005, 0x0001,
        13,       -1,       -1, G_FTEXT           ,   /* Object 12 INFOTIME */
  NONE, NORMAL, (LONG)&rs_tedinfo[221],
  0x0015, 0x0006, 0x0008, 0x0001,
  INFOSIZE        ,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"Size:",
  0x000C, 0x0007, 0x0005, 0x0001,
        15,       -1,       -1, G_FTEXT           ,   /* Object 14 INFOSIZE */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[222],
  0x0015, 0x0007, 0x0008, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"Bytes",
  0x001E, 0x0007, 0x0005, 0x0001,
  INFOLINE        ,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"Lines:",
  0x000B, 0x0008, 0x0006, 0x0001,
        18,       -1,       -1, G_FTEXT           ,   /* Object 17 INFOLINE */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[223],
  0x0018, 0x0008, 0x0005, 0x0001,
  INFOPAGE        ,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"Pages:",
  0x000B, 0x0009, 0x0006, 0x0001,
        20,       -1,       -1, G_FTEXT           ,   /* Object 19 INFOPAGE */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[224],
  0x0018, 0x0009, 0x0005, 0x0001,
  INFOLONG        ,       -1,       -1, G_STRING          ,   /* Object 20  */
  NONE, NORMAL, (LONG)"Longest line:",
  0x0004, 0x000A, 0x000D, 0x0001,
        22,       -1,       -1, G_FTEXT           ,   /* Object 21 INFOLONG */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[225],
  0x0014, 0x000A, 0x0009, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  NONE, NORMAL, (LONG)"Chars",
  0x001E, 0x000A, 0x0005, 0x0001,
  INFOTOTL        ,       -1,       -1, G_STRING          ,   /* Object 23  */
  NONE, NORMAL, (LONG)"Active:",
  0x000A, 0x000C, 0x0007, 0x0001,
        25,       -1,       -1, G_FTEXT           ,   /* Object 24 INFOTOTL */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[226],
  0x001C, 0x000C, 0x0001, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  NONE, NORMAL, (LONG)"Datei(en)",
  0x001E, 0x000C, 0x0009, 0x0001,
  INFOTBYT        ,       -1,       -1, G_STRING          ,   /* Object 26  */
  NONE, NORMAL, (LONG)"All together:",
  0x0004, 0x000D, 0x000D, 0x0001,
        28,       -1,       -1, G_FTEXT           ,   /* Object 27 INFOTBYT */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[227],
  0x0015, 0x000D, 0x0008, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  NONE, NORMAL, (LONG)"Zeichen",
  0x001E, 0x000D, 0x0007, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  NONE, NORMAL, (LONG)"Free memory",
  0x0006, 0x000F, 0x000B, 0x0001,
  INFOFRAM        ,       -1,       -1, G_STRING          ,   /* Object 30  */
  NONE, NORMAL, (LONG)"Alt least:",
  0x0007, 0x0010, 0x000A, 0x0001,
        32,       -1,       -1, G_FTEXT           ,   /* Object 31 INFOFRAM */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[228],
  0x0015, 0x0010, 0x0008, 0x0001,
  INFOHELP        ,       -1,       -1, G_STRING          ,   /* Object 32  */
  NONE, NORMAL, (LONG)"Bytes",
  0x001E, 0x0010, 0x0005, 0x0001,
  INFOABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 33 INFOHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[229],
  0x0010, 0x0012, 0x0008, 0x0001,
  INFOOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 34 INFOABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[230],
  0x001A, 0x0012, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 35 INFOOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[231],
  0x0024, 0x0012, 0x0008, 0x0001,
  
  /******** Tree 21 MARKMENU ****************************************************/
        -1,        1, MARKABBR        , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x001D, 0x000F,
  MARKTITL        ,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[232],
  0x001B, 0x0000, 0x0002, 0x0001,
  MARK1           ,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2 MARKTITL */
  NONE, NORMAL, (LONG)&rs_tedinfo[233],
  0x0003, 0x0001, 0x0015, 0x0001,
  MARK1MEM        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 3 MARK1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[234],
  0x0003, 0x0003, 0x0003, 0x0001,
  MARK2           ,       -1,       -1, G_FTEXT           ,   /* Object 4 MARK1MEM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[235],
  0x0008, 0x0003, 0x0012, 0x0001,
  MARK2MEM        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 5 MARK2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[236],
  0x0003, 0x0005, 0x0003, 0x0001,
  MARK3           ,       -1,       -1, G_FTEXT           ,   /* Object 6 MARK2MEM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[237],
  0x0008, 0x0005, 0x0012, 0x0001,
  MARK3MEM        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 7 MARK3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[238],
  0x0003, 0x0007, 0x0003, 0x0001,
  MARK4           ,       -1,       -1, G_FTEXT           ,   /* Object 8 MARK3MEM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[239],
  0x0008, 0x0007, 0x0012, 0x0001,
  MARK4MEM        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 9 MARK4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[240],
  0x0003, 0x0009, 0x0003, 0x0001,
  MARK5           ,       -1,       -1, G_FTEXT           ,   /* Object 10 MARK4MEM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[241],
  0x0008, 0x0009, 0x0012, 0x0001,
  MARK5MEM        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 11 MARK5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[242],
  0x0003, 0x000B, 0x0003, 0x0001,
  MARKABBR        ,       -1,       -1, G_FTEXT           ,   /* Object 12 MARK5MEM */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[243],
  0x0008, 0x000B, 0x0012, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 13 MARKABBR */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[244],
  0x0012, 0x000D, 0x0008, 0x0001,
  
  /******** Tree 22 POPMENU ****************************************************/
        -1, POPF1           , POPF10          , G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0006, 0x000A,
         2,       -1,       -1, G_STRING          ,   /* Object 1 POPF1 */
  SELECTABLE, NORMAL, (LONG)" ^F1 ",
  0x0000, 0x0000, 0x0006, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  SELECTABLE, NORMAL, (LONG)" ^F2 ",
  0x0000, 0x0001, 0x0006, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  SELECTABLE, NORMAL, (LONG)" ^F3 ",
  0x0000, 0x0002, 0x0006, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  SELECTABLE, NORMAL, (LONG)" ^F4 ",
  0x0000, 0x0003, 0x0006, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  SELECTABLE, NORMAL, (LONG)" ^F5 ",
  0x0000, 0x0004, 0x0006, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  SELECTABLE, NORMAL, (LONG)" ^F6 ",
  0x0000, 0x0005, 0x0006, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  SELECTABLE, NORMAL, (LONG)" ^F7 ",
  0x0000, 0x0006, 0x0006, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  SELECTABLE, NORMAL, (LONG)" ^F8 ",
  0x0000, 0x0007, 0x0006, 0x0001,
  POPF10          ,       -1,       -1, G_STRING          ,   /* Object 9  */
  SELECTABLE, NORMAL, (LONG)" ^F9 ",
  0x0000, 0x0008, 0x0006, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 10 POPF10 */
  SELECTABLE|LASTOB, NORMAL, (LONG)" ^F10",
  0x0000, 0x0009, 0x0006, 0x0001,
  
  /******** Tree 23 DIVERSES ****************************************************/
        -1,        1, DIVOK           , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0041, 0x0017,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  TOUCHEXIT, OUTLINED, (LONG)&rs_tedinfo[245],
  0x003F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[246],
  0x0003, 0x0001, 0x000D, 0x0001,
        10,        4, DIVSCROLL       , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x0021, 0x0806,
  DIVPATH         ,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[247],
  0x0001, 0x0000, 0x0008, 0x0001,
  DIVINFO         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 5 DIVPATH */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[248],
  0x0002, 0x0001, 0x001D, 0x0001,
  DIVSTOOL        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 6 DIVINFO */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[249],
  0x0002, 0x0002, 0x000E, 0x0001,
  DIVTABBAR       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 7 DIVSTOOL */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[250],
  0x0002, 0x0003, 0x000D, 0x0001,
  DIVSCROLL       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 8 DIVTABBAR */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[251],
  0x0002, 0x0004, 0x0013, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 9 DIVSCROLL */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[252],
  0x0002, 0x0005, 0x0018, 0x0001,
        16,       11, DIVDWIN         , G_BOX             |0x0900,   /* Object 10  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0026, 0x0003, 0x0018, 0x0806,
  DIVZENT         ,       -1,       -1, G_TEXT            |0x2000,   /* Object 11  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[253],
  0x0001, 0x0000, 0x0009, 0x0001,
  DIVFREE         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 12 DIVZENT */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[254],
  0x0002, 0x0001, 0x000D, 0x0001,
  DIVMAUS         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 13 DIVFREE */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[255],
  0x0002, 0x0002, 0x0011, 0x0001,
  DIVDWIN         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 14 DIVMAUS */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[256],
  0x0002, 0x0003, 0x000E, 0x0001,
        10,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 15 DIVDWIN */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[257],
  0x0002, 0x0004, 0x000D, 0x0001,
        30,       17,       29, G_BOX             |0x0900,   /* Object 16  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x080A, 0x0021, 0x0809,
  DIVTABEX        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 17  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[258],
  0x0001, 0x0000, 0x0006, 0x0001,
  DIVKONV         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 18 DIVTABEX */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[259],
  0x0002, 0x0001, 0x001A, 0x0001,
  DIVUMLAUT       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 19 DIVKONV */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[260],
  0x0002, 0x0002, 0x0018, 0x0001,
  DIVWRET         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 20 DIVUMLAUT */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[261],
  0x0002, 0x0003, 0x0011, 0x0001,
  DIVBLANK        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 21 DIVWRET */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[262],
  0x0002, 0x0004, 0x000C, 0x0001,
  DIVCRLF         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 22 DIVBLANK */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[263],
  0x0002, 0x0005, 0x0016, 0x0001,
  DIVLF           ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 23 DIVCRLF */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[264],
  0x0002, 0x0006, 0x0008, 0x0001,
  DIVCR           ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 24 DIVLF */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[265],
  0x000C, 0x0006, 0x0005, 0x0001,
  DIVBACK         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 25 DIVCR */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[266],
  0x0013, 0x0006, 0x0005, 0x0001,
  DIVBUTIM        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 26 DIVBACK */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[267],
  0x0002, 0x0007, 0x001D, 0x0001,
  DIVBACK2        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 27 DIVBUTIM */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[268],
  0x0002, 0x0008, 0x000D, 0x0001,
        29,       -1,       -1, G_FTEXT           ,   /* Object 28 DIVBACK2 */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[269],
  0x0010, 0x0008, 0x0002, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 29  */
  NONE, NORMAL, (LONG)"min.",
  0x0013, 0x0008, 0x0004, 0x0001,
        39,       31, DIVTOSDOMIAN    , G_BOX             |0x0900,   /* Object 30  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0026, 0x080A, 0x0018, 0x0809,
  DIVDESK         ,       -1,       -1, G_TEXT            |0x2000,   /* Object 31  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[270],
  0x0001, 0x0000, 0x000F, 0x0001,
  DIVPAPER        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 32 DIVDESK */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[271],
  0x0002, 0x0001, 0x0013, 0x0001,
  DIVCLIP         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 33 DIVPAPER */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[272],
  0x0002, 0x0002, 0x0013, 0x0001,
  DIVVAST         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 34 DIVCLIP */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[273],
  0x0002, 0x0003, 0x0011, 0x0001,
  DIVNAME         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 35 DIVVAST */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[274],
  0x0002, 0x0004, 0x0011, 0x0001,
  DIVSAVE         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 36 DIVNAME */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[275],
  0x0002, 0x0005, 0x0014, 0x0001,
  DIVTOSDOMIAN    ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 37 DIVSAVE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[276],
  0x0002, 0x0006, 0x0011, 0x0001,
        30,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 38 DIVTOSDOMIAN */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[277],
  0x0002, 0x0007, 0x0011, 0x0001,
  DIVHDA          ,       -1,       -1, G_STRING          ,   /* Object 39  */
  NONE, NORMAL, (LONG)"Help accessory:",
  0x0003, 0x0015, 0x000F, 0x0001,
  DIVHELP         ,       -1,       -1, G_BUTTON          ,   /* Object 40 DIVHDA */
  SELECTABLE|EXIT|FLAGS9|FLAGS10, NORMAL, (LONG)"1STGUIDE",
  0x0014, 0x0015, 0x0009, 0x0001,
  DIVABBR         ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 41 DIVHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[278],
  0x0022, 0x0015, 0x0008, 0x0001,
  DIVOK           ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 42 DIVABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[279],
  0x002C, 0x0015, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 43 DIVOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[280],
  0x0036, 0x0015, 0x0008, 0x0001,
  
  /******** Tree 24 BRACES ****************************************************/
        -1,        1, BRACOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0023, 0x0016,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[281],
  0x0021, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[282],
  0x0003, 0x0001, 0x000E, 0x0001,
  BRACHELP        ,        4, FREE5END        , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x001D, 0x080F,
  CBRACE          ,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[283],
  0x0001, 0x0000, 0x0005, 0x0001,
  SBRACE          ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 5 CBRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[284],
  0x0002, 0x0001, 0x0019, 0x0001,
  RBRACE          ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 6 SBRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[285],
  0x0002, 0x0002, 0x0019, 0x0001,
  PBRACE          ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 7 RBRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[286],
  0x0002, 0x0003, 0x0019, 0x0001,
  PKOMBRAC        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 8 PBRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[287],
  0x0002, 0x0004, 0x0019, 0x0001,
  CKOMBRAC        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 9 PKOMBRAC */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[288],
  0x0002, 0x0005, 0x0019, 0x0001,
  FREE1BRACE      ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 10 CKOMBRAC */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[289],
  0x0002, 0x0006, 0x0019, 0x0001,
  FREE1BEG        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 11 FREE1BRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[290],
  0x0002, 0x0807, 0x0002, 0x0001,
  FREE1END        ,       -1,       -1, G_FTEXT           ,   /* Object 12 FREE1BEG */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[291],
  0x0005, 0x0807, 0x000A, 0x0001,
  FREE2BRACE      ,       -1,       -1, G_FTEXT           ,   /* Object 13 FREE1END */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[292],
  0x0011, 0x0807, 0x000A, 0x0001,
  FREE2BEG        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 14 FREE2BRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[293],
  0x0002, 0x0009, 0x0002, 0x0001,
  FREE2END        ,       -1,       -1, G_FTEXT           ,   /* Object 15 FREE2BEG */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[294],
  0x0005, 0x0009, 0x000A, 0x0001,
  FREE3BRACE      ,       -1,       -1, G_FTEXT           ,   /* Object 16 FREE2END */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[295],
  0x0011, 0x0009, 0x000A, 0x0001,
  FREE3BEG        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 17 FREE3BRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[296],
  0x0002, 0x080A, 0x0002, 0x0001,
  FREE3END        ,       -1,       -1, G_FTEXT           ,   /* Object 18 FREE3BEG */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[297],
  0x0005, 0x080A, 0x000A, 0x0001,
  FREE4BRACE      ,       -1,       -1, G_FTEXT           ,   /* Object 19 FREE3END */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[298],
  0x0011, 0x080A, 0x000A, 0x0001,
  FREE4BEG        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 20 FREE4BRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[299],
  0x0002, 0x000C, 0x0002, 0x0001,
  FREE4END        ,       -1,       -1, G_FTEXT           ,   /* Object 21 FREE4BEG */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[300],
  0x0005, 0x000C, 0x000A, 0x0001,
  FREE5BRACE      ,       -1,       -1, G_FTEXT           ,   /* Object 22 FREE4END */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[301],
  0x0011, 0x000C, 0x000A, 0x0001,
  FREE5BEG        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 23 FREE5BRACE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[302],
  0x0002, 0x080D, 0x0002, 0x0001,
  FREE5END        ,       -1,       -1, G_FTEXT           ,   /* Object 24 FREE5BEG */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[303],
  0x0005, 0x080D, 0x000A, 0x0001,
         3,       -1,       -1, G_FTEXT           ,   /* Object 25 FREE5END */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[304],
  0x0011, 0x080D, 0x000A, 0x0001,
  BRACABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 26 BRACHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[305],
  0x0004, 0x0014, 0x0008, 0x0001,
  BRACOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 27 BRACABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[306],
  0x000E, 0x0014, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 28 BRACOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[307],
  0x0018, 0x0014, 0x0008, 0x0001,
  
  /******** Tree 25 SORTMENU ****************************************************/
        -1,        1, SORTOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0033, 0x0010,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[308],
  0x0031, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[309],
  0x0003, 0x0001, 0x000A, 0x0001,
        11,        4, SORTCOL         , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x0018, 0x0004,
         5,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[310],
  0x0001, 0x0000, 0x0006, 0x0001,
  SORTFROM        ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Line",
  0x0002, 0x0001, 0x0004, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 SORTFROM */
  NONE, NORMAL, (LONG)&rs_tedinfo[311],
  0x0009, 0x0001, 0x0004, 0x0001,
  SORTTO          ,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"to",
  0x000E, 0x0001, 0x0002, 0x0001,
         9,       -1,       -1, G_FTEXT           ,   /* Object 8 SORTTO */
  NONE, NORMAL, (LONG)&rs_tedinfo[312],
  0x0012, 0x0001, 0x0004, 0x0001,
  SORTCOL         ,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"Column",
  0x0002, 0x0002, 0x0009, 0x0001,
         3,       -1,       -1, G_FTEXT           ,   /* Object 10 SORTCOL */
  NONE, NORMAL, (LONG)&rs_tedinfo[313],
  0x0009, 0x0002, 0x0003, 0x0001,
        15,       12, SORTDN          , G_BOX             |0x0900,   /* Object 11  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x001D, 0x0003, 0x0013, 0x0004,
  SORTUP          ,       -1,       -1, G_TEXT            |0x2000,   /* Object 12  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[314],
  0x0001, 0x0000, 0x000B, 0x0001,
  SORTDN          ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 13 SORTUP */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[315],
  0x0002, 0x0001, 0x000E, 0x0001,
        11,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 14 SORTDN */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[316],
  0x0002, 0x0002, 0x000D, 0x0001,
        21,       16, SORTEXTRA       , G_BOX             |0x0900,   /* Object 15  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0008, 0x0018, 0x0005,
  SORTLINE        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 16  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[317],
  0x0001, 0x0000, 0x0008, 0x0001,
  SORTGROUP       ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 17 SORTLINE */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[318],
  0x0002, 0x0001, 0x0008, 0x0001,
  SORTKRIT        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 18 SORTGROUP */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[319],
  0x000C, 0x0001, 0x0009, 0x0001,
  SORTEXTRA       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 19 SORTKRIT */
  SELECTABLE, DISABLED, (LONG)&rs_tedinfo[320],
  0x0002, 0x0002, 0x0013, 0x0001,
        15,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 20 SORTEXTRA */
  SELECTABLE, DISABLED, (LONG)&rs_tedinfo[321],
  0x0002, 0x0003, 0x000D, 0x0001,
  SORTHELP        ,       22, SORTIGNO        , G_BOX             |0x0900,   /* Object 21  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x001D, 0x0008, 0x0013, 0x0005,
  SORTIGNO        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 22  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[322],
  0x0001, 0x0000, 0x0009, 0x0001,
        21,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 23 SORTIGNO */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[323],
  0x0002, 0x0001, 0x000D, 0x0001,
  SORTABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 24 SORTHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[324],
  0x0014, 0x000E, 0x0008, 0x0001,
  SORTOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 25 SORTABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[325],
  0x001E, 0x000E, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 26 SORTOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[326],
  0x0028, 0x000E, 0x0008, 0x0001,
  
  /******** Tree 26 POPPRN ****************************************************/
        -1, POPNO           , POP30           , G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0019, 0x000D,
  POPEPS          ,       -1,       -1, G_STRING          ,   /* Object 1 POPNO */
  SELECTABLE, CHECKED, (LONG)"  undefiniert            ",
  0x0000, 0x0000, 0x0019, 0x0001,
  POPIBM          ,       -1,       -1, G_STRING          ,   /* Object 2 POPEPS */
  SELECTABLE, NORMAL, (LONG)"  EPSON FX               ",
  0x0000, 0x0001, 0x0019, 0x0001,
  POP21           ,       -1,       -1, G_STRING          ,   /* Object 3 POPIBM */
  SELECTABLE, NORMAL, (LONG)"  IBM kompat.            ",
  0x0000, 0x0002, 0x0019, 0x0001,
  POP22           ,       -1,       -1, G_STRING          ,   /* Object 4 POP21 */
  SELECTABLE, NORMAL, (LONG)"  21 GDOS                ",
  0x0000, 0x0003, 0x0019, 0x0001,
  POP23           ,       -1,       -1, G_STRING          ,   /* Object 5 POP22 */
  SELECTABLE, NORMAL, (LONG)"  22 GDOS                ",
  0x0000, 0x0004, 0x0019, 0x0001,
  POP24           ,       -1,       -1, G_STRING          ,   /* Object 6 POP23 */
  SELECTABLE, NORMAL, (LONG)"  23 GDOS                ",
  0x0000, 0x0005, 0x0019, 0x0001,
  POP25           ,       -1,       -1, G_STRING          ,   /* Object 7 POP24 */
  SELECTABLE, NORMAL, (LONG)"  24 GDOS                ",
  0x0000, 0x0006, 0x0019, 0x0001,
  POP26           ,       -1,       -1, G_STRING          ,   /* Object 8 POP25 */
  SELECTABLE, NORMAL, (LONG)"  25 GDOS                ",
  0x0000, 0x0007, 0x0019, 0x0001,
  POP27           ,       -1,       -1, G_STRING          ,   /* Object 9 POP26 */
  SELECTABLE, NORMAL, (LONG)"  26 GDOS                ",
  0x0000, 0x0008, 0x0019, 0x0001,
  POP28           ,       -1,       -1, G_STRING          ,   /* Object 10 POP27 */
  SELECTABLE, NORMAL, (LONG)"  27 GDOS                ",
  0x0000, 0x0009, 0x0019, 0x0001,
  POP29           ,       -1,       -1, G_STRING          ,   /* Object 11 POP28 */
  SELECTABLE, NORMAL, (LONG)"  28 GDOS                ",
  0x0000, 0x000A, 0x0019, 0x0001,
  POP30           ,       -1,       -1, G_STRING          ,   /* Object 12 POP29 */
  SELECTABLE, NORMAL, (LONG)"  29 GDOS                ",
  0x0000, 0x000B, 0x0019, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 13 POP30 */
  SELECTABLE|LASTOB, NORMAL, (LONG)"  30 GDOS                ",
  0x0000, 0x000C, 0x0019, 0x0001,
  
  /******** Tree 27 POPPAPER ****************************************************/
        -1, POPBASIS        , POPWIDE         , G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x000C, 0x000A,
  POPLETT         ,       -1,       -1, G_STRING          ,   /* Object 1 POPBASIS */
  NONE, CHECKED, (LONG)"  Default   ",
  0x0000, 0x0000, 0x000C, 0x0001,
  POPLEGA         ,       -1,       -1, G_STRING          ,   /* Object 2 POPLETT */
  SELECTABLE, NORMAL, (LONG)"  Letter    ",
  0x0000, 0x0001, 0x000C, 0x0001,
  POPHALF         ,       -1,       -1, G_STRING          ,   /* Object 3 POPLEGA */
  SELECTABLE, NORMAL, (LONG)"  Legal     ",
  0x0000, 0x0002, 0x000C, 0x0001,
  POPLEDG         ,       -1,       -1, G_STRING          ,   /* Object 4 POPHALF */
  SELECTABLE, NORMAL, (LONG)"  Half      ",
  0x0000, 0x0003, 0x000C, 0x0001,
  POPA3           ,       -1,       -1, G_STRING          ,   /* Object 5 POPLEDG */
  SELECTABLE, NORMAL, (LONG)"  Ledger    ",
  0x0000, 0x0004, 0x000C, 0x0001,
  POPA4           ,       -1,       -1, G_STRING          ,   /* Object 6 POPA3 */
  SELECTABLE, NORMAL, (LONG)"  DIN A3    ",
  0x0000, 0x0005, 0x000C, 0x0001,
  POPA5           ,       -1,       -1, G_STRING          ,   /* Object 7 POPA4 */
  SELECTABLE, NORMAL, (LONG)"  DIN A4    ",
  0x0000, 0x0006, 0x000C, 0x0001,
  POPB5           ,       -1,       -1, G_STRING          ,   /* Object 8 POPA5 */
  SELECTABLE, NORMAL, (LONG)"  DIN A5    ",
  0x0000, 0x0007, 0x000C, 0x0001,
  POPWIDE         ,       -1,       -1, G_STRING          ,   /* Object 9 POPB5 */
  SELECTABLE, NORMAL, (LONG)"  DIN B5    ",
  0x0000, 0x0008, 0x000C, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 10 POPWIDE */
  SELECTABLE|LASTOB, NORMAL, (LONG)"  Wide      ",
  0x0000, 0x0009, 0x000C, 0x0001,
  
  /******** Tree 28 GREPMENU ****************************************************/
        -1,        1, GREPOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0041, 0x000D,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[327],
  0x003F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[328],
  0x0003, 0x0001, 0x000F, 0x0001,
  GREPPATT        ,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[329],
  0x0003, 0x0003, 0x0208, 0x0001,
         5,       -1,       -1, G_FTEXT           ,   /* Object 4 GREPPATT */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[330],
  0x0003, 0x0004, 0x003B, 0x0001,
        11,        6, GREPMARK        , G_BOX             |0x0900,   /* Object 5  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0006, 0x001C, 0x0006,
  GREPGREP        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 6  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[331],
  0x0001, 0x0000, 0x0009, 0x0001,
  GREPALL         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 7 GREPGREP */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[332],
  0x0002, 0x0001, 0x0007, 0x0001,
  GREPFOLD        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 8 GREPALL */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[333],
  0x0002, 0x0002, 0x0017, 0x0001,
  GREPMARK        ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 9 GREPFOLD */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[334],
  0x0002, 0x0003, 0x0016, 0x0001,
         5,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 10 GREPMARK */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[335],
  0x0002, 0x0004, 0x0018, 0x0001,
  GREPNAME        ,       -1,       -1, G_TEXT            ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[336],
  0x0022, 0x0006, 0x0609, 0x0001,
  GREPHELP        ,       -1,       -1, G_FTEXT           ,   /* Object 12 GREPNAME */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[337],
  0x0022, 0x0007, 0x001C, 0x0001,
  GREPABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 13 GREPHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[338],
  0x0022, 0x000B, 0x0008, 0x0001,
  GREPOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 14 GREPABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[339],
  0x002C, 0x000B, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 15 GREPOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[340],
  0x0036, 0x000B, 0x0008, 0x0001,
  
  /******** Tree 29 POPDIST ****************************************************/
        -1, POPD1           , POD2            , G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0005, 0x0003,
         2,       -1,       -1, G_STRING          ,   /* Object 1 POPD1 */
  SELECTABLE, CHECKED, (LONG)"  1  ",
  0x0000, 0x0000, 0x0005, 0x0001,
  POD2            ,       -1,       -1, G_STRING          ,   /* Object 2  */
  SELECTABLE, NORMAL, (LONG)"  1\253 ",
  0x0000, 0x0001, 0x0005, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 3 POD2 */
  SELECTABLE|LASTOB, NORMAL, (LONG)"  2  ",
  0x0000, 0x0002, 0x0005, 0x0001,
  
  /******** Tree 30 NUMMENU ****************************************************/
        -1,        1, NUMOK           , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0022, 0x0013,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[341],
  0x0020, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[342],
  0x0003, 0x0001, 0x000D, 0x0001,
         8,        4, NUMTSEP         , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x001C, 0x0005,
  NUMNDT          ,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[343],
  0x0001, 0x0000, 0x000A, 0x0001,
  NUMNAMI         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 5 NUMNDT */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[344],
  0x0002, 0x0001, 0x000A, 0x0001,
  NUMTSEP         ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 6 NUMNAMI */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[345],
  0x000E, 0x0001, 0x000B, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 7 NUMTSEP */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[346],
  0x0002, 0x0003, 0x000D, 0x0001,
        16,        9,       15, G_BOX             |0x0900,   /* Object 8  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0009, 0x001C, 0x0804,
  NUMNORM         ,       -1,       -1, G_TEXT            |0x2000,   /* Object 9  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[347],
  0x0001, 0x0000, 0x0005, 0x0001,
  NUMMWSTN        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 10 NUMNORM */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[348],
  0x0002, 0x0001, 0x000A, 0x0001,
        12,       -1,       -1, G_FTEXT           ,   /* Object 11 NUMMWSTN */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[349],
  0x000E, 0x0001, 0x0005, 0x0001,
  NUMERM          ,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"%",
  0x0014, 0x0001, 0x0001, 0x0001,
  NUMMWSTE        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 13 NUMERM */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[350],
  0x0002, 0x0802, 0x000B, 0x0001,
        15,       -1,       -1, G_FTEXT           ,   /* Object 14 NUMMWSTE */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[351],
  0x000E, 0x0802, 0x0005, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"%",
  0x0014, 0x0802, 0x0001, 0x0001,
  NUMKOMMA        ,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"Nachkommastellen:",
  0x0003, 0x000F, 0x0011, 0x0001,
  NUMHELP         ,       -1,       -1, G_FTEXT           ,   /* Object 17 NUMKOMMA */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[352],
  0x0015, 0x000F, 0x0001, 0x0001,
  NUMABBR         ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 18 NUMHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[353],
  0x0003, 0x0011, 0x0008, 0x0001,
  NUMOK           ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 19 NUMABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[354],
  0x000D, 0x0011, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 20 NUMOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[355],
  0x0017, 0x0011, 0x0008, 0x0001,
  
  /******** Tree 31 PICKLIST ****************************************************/
        -1,        1, PICKOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0034, 0x0017,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[356],
  0x0032, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[357],
  0x0003, 0x0001, 0x0008, 0x0001,
  PICKINFO        , PICKFIRST       , PICKBOX         , G_BOX             ,   /* Object 3  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x002E, 0x000A,
         5,       -1,       -1, G_TEXT            ,   /* Object 4 PICKFIRST */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[358],
  0x0001, 0x0001, 0x002A, 0x0001,
         6,       -1,       -1, G_TEXT            ,   /* Object 5  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[359],
  0x0001, 0x0002, 0x002A, 0x0001,
         7,       -1,       -1, G_TEXT            ,   /* Object 6  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[360],
  0x0001, 0x0003, 0x002A, 0x0001,
         8,       -1,       -1, G_TEXT            ,   /* Object 7  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[361],
  0x0001, 0x0004, 0x002A, 0x0001,
         9,       -1,       -1, G_TEXT            ,   /* Object 8  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[362],
  0x0001, 0x0005, 0x002A, 0x0001,
        10,       -1,       -1, G_TEXT            ,   /* Object 9  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[363],
  0x0001, 0x0006, 0x002A, 0x0001,
  PICKLAST        ,       -1,       -1, G_TEXT            ,   /* Object 10  */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[364],
  0x0001, 0x0007, 0x002A, 0x0001,
  PICKUP          ,       -1,       -1, G_TEXT            ,   /* Object 11 PICKLAST */
  SELECTABLE|EXIT|RBUTTON|TOUCHEXIT|FLAGS11, NORMAL, (LONG)&rs_tedinfo[365],
  0x0001, 0x0008, 0x002A, 0x0001,
  PICKDN          ,       -1,       -1, G_BOXCHAR         ,   /* Object 12 PICKUP */
  TOUCHEXIT|FLAGS12, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00FF1100L,
  0x002C, 0x0000, 0x0002, 0x0001,
  PICKBOX         ,       -1,       -1, G_BOXCHAR         ,   /* Object 13 PICKDN */
  TOUCHEXIT|FLAGS13, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x002C, 0x0009, 0x0002, 0x0001,
         3, PICKSLID        , PICKSLID        , G_BOX             ,   /* Object 14 PICKBOX */
  TOUCHEXIT|FLAGS14, NORMAL, (LONG)0x00FF1111L,
  0x002C, 0x0001, 0x0002, 0x0008,
  PICKBOX         ,       -1,       -1, G_BOX             ,   /* Object 15 PICKSLID */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0002,
        17,       -1,       -1, G_BOXTEXT         ,   /* Object 16 PICKINFO */
  NONE, NORMAL, (LONG)&rs_tedinfo[366],
  0x0003, 0x000D, 0x002E, 0x0001,
  PICKCOMMENT     ,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"Info:",
  0x0003, 0x000F, 0x0005, 0x0001,
  PICKAKTIV       ,       -1,       -1, G_FTEXT           ,   /* Object 18 PICKCOMMENT */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[367],
  0x0009, 0x000F, 0x0028, 0x0001,
  PICKCLEAN       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 19 PICKAKTIV */
  SELECTABLE, SELECTED, (LONG)&rs_tedinfo[368],
  0x0003, 0x0011, 0x0009, 0x0001,
  PICKDEL         ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 20 PICKCLEAN */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[369],
  0x0003, 0x0013, 0x000D, 0x0001,
  PICKHELP        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 21 PICKDEL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[370],
  0x0003, 0x0015, 0x0008, 0x0001,
  PICKABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 22 PICKHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[371],
  0x0015, 0x0015, 0x0008, 0x0001,
  PICKOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 23 PICKABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[372],
  0x001F, 0x0015, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 24 PICKOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[373],
  0x0029, 0x0015, 0x0008, 0x0001,
  
  /******** Tree 32 MENUEDITOR ****************************************************/
        -1,        1, MENUOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0031, 0x0014,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[374],
  0x002F, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[375],
  0x0003, 0x0001, 0x0009, 0x0001,
        14,        4, MENUMLOAD       , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x002B, 0x0008,
         5,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[376],
  0x0001, 0x0000, 0x0007, 0x0001,
  MENUMVOR        ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Title:",
  0x0002, 0x0002, 0x0006, 0x0001,
  MENUMNAME       ,       -1,       -1, G_BOXCHAR         ,   /* Object 6 MENUMVOR */
  TOUCHEXIT|FLAGS9|FLAGS10, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0027, 0x0002, 0x0002, 0x0001,
         8,       -1,       -1, G_FBOXTEXT        ,   /* Object 7 MENUMNAME */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[377],
  0x0009, 0x0002, 0x001D, 0x0001,
  MENUMIDX        ,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"Index:",
  0x0002, 0x0004, 0x0006, 0x0001,
  MENUMFILE       ,       -1,       -1, G_FBOXTEXT        ,   /* Object 9 MENUMIDX */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[378],
  0x0009, 0x0004, 0x0005, 0x0001,
  MENUMHELP       ,       -1,       -1, G_TEXT            ,   /* Object 10 MENUMFILE */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[379],
  0x001C, 0x0004, 0x000C, 0x0001,
  MENUMSAVE       ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 11 MENUMHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[380],
  0x0006, 0x0006, 0x0008, 0x0001,
  MENUMLOAD       ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 12 MENUMSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[381],
  0x0010, 0x0006, 0x000B, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 13 MENUMLOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[382],
  0x001D, 0x0006, 0x000B, 0x0001,
  MENUOK          ,       15, MENUTLOAD       , G_BOX             |0x0900,   /* Object 14  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x000C, 0x002B, 0x0005,
  MENUTAKTIV      ,       -1,       -1, G_TEXT            |0x2000,   /* Object 15  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[383],
  0x0001, 0x0000, 0x000A, 0x0001,
  MENUTFILE       ,       -1,       -1, G_BOXTEXT         |0x0200,   /* Object 16 MENUTAKTIV */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[384],
  0x0002, 0x0001, 0x0009, 0x0001,
  MENUTHELP       ,       -1,       -1, G_TEXT            ,   /* Object 17 MENUTFILE */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[385],
  0x001C, 0x0001, 0x000C, 0x0001,
  MENUTSAVE       ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 18 MENUTHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[386],
  0x0006, 0x0003, 0x0008, 0x0001,
  MENUTLOAD       ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 19 MENUTSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[387],
  0x0010, 0x0003, 0x000B, 0x0001,
        14,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 20 MENUTLOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[388],
  0x001D, 0x0003, 0x000B, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 21 MENUOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[389],
  0x0026, 0x0012, 0x0008, 0x0001,
  
  /******** Tree 33 REGISTER ****************************************************/
        -1,        1, REGISTOK        , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002A, 0x0010,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[390],
  0x0028, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[391],
  0x0003, 0x0001, 0x0010, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Please insert your name and your",
  0x0003, 0x0003, 0x0020, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"personal registration key.",
  0x0003, 0x0004, 0x001A, 0x0001,
  REGISTNAME      ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Name:",
  0x0003, 0x0007, 0x0005, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 REGISTNAME */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[392],
  0x000A, 0x0007, 0x001D, 0x0001,
  REGISTKEY       ,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"Key:",
  0x0003, 0x0009, 0x0004, 0x0001,
  REGISTABBR      ,       -1,       -1, G_FTEXT           ,   /* Object 8 REGISTKEY */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[393],
  0x000A, 0x0009, 0x000A, 0x0001,
  REGISTOK        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 9 REGISTABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[394],
  0x0015, 0x000E, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 10 REGISTOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[395],
  0x001F, 0x000E, 0x0008, 0x0001,
  
  /******** Tree 34 TEXTFORMAT ****************************************************/
        -1,        1, FORMOK          , G_BOX             ,   /* Object 0  */
  FLAGS10, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0034, 0x000D,
         2,       -1,       -1, G_BOXTEXT         |0x0500,   /* Object 1  */
  SELECTABLE|EXIT, OUTLINED, (LONG)&rs_tedinfo[396],
  0x0032, 0x0000, 0x0002, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0400,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[397],
  0x0003, 0x0001, 0x0006, 0x0001,
         9,        4, FORMBLOCK       , G_BOX             |0x0900,   /* Object 3  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x0003, 0x0003, 0x0016, 0x0007,
  FORMLEFT        ,       -1,       -1, G_TEXT            |0x2000,   /* Object 4  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[398],
  0x0001, 0x0000, 0x0009, 0x0001,
  FORMCENTER      ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 5 FORMLEFT */
  SELECTABLE|EXIT|RBUTTON, SELECTED, (LONG)&rs_tedinfo[399],
  0x0002, 0x0001, 0x000E, 0x0001,
  FORMRIGHT       ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 6 FORMCENTER */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[400],
  0x0002, 0x0002, 0x000C, 0x0001,
  FORMBLOCK       ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 7 FORMRIGHT */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[401],
  0x0002, 0x0003, 0x000F, 0x0001,
         3,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 8 FORMBLOCK */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[402],
  0x0002, 0x0005, 0x000C, 0x0001,
        14,       10, FORMMARK        , G_BOX             |0x0900,   /* Object 9  */
  FLAGS10, NORMAL, (LONG)0x00FF1100L,
  0x001B, 0x0003, 0x0016, 0x0007,
  FORMCURSOR      ,       -1,       -1, G_TEXT            |0x2000,   /* Object 10  */
  FLAGS10, NORMAL, (LONG)&rs_tedinfo[403],
  0x0001, 0x0000, 0x0009, 0x0001,
  FORMPARA        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 11 FORMCURSOR */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[404],
  0x0002, 0x0001, 0x000E, 0x0001,
  FORMMARK        ,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 12 FORMPARA */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[405],
  0x0002, 0x0002, 0x000C, 0x0001,
         9,       -1,       -1, G_BOXTEXT         |0x0100,   /* Object 13 FORMMARK */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[406],
  0x0002, 0x0003, 0x0013, 0x0001,
  FORMLENGHT      ,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"Line lenght:",
  0x0003, 0x000B, 0x000C, 0x0001,
  FORMHELP        ,       -1,       -1, G_FTEXT           ,   /* Object 15 FORMLENGHT */
  EDITABLE|FLAGS10, NORMAL, (LONG)&rs_tedinfo[407],
  0x0010, 0x000B, 0x0003, 0x0001,
  FORMABBR        ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 16 FORMHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[408],
  0x0015, 0x000B, 0x0008, 0x0001,
  FORMOK          ,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 17 FORMABBR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[409],
  0x001F, 0x000B, 0x0008, 0x0001,
         0,       -1,       -1, G_BOXTEXT         |0x0300,   /* Object 18 FORMOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[410],
  0x0029, 0x000B, 0x0008, 0x0001,
  
  /******** Tree 35 SLISTBOX ****************************************************/
        -1, SLBT1           , SLBGR           , G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x001B, 0x0005,
         2,       -1,       -1, G_TEXT            ,   /* Object 1 SLBT1 */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[411],
  0x0000, 0x0000, 0x0019, 0x0001,
         3,       -1,       -1, G_TEXT            ,   /* Object 2  */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[412],
  0x0000, 0x0001, 0x0019, 0x0001,
         4,       -1,       -1, G_TEXT            ,   /* Object 3  */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[413],
  0x0000, 0x0002, 0x0019, 0x0001,
  SLBT5           ,       -1,       -1, G_TEXT            ,   /* Object 4  */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[414],
  0x0000, 0x0003, 0x0019, 0x0001,
  SLBUP           ,       -1,       -1, G_TEXT            ,   /* Object 5 SLBT5 */
  SELECTABLE|EXIT|RBUTTON, NORMAL, (LONG)&rs_tedinfo[415],
  0x0000, 0x0004, 0x0019, 0x0001,
  SLBDN           ,       -1,       -1, G_BOXCHAR         ,   /* Object 6 SLBUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\001' << 24)|0x00FF1100L,
  0x0019, 0x0000, 0x0002, 0x0001,
  SLBGR           ,       -1,       -1, G_BOXCHAR         ,   /* Object 7 SLBDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\002' << 24)|0x00FF1100L,
  0x0019, 0x0004, 0x0002, 0x0001,
         0, SLBSL           , SLBSL           , G_BOX             ,   /* Object 8 SLBGR */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1111L,
  0x0019, 0x0001, 0x0002, 0x0003,
  SLBGR           ,       -1,       -1, G_BOX             ,   /* Object 9 SLBSL */
  LASTOB|TOUCHEXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0001, 0x0002, 0x0001
};

static OBJECT *rs_trindex[] =
{ &rs_object[0],   /* Tree  0 WINMENU          */
  &rs_object[131],   /* Tree  1 DESKTOP          */
  &rs_object[145],   /* Tree  2 FINDMENU         */
  &rs_object[178],   /* Tree  3 GOTOMENU         */
  &rs_object[185],   /* Tree  4 TABMENU          */
  &rs_object[192],   /* Tree  5 COPYINFO         */
  &rs_object[214],   /* Tree  6 SHELMENU         */
  &rs_object[223],   /* Tree  7 CHARTBL          */
  &rs_object[484],   /* Tree  8 FSELBOX          */
  &rs_object[486],   /* Tree  9 SHARWARE         */
  &rs_object[499],   /* Tree 10 FONTSEL          */
  &rs_object[537],   /* Tree 11 PRTMENU          */
  &rs_object[544],   /* Tree 12 USERIMG          */
  &rs_object[570],   /* Tree 13 SHELL2           */
  &rs_object[592],   /* Tree 14 FKEYMENU         */
  &rs_object[623],   /* Tree 15 UMBRMENU         */
  &rs_object[630],   /* Tree 16 PRNINST          */
  &rs_object[696],   /* Tree 17 PRNINST2         */
  &rs_object[734],   /* Tree 18 PREVIEW          */
  &rs_object[742],   /* Tree 19 REPLMENU         */
  &rs_object[750],   /* Tree 20 INFOMENU         */
  &rs_object[786],   /* Tree 21 MARKMENU         */
  &rs_object[800],   /* Tree 22 POPMENU          */
  &rs_object[811],   /* Tree 23 DIVERSES         */
  &rs_object[855],   /* Tree 24 BRACES           */
  &rs_object[884],   /* Tree 25 SORTMENU         */
  &rs_object[911],   /* Tree 26 POPPRN           */
  &rs_object[925],   /* Tree 27 POPPAPER         */
  &rs_object[936],   /* Tree 28 GREPMENU         */
  &rs_object[952],   /* Tree 29 POPDIST          */
  &rs_object[956],   /* Tree 30 NUMMENU          */
  &rs_object[977],   /* Tree 31 PICKLIST         */
  &rs_object[1002],   /* Tree 32 MENUEDITOR       */
  &rs_object[1024],   /* Tree 33 REGISTER         */
  &rs_object[1035],   /* Tree 34 TEXTFORMAT       */
  &rs_object[1054]    /* Tree 35 SLISTBOX         */
};
