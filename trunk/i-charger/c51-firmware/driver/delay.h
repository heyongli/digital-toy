
#ifndef _DELAY_H_
#define _DELAY_H_

void _2cycle(unsigned char cyc);


/*小于6us严重不准确, 奇数非常准确,偶数误差约(+ -)1us(仿真) */
/*#define udelay(t)  _2cycle((t/2)-2) 小于6不准因为 t/2-2 晕倒了*/


/*7-512us*/
#define udelay(t)  _2cycle(  ((t>>1)&0xFE) +1 ) //_2cycle can't accept 0
//#define udelay(t)  _2cycle((t/2)-2) /* _2cycle( ((t>>1)&0xFC) )*/


/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void mdelay(unsigned char ms);




/*
 * +- 50ms
 */
void sleep(unsigned char t);

#endif

