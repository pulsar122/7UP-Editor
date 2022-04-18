/*
 * Header file for using the drag & drop
 * protocol.
 * Copyright 1992 Atari Corporation.
 *
 * 1997-03-26 (MJK): prototypes added
 */

#define AP_DRAGDROP	63

#define	DD_OK		0
#define DD_NAK		1
#define DD_EXT		2
#define DD_LEN		3

/* timeout in milliseconds */
#define DD_TIMEOUT	4000

/* number of bytes of "preferred
 * extensions" sent by receipient during
 * open
 */
#define DD_NUMEXTS	8
#define DD_EXTSIZE	32L

/* max size of a drag&drop item name */
#define DD_NAMEMAX	128

/* max length of a drag&drop header */
#define DD_HDRMAX	(8+DD_NAMEMAX)

#ifndef Word
#define Word short
#define Long long
#endif

/*
	Prototypes
*/

/*
 * rec_ddmsg: given a drag & drop message, act as
 * a receipient and get the data
 *
 * Input Parameters:
 * msg:			Pointer to the 16 byte AES message
 *				  returned by evnt_multi or evnt_mesag
 *
 * calls the following functions to actually perform the paste
 * operation:
 *
 * parseargs(char cmdlin[]):
 *		Given a null terminated string of arguments, separated
 *		by spaces, parse it and perform whatever actions are
 *		appropriate on the arguments (usually file and folder
 *		names)
 *
 * paste_rtf(int win, int fd, long size):
 *		Read "size" bytes from the open file descriptor "fd",
 *		and paste it into the window whose handle is "win".
 *		The data is assumed to be in RTF format.
 *
 * paste_txt(int win, int fd, long size):
 *		Read "size" bytes from the open file descriptor "fd",
 *		and paste it into the window whose handle is "win".
 *		The data is assumed to be ASCII text.
 */
void rec_ddmsg(int msg[], char *pathname);
