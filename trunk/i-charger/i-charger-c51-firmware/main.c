#include "config.h"
#include <adc.h>
#include <pwm.h>

#include <adc0832.h>

#include <charger.h>

#include <1602.h>
#include <timer.h>




//easy charging
unsigned char duty=0;
float current=0, voltage=0;
float vlimit = 0 ; //V
float climit = 0 ; //A
float ir = 0 ;  //battery internal resistor
unsigned long stime=0;
void easy_charging();

char charging = 0;  //not incharging
void isdone();
void easy_discharing();

void io_init()
{
   pwm_init(); //pull down pwm
   lcd1602_init();
   lcd_cursor(0,0);
   //lcd_puts("Welcom!");
  // seg_init();
}



unsigned char mode=0;
/*mode==2/ -Dv*/
/**/


char dvc=0,burst=0;

void select_battery()
{
  static short loop=0, num_nicd=0;
  if(mode) //mode selected done
      return; 

  climit = 1.2;

  while( 1 ){ //loop++<300, wait forerver, if unknown reset occur, stay there 

            
	 
	  if(key(KEY_OK)){

          if(0==vlimit && DIS_CHA!=mode) { //if not select mode&& not discharging, start charging 6V lead acid now
	         motor_charging:
	     	   vlimit = 7.1;
  	           climit = 1.2;
		       mode=1;
			   lcd_cursor(0,0);
			   lcd_puts("vlimit: ");   
               showVA(vlimit*100);
			   _delay_ms(200);
			   return;

		 }
         
		 if(DIS_CHA!=mode){ /*if not discharging, charging at selected vlimit*/
			mode =1; /*mode2:-dv cannot work now*/
		 }

		 return;
	  }

   	 if(key(KEY_RES))
	       num_nicd++;

	 if(key(KEY_DIS)){ 
	     if(num_nicd) 
	      	num_nicd--;
		 else{
		    mode= DIS_CHA;
			climit=300;	
		  }
	 }
	 
	 lcd_cursor(0,0);
	 if(DIS_CHA==mode){
	      lcd_puts("Discharging?..");   
	 }else{
	     lcd_puts("vlimit: ");   
         vlimit = num_nicd*1.4;    
		 if(1==num_nicd) {
		    vlimit = 1.45-0.04; //40mV resolution for adc, addtional 10mV
		  	climit = 1.4;
		  }
  	     showVA(vlimit*100);
	 }
   }
   //time out
   //goto motor_charging;

}

float maH = 0;
void  countmaH()
{
   	static long now=0;
	if(0==now)
	    now=jiffers;
    if(timeafter(jiffers, now+200))	{
	   maH+=current/3.6; /* I*time 1s, I*1s/(3600)aH *1000maH = I/3.6*/
	   now=jiffers;
	}

}
void charging_update_lcd()
{
  	  voltage = adc_V();;
      lcd_cursor(0,0);
      lcd_puts("V:");
	  showVA(voltage*100);

      lcd_puts(" ");
	  
	  //lcd_puts("duty:");print10(duty);
	   lcd_puts("ir"); print10(ir*1000);//print10((jiffers-stime)/200); 
        	  
	  //showVA(top_voltage*100);

	  current=adc_A();
  	  lcd_cursor(0,1);
	  lcd_puts("A:");
  	  showVA(current*100);
	  
	  //lcd_puts(" climit:"); print10(climit*100);
	  lcd_puts(" maH:"); print10(maH); /*maH*/


}



void main()
{
  
   
   io_init();
	   
   timer0_init(); //pwm
   timer1_init(); //1ms

   //adc_init(); 
   irqon();   //enable global interrupt		
   
   while(1){
   
// KEY_FUNC P3_3  KEY_DISCHARGER  P3_2  KEY_RESUME    P3_1  

      if(mode==0)
		   select_battery();

	  charging_update_lcd();

	/*charging core*/
	 if(DIS_CHA != mode){
	   isdone();
	   easy_charging();
	 }else{
	   easy_discharing();
	 }

  	 
	  countmaH();
   /*end charging*/	
	  
   }
  
  
}

