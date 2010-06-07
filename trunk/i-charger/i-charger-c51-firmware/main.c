#include "config.h"
#include <adc.h>
#include <pwm.h>

#include <adc0832.h>

#include <charger.h>

#include <1602.h>



extern float  refV ; 
char t=0;
unsigned short i=0;


//i_charger charger = INIT_CHARGER;


void io_init()
{
   pwm_init(); //pull down pwm
   lcd1602_init();
    lcd_cursor(0,0);
   lcd_puts("Welcom!! Dtoy");
   seg_init();
   mdelay(100);
 
}
void main()
{
   unsigned short n,x=0;
   unsigned short onduty=0;
   unsigned short adc;


   io_init();
	   
   timer0_init(); //pwm
  // timer1_init();

   //adc_init(); 
   irqon();   //enable global interrupt		
   sleep(0); // just refrence 
   while(1){
   
// KEY_FUNC P3_3  KEY_DISCHARGER  P3_2  KEY_RESUME    P3_1  
    
     if(key(KEY_OK))
	    pwm_setduty(i++);

   	 if(key(KEY_DIS))
	    bl_on();
    
	 if(key(KEY_RES))
	    bl_off();

	  adc=adc0832(0);
      lcd_cursor(0,0);
      lcd_puts("Ch 0: ");
	  print10(adc);

	  adc=adc0832(1);
  	  lcd_cursor(0,1);
      lcd_puts("Ch 1: ");
  	  print10(adc);
	  
	  lcd_puts("  ");
  	  print10(x++);
	  
   }
  
  
}
