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

//i_charger charger = INIT_CHARGER;


void io_init()
{
   pwm_init(); //pull down pwm
   lcd1602_init();
   lcd_cursor(0,0);
   //lcd_puts("Welcom!");
  // seg_init();
   mdelay(100);
 
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
			   mdelay(200);	  mdelay(200);	 mdelay(200);
			   return;

		 }
         
		 if(DIS_CHA!=mode){ /*if not discharging, charging at selected vlimit*/
			climit=1.2;
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
		 if(1==num_nicd)
		    vlimit = 1.45+0.04; //40mV for adc 
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
  	  voltage=adc_V();
      lcd_cursor(0,0);
      lcd_puts("V:");
	  showVA(voltage*100);

      lcd_puts(" ");
	  print10((jiffers-stime)/200);
	  lcd_puts("Sec");
        	  
	  //showVA(top_voltage*100);

	  current=adc_A();
  	  lcd_cursor(0,1);
	  lcd_puts("A:");
  	  showVA(current*100);
	  
	  //lcd_puts(" pwm:");
  	  //print10(duty);
	  lcd_puts(" maH:");
	  print10(maH); /*maH*/


}



void main()
{
   unsigned short n,loop=0;
   float adc;


   io_init();
	   
   timer0_init(); //pwm
   timer1_init(); //1ms

   //adc_init(); 
   irqon();   //enable global interrupt		
   sleep(0); // just refrence 
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

#define max_duty 15
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
	mdelay(200);mdelay(100);
	bv=adc_V();
    
    if(setpwm)
	  adj_c();
	else 
	   	pwm_setduty(duty);

	mdelay(150);
	voltage=adc_V();
	current=adc_A();
	
	mdelay(130);
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
	  mdelay(50);
	  bl_on();
  	  mdelay(150);

   }
    if(1==charging) return; /*in charging*/

    /*try new charging */
    /*para*/


    /*test battery*/
     pwm_setduty(0);
	 //PWM_DIS=1;
 	 mdelay(100); mdelay(100);
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
    mdelay(100); mdelay(100);
       
	if((ir>1.5) && duty>3) {
	   lcd_cursor(9,0) ;
	 
       lcd_puts("bad ir  ");
	   mdelay(100);
	   if(climit>0.2)
	   	  climit -= 0.1; 
	   goto re_tryir;
	}

	/*set parameter */

     /*init start*/
     pwm_setduty(1);
	 mdelay(100);
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
       	
   if(stable++<100) return; /*stable the pwm and adc*/
   
#if 0
   if(mode==2){
   if(voltage >top_voltage){
        burst ++;
		if(burst>5){ //elemite the burst to high	    
	      top_voltage += (voltage-top_voltage)/5; /*low pass filter again*/
		//  low_voltage += (voltage-top_voltage)/8  ;
		  burst = 0;
		}

		dvc=0;  /*elimate the burst to low*/
   }
  
   if(top_voltage-voltage>0.022) /*20mV*/{
      // low_voltage -= (top_voltage-voltage)/5;
	    dvc++;
        stable =70;		
    }
	 if(dvc>5) {
	 	lcd_cursor(9,0) ;
	    lcd_puts("dvc>5");
		mdelay(100);mdelay(100);
	    mdelay(100);
	    mdelay(100);
	    
	    goto done;
	  }
   }
#endif          
   /*ending test*/
   if(mode!=2)
   if(current <0.08) /*drop to 50mA, so done*/
       goto done;


   if(mode!=2)
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
	  mdelay(50);
	  bl_on();
  	  mdelay(150);

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
	  mdelay(50);
	  bl_on();
  	  mdelay(150);

      duty=0;
	  pwm_setduty(0);
   }
}
