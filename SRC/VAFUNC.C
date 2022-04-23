/*****************************************************************
	VAPROTO - Funktionen fÅr das AV/VA-Protokoll
	Modul: VAFUNC.C
	Copyright (c) Markus Kohm
	
	Dieses Modul wurde dem Modul VAFUNC.O von Stephan Gerle
	nachprogrammiert. Funktionsnamen und Variablennamen wurden
	dem Modul direkt entnommen. Der Source-Code ist jedoch komplett
	neu erstellt!
*****************************************************************/

#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
	extern short _app;
#endif
#include <string.h>
#include <stdlib.h>

#include "vafunc.h"

#define GEMINI   "GEMINI  "
#define AVSERVER "AVSERVER"

static int	 AVActiveFlag=0,     /* Ist AVStatus-Wert gÅltig? */
             AVMyProtoStatus = 0,/* Protokollstatus des Acc's */
             AVMyApId=0,         /* ApId des Acc's */
             AVServerId=0;       /* ApId des AV-Servers */
static char *AVMyName = "";     /* AV-Name des Acc's */

char AVName[9]=""; /* AV-Name der Hauptapplikation */
int	 AVStatus[3];  /* Was kann die Hauptapplikation */

void AVResetProtoStatus(void) {
	AVActiveFlag = 0;
	*AVName      = 0;
	AVStatus[0]  = 
	AVStatus[1]  =
	AVStatus[2]  = 0;
}

/*
	Die folgende Funktion weicht in EINIGEN Punkten vom Original
	ab.
*/
int	AVActive(void) {
	short	msg[8];
	const char *avserver;
	int i;
	
	if ( AVActiveFlag &&
	     /* Sicherstellen, daû der AVServer auch noch lÑuft! */
	     ( !*AVName && !AVServerId ||
	       *AVName && AVServerId == appl_find( AVName ) ) )
		return AVStatus[0]|AVStatus[1]|AVStatus[2];
	else
		AVResetProtoStatus();
	
	if ( !_app || MultiAES() ) {
		if ( MultiAES() &&
		     ( ( AVServerId = appl_find( avserver = GEMINI ) ) >= 0 ||
		       ( AVServerId = appl_find( avserver = AVSERVER ) ) >= 0 ||
		       ( avserver = getenv( AVSERVER ) ) != NULL &&
		       strlen( avserver ) <= 8 ) ) {
		  strcpy( AVName, avserver );
		  for ( i = (int)strlen( avserver ); i < 8;  )
		  	AVName[i++] = ' ';
		  AVName[i] = '\0';
			AVServerId = appl_find( AVName );
		} else
			AVServerId = -1;
		if ( AVServerId < 0 )
			AVServerId = 0;
		msg[0] = AV_PROTOKOLL;
		msg[1] = AVMyApId;
		msg[2] = 0;
		AVSTR2MSG(msg,6,AVMyName);
		msg[3] = AVMyProtoStatus;
		msg[4] = msg[5] = 0;
		appl_write(AVServerId,16,msg); 
		AVStatus[0] = AVStatus[1] = AVStatus[2] = 0;
		AVActiveFlag = 1;
	}
	return AVActiveFlag;
}

void AVGetNewProtoStatus(void) {
	AVResetProtoStatus();
	AVActive();
}

void AVInit(int myapid,char *myname,int myprotostatus) {
	AVMyName        = myname;
	AVMyProtoStatus = myprotostatus;
	AVMyApId        = myapid;
	AVActive();
}

void AVExit(void) {
	short msg[8];
	
	if ( AVActiveFlag && AVHasProto(1,3) &&
	     /* Sicherstellen, daû der AVServer auch noch lÑuft! */
	     ( !*AVName && !AVServerId ||
	       *AVName && AVServerId == appl_find( AVName ) ) ) {
		msg[0] = AV_EXIT;
		msg[1] = AVMyApId;
		msg[2] = 0;
		msg[3] = AVMyApId;
		msg[4] = msg[5] = msg[6] = msg[7] = 0;
		appl_write(AVServerId,16,msg);
		AVResetProtoStatus();
	}
}

