
/******************
  12M Hz

  1 cycle:  12/12M = 1us 

*******************/

#include <delay.h>


 


/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void _delay_ms(unsigned char ms)
{

/*
    35:    while(--t){     
C:0x0005    1E       DEC      R6               1 cyc
C:0x0006    EE       MOV      A,R6	           1 cyc
C:0x0007    600C     JZ       C:0015           1 cyc
    36:      _2cycle(250); 
C:0x0009    7FFA     MOV      R7,#0xFA	       1 cyc
C:0x000B    120027   LCALL    _2cycle(C:0027)   2 cyc
    37:         _2cycle(250); 
C:0x000E    7FFA     MOV      R7,#0xFA		   1 cyc , compasation by _delay_us
C:0x0010    120027   LCALL    _2cycle(C:0027)   2 cyc, compasation by _delay_us
    38:    } 
C:0x0013    80F0     SJMP     C:0005           2 cyc
*/
   while(ms--){	  
     _delay_us(497); //²¹³¥3 cycle for _delay_ms call
	 _delay_us(492); //8 cycle s for all of _delay_ms instuctinos
   }
  
}

/*
 * +- 50ms
 */
void sleep(unsigned char t)
{
   while(t--){
   	 _delay_ms(250);
   	 _delay_ms(250);
     _delay_ms(250);
     _delay_ms(250);
   }
     
}
