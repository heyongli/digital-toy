#ifndef METAL_CELL_RESOURCE
#define METAL_CELL_RESOURCE

/*
 *Fuse config:
 * 	CKSEL: 0111   外部满幅晶振，CLKDIV8:1 ==> 10M 其他默认
 *  SUT：  11   16K CK/65ms
 *  low byte:77  high byte: DF  exten byte: f9
 *   
 */

/* select CPU  ARCH */
//#define ARCH_AVR 1
#define ARCH_C51 1


//C51 F_CPU define 
#ifndef F_CPU
#ifdef   ARCH_C51
#define  F_CPU 12000000UL
#endif
#endif


//library option
#define HAVE_SWAP8 1




/*////////////////////////////////////////////////////////////////////// */
/* Timer resource  */
#undef METAL_CELL_TIMER0    /*metal cell use timer0 update jiffers */
//#define METAL_CELL_TIMER0 1




/*////////////////////////////////////////////////////////////////////// */
//4W 1602 LCD
//#define METAL_4W1602_VIA_74HC595  1  //use 4w 1602 via 74hc595
/* 74HC595  */
//#define METAL_4W1602_74HC595_PORT   PORTB
//if not use 595
#define METAL_4W1602_DATA_PORT  P2
#define METAL_4W1602_RSWE_PORT  P2 
/* bus4w
 *  |d4| d5| d6 | d7 | _EN RW  RS  BL |
 *   0   1    2   3     4   5   6  7 
 */
#define  _rswe() METAL_4W1602_RSWE_PORT= ((bus4w&0xF0)|(METAL_4W1602_RSWE_PORT&0x0F))
#define  _data() METAL_4W1602_DATA_PORT= ((_swap8(bus4w)&0xF0)>>4) |((METAL_4W1602_RSWE_PORT&0xF0))



/*////////////////////////////////////////////////////////////////////// */
/*74HC595 */
#define METAL_74HC595_PORT METAL_4W1602_74HC595_PORT
#define  METAL_74HC595_CLK    5    /* raise-edge , shift clock: 74HC595 pin 11*/
#define  METAL_74HC595_LATCH  4	 /* raise-edge output to Qx, latch clock: 74HC595 pin 12*/
#define  METAL_74HC595_SDI    3	 /* CLK raise edge deposit, sserial data in: 74HC595 pin14*/





/*////////////////////////////////////////////////////////////////////// */
/* METAL 3 button key */
#define METAL_KEY_PORT   
#define METAL_KEYUP  	 0
#define METAL_KEYDOWN  1
#define METAL_KEYOK    2


// P2 ：0...4: 4x8 block select, 4 is the vled, /1602 chip select
//      5:current AD   6:   7:PWM driver

	
#define BLK P2_7
#define LCD_PORT P2


// P3_1.. P3-3: 3 keys
#define KEY_PORT    P1  
#define KEY_OK      0x1/*(1<<1)*/
#define KEY_UP		0x2/*(1<<2)*/  /*UP*/
#define KEY_DOWN    0x4/*(1<<3)*/  /*DOWN*/
#define KEY_ALL     (KEY_OK|KEY_UP|KEY_DOWN)

#endif






