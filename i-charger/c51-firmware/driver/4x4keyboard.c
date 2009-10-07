
#include <config.h>
		   

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
  udelay(5);
     
  P1=0xfb; //拉低第一行	, 1011
  udelay(5);
  n=P1;
  if(n!=0xfb){
        switch(n){
             case(0xf9):
			        break;
             case(0xfa):
			        break;
			 case 0xf8 :
			        break;
			 default: 
			        break;
      }
  }


}


