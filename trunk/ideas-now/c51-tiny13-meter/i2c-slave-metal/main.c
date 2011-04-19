#include <config.h>

#include <atmel/avr-io.h>

#include <drivers/key.h>
#include <drivers/4wire-1602.h>


char key(char key)
{
   _test_key(key);
}

char wait_key(char key)
{
   _wait_key(key);

}


int main()
{

   
	cli();
    //system timer init
	timer0_init();
	enable_timer0();
    sti();

     //key LCD shiled
	lcd1602_init();


  
	_key_init();

    
    
	//_pins_mode(LED_PORT,0,2,OUTPUT);//_nm8(0b11,0,1);
	while(1){
	

	  i2c_demo();
	  
	
	}

}
