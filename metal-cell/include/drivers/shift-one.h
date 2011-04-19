#ifndef _METAL_SHIFT_ONE_
#define _METAL_SHIFT_ONE_



/*shift1  one wired 595*/
void _write_shif1(volatile unsigned char* port_addr, unsigned char bit, unsigned char data);
void _init_shift1(volatile unsigned char *port_addr, unsigned char bit);
#define write_shift1(PORT, bit, data)  _write_shif1(&PORT, bit, data)
#define init_shift1(PORT,bit)  _init_shift1(&PORT,bit)


#endif
