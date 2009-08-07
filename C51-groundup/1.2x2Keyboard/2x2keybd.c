

//#include <REGX52.H>
#include <AT89X52.H>
		   
xdelay()
{
  int a;
  for (a=0; a<5000; a++); 
}

ldelay()
{
  int a;
  for (a=0; a<30000; a++); 
}

#define led1on 	    P2_0=0; xdelay();
#define led2on 	    P2_1=0; xdelay();
#define led3on 	    P2_2=0; xdelay();

#define led1off    	 P2_0=1; xdelay();
#define led2off    	 P2_1=1; xdelay();
#define led3off    	 P2_2=1; xdelay();



// h1,h2--> p1_0, p1_1
void keyscan(void)

{

  unsigned char n;
/*
  电路
  p1_0     -- R ---	+	   +
  p1_1     --R---	+	   +
  p1_2 行1------------	   +
  p1_3 行2-----------------
*/
  //扫描第一行
  P1=0xff; 
  xdelay();
     
  P1=0xfb; //拉低第一行	, 1011
  xdelay();
  n=P1;
  if(n!=0xfb){
        switch(n){
             case(0xf9):
			        led2on;break;
             case(0xfa):
			         led3on;break;
			 case 0xf8 :
			         led2on;led3on;break;
			 default: led1on;ldelay(); ldelay();led1off; ldelay(); ldelay();break;
      }
  }

  led2off;
  led3off
}




void main(void)
{

   while(1){
 
   	 led1on;

     keyscan();


     
   }

}

