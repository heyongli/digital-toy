#ifndef _METAL_IR_
#define _METAL_IR_



/* IR decoding*/
int _IR_NEC_busy_decode(volatile unsigned char* port_addr, unsigned char bit);
#define IR_NEC_busy_decode(PORT,bit) _IR_NEC_busy_decode(&PORT,bit)



#endif
