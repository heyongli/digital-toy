//#include "reg51.h"
#include <AT89X51.h>

		   
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


ona()
{
   P2_0=1; xdelay();
   P2_1=1; xdelay();
   P2_2=1; xdelay();
}

offa()
{

   P2_0=0; xdelay();
   P2_1=0; xdelay();
   P2_2=0; xdelay();
}

qon()
{
   P2_0=1;
      	 ldelay();	ldelay();

   P2_1=1;
      	 ldelay(); ldelay();
   P2_2=1;
      	 ldelay();	 ldelay();
							
}
void main(void)
{
   while(1){
   	 qon();
	 ldelay();
	 ldelay();ldelay();
	 offa();

     ldelay();
     ona();
     ldelay();
     offa();
     ldelay();
     ona();
	 ldelay();
     offa();


   }

}

