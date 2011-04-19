
#ifndef _uC_COMPILER_
#define _uC_COMPILER_



#define barrier() __asm__ __volatile__("": : :"memory")


#endif
