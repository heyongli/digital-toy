#ifndef HRDUINO_RESOURCE
#define HRDUINO_RESOURCE

//debug key lcd shield via io-extention board 
#define HRDU_KEY_LCD_SHIELD  1
#define KEY_LCD_SHIELD_EXT_CONNETOR  1 //via PORTB*/



/*************************************************************/
#ifdef HRDU_KEY_LCD_SHIELD

#define KEY_SHILD_LOGO  "Key LCD shiled"
#define KEY_SHILD_LOGO1 "PORTB 3-5:key" 

#define KEY_PORT PORTB
#define KEYUP  	 3
#define KEYDOWN  4
#define KEYOK    5
#define _key_init() \
			 _pin_mode(KEY_PORT,KEYUP, INPUT);\
			 _pin_mode(KEY_PORT,KEYDOWN, INPUT); \
			  _pin_mode(KEY_PORT,KEYOK, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0;

#define _wait_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){		\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) ){	\
			while( ! _test_bit(_inb(KEY_PORT), n) );\
				return 1; 							\
			}										\
        }											\
       return 0;


/*HC595 shield LCD*/


#ifdef KEY_LCD_SHIELD_EXT_CONNETOR
/*via io-extention board*/
#define  PORT_74HC595   PORTB 
#define  CLK    2    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  LATCH  1	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  SDI    0	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/
#define pin_init() _pins_mode(PORT_74HC595, SDI,CLK,OUTPUT)

/* stack on Hrdunio */
#else  
#define  PORT_74HC595   PORTD 
#define  CLK    3    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  LATCH  4	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  SDI    2	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/
#define pin_init() _pins_mode(PORT_74HC595, SDI,LATCH,OUTPUT)
#endif


#endif  //HRDU_KEY_LCD_SHIELD

/*************************************************************/



/*************************************************************/


#endif //end file include 
 
