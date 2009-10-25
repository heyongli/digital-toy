#include <config.h>

#include "adc0832.h"



#define CS	 P3_0
#define CLK	 P3_1

#define DI   P3_2
#define DO   P3_3

unsigned char adc0832(bit ch)
{

   unsigned char i, dat0=0, dat1=0;

   
   CLK = 0;   //init clk
   
   DO = 1;    //prepare start bit
   CS=0; 	  //enable 
   udelay(0); //delay tsu
   CLK = 1;   //up edge, send start bit
   udelay(0);

   
   CLK = 0;   //prepare send SGL/DIF
   udelay(0);
   DO = 1;    //SGL/DIF = 1, channel mode
   CLK =1;    // up edge, send SGL/DIF
   udelay(0);

   
   CLK=0;    // prepare send ODD/EVEN, ch
   udelay(0);
   DO = ch;	  //send channel, 0,or 1
   CLK=1;     //up ledge;
   udelay(0);

   
   CLK=0;  // 
   DI=1;   //prepare input
   udelay(0); //delay tsu
   for(i=0;i<8;i++){
     CLK =1;
     udelay(0);         //没有这些delay则LSB,MSB方式读取的值会不等, 太快了
	 CLK =0;  //donw edge /MSB first
     udelay(0);
	 if(DI)
	 	dat0 |= 0x80>>i;   
   }

   for(i=0;i<8;i++){ //LSB first
     udelay(0);
     if(DI)
	 	dat1 |= 0x01<<i;   
	  CLK =1;
      udelay(0);
	  CLK =0;  //donw edge
	 
   }
   CS=1;  //deselect chip
   DI=1;
   CLK=1;

   if(dat0==dat1) {
	//	vledmod('V'); 
     	return dat1;

	}
   //vledmod('H'); 
  
   return dat0;

}
