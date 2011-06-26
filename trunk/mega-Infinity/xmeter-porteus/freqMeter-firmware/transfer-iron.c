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
	
	print10(adc); //
	lcd_puts(" ");

	v = ((float)adc/1023.0)*5.09; //ref volatage 5.09V
    i = (v/50.00)/0.1; //amp 50x, 0.1R sample

	v-=0.039; //hard code offset

    print10(v*1000); //to mV
	lcd_puts("mV ");

	v/=16.7;
	lcd_puts(" ");

    print10(v*10000); //to mV
	lcd_puts("mV ");

    
	
}


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
