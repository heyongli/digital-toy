 #include <AT89X52.H>


/*
 *  run about cyc*2+2 cycle
 */
void _2cycle(unsigned char cyc)
{
   while(--cyc);
   /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/
}

/*小于6us严重不准确, 奇数非常准确,偶数误差约(+ -)1us(仿真) */
/*7-512us*/
#define udelay(t)  _2cycle((t/2)-2)



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
     udelay(489); //补偿12cyc*2 us
	 udelay(499);
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


main()
{

/*
  udelay(5);
  udelay(6);
  udelay(7);
  udelay(8);
  udelay(9);
  udelay(10);
  udelay(15);
  udelay(17);

  udelay(100);
  udelay(150);
  udelay(500);
*/
#if 0
  udelay(511);
  udelay(512);
  udelay(489);
#endif

  
  mdelay(1);
 
  mdelay(4);
 
  mdelay(10);
 
  mdelay(15);

  mdelay(100);
  mdelay(150);
  
  sleep(1);
  sleep(10);
  sleep(17);
  sleep	(20);
  while(1);
}

