#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

#include "tiny-uc.h"

#define uckey_init()  _pin_mode(PORTB,ucKEY,INPUT)

#define _test_key(x,n) 								\
	


char keydown()
{
	if( ! _test_bit(_inb(PORTB),ucKEY)) {			
	    	_delay_ms(7); 							
	    	if(! _test_bit(_inb(PORTB),ucKEY ))		
				return 1;							
        }											
       return 0	;									
}


//////////////////////////////////////////////////////////////////////////////////

#define I2C_PORT PORTB
#define I2C_SCL  PB2  //T0,  PIN 7 (yellow line)
#define I2C_SDA  PB1  //INT0 PIN 6


#define  SCL()  (_test_bit(_inb(I2C_PORT),I2C_SCL))
#define  SDA()  (_test_bit(_inb(I2C_PORT),I2C_SDA))

#define i2c_led_on()   ucLED_On()
#define i2c_led_off()  ucLED_Off()

char i2caddr = 0xA2; 
char i2cstat = 0;
#define I2C_IDLE 1
#define idle()     (_test_bit(i2cstat,I2C_IDLE))
#define i2c_idle() (_set_bit(i2cstat,I2C_IDLE))

char clk = 0;

void i2c_init()
{
  _pin_mode(I2C_PORT,I2C_SCL,INPUT);
  _pin_mode(I2C_PORT,I2C_SDA,INPUT);
  i2c_led_off();

}

char i2c_shutdown()
{
   if(SCL()&&(!SDA())){ 
      while( SCL()&&(!SDA()) );
	  if(SCL()&&SDA()){

	  	i2c_led_off();
		_delay_ms(122);
	  	i2c_led_on();
		_delay_ms(122);
	  	i2c_led_off();
		
		i2c_idle();
	    return 0;
	  }
   }
   return 1;
}

char i2c_wakeup()
{
   if(SCL()&&SDA()){
      while(SCL()&&SDA());
	  if( SCL()&&(!SDA()) ){
         i2c_led_on();
	     return 0; //bus active
	  }
	  //while(i2c_shutdown());
	  //SCL :high, SDA:low
   }
   return 1; //need wait...
}


char i2c_receive_address()
{
   unsigned char addr=0;
   char i = 7; 
   while(SCL()); //after start ...
   //mast set bit 1
   for(i=0;i<8;i++){
   		while(!SCL());//wait master
		_delay_us(2);
		//SCL up, get bit 
		addr |= SDA()<<i; 
   		while(SCL());
		
   }
  
   if(addr == 0xA2)
     i2c_led_off();
   
    //ack
    //while(!SCL()); //first bit prepare by master
   	//if(SDA())  //write
	  //i2c_led_off();
	//else
	  //;   
	//while(SCL());
    
  
}




//////////////////////////////////////////////////////////////////////////////////



unsigned char duty=240; 
int main()
{
    unsigned int adc = 0,clock=0;

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

  
    led_init();
    uckey_init();
	adc_init();	
#ifdef PWM
    pwm_init();
#endif
    i2c_init();
	while (1){
		while(i2c_wakeup());
		i2c_receive_address();
 		while(i2c_shutdown());
	}
}
