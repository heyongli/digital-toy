
/******************
  12M Hz

  1 cycle:  12/20M = 1us 

*******************/
#include  "config.h"

/*
 *  run about cyc*2+2 cycle
 */
void _2cycle(unsigned char cyc)
{
   while(--cyc);	 //while(--cyc) have bug when cyc==0, but we must use --cyc to genarate one inst
   /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/
}





/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void mdelay(unsigned char ms)
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
C:0x000E    7FFA     MOV      R7,#0xFA		   1 cyc
C:0x0010    120027   LCALL    _2cycle(C:0027)   2 cyc
    38:    } 
C:0x0013    80F0     SJMP     C:0005           2 cyc
*/
   while(ms--){	  
     udelay(400); //²¹³¥12cyc*2 us
	 udelay(412);
   }
  
}

/*
 * +- 50ms
 */
void sleep(unsigned char t)
{
   while(t--){
   	 mdelay(250);
   	 mdelay(250);
     mdelay(250);
     mdelay(250);
   }
     
}
