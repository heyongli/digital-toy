#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>
#include <io.h> //uc IO system
#include <util/delay.h>

#include <math.h>


/*ADC0, PC0*/
#define  COUPLE_CH  0   

/* start/stop IRON */
#define  IRON_CRL   PD7  

#define  START_KEY  PB0

#define heat_on()  _set_bit(PORTD, IRON_CRL)
#define heat_off() _clear_bit(PORTD,IRON_CRL)



char press(char n)
{
	
	if( ! _test_bit(_inb(PORTB), n) ){		
	    	_delay_ms(7); 							
	    	if(! _test_bit(_inb(PORTB), n) ){	
			while( ! _test_bit(_inb(PORTB), n) );
				return 1; 						
			}									
        }										
       return 0;

}

void iron_init()
{
	_pins_mode(PORTD,IRON_CRL,IRON_CRL,OUTPUT);
	
	_pins_mode(PORTC,PINC0,PINC0,INPUT);
	_pins_pullup(PORTC,PINC0,PINC0,FLOAT);
	

	//key
	_pin_mode(PORTB,START_KEY,INPUT); 
	_pin_pullup(PORTB,START_KEY,PULLUP);
	heat_off();
}


unsigned int vc=0, vl=0; //simple filter
unsigned long v_filter()
{
	
	if(vl==0){
		vl=vc=_adc(COUPLE_CH);
		return vc;
	}else{
		vc = _adc(COUPLE_CH);
		vc=(unsigned long)(0.88*(float)vc+0.12*(float)vl);
		vl=vc;
	}
}


void update_lcd()
{
	
	unsigned int adc; 
	unsigned long x;
	float v, i;

	lcd_cursor(0,0);


	adc=v_filter();
	
	v = ((float)adc/1023.0)*5.09; //ref volatage 5.09V
    i = (v/50.00)/0.1; //amp 50x, 0.1R sample

	//if(v>0.039)
	///	v-=0.039; //hard code offset
	v -= 0.0321; //proteus
    print10L(v*1000,3,0); //to mV  xxx
	//lcd_puts("mV");

	v/=46.45455;
	lcd_puts(" ");
    print10L(v*100000,3,2); //to mV, x.xx
	lcd_puts("mV ");

	print10L((v*1000000)/41,3,0); //xxx oC

	lcd_cursor(0,1);



	
}


/*
K type:
6.0   166.46
6.1   168.96
6.2   171.46
6.3   173.96
6.4mv 176.46
6.5mV 178.96 
6.6   181.46
6.7   183.96
6.8   186.47
6.9   188.97
7.0   191.47
*/
void iron_loop()
{
	static unsigned  long t;


	if(press(START_KEY)){
		heat_on();
		t=jiffers;
	}

	if(timeafter(jiffers, t+5*HZ)){
		heat_off();
	}

    



}
