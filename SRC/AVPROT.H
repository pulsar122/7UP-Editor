/*****************************************************************************
	7UP
	Header: AVPROT.H

	1997-03-25 (MJK):	Header begonnen
*****************************************************************************/

#ifndef AVPROT_H
#define AVPROT_H

/*****************************************************************
	Prototypen
*****************************************************************/

#ifdef TCC_GEM
int appl_getinfo(int ap_gtype, int *ap_gout1, int *ap_gout2, int *ap_gout3, int *ap_gout4);
int appl_search( int ap_smode, char *ap_sname, int *ap_stype, int *ap_sid );
#endif

void hndl_AVProt(int msgbuf[]);
int relay( int myid, int apid, int argc, char *argv[]);
int My_appl_find(int my_id, char *ap_sname);

/* 1997-04-23 (MJK): neuerings in VAFUNC
void AVExit(int myapid);
*/

#endif /* AVPROT_H */