/*
 * CopyLeft hyl 2009/12/21
 */

#ifndef _BITOPS_H_
#define _BITOPS_H_



/* bit ops*/
#define  _clear_bit(x,n)  (x)&=~(1<<(n))
#define  _set_bit(x,n)    (x)|= (1<<(n))
#define  _test_bit(x,n)    ( (x)&(1<<(n)))
#define  _toggle_bits(x,mask) ((x)^=(mask)) 



/* mask ops */
#define _MASK8(n,m)      ((0xFF<<(n))&(0xFF>>(7-(m))))
                          /*n=2, m=3*/
				          /* xxxx XX00  &  0000 XXXX*/

#define  _bits8(val,n,m)  ( (val<<(n))&(_MASK8(n,m)) )


/*
 * mov specifed bits in src to specified bits in dst
 * by ONE move INST, and KEEP other bits in dst un-touched 
 * useful bits ops, slow but convinent, PowePC rlwimi style
 */
#define __only_bits8(v,n,m)  ( (_MASK8((n),(m)))&(v))
#define __clear_bits8(v,n,m)  ( (~_MASK8((n),(m)))&(v))

#define _mov_bits8(dst,src,dn,dm,sn,sm) \
			dst = (   \
			         (  \
			          __only_bits8((src),(sn),(sm))>>(sn) \
					 ) <<(dn) \
				  ) |  \
			      (     \
				    __clear_bits8(dst,dn,dm) \
				  )



#define rol8(a,n) \
           ( \
		     ((a)>>(8-(n))) | \
			 ((a)<<(n)) \
		   ) 

#define ror8(a,n) \
           ( \
		     ((a)<<(8-(n))) | \
			 ((a)>>(n)) \
		   ) 

unsigned char _swap8(unsigned char x);



/*******************************************************************/
/*  obsolute                 */
#if 0


/*advance bitops*/
#define  _clear_nm8(x,n,m)  x&= ~( _MASK8(n,m))
#define  _nm8(val,n,m)      (val<<(n))&(_MASK8(n,m))
#define  _set_nm8(x,val,n,m) _clear_nm8(x,n,m); \
							x |= _nm8(val,n,m)


/*for AVR */
#define  _pin_in(port,n) /* (D, 3)*/ \
			DDR##port &= ~(1<< DD##port##n);  /*enable input*/   \
			PORT##port |= (1<< P##port##n)    /*pull-up-enable*/  

#define  _pin_out(port,n)\
			DDR##port |= (1<< DD##port##n);  /*enable input*/   \
			
#endif


#endif

