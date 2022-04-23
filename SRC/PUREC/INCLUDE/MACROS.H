/*
 *	MACROS.H	commonly useful macros
 */

#ifndef	_MACROS_H
#define	_MACROS_H

#ifdef __GNUC__	
/* with GNUC we will use safe versions, these may look like that they
 * have overhead, but they do not -- trust me!
 */

/* absolute value for any type of number */
#define abs(a) \
    ({typedef _ta = (a);  \
	  _ta _a = (a);     \
	      _a  < ((_ta)0) ? -(_a) : _a; })

/* maximum and minumum for any type of number */
#define max(a,b) \
    ({typedef _ta = (a), _tb = (b);  \
	  _ta _a = (a); _tb _b = (b);     \
	      _a > _b ? _a : _b; })
#define min(a,b) \
    ({typedef _ta = (a), _tb = (b);  \
	  _ta _a = (a); _tb _b = (b);     \
	      _a < _b ? _a : _b; })

/* swap any objects (even identically typed structs!) */
/* WARNING: not safe */
#define swap(a,b) \
    ({typedef _ta = (a);  \
	  _ta _t;     \
	      _t = (a); (a) = (b); (b) = _t; })

#else	/* be careful !! */

/* absolute value for any type of number */
#define	abs(x)		((x)<0?(-(x)):(x))

/* maximum and minumum for any type of number */
#define max(x,y)   	(((x)>(y))?(x):(y))
#define	min(x,y)   	(((x)<(y))?(x):(y))

/* swap any objects which can be XORed */
#define	swap(a,b)	((a)=(a)^((b)=(b)^((a)=(a)^(b))))

#endif /* __GNUC__ */

/* lo and hi byte of a word */
#define	lobyte(x)	(((unsigned char *)&(x))[1])
#define	hibyte(x)	(((unsigned char *)&(x))[0])

/* lo and hi word of a long */
#define	loword(x)	(((unsigned short *)&(x))[1])
#define	hiword(x)	(((unsigned short *)&(x))[0])

#endif /* _MACROS_H */
