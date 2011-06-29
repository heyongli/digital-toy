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
	unsigned int v[5], avg,i;
	
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
    return avg;

}



void update_lcd()
{
	
	unsigned int adc; 
	float v;

	lcd_cursor(0,0);

	
	//debug only show the raw voltage get from the AMP out
    adc=adc_filter(COUPLE_CH);
    v = ((float)adc/1023.0)*5.09; //ref volatage 5.09V
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
		led_lock();
    }else
		led_unlock();

	lcd_cursor(0,1);
}




/*
K type:
	41uV per oC
*/
void iron_loop()
{
	static unsigned  long t;
	char stop=0;

	//only meter the temp when relay if off avoid AC power interfere
	themo_V = adc_filter(COUPLE_CH);
	themo_V = (themo_V/1023.0)*5.09; //ref volatage 5.09V
	
	themo_V-=0.0321; //hard code amp offset 
	
	themo_V/=45.5;  //AMP dB
        
	temp = (themo_V*1000000)/41;


	//which states 
	if(iron_off()){ //determin the standby time
		
		if(temp>170) //wait time to get long when reach the goal
			heat_cycles = 10*HZ - (PEFECT_TEMP-temp)*((9*HZ)/15);
			    //from 170oC wiat 1.6S to 10s between heat the iron
		else
			heat_cycles = HZ/3; //wait 1/3 seconds if temp is very low

		if(temp>PEFECT_TEMP)
			heat_cycles = 0 ;// keeping off...
			
	}else{//determin the heating time
	
		
		if(temp<170) //170-185
			heat_cycles = 5*HZ;
		else //minimal heat time is 0.26 seconds,1/4, heat cycle get short to reach goal
		    heat_cycles = (PEFECT_TEMP-temp)*((4*HZ)/15);
			//from the 170oC heat time from 4S reduce to 1/4
		
		if(temp>PEFECT_TEMP)  //gating the heat_cycles...
			heat_cycles = 0;
		
    }
    
	

	if(press(START_KEY)){
	   	heat_off();
		led_idle();
		stop^=1;
	}
	
	
	if(timeafter(jiffers, t+heat_cycles)){
		if(heat_cycles == 0 || stop){
			heat_off();
			led_idle();
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
