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

unsigned char seg0_9[]={
        0XC0,/*0*/
        0XF9,/*1*/
        0XA4,/*2*/
        0XB0,/*3*/
        0X99,/*4*/
        0X92,/*5*/
        0X82,/*6*/
        0XF8,/*7*/
        0X80,/*8*/
        0X90,/*9*/

#if 0
        0X3F,/*0*/
        0X06,/*1*/
        0X5B,/*2*/
        0X4F,/*3*/
        0X66,/*4*/
        0X6D,/*5*/
        0X7D,/*6*/
        0X07,/*7*/
        0X7F,/*8*/
        0X6F,/*9*/
#endif
};

#define segA   0X77 /*A*/
#define segb   0X7C,/*b*/
#define segC   0X39,/*C*/
#define segc   0X58,/*c*/
#define segd   0X5E,/*d*/
#define segE   0X79,/*E*/
#define segF   0X71,/*F*/

#define segH   0X76,/*H*/
#define segh   0X74,/*h*/
#define segL   0X38,/*L*/
#define segP   0X73,/*P*/
#define segt   0X78,/*t*/
#define seg-   0X40,/*-*/

#define segDOT  0X7F  /*dot*/


#define segENA1  P2_0
#define segENA2  P2_1
#define segENA3  P2_2
#define segENA4  P2_3
#define vledENA  P2_4

#define segData	 P0



void segoff( unsigned char segbit)
{
   if(segbit&0x1)
        segENA1=1;
   if(segbit&0x2)
        segENA2=1;
   if(segbit&0x4)
       segENA3=1;
   if(segbit&0x8)
       segENA4=1;
/*
   switch bit {
     case 1:
        segENA1=1;
     case 2:
        segENA2=1;
     case 3:
        segENA3=1;
     case 4:
        segENA4=1;
     
   }
   */
}

void segon(unsigned char segbit)
{
   if(segbit&0x1)
        segENA1=0;
   if(segbit&0x2)
        segENA2=0;
   if(segbit&0x4)
       segENA3=0;
   if(segbit&0x8)
       segENA4=0;
   
}

#define vledon()   segoff(0xF); vledENA=0;
#define vledoff()  vledENA=1;

void segchar(unsigned char c, unsigned char segbit)
{
   segoff(0xF);
   segData = c;	     
   switch ( segbit ){
     case 1:
        segENA1=0;
		break;
     case 2:
        segENA2=0;
		break;
     case 3:
        segENA3=0;
		break;
     case 4:
	    segENA4=0;
		break;
	 default:
	    segoff(0xF);
		segData = segDOT;
     
   }
}
testseg()
{
   unsigned char n;
   segon(0xF);
	  
   for(n=0;n<=9;n++){
	   segData = seg0_9[n];
	  mdelay(255);
	  mdelay(128);
	   
   }
   
   for(n=0;n<=9;n++){
	   segData = seg0_9[n] & segDOT;
	  mdelay(255);
      mdelay(128);	   
   }

}


testvled()
{
  unsigned char n;
  vledon();
  
  segData = 0xff;
  segData = 0;
  mdelay(255);
  segData = 0xff;
  mdelay(255);
  segData = 0;
  mdelay(255);
  segData = 0xff;
   
  for(n=0;n<=8;n++){
	  segData = ~(1<<n);
	  mdelay(255);
	  mdelay(128);
  }

  vledoff();
}


void main()
{
  segoff(0xF);

again:
  testseg();
  testvled();
  goto again;
}


