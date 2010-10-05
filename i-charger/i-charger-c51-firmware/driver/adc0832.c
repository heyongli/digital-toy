#include <config.h>

#include "adc0832.h"

#define  refV  5.08
		
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
     	return dat1;

	}
    
   return dat0;

}





float adc_V()
{
    unsigned int adc=0;
	unsigned char i,max=0,min=0,adc_t;
	
    float a=0;		    

	max=min= adc0832(1);
	     /*sample the charger current*/
    for(i=0;i<18;i++){
	    adc_t = adc0832(1);
		if(adc_t> max)
		 	max = adc_t;
		if(adc_t < min)
			min = adc_t;

		adc += adc_t;

	}
	adc -= max;
	adc -= min;

	adc >>= 4;//div 8
   
    a=adc_A();	 
	return (((float)adc)/255)*refV*1.9-a*0.17; //10mV
}



float adc_A()
{
    unsigned int adc=0;    
   	unsigned char i,max=0,min=0,adc_t;
	float tmp=0.0;

   	max=min= adc0832(0);
	     /*sample the charger current*/
    for(i=0;i<18;i++){
	    adc_t = adc0832(0);
		if(adc_t> max)
		 	max = adc_t;
		if(adc_t < min)
			min = adc_t;

		adc += adc_t;

	}
	adc -= max;
	adc -= min;
	adc >>=4; //16 times
 	
  	tmp= ((float)adc/255)*refV;
	tmp = tmp/19;
	return tmp = tmp/0.17;
	
}