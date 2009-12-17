#ifndef _BITOPS_H_
#define _BITOPS_H_



/* bit ops*/
#define  _clear_bit(x,n)  (x)&=~(1<<(n))
#define  _set_bit(x,n)    (x)|= (1<<(n))
#define  _test_bit(x,n)   ( (x)&(1<<(n)))



/* mask ops */
#define _NM_MASK8(n,m)    (0xFF<<(n))&(0xFF>>(7-(m)))
                          /*n=2, m=3*/
				          /* xxxx XX00  &  0000 XXXX*/

#define  _clear_nm8(x,n,m)  x&= ~( _NM_MASK8(n,m))
#define  _nm8(val,n,m)      (val<<(n))&(_NM_MASK8(n,m))
#define  _set_nm8(x,val,n,m) _clear_nm8(x,n,m); \
							x |= _nm8(val,n,m)



#endif
