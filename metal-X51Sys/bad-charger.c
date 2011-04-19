//easy charging
#include "config.h"
#include <adc.h>
#include <pwm.h>
#include <adc0832.h>

#include <charger.h>

#include <1602.h>
#include <timer.h>




#define settimer0(us) TH0 = (65536 - (us))>>8 ; /* (655535-us)/256 ³¬Âý!!!!*/  \
                      TL0 = (65536 - (us))&0xFF ; 



extern unsigned char mode;
extern unsigned char on_duty ;
void timer0(void) interrupt 1 using 2   
{   

     if(DIS_CHA==mode)
	     pwm_discharging(); 
	 else
	     pwm_1kHz();
	  

#if 0
///////// mode 1: tickless mode
    if(PWM_PIN) { //need close PWM
   
   	   PWM_PIN = 0;
	   settimer0(PWM_CYCLE-on_duty+1);
     //  vledx0();

	 
     }else { //need open PWM
  
       PWM_PIN = 1;
	   settimer0(on_duty+1);//on_duty);
//	   vledx1();

     }
#endif  

}  


void timer0_init()
{
  /*mode1: 16bit, need reload*/
   /*mode2: 8bit, auto reload*/
   /*  timer 0 in mode 1   */
   
   TMOD |= 0x2;   
      
   //settimer0(PWM_CYCLE); //mode1
   TH1 = TH0 = (256-PWM_CYCLE); //100us  15 step , 600Hz
   // enable timer 0 interrupt   
   ET0 = 1;   
   TR0 = 1; //enable timer

}



unsigned char duty=0;
float current=0, voltage=0;
float vlimit = 0 ; //V
float climit = 0 ; //A
float ir = 0 ;  //battery internal resistor
unsigned long stime=0;
void easy_charging();
void select_battery();

char charging = 0;  //not incharging
void isdone();
void easy_discharing();

float maH = 0;
void  countmaH();
unsigned char mode=0;
/*mode==2/ -Dv*/
/**/


char dvc=0,burst=0;

void charging_update_lcd()
{
  	  voltage = adc_V();;
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



bad_charger_main()
{
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

   	 if(key(KEY_UP))
	       num_nicd++;

	 if(key(KEY_DOWN)){ 
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




#define max_duty 19
adj_c()
{

  char  i=0;

   for( i=1;i<max_duty;i++){
     _delay_ms(10);
     current=adc_A();    
     if(current < climit){
        duty+=1;
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
	_delay_ms(100);
	bv=adc_V();
    
    if(setpwm)
	  adj_c();
	else 
	   	pwm_setduty(duty);

	_delay_ms(100);
	voltage=adc_V();
	current=adc_A();
	
	_delay_ms(100);
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
 	 _delay_ms(100); 
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
    _delay_ms(150);       

	if((ir>1.5) && duty>3) {
	   lcd_cursor(9,0) ;
	 
       lcd_puts("bad ir  ");
	   _delay_ms(100);
	   if(climit>0.2)
	   	  climit -= 0.1; 
	   goto re_tryir;
	}

	/*set parameter */

     /*init start*/
     pwm_setduty(1);
	 _delay_ms(50);
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
		_delay_ms(100);_delay_ms(100);
	    _delay_ms(100);
	    _delay_ms(100);
	    
	    goto done;
	  }
   }
#endif          
   /*ending test*/
   if(mode!=2)
   if(current <0.08) /*drop to 50mA, so done*/
       goto done;

   current = adc_A();
	voltage = adc_V();

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
