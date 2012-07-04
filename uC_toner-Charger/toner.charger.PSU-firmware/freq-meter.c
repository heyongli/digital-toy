

#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>

#include "atmel/avr-io.h" //uc IO system

#include <avr/interrupt.h> //avr gcc
#include <util/delay.h>
#include <math.h>



#define factor (4.238552517) //Time base for 10 Mhz CLK, calibrate this value

#define calb (1+0)


unsigned char T1_ovc=0; //Store the number of overflows of COUNTER1
unsigned long F[8]; //history for last 5 values of the COUNTER0/1
float f_avg;

unsigned long frequency; //the last calculated frequency is stored here



//Atemel external clock source MAX< F_CPU/2.5, 8M/2.5=3.2M


//T1 conter the prescale output
SIGNAL(SIG_OVERFLOW1) 
{
	T1_ovc++;
}


//Timer/Counter 2 is configured as timer with a 1024 prescaller (counting CPU frequency divided by 1024). 
//It is used to call the "frequency calculation and selection algorithm" every timer period T.
//T is defined as "1024*256/(F_cpu)". (30.5Hz)
//ISR(TIMER0_OVF_vect)
unsigned char sTCNT1L, sTCNT1H, sT1_ovc;
unsigned long scounter;


unsigned char T2_ovc=0; 
//sample every about 1/8 seconds,  4 T2_ovc overflow
unsigned long freqH;


SIGNAL(SIG_OVERFLOW2) 
{ 

	unsigned long counter;
	jiffers++;
    T2_ovc++;

	if((T2_ovc%9)!=0) //stable time, 2-3 seconds
		return; 


	//timer 2 overflow: measure frequency
	counter = sTCNT1L=TCNT1L;
	barrier();
	
	sTCNT1H = TCNT1H;
	counter |=(unsigned long)sTCNT1H<<8;
	counter |=(unsigned long)T1_ovc<<16;
    sT1_ovc = T1_ovc;

    
	F[7] = F[6];
	F[6] = F[5];
    F[5] = F[4];
 	F[4] = F[3];
	F[3] = F[2];
	F[2] = F[1];
	F[1] = F[0];
	scounter=F[0] = counter;
											//'once in a wile'...

	f_avg = (F[0]+F[1]+F[2]+F[3]+F[4]+F[5]+F[6]+F[7])/8;
	
	//***END of Anti flickering mechanism
	counter = (unsigned long)f_avg;  
		

	frequency = (unsigned long)((double)counter*factor*calb);
	
	//RESET COUNTERS
	T1_ovc = 0;
	TCNT1H = 0;
	barrier();
	TCNT1L = 0;
	TCNT1 = 0;

}






unsigned int vc=0, vl=0; //simple filter
unsigned long v_filter()
{
	return _adc(0);

	if(vl==0){
		vl=vc=_adc(0);
		return vc;
	}else{
		vc = _adc(0);
		vc=(unsigned long)(0.88*(float)vc+0.12*(float)vl);
		vl=vc;
	}
}

void post_display(long number)
{
  
	lcd_cursor(0,0);
    //lcd_puts("              ");
	lcd_cursor(0,0);
    
	if((number>999)&&(number<999999)){
	   printLL(number,0,0);
	   lcd_puts("KHz");
     	lcd_puts("      ");

	}

    if(number>999999){
	   printLL(number,0,0); //omit xxHz
	   lcd_puts("MHz");
       lcd_puts("     ");

   	
	}
	if(number<=999)
	{
	   printLL(number,0,0);
	   lcd_puts("Hz");
     	lcd_puts("        ");

   	
	}


	
	lcd_cursor(0,1);
{
    static unsigned char ch=0;
	unsigned int adc= v_filter(ch);
	float v, i;

 	print10(adc); //
	lcd_puts(" ");

	v = ((float)adc/1023.0)*5.09; //ref volatage 5.09V
    i = (v/50.00)/0.1; //amp 50x, 0.1R sample

    print10(v*1000); //to mV
	lcd_puts("mV ");
    
	print10(i*1000);
	lcd_puts("mA");
	
}
 	//print10(sTCNT1L);
	//lcd_putc(' ');
 	//print10(sTCNT1H);
    //lcd_putc(' ');
 	//print10(sT1_ovc);

}

void setup_timers(){
	TCCR1A = 0x00; //Setup TC1 to count PD5/T1
	TCCR1B = 0x07; //TC1 up edge triger
	
	TCCR2 = 0x07;  //TC2 counts Clock_io/1024, use as time base caller 


}

void setup_interrupts()
{
	TIMSK =  _bits8(1,TOIE1,TOIE1)|_bits8(1,TOIE2,TOIE2);//enable timer 0,1,2 overflow intrrupt	

	//clear T1 counters
	T1_ovc = 0;

	TCNT1H = 0;
	barrier();
	TCNT1L = 0;
	TCNT1 = 0;
	sti();
}






unsigned int display_refresh; //counter used for the refresh rate
unsigned long measure_counter; //measurment refresh rate

void freq_main(void) 
{


    cli();


 	F[4] = F[3] = F[2]= F[1] = F[0] = 0;


    //ADC init  
   _pins_mode(PORTC, PINC0,PINC1,INPUT);
   _pins_pullup(PORTC, PINC0,PINC1,FLOAT);
   adc_init();
  
    //T1 input init
    _pins_mode(PORTD, 0,PIND5,INPUT);
    _pins_pullup(PORTD,0,PIND5,FLOAT);

	    
		
	setup_timers();
	setup_interrupts();
 	//for testing
	display_refresh=jiffers;

   	while(1) {             // Infinite loop
	  	if (timeafter(jiffers,display_refresh+38/5)){
		   	display_refresh=jiffers;
			post_display(frequency);
		}
	
	   
  	}

}
