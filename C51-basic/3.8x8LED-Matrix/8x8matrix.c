
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

/* 按行区模(横向取模),
   第一行显示点, 第二行显示点...
    ---> col
   | b1    .
   | b2	   .
 row b3	   .
	 b4	   .
*/
unsigned char hztab[]=
{
  0x00,0x3C,0x24,0x24,0x24,0x3C,0x00,0x00,  //0
  0x00,0x08,0x18,0x08,0x08,0x1C,0x00,0x00,
  0x00,0x3C,0x04,0x04,0x3C,0x20,0x3C,0x00,
  0x00,0x3C,0x04,0x3C,0x04,0x3C,0x00,0x00,
  0x00,0x28,0x28,0x3C,0x08,0x08,0x00,0x00,
  0x00,0x3C,0x20,0x3C,0x04,0x3C,0x00,0x00,
  0x00,0x3C,0x20,0x3C,0x24,0x3C,0x00,0x00,
  0x00,0x3C,0x04,0x08,0x10,0x10,0x00,0x00,
  0x00,0x3C,0x24,0x3C,0x24,0x3C,0x00,0x00,
  0x00,0x3C,0x24,0x3C,0x24,0x3C,0x00,0x00,  //9
};


#define Rrow P2  //Row been pull high!!!
#define Rcol P3

unsigned char row[]={ 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};


show( unsigned char c, int ms)
{

   unsigned char *tab=hztab+8*c;
   char i;
   int t;
   for(t=0;t<ms;t++)
     for(i=0;i<8;i++) //scan from row1 to row8
     {
        Rrow=row[i];
        Rcol=~tab[i]; //low to light the led
	    udelay(500);
	
     }
}

void main(void)
{
   unsigned char i=0; 
   while(1)
    {
  	  //blink();
	  //blink1();
      for(i=0; i<9; i++){
         show(i,250);  //show 1s
	     sleep(1);
		 
      }
      
    }
}

					 
				