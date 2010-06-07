#include "config.h"
#include <adc.h>
#include <pwm.h>

#include <adc0832.h>

#include <charger.h>

#include <1602.h>



extern float  refV ; 
char t=0;
unsigned short i=0;

//easy charging
unsigned char duty=0, max_duty=15;
float current=0, voltage=0;
float vlimit = 7.0 ; //V
float climit = 1.1 ; //A
float ir = 0 ;  //battery internal resistor
void easy_charging();

char charging = 0;  //not incharging
void isdone();

//i_charger charger = INIT_CHARGER;


void io_init()
{
   pwm_init(); //pull down pwm
   lcd1602_init();
    lcd_cursor(0,0);
   lcd_puts("Welcom!");
   seg_init();
   mdelay(100);
 
}
void main()
{
   unsigned short n,x=0;
   unsigned short onduty=0;
   float adc;


   io_init();
	   
   timer0_init(); //pwm
  // timer1_init();

   //adc_init(); 
   irqon();   //enable global interrupt		
   sleep(0); // just refrence 
   while(1){
   
// KEY_FUNC P3_3  KEY_DISCHARGER  P3_2  KEY_RESUME    P3_1  
    
     if(key(KEY_OK))
	    ;

   	 if(key(KEY_DIS)) i++;
	   
    
	 if(key(KEY_RES)) i--;

   
	  voltage=adc_V();
      lcd_cursor(0,0);
      lcd_puts("V: ");
	  showVA(voltage*100);

	  current=adc_A();
  	  lcd_cursor(0,1);
      lcd_puts("A: ");
  	  showVA(current*100);
	  
	  lcd_puts(" pwm:");
  	  print10(duty);

	/*charging core*/
	 isdone();
	  easy_charging();
  	 
   /*end charging*/	
	  
   }
  
  
}

adj_c()
{

  char  i=0;

   for( i=1;i<max_duty;i++){
     current=adc_A();    
     if(current < climit){
        duty+=1;
	   if(duty>max_duty)
	       duty=max_duty;

     }
     
     if(current > climit){
        if(duty>0)
	         duty-=1;
	    if(duty==0)
	       duty==1;
    }

	 pwm_setduty(duty);
	 current=adc_A();
	 voltage=adc_V();
		  voltage=adc_V();
      lcd_cursor(0,0);
      lcd_puts("V: ");
	  showVA(voltage*100);

	  current=adc_A();
  	  lcd_cursor(0,1);
      lcd_puts("A: ");
  	  showVA(current*100);
   }  

}
void isdone()
{
    float bv=0;
  
   if(voltage < 0.5 || voltage>8.0) /*no battery or pull out*/
      charging = 0;

    if(1==charging) return; /*in charging*/

    /*try new charging */
    /*para*/
	vlimit = 7.0; /*test battry*/

    /*test battery*/
     pwm_setduty(0);
 	 mdelay(100);
     bv=voltage=adc_V();    	
     if(voltage> 3)
	    charging = 1;

	 if(0==charging) /*no battery*/
	    return;

	/*test internal resistor*/
    pwm_setduty(5);
	mdelay(100);
	mdelay(100);
  	  current=adc_A();
	  voltage=adc_V();
		  voltage=adc_V();
      lcd_cursor(0,0);
      lcd_puts("V: ");
	  showVA(voltage*100);

	  current=adc_A();
  	  lcd_cursor(0,1);
      lcd_puts("A: ");
  	  showVA(current*100);

	pwm_setduty(0);
	ir = (voltage-bv)/current;
    
	lcd_cursor(9,0);
    lcd_puts("ir:");
	print10(1000*ir);

	if(ir>1.5) {
	   lcd_cursor(9,0) ;
	   mdelay(100); mdelay(100);	mdelay(100);
       mdelay(100);	mdelay(100);
       lcd_puts("bad ir  ");
	}
	   
	/*set parameter */
	 vlimit = 6.9;
	 climit = 1.2;

     /*init start*/
     pwm_setduty(1);
	 mdelay(100);mdelay(100);
	 mdelay(100);mdelay(100);
	 current=adc_A();
	 voltage=adc_V();
	 duty=1;
	    /*adjust the current*/

	 adj_c();

}

void easy_charging()
{
   if(!charging) return;

   if(duty==0) goto done; /*already complete charging*/
  


   

   /*ending test*/
   if(current <0.100) /*drop to 50mA, so done*/
       goto done;

   if( voltage > (vlimit+ir*current)){
   	   if(duty>0)
	        duty-=1;
	   if(duty==0)
	      goto done;
   
   }
    
   pwm_setduty(duty);	  
        
      

   return;
/* complete the charging */
   done:
   {
      duty=0;
      pwm_setduty(duty);

	  bl_off();
      mdelay(55);
	  bl_on();
   	  mdelay(55);
	  bl_off();
      mdelay(55);
	  bl_on();
   	  mdelay(55);
	  bl_off();
      mdelay(55);
	  bl_on();
   	  mdelay(55);
	  
   }


}

