
#include <config.h>
//#include <drivers/adc0832.h>
//#include <charger.h>
#include <drivers/lcd.h>
#include <drivers/4wire-1602.h>

//#include <timer.h>
#include <drivers/key.h>



void io_init()
{

	   
   // timer0_init(); //pwm
   //timer1_init(); //1ms
   lcd1602_init();
   lcd_cursor(0,0);
   lcd_puts("demo!!!");
   //adc_init(); 
   sti();   //enable global interrupt		
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
	 ds18b20_demo();
}



void main()
{
  
   
   io_init();
   sleep(0); // just refrence 

   lcm1630_demo();
   lcm1630_demo2();

   while(1){
	
	   update_lcd();
	   // uart_demo();
   }
  
  
}
