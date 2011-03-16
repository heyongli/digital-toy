

#include "config.h"
#include <uprint.h>
#include <bitops.h>  //uC system
#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>



#define factor 31.78581  //Time base for 8 Mhz CLK, calibrate this value




unsigned long counter;
float f_avg;

unsigned int display_refresh; //counter used for the refresh rate

unsigned long frequency; //the last calculated frequency is stored here

unsigned long measure_counter; //measurment refresh rate
unsigned int F[5]; //history for last 5 values of the COUNTER0/1

unsigned int anti_freeze_counter;
unsigned int T0_ovc,T1_ovc; //Store the number of overflows of COUNTER0/1


//Atemel external clock source MAX< F_CPU/2.5, 8M/2.5=3.2M


//T1 conter the prescale output
SIGNAL(SIG_OVERFLOW1) 
{
	T1_ovc++;	
}


unsigned long timer16_readTCNT1(void)
{
	unsigned char sreg;
	unsigned int i;
	sreg = SREG;
	cli();

	i=TCNT1;
	
	SREG = sreg;

    return i;

}

//Timer/Counter 2 is configured as timer with a 1024 prescaller (counting CPU frequency divided by 1024). 
//It is used to call the "frequency calculation and selection algorithm" every timer period T.
//T is defined as "1024*256/(F_cpu)". (30.5Hz)
//ISR(TIMER0_OVF_vect)
SIGNAL(SIG_OVERFLOW2) 
{ 	//timer 2 overflow: measure frequency
	
 	//***Anti flickering mechanism
	if ((
			(F[0] != (TCNT1))     //If That measure is not allready present,
			&(F[1] != (TCNT1))    //in any of the last 5 frequency
			&(F[2] != (TCNT1))    //measurments. Or...
			&(F[3] != (TCNT1))
			&(F[4] != (TCNT1))
		)
		|(F[0]&F[1]&F[2]&F[3]&F[4] == 0) 		//If the last five measures are all = 0, Or..
		|(anti_freeze_counter > 100) //IF a new measurment is to be forced in anyway, Then,
		)
	{
		F[4] = F[3];
		F[3] = F[2];
		F[2] = F[1];
		F[1] = F[0];
		F[0] = (TCNT1);
		anti_freeze_counter=0;
	 }else{
			anti_freeze_counter++;  //Increase that counter, that is used to
	 }									//force the program to read a new measure,
											//'once in a wile'...

	 f_avg = (F[0]+F[1]+F[2]+F[3]+F[4])/5;
	 //***END of Anti flickering mechanism
	
	 counter = f_avg;  
		

	frequency = (counter*factor);
	
	//RESET COUNTERS
	T1_ovc = 0;
	TCNT1 = 0;

}



void post_display(long number)
{
	lcd_cursor(0,0);
	printLL(number);
}

void setup_timers(){
	TCCR1A = 0x00; //Setup TC1 to count PD5/T1
	TCCR1B = 0x07; //TC1 up edge triger

	TCCR2 = 0x07;   //TC2 counts Clock_io/1024, use as time base caller 
}

void setup_interrupts()
{
	sti();
	TIMSK =  _bits8(1,TOIE1,TOIE1)|_bits8(1,TOIE2,TOIE2);//enable timer 0,1,2 overflow intrrupt	
}



void freq_main(void) 
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
		   }
	
	   if (measure_counter > 5500){
   		measure_counter=0;
	  	 post_display(frequency);
   		}
  	}

}
