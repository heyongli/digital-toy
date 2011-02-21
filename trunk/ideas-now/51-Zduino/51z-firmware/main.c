#include "config.h"
#include <adc.h>
#include <pwm.h>

#include <adc0832.h>

#include <charger.h>

#include <1602.h>
#include <timer.h>



void io_init()
{

	   
   // timer0_init(); //pwm
   timer1_init(); //1ms
   lcd1602_init();
   lcd_cursor(0,0);
   //adc_init(); 
   irqon();   //enable global interrupt		
   DS1302_init();    
}

 void  update_lcd()
{
  	 
	 if(key(KEY_OK)){
	    lcd_cursor(0,0);
	    lcd_puts("oK ")	;

	  }

   	 if(key(KEY_UP)){
	    lcd_cursor(0,0);
	 	lcd_puts(" UP ");
    
	 }

	 if(key(KEY_DOWN)){ 
	   lcd_cursor(0,0);
	   lcd_puts(" down ");
	 }

	 ds1302_test();
}



void main()
{
  
   
   io_init();
   sleep(0); // just refrence 
   while(1){
		   
	   update_lcd();
   }
  
  
}
