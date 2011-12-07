#ifndef PPC_DUMP_H
#define PPC_DUMP_H

/* mask ops */
/*bits define */
/* 31 30 .....      4  3  2  1  0*/
//				e      s
/* 0 1 2 3 4 ......................31*/  //BE
//    s      e
#define _MASK32(s,e)      ((0xFFFFffff<<(s))&(0xFFFFffff>>(31-(e))))
                          /*s=2, e=3*/
					          /* xxxx XX00  	& 	 0000 XXXX*/

#define  _VAL32(val,s,e) (  ((val)&(_MASK32((s),(e))) )>>(s))
#define  _VAL32BE(val,s,e)  (_VAL32(val,(31-e), (31-s)))

//make it simple
//#define  _VBE(val,s,e) ({ typeof(val) _xval=-1;  if(sizeof(val)==32)_xval=_VAL32BE(val,s,e);  _xval; })







#endif
