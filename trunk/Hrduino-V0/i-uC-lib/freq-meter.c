#include "hrduino.h"
#include "include/bitops.h"
#include "include/avrio.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pwm.h"



// ***********************************************************
// Project: Digital Frequency meter using ATMEGA16
// Author: Ibrahim KAMAL
// Company: ikalogic.com
// ***********************************************************

#include <avr\io.h>
#include <avr\interrupt.h>
#include <math.h>
#define factor 31.78581  //Time base for 8 Mhz CLK, calibrate this value




unsigned long counter,counter_0;
unsigned long var1;
unsigned long var2;
float var3,f_avg;

signed char pt; //decimal point position
unsigned char display_counter; //contain the number of the active 7 segment cell
unsigned int display_refresh; //counter used for the refresh rate
unsigned long frequency; //the last calculated frequency is stored here
unsigned long measure_counter; //measurment refresh rate
unsigned int F[5]; //history for last 5 values of the COUNTER0/1
unsigned int anti_freeze_counter;
unsigned int T0_ovc,T1_ovc; //Store the number of overflows of COUNTER0/1
unsigned char source; //To select the source of the frequency: COUNTER 0 or 1 ?

SIGNAL(SIG_OVERFLOW0) {
	T0_ovc++;	
}

SIGNAL(SIG_OVERFLOW1) {
	T1_ovc++;	
}

SIGNAL(SIG_OVERFLOW2) { 	//timer 2 overflow: measure frequency
	counter_0 = TCNT1; //temporarly stock TCNT1 into counter_0

	if ((10+TCNT0+(256*T0_ovc)) < (counter_0*16))	// detect if the
	{                                         		// frequency is too high to be
		if (source < 10){										// sampled
			source++;
		}
	}else{
		if (source > 0){
			source--;
		}
	}

	if(source > 5){ //the frequency in definately too high
	
		//***Anti flickering mechanism
	  	if (
		(
			(F[0] != (TCNT1))     //If That measure is not allready present,
			&(F[1] != (TCNT1))    //in any of the last 5 frequency
			&(F[2] != (TCNT1))    //measurments. Or...
			&(F[3] != (TCNT1))
			&(F[4] != (TCNT1))
		)
		|(TCNT0 == 0)					//If that measure is 0. Or...
		|(F[0]&F[1]&F[2]&F[3]&F[4] == 0) 		//If the last five measures are all = 0, Or..
		|(anti_freeze_counter > 100) //IF a new measurment is to be forced in anyway, Then,
		){
		F[4] = F[3];
		F[3] = F[2];
		F[2] = F[1];
		F[1] = F[0];
		F[0] = (TCNT0);
			anti_freeze_counter=0;
		}else{
			anti_freeze_counter++;  //Increase that counter, that is used to
		}									//force the program to read a new measure,
											//'once in a wile'...

		f_avg = (F[0]+F[1]+F[2]+F[3]+F[4])/5;
	   //***END of Anti flickering mechanism
	
		counter = (f_avg*16);  //because the signal is orriginally divided by 16
		
	}else{
		//***Anti flickering mechanism	
		if (
		(
			(F[0] != (TCNT0))   //If That measure is not allready present,
			&(F[1] != (TCNT0))  //in any of the last 5 frequency
			&(F[2] != (TCNT0))	//measurments. Or...
			&(F[3] != (TCNT0))
			&(F[4] != (TCNT0))
		)
		|(TCNT0 == 0)					//If that measure is 0. Or...
		|(F[0]&F[1]&F[2]&F[3]&F[4] == 0) 		//If the last five measures are all = 0, Or..
		|(anti_freeze_counter > 100) //IF a new measurment is to be forced in anyway, Then,
		){
		F[4] = F[3];
		F[3] = F[2];
		F[2] = F[1];
		F[1] = F[0];
		F[0] = (TCNT0);
			anti_freeze_counter=0;
		}else{
			anti_freeze_counter++;  //Increase that counter, that is used to
		}									//force the program to read a new measure,
											//'once in a wile'...
											
		f_avg = (F[0]+F[1]+F[2]+F[3]+F[4])/5;
		
	   //***END of Anti flickering mechanism
		counter = f_avg+(256*T0_ovc);
	}

	frequency = (counter*factor);
	
	//RESET COUNTERS
	T0_ovc = 0;
	T1_ovc = 0;
	TCNT0 = 0;
	TCNT1 = 0;

}


display(){

}


post_display(long number){
	pt = 3; //at first estimate the decimal point on the first digit from the left
	
	while(number > 9999){
		number = number / 10;
		pt--; //shift the decimal to the right until the number is below 9999
	}
	
	print10(number);
	
	//switch to Mhz display if needed
	if(pt < 0){
		pt = pt + 3; //shift back decimal point 3 steps for E3 to E6 conversion
		//digit[0] = 10; //last digit is replaced with the M suffix
	}
}

setup_timers(){
	TCCR0 = 0x07; //setup TC0 to count external events on PB0
	TCCR1A = 0x00; //Setup TC1 to count PB1
	TCCR1B = 0x07;
	TCCR2 = 0x07;   //TC2 counts Clock_io/1024
}

setup_interrupts(){
	SREG = 0x80; //enable general interrupts in SR
	TIMSK = 0x41;//Enable timers interrupts	
}



int freq_main(void) 
{
DDRC = 0xFF; //PORTC is all used for output on 7-seg.

DDRB = 0xFC;
DDRD = 0xFF;        	
setup_timers();
setup_interrupts();
 //for testing

   while(1) {             // Infinite loop
		measure_counter++;
		display_refresh++;
		
	   if (display_refresh > 200){
	   	display_refresh=0;
	   	display();
	   }
	
	   if (measure_counter > 5500){
   		measure_counter=0;
	  	 post_display(frequency);
   	}
  	}

}
