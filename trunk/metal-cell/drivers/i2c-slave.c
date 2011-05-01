#include <config.h>

#include <atmel/avr-io.h>
#include <timer.h>
#include <uprint.h>

//////////////////////////////////////////////////////////////////////////////////

#define I2C_PORT PORTC //PORTB
#define I2C_SCL  PC5  //PB2  //T0,  PIN 7 (yellow line)
#define I2C_SDA  PC4  //PB1  //INT0 PIN 6


#define  SCL()  (_test_bit(_inb(I2C_PORT),I2C_SCL))
#define  SDA()  (_test_bit(_inb(I2C_PORT),I2C_SDA))

#define  HOLD_SCL() do{ _pin_mode(I2C_PORT,I2C_SCL,OUTPUT);}while(0)
#define  RELE_SCL() do{ _pin_mode(I2C_PORT,I2C_SCL,INPUT); }while(0) 

#define  HOLD_SDA() do{ _pin_mode(I2C_PORT,I2C_SDA,OUTPUT); }while(0)
#define  RELE_SDA() do{ _pin_mode(I2C_PORT,I2C_SDA,INPUT); }while(0) 

#define  SDA_H()  do{ _set_bit(I2C_PORT,I2C_SDA); }while(0)
#define  SDA_L()  do{ _clear_bit(I2C_PORT,I2C_SDA); }while(0)
#define  SCL_H()  do{ _set_bit(I2C_PORT,I2C_SCL); }while(0)
#define  SCL_L()  do{ _clear_bit(I2C_PORT,I2C_SCL); }while(0)


#define i2c_led_on()   // ucLED_On()
#define i2c_led_off()  //ucLED_Off()

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
   signed char i = 7; 
   while(SCL()); //after start ...
   //mast set bit 1
   for(i=7;i>=0;i--){
   		while(!SCL());//wait master
		//SCL up, get bit 
		if(SDA())
			addr |= 1<<i;
   		while(SCL());
		
   }

    //Sent back ACK
	HOLD_SCL(); //clock strench
	SCL_L();
  
  	lcd_cursor(0,1);
   	lcd_puts("i2c_addr:0x");
    //	printhex(addr);
     print10(addr);

    if(addr == 0x55)
      i2c_led_off();
   
    if( addr&0x1)
   		lcd_puts(" w");
    else 
   		lcd_puts(" r");

   
    //SENT_BACK ACK 
	SDA_L(); 
	RELE_SCL();

    		
    return addr;
	

    //ack
    //while(!SCL()); //first bit prepare by master
   	//if(SDA())  //write
	  //i2c_led_off();
	//else
	  //;   
	//while(SCL());
    
  
}

i2c_demo()
{
   i2c_init();
   lcd_cursor(0,0);
   lcd_puts("i2c slave 0x");
   //print16(0xA2);
    print10(0xA2);

   char addr=0;
	while (1){
		while(i2c_wakeup());
		addr=i2c_receive_address();
 		while(i2c_shutdown());
	}
}
