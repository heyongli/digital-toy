#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>
#include <io.h> //uc IO system
#include <util/delay.h>

#include <math.h>


/*
 * Iron control parmeters
 */
int  temp;  //in oC 
float themo_V = 0; //caculated volatage on the thermocouple,in volt
unsigned long heat_cycles=5*HZ;//how long to heat the iron in this round

#define MIN_HEAT_CYCLES HZ/2  //at lest halft second turn on the relay to avoid relay damage
#define PEFECT_TEMP   (int)185  //good toner transfer temputure
char stop=1;

void io_init()
{
	iron_io_init();
    key_io_init();
	led_io_init();

	//red-green status led
	heat_off();
}


unsigned long adc_filter(char ch)
{
	float v[5], avg;
	char i,save;
	static  float last;
	
	save = PORTD;
	PORTD &= 0xF8;
	

	v[0]=_adc(ch);
	v[1]=_adc(ch);
	v[2]=_adc(ch);
	v[3]=_adc(ch);
	v[4]=_adc(ch);
    avg=0;
	i=0;
	while(i<5)
		avg+=v[i++];
		
	//avg = v[0]+v[1]+v[2]+v[3]+v[4];
	avg/=5;
	if(0==last)
		last =avg;

	PORTD = save; 
    return avg;
   

}



void update_lcd()
{
	
	unsigned int adc; 
	float v;

	lcd_cursor(0,0);

	//debug only show the raw voltage get from the AMP out
    adc=adc_filter(COUPLE_CH);

    v = ((float)adc/1023.0)*5;//ref volatage 5.09V
	if(v>1.2){
		lcd_puts("NO IRON            ");
		goto line2;
	}
    print10L(v*1000,3,0); //to mV  xxx
	//thermocouple voltage
	lcd_puts(" TC");
    print10L(themo_V*100000,3,2); //to mV, x.xx
	lcd_puts("mV ");

    //the Iron tempture
	print10L(temp,3,0); //xxx oC
    lcd_puts("oC");

    //pannel led status
	if( (temp<185)&&(temp>180)){
		if(!stop)
			led_lock();
    }else
		if(!stop)
			led_unlock();

line2:
	lcd_cursor(0,1);
	lcd_puts("Bat:");
	adc=adc_filter(1);
    v = ((float)adc/1023.0)*5;//ref volatage 5.09V

    print10L(v*200,3,2); //to mV  xxx
 	

}




void iron_status()
{
	static unsigned  long t;
	if(timeafter(jiffers, t+HZ/4)){ //7times per seconds
		
		//only meter the temp when relay if off avoid AC power interfere
		themo_V = adc_filter(COUPLE_CH);
		themo_V = (themo_V/1023.0)*5; //ref volatage 5.09V
	
		themo_V-=0.205; //hard code amp offset 
		if(themo_V<0)
			themo_V=0;
	
		themo_V/=45.5;  //AMP dB
        
		temp = (themo_V*1000000)/41;

		t=jiffers;

     }
}

/*1
K type:
	41uV per oC
*/
void iron_loop()
{
	static unsigned  long t;





	//which states 
	if(iron_off()){ //determin the standby time
		iron_status();

		if(temp>170) //wait time to get long when reach the goal
			heat_cycles = 10*HZ - (PEFECT_TEMP-temp)*((9*HZ)/15);
			    //from 170oC wiat 1.6S to 10s between heat the iron
		else
			heat_cycles = HZ/2; //wait 1/3 seconds if temp is very low

		if(temp>PEFECT_TEMP)
			heat_cycles = 0 ;// keeping off...
			
	}else{//determin the heating time
	//	iron_status();
		
		if(temp<170) //170-185
			heat_cycles = 5*HZ;
		else //minimal heat time is 0.26 seconds,1/4, heat cycle get short to reach goal
		    heat_cycles = (PEFECT_TEMP-temp)*((6*HZ)/15);
			//from the 170oC heat time from 4S reduce to 1/4
		
		if(temp>PEFECT_TEMP)  //gating the heat_cycles...
			heat_cycles = 0;
		
    }
    
	

	if(press(START_KEY)){
	   	heat_off();
		led_idle();
		stop^=1;
	}
	
	if(stop)
		heat_cycles = HZ/4;
	
	if(timeafter(jiffers, t+heat_cycles)){

		if(heat_cycles == 0 || stop){
			heat_off();
			if(stop){
				if(is_led_lock()){
					led_unlock();
				}else
					led_lock();
 			}else
				led_idle();
			t = jiffers;
			return;
		}
			
		if(iron_off()){
			heat_on();
			led_working();
	    }else {
			heat_off();
			led_idle();
		}
		t = jiffers;
	}


}