#define max_duty 19
adj_c()
{

   char  i=0;
   duty=0;//reslect duty

   for( i=1;i<max_duty;i++){
     _delay_ms(5);
     current=adc_A();    
     if(current < climit){
        duty+=5;
	   if(duty>max_duty)
	       duty=max_duty;

     }
     
     if(current > climit){
        if(duty>0){	//from 1 to max, use the latest duty
	         duty-=1;
			 return;
		}
	    if(duty==0)
	       duty=1;
    }

	 pwm_setduty(duty);
	 charging_update_lcd();
   }  

}

void try_ir(char setpwm)
{
    
    
	float bv;
	pwm_setduty(0);
	_delay_ms(50);
	bv=adc_V();
    
    if(setpwm)
	  adj_c();
	else 
	   	pwm_setduty(duty);

	_delay_ms(150);
	voltage=adc_V();
	current=adc_A();
	
	_delay_ms(150);
	voltage+=adc_V();
	current+=adc_A();
	
   	voltage/=2;
	current/=2;
	
	if(setpwm)
	    pwm_setduty(0);
	
	ir = (voltage-bv)/current;
	   
}
void isdone()
{

  
   if(voltage < 0.5 || voltage>10.0) /*no battery or pull out*/	   {
      charging = 0;
	  charging_update_lcd();
	  bl_off();
	  _delay_ms(50);
	  bl_on();
  	  _delay_ms(50);

   }
    if(1==charging) return; /*in charging*/

    /*try new charging */
    /*para*/


    /*test battery*/
     pwm_setduty(0);
	 //PWM_DIS=1;
 	 _delay_ms(50); 
	 //PWM_DIS=0;
	 charging_update_lcd();
     voltage=adc_V();
	     	
     if(voltage> 0.8 && voltage <9)
	    charging = 1;

	 if(0==charging) /*no battery*/
	    return;

	/*test internal resistor*/
re_tryir:
	try_ir(1);
	lcd_cursor(0,0);
    lcd_puts("                  ");
	lcd_cursor(0,0);
    lcd_puts("ir:");
	print10(1000*ir);
  
  	if((ir>1.5) && duty>6) {
	   lcd_cursor(9,0) ;
	 
       lcd_puts("bad ir  ");
	   _delay_ms(50);
	   if(climit>0.2)
	   	  climit -= 0.1; 
	   goto re_tryir;
	}

	/*set parameter */

     /*init start*/
     pwm_setduty(1);
	 charging_update_lcd();
	 duty=1;
	 /*adjust the current*/

	 adj_c();

}

void easy_charging()
{
   static short stable=0;

   if(!charging) return;

   if(duty==0) goto done; /*already complete charging*/

   if(stime==0)
       stime=jiffers;
       	
   if(stable++<5) return; /*stable the pwm and adc*/
   
    /*ending test*/
    if(current <0.08) /*drop to 50mA, so done*/
       goto done;

   current = adc_A();
   voltage = adc_V();

   if( voltage > (vlimit+ir*current)){
   	   if(duty>0){
	        duty-=1;
			pwm_setduty(duty);	
			try_ir(0);//retest ir

		}
	    stable=0;

	   if(duty==0)
	      goto done;

   }
    
   pwm_setduty(duty);	  

   return;
/* complete the charging */
   done:
   {
      stime=0;
   	  charging_update_lcd();
	  bl_off();
	  _delay_ms(50);
	  bl_on();
  	  _delay_ms(50);

      duty=0;
	  pwm_setduty(0);
   }


}


void easy_discharing()
{
   PWM_PIN = 0;
   if(voltage < 1.0)
      goto done;

   if(0==duty)
      adj_c();

   return;
   done:
   {
   	  charging_update_lcd();
	  bl_off();
	  _delay_ms(50);
	  bl_on();
  	  _delay_ms(150);

      duty=0;
	  pwm_setduty(0);
   }
}