int	AVProcessMsg(int *msg) {
	int	ret;
	
	switch (msg[0]) {
		case AC_CLOSE:
			AVGetNewProtoStatus();
			return 1;
		
		case AP_TERM:
			AVExit();
			return 1;
							
		case VA_PROTOSTATUS:	
			if ( msg[3]==AVStatus[0] &&
			     msg[4]==AVStatus[1] &&
			     msg[5]==AVStatus[2])
				ret = 0;
			else {
				ret = 1; 
				AVStatus[0] = msg[3];
				AVStatus[1] = msg[4];
				AVStatus[2] = msg[5];
				AVActiveFlag = 1;
			}
			if (AVMSG2STR(msg,6))
				strcpy(AVName,AVMSG2STR(msg,6));
			else
				*AVName = 0;
			return ret;
	}
	return 0;
}

int	AVSendStatus(char *status) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,7)) {
		msg[0] = AV_STATUS;
		msg[1] = AVMyApId;
		msg[2] = 0;
		AVSTR2MSG(msg,3,status);
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

void AVReceiveStatus(void) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,7)) {
		msg[0] = AV_GETSTATUS;
		msg[1] = AVMyApId;
		msg[2] = 0;
		appl_write(AVServerId,16,msg);
	}
}

int	AVAskFileFont(void) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,1)) {
		msg[0] = AV_ASKFILEFONT;
		msg[1] = AVMyApId;
		msg[2] = 0;
		appl_write(AVServerId,16,msg);
		return 1;
	}
	return 0;
}

int	AVAskConsoleFont(void) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,2)) {
		msg[0] = AV_ASKCONFONT;
		msg[1] = AVMyApId;
		msg[2] = 0;
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

void AVAskSelectedObjects(void) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,3)) {
		msg[0] = AV_ASKOBJECT;
		msg[1] = AVMyApId;
		msg[2] = 0;
		appl_write(AVServerId,16,msg); 
	}
}

int	AVOpenConsole(void) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,2)) {
		msg[0] = AV_OPENCONSOLE;
		msg[1] = AVMyApId;
		msg[2] = 0;
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

int	AVOpenWindow(char *pfad,char *mask) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,4)) {
		msg[0] = AV_OPENWIND;
		msg[1] = AVMyApId;
		msg[2] = 0;
		if (pfad[strlen(pfad)-1] != '\\')
			strcat(pfad,"\\");
		AVSTR2MSG(msg,3,pfad);
		AVSTR2MSG(msg,5,mask);
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

int	AVStartProgram(char *pfad, char *cmdline) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,5)) {
		msg[0] = AV_STARTPROG;
		msg[1] = AVMyApId;
		msg[2] = 0;
		AVSTR2MSG(msg,3,pfad);
		AVSTR2MSG(msg,5,cmdline);
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

int	AVAccOpenedWindow(int winhandle) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,6)) {
		msg[0] = AV_ACCWINDOPEN;
		msg[1] = AVMyApId;
		msg[2] = 0;
		msg[3] = winhandle;
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

int	AVAccClosedWindow(int winhandle) {
	short	msg[8];

	if (AVActive() && AVHasProto(0,6)) {
		msg[0] = AV_ACCWINDCLOSED;
		msg[1] = AVMyApId;
		msg[2] = 0;
		msg[3] = winhandle;
		appl_write(AVServerId,16,msg); 
		return 1;
	}
	return 0;
}

int	AVSendKeyEvent(unsigned int state, unsigned int key) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,0)) {
		msg[0] = AV_SENDKEY;
		msg[1] = AVMyApId;
		msg[2] = 0;
		msg[3] = state;
		msg[4] = key;
		appl_write(AVServerId,16,msg); 
		wind_update(BEG_UPDATE);
		wind_update(END_UPDATE);
		return 1;
	}
	return 0;
}

int	AVCopyDragged(unsigned int kstate,char *dest) {
	short	msg[8];
	
	if (AVActive() && AVHasProto(0,8)) {
		msg[0] = AV_COPY_DRAGGED;
		msg[1] = AVMyApId;
		msg[2] = 0;
		msg[3] = kstate;
		AVSTR2MSG(msg,4,dest);
		appl_write(AVServerId,16,msg);
		return 1;
	}
	return 0;
}
