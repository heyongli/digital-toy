#define F_CPU           16000000

#ifndef uchar
#define	uchar	unsigned char
#endif

#define NOP asm("nop")

void Delay_1(void);
void ispDelay(void);
void ispWriteDelay(void);
