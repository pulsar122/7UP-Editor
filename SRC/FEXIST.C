/*****************************************************************
	7UP
	Modul: FEXIST.C
	(c) by TheoSoft '94
	1997-04-21 (MJK): Unabh„ngig von den TC-/PC-Libraries
*****************************************************************/
#if defined( __TURBOC__ ) && !defined( __MINT__ )
#	include <ext.h>
#else
#	include <stat.h>
#endif

int fexist(char *pathname)
{
#if defined( __TURBOC__ ) && !defined( __MINT__ )
	struct ffblk fileRec;
	return(!findfirst(pathname,&fileRec,0));
#else
	/* diese Alternative ist langsamer aber weit besser */
	struct stat dummy;
	return (!stat(pathname, &dummy) &&
	        S_ISREG(dummy.st_mode));
#endif
}
