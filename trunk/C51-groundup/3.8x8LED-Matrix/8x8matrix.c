
#include <AT89X52.H>
		   
xdelay()
{
  int a;
  for (a=0; a<50; a++); 
}

ldelay()
{
  int a;
  for (a=0; a<30000; a++); 
}

#define wait1s ldelay(); ldelay(); xdelay();


  
unsigned char  taba[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char  tabb[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
void delay(void)
{
  unsigned char i,j;
  for(i=10;i>0;i--)
  for(j=248;j>0;j--);
}
void delay1(void)
{
  unsigned char i,j,k;
  for(k=10;k>0;k--)
  for(i=20;i>0;i--)
  for(j=248;j>0;j--);
}


blink1()
{
   char i,j,k;
   for(j=0;j<3;j++)
        {
          for(i=0;i<8;i++)
            {
              P3=taba[i];
              P2=0xff;
              delay1();
            }
        }
      for(j=0;j<3;j++)
        {
          for(i=0;i<8;i++)
            {
              P3=taba[7-i];
              P2=0xff;
              delay1();
            }
        }
      for(j=0;j<3;j++)
        {
          for(i=0;i<8;i++)
            {
              P3=0x00;
              P2=tabb[7-i];
              delay1();
            }
        }
 
      for(j=0;j<3;j++)
        {
          for(i=0;i<8;i++)
            {
              P3=0x00;
              P2=tabb[i];
              delay1();
            }
        }

}

blink(){

   unsigned char x=0, y=0;
   for(x=0; x<=0xfe; x++){
	  for(y=0xff;y>=1; y--)
	   {
	 
	     P3=x;
		 P2=y;
		 xdelay();
	   
	   }

	}
     

 

}

void main(void)
{
 
  while(1)
    {
  	  blink();
	  blink1();
 
    }
}

					 
				