#include "config.h"

#include <timer.h>
#include <i2c-soft.h>




//easy charging
unsigned char duty=0;


void io_init()
{
   seg_init();
   mdelay(200);
 
}



void charging_update_lcd()
{
  	   
  	  //showVA(100);

}



void main()
{
   unsigned short i=0;
   io_init();
	   
   //timer0_init(); //pwm
   timer1_init(); //1ms

   irqon();   //enable global interrupt		
   sleep(0); // just refrence 

#if 0
   for(i=0;1;i++){
	  setdot(i%4);
	  segprint10(i);
	  mdelay(222);
	  if(i>9999)
	    i=0;
   }
#endif

#define DL 1
/*sent start ADC to attiny13, address A2, cmd 12*/
again:
 segprint10(0);
 //mdelay(DL);

   i2c_start();              // send start sequence
  segprint10(1);
  //mdelay(DL);

   i2c_tx(0xA2);             // SRF08 I2C address with R/W bit clear
   segprint10(2);
   //mdelay(DL);

   i2c_tx(0x12);             // SRF08 command register address
   segprint10(3);
   //mdelay(DL);

   i2c_stop();               // send stop sequence
   segprint10(4);
   //mdelay(DL);

goto again;

   while(1){
   
// KEY_FUNC P3_3  KEY_DISCHARGER  P3_2  KEY_RESUME    P3_1  

     charging_update_lcd();
  
   }
  
  
}
