
#include <config.h>

#include <digit-segs.h>
#include <string.h>






#define segDOT  0X7F  /*dot*/



#define segH   0X76,/*H*/
#define segh   0X74,/*h*/
#define segL   0X38,/*L*/
#define segP   0X73,/*P*/
#define segt   0X78,/*t*/
#define seg-   0X40,/*-*/

#if 0  /*节省变量空间, ft*/
unsigned char seg0_f[]={
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
      
        0X88,/*A*/
        0X83,/*b*/
        0XC6,/*C*/
        0XA1,/*d*/
        0X86,/*E*/
        0X8E,/*F*/

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

#endif

unsigned seg0_f(char c)
{
   switch(c){
      case 0:
	      return  0XC0;
   	  case 1:
	      return  0XF9;
   	  case 2:
	      return  0XA4;
   	  case 3:
	      return  0XB0;
      case 4:
	      return  0X99;
      case 5:
	      return  0X92;
      case 6:
	      return  0X82;
      case 7:
	      return  0XF8;
      case 8:
	      return  0X80;
      case 9:
	      return  0X90;
      case 0xa:
	      return  0X88;
      case 0xb:
	      return  0X83;
      case 0xc:
	      return  0XC6;
      case 0xd:
	      return  0XA1;
      case 0xe:
	      return  0X86;
      case 0xf:
	      return  0X8e;
      default:
	      return  0XFF;
   }
}
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


#if 0
void segchar(unsigned char c, unsigned char segbit)
{
   segoff(0xF);
   segData = c;	     
   switch ( segbit ){
     case 0:
        segENA1=0;
		break;
     case 1:
        segENA2=0;
		break;
     case 2:
        segENA3=0;
		break;
     case 3:
	    segENA4=0;
		break;
	 default:
	    ;
     
   }
}
#endif
void testseg()
{
   unsigned char n;
   segon(0xF);
	  
   for(n=0;n<=9;n++){
	   segData = seg0_f(n);
	  mdelay(10);
   
   }
   
   for(n=0;n<=9;n++){
	   segData = seg0_f(n) & segDOT;
	  mdelay(10);
   }

}

/*===========================================================================*/
/*
 * 数码管扫描显示 core
 */





/*
 *
 *  vblock[0] .... vblock[3]    vled
 *
 */
//unsigned vblock[5]={0xFF,0xFF,0xFF,0xFF,0xFF};

unsigned char vblock[segDigits]={
               0XC0,/*0*/
               0XC0,/*1*/
               0XC0,/*2*/
			   0XC0,/*3*/
			   };
volatile unsigned char dot = segDigits,lastdot=segDigits;

/*
 * 根据 vblock内容 扫描驱动数码管和电平管显示
 */
void ms_scan_segvled() using 1 
{
	 static unsigned char block = 0;
	 
	 //process dots
	 if(lastdot<segDigits)
	   vblock[lastdot] |= ~segDOT; //set dot bit, clear the dot

	 if(dot<segDigits){
	   	vblock[dot] &=segDOT;  //clear bit  
		lastdot = dot;
	 }
	
	  //scan segDigits x8 seg
	 {
	 	segoff(0xF);
		segData = vblock[block];	     
	    segon(1<<block);	
   
	 } 
	 block++;
	 if(segDigits == block)
	   block = 0;

}

void seg_init()
{
   //power on test
   segoff(0xF);
   testseg();

   //close all
   segoff(0xF);
   dot=100; //disable
}

void printhex(unsigned short n)
{
     //irqoff();
	 vblock[0]= seg0_f( (n/100) );
	 n = n%100;
	 vblock[1]= seg0_f( n/10 );
	 n = n%10;
		   	
	 vblock[2]= seg0_f((n));
	 //irqon();
}


/*
 * set dot position
 */		   
void setdot(unsigned char n) using 1
{
   dot=n;

}
void segprint10(unsigned short n)
{
   char i=0;

	 vblock[0]= seg0_f( (n/1000) );
	 n = n%1000;

     //irqoff();
	 vblock[1]= seg0_f( (n/100) );
	 n = n%100;
	 vblock[2]= seg0_f(n/10);
	 n = n%10;
		   	
	 vblock[3]= seg0_f( (n) );
	 //irqon();
}
		   		   
