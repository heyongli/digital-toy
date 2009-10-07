
#include <config.h>

#include <4x8seg_vled.h>

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
void testseg()
{
   unsigned char n;
   segon(0xF);
	  
   for(n=0;n<=9;n++){
	   segData = seg0_9[n];
	  mdelay(128);
   
   }
   
   for(n=0;n<=9;n++){
	   segData = seg0_9[n] & segDOT;
	  mdelay(128);
   }

}


void testvled()
{
  unsigned char n;
  vledon();
  
  segData = 0xff;
  segData = 0;
  mdelay(255);
  segData = 0xff;
  mdelay(255);
   
  for(n=0;n<=8;n++){
	  segData = ~(1<<n);
	  mdelay(128);
  }

  vledoff();
}

void vled_flowtick()
{
  static char n=0; 
  vledon();

  segData = ~(1<<n);

  if(7==n)
      n=0;
  else
      n++;

  vledoff();
}


