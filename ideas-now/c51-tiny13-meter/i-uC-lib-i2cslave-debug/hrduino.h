#ifndef HRDUINO_RESOURCE
#define HRDUINO_RESOURCE

#include "include/avrio.h"

/*
 *Fuse config:
 * 	CKSEL: 0111   外部满幅晶振，CLKDIV8:1 ==> 10M 其他默认
 *  SUT：  11   16K CK/65ms
 *  low byte:77  high byte: DF  exten byte: f9
 *   
 */

//debug key lcd shield via io-extention board 
#define HRDU_KEY_LCD_SHIELD  1
#define KEY_LCD_SHIELD_EXT_CONNETOR  1 //via PORTB*/





unsigned int _adc(unsigned char ch);
void adc_init();

void lmotor(char fwd_bk,char duty);
void rmotor(char fwd_bk,char duty);

/*595*/
void init_74hc595(void);
void write_74hc595(unsigned char data);


/*shift1  one wired 595*/
void _write_shif1(volatile unsigned char* port_addr, unsigned char bit, unsigned char data);
void _init_shift1(volatile unsigned char *port_addr, unsigned char bit);
#define write_shift1(PORT, bit, data)  _write_shif1(&PORT, bit, data)
#define init_shift1(PORT,bit)  _init_shift1(&PORT,bit)

/* IR decoding*/
int _IR_NEC_busy_decode(volatile unsigned char* port_addr, unsigned char bit);
#define IR_NEC_busy_decode(PORT,bit) _IR_NEC_busy_decode(&PORT,bit)


/*************************************************************/
/*  
 * tick system      
 */



/* timer0,  fcpu=8Mhz 
 * TCN: 8M/(8*256) = 3906.25HZ (PWM)
 * 
 */
#define TIMER0_CS 0b11  //CS = 64
#define TIMER0_HZ  ((F_CPU)/(64UL))
#define TIMER0_TCN (255UL- ((TIMER0_HZ)/(HZ)) )  /*62.5 times OVERFLOW, 0.5 for ISR overhead...*/



#define HZ  1000  
#define timeafter(a,b)         \
         (((long)(b) - (long)(a) < 0))



extern volatile unsigned  long jiffers;

void timer0_init();
void enable_timer0();

void _delay_s(unsigned char s);
void delay(unsigned long ticks);













/*************************************************************/
#ifdef HRDU_KEY_LCD_SHIELD

void lcd1602_init(void) ;
void lcd_putc(char data) ;
void print10(unsigned short n);
void lcd_cursor(char x, char y);
void lcd_puts(char *s);
void lcd_clear();

#define KEY_SHILD_LOGO  "Key LCD shiled"
#define KEY_SHILD_LOGO1 "PORTB 3-5:key" 



#define KEY_PORT PORTD
#define KEYUP  	 0
#define KEYDOWN  1
#define KEYOK    2
#define _key_init() \
			 _pin_mode(KEY_PORT,KEYUP, INPUT);\
			 _pin_mode(KEY_PORT,KEYDOWN,INPUT); \
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
#define pin_init() _pins_mode(PORT_74HC595, (char)SDI,(char)LATCH,(char)OUTPUT)
#endif


#endif  //HRDU_KEY_LCD_SHIELD











/*************************************************************/








/*************************************************************/


#endif //end file include 
 
