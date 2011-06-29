#include "config.h"
#include <compiler.h>
#include <uprint.h>
#include <bitops.h>  //uC system
#include <timer.h>
#include <io.h> //uc IO system
#include <util/delay.h>

#include <math.h>




void io_init()
{
	iron_io_init();
    key_io_init();
	led_io_init();

	//red-green status led
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

	if(v>=0.223)
		v-=0.223; //hard code offset
	print10L(v*1000,3,0); //to mV  xxx
	//lcd_puts("mV");

	v/=48;
	lcd_puts(" ");
    print10L(v*100000,3,2); //to mV, x.xx
	lcd_puts("mV ");

	print10L((v*1000000)/41,3,0); //xxx oC

	if((v*1000000)/41 > 180)
		led_lock();
	lcd_cursor(0,1);
}


/*
K type:
	41uV per oC
*/
void iron_loop()
{
	static unsigned  long t;

	
	if(press(START_KEY)){
		heat_on();
		led_working();
		t=jiffers;
	}

	if(timeafter(jiffers, t+5*HZ)){
		heat_off();
		led_idle();
	}


}
