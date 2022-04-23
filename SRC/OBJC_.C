/*****************************************************************
	7UP
	Modul: OBJC_.C
	(c) by TheoSoft '90
	
	Erg„nzen und korrigieren fehlender objc-Funktionen
	
	1997-04-09 (MJK): Funktionen in dieses neue Modul ausgelagert
*****************************************************************/

#ifdef TCC_GEM
#	include <aes.h>
#else
#	include <gem.h>
#endif

#include "forms.h"   /* wegen windial */
#include "windows.h" /* wegen rc_intersect */
#include "wind_.h"   /* wegen _wind_get, _wind_calc */

#include "objc_.h"

#define FLAGS15 0x8000

void objc_update(OBJECT *tree, int obj, int depth)
{
	int full[4],area[4],array[4];
		
	objc_offset(tree,obj,&array[0],&array[1]);
	if(obj==ROOT) /* 3 Pixel Rand beachten */
	{
		array[0]-=3;
		array[1]-=3;
		array[2]=tree[obj].ob_width+6;
		array[3]=tree[obj].ob_height+6;
	}
	else
	{
		array[2]=tree[obj].ob_width;
		array[3]=tree[obj].ob_height;
	}
	if(windials && dial_handle!=-1 && !(tree->ob_flags & FLAGS15))
	{
		wind_update(BEG_UPDATE);
		_wind_get( 0, WF_WORKXYWH,  &full[0], &full[1], &full[2], &full[3]);
		_wind_get(dial_handle, WF_FIRSTXYWH, &area[0], &area[1], &area[2], &area[3]);
		while( area[2] && area[3] )
		{
			if(rc_intersect((GRECT *)full,(GRECT *)area))
				if(rc_intersect((GRECT *)array,(GRECT *)area))
					objc_draw(tree,obj,depth,area[0],area[1],area[2],area[3]);
			_wind_get(dial_handle, WF_NEXTXYWH,&area[0],&area[1],&area[2],&area[3]);
		}
		wind_update(END_UPDATE);
	}
	else
	{
		objc_draw(tree,obj,depth,array[0],array[1],array[2],array[3]);
	}
}
