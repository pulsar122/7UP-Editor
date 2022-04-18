/*****************************************************************
	7UP
	Header: WIND_.H
	(c) by TheoSoft '90
	
	1997-04-14 (MJK): Header neu angelegt
*****************************************************************/

#ifndef WIND__H
#define WIND__H

/*****************************************************************
	Prototypen
*****************************************************************/

int _wind_calc(int wh, int type, int kind, 
               int x, int y, int w, int h,
               int *xr, int *yr, int *wr, int *hr);
int _wind_get(int handle, int field,
              int *xr, int *yr, int *wr, int *hr);

#endif /* WIND__H */