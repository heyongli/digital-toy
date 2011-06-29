#ifndef METAL_CELL_RESOURCE
#define METAL_CELL_RESOURCE

/*
 *Fuse config:
 * 	CKSEL: 0111   外部满幅晶振，CLKDIV8:1 ==> 10M 其他默认
 *  SUT：  11   16K CK/65ms
 *  low byte:77  high byte: DF  exten byte: f9
 *   
 */

#define ARCH_AVR 1

//////////////////////////////////////////////////////////////////////
//Timer resource 
#define METAL_CELL_TIMER0    //metal cell use timer0 update jiffers
//#define METAL_CELL_TIMER0 1




//////////////////////////////////////////////////////////////////////
//4W 1602 LCD
#define METAL_4W1602_VIA_74HC595  1  //use 4w 1602 via 74hc595
//74HC595 
#define METAL_4W1602_74HC595_PORT   PORTB
//if not use 595
//#define METAL_4W1602_DATA_PORT  PORTD 
//#define METAL_4W1602_RSWE_PORT  PORTC  




//////////////////////////////////////////////////////////////////////
//74HC595 
#define METAL_74HC595_PORT  METAL_4W1602_74HC595_PORT
#define  METAL_74HC595_CLK    5    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  METAL_74HC595_LATCH  4	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  METAL_74HC595_SDI    3	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/





/*
 * Pannel resource : key, led, encoder...
 */
/* All key connect to PORTB */
#define  START_KEY  PB0

#define key_io_init() \
	_pin_mode(PORTB,START_KEY,INPUT); \
	_pin_pullup(PORTB,START_KEY,PULLUP); \


/*working led : 3pin connect to PD 0,1,2*/
#define WORK_LED_C  PD1
#define WORK_LED_RED PD0
#define WORK_LED_GREEN PD2

#define led_io_init() \
	_pins_mode(PORTD,PD0, PD2,OUTPUT); \
	_clear_bit(PORTD,WORK_LED_C); \

#define led_working() \
		_set_bit(PORTD, WORK_LED_RED);
#define led_idle() \
		_clear_bit(PORTD, WORK_LED_RED);


#define led_lock() \
		_set_bit(PORTD, WORK_LED_GREEN); 

#define is_led_lock() \
	 	_test_bit(PORTD, WORK_LED_GREEN) 

#define led_unlock() \
		_clear_bit(PORTD, WORK_LED_GREEN); 

#if 0
#define led_toggle() do{ \
			unsigned char x= PORTD;  \
			x =PORTD^(_set_bit(x,WORK_LED_GREEN) | _set_bit(x,WORK_LED_RED) );\
			_clear_bit(PORTD,WORK_LED_GREEN);\
			_clear_bit(PORTD,WORK_LED_RED); \
			PORTD|= x; \
         }while(0)

#endif

	

/*
 * Iron control interface define
 */
/*ADC0, PC0*/
#define  COUPLE_CH  0   
/* start/stop IRON */
#define  IRON_CRL   PD7  

#define iron_io_init() \
	_pins_mode(PORTD,IRON_CRL,IRON_CRL,OUTPUT); \
												\
	_pins_mode(PORTC,PINC0,PINC0,INPUT);		\
	_pins_pullup(PORTC,PINC0,PINC0,FLOAT);		\


#define heat_on()  _set_bit(PORTD, IRON_CRL)
#define heat_off() _clear_bit(PORTD,IRON_CRL)
#define iron_off() (!_test_bit(PORTD,IRON_CRL))




#endif




