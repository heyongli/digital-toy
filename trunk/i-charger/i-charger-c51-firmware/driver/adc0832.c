#include <config.h>

#include "adc0832.h"


		
#define CS	 P1_1
#define CLK	 P1_4

#define DI   P1_3
#define DO   P1_2

unsigned char adc0832(bit ch)
{

   unsigned char i, dat0=0, dat1=0;

   
   CLK = 0;   //init clk
   
   DO = 1;    //prepare start bit
   CS=0; 	  //enable 
   udelay(1); //delay tsu
   CLK = 1;   //up edge, send start bit
   udelay(1);

   
   CLK = 0;   //prepare send SGL/DIF
   udelay(1);
   DO = 1;    //SGL/DIF = 1, channel mode
   CLK =1;    // up edge, send SGL/DIF
   udelay(1);

   
   CLK=0;    // prepare send ODD/EVEN, ch
   udelay(1);
   DO = ch;	  //send channel, 0,or 1
   CLK=1;     //up ledge;
   udelay(1);

   
   CLK=0;  // 
   DI=1;   //prepare input
   udelay(1); //delay tsu
   for(i=0;i<8;i++){
     CLK =1;
     udelay(1);         //没有这些delay则LSB,MSB方式读取的值会不等, 太快了
	 CLK =0;  //donw edge /MSB first
     udelay(1);
	 if(DI)
	 	dat0 |= 0x80>>i;   
   }

   for(i=0;i<8;i++){ //LSB first
     udelay(1);
     if(DI)
	 	dat1 |= 0x01<<i;   
	  CLK =1;
      udelay(1);
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



float  refV = 4.10 ; 

float adc_V()
{
     unsigned short adc_v;
		    
     adc_v = adc0832(1);  
	 mdelay(10);
	 adc_v += adc0832(1);  

	 adc_v /=2;
	 
	 return (((float)adc_v)/255)*refV*1.9; //10mV
}



float adc_A()
{
   	unsigned char adc_i;
	float tmp=0.0;
    /*sample the charger current*/
	adc_i = adc0832(0);
	mdelay(10);
	adc_i += adc0832(0);
	adc_i /=2;
  	
  	tmp= ((float)adc_i/255)*refV;
	tmp = tmp/20;
	return tmp = tmp/0.10;
	
}