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
#define METAL_74HC595_PORT METAL_4W1602_74HC595_PORT
#define  METAL_74HC595_CLK    5    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  METAL_74HC595_LATCH  4	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  METAL_74HC595_SDI    3	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/





//////////////////////////////////////////////////////////////////////
//METAL 3 button key
#define METAL_KEY_PORT   
#define METAL_KEYUP  	 0
#define METAL_KEYDOWN  1
#define METAL_KEYOK    2





#endif




