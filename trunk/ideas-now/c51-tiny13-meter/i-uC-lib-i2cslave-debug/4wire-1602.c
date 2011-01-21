/*
 * ref:
 * 	http://www.bpcd.net/teacher/electrical/web/PIC_MICRO.html
 *  http://en.wikipedia.org/wiki/HD44780_Character_LCD
 *
 *  其他解决方案： MAXIM i2c 转HD44780 介绍
 *  http://china.maxim-ic.com/app-notes/index.mvp/id/3658
 *
 * CopyLeft hyl 2009.12.19
 */
#include "hrduino.h"
#include "include/avrio.h"
#include <util/delay.h>


#define USE_74HC595
#undef  USE_74HC595

#define LCD_RSWE_PORT  PORTD 
#define LCD_DATA_PORT  PORTD  

#ifdef USE_74HC595  
  #define io_init() init_74hc595()
#else
  #define io_init() \
      _pins_mode(LCD_DATA_PORT,4,7,OUTPUT); \
	  _pins_mode(LCD_RSWE_PORT,0,3,OUTPUT)
#endif

static char bus4w = 0xC0;

#ifdef USE_74HC595  
  	#define  _rswe()  write_74hc595(bus4w)
  	#define  _data()  write_74hc595(bus4w)
#else
  #define  _rswe()  _mov_bits8(LCD_RSWE_PORT,bus4w,0,2,4,6)
  #define  _data()  _mov_bits8(LCD_DATA_PORT,bus4w,4,7,0,3);
#endif
/*************以下内容无需修改，移植请修改以上内容******************/

/*  74hc595 data format 
 *  Ver0.1 
 *  |d4| d5| d6 | d7 | _EN RW  RS  BL |
 *   0   1    2   3     4   5   6  7 
 */
#define _RS          (1<<6) 
#define _RW          (1<<5)
#define _EN          (1<<4) 
#define _RSWE        (_RS|_RW|_EN)
#define _DATA        (0x0f)



/*************以下内容无需修改，移植请修改以上内容******************/

/*
 #define  io_delay()  _delay_us(11);
	15us tested, 12us occation failed, 11us not stable, 10us failed
 for safe and reserve space, use function and 12us	
*/
void io_delay()
{
   _delay_us(15);
}
void io_50ms()
{
   _delay_ms(70);
}

static void hd44870_send(unsigned char data, char is_cmd) 
{
  io_delay();
  
  bus4w &=~(_EN|_RW);       
  _rswe();
  //io_delay();
  
  if(is_cmd)
  	 bus4w&=~_RS;  //RS=0，command
  else
  	 bus4w|=_RS;  //RS=1, data
  _rswe();
  io_delay();
  
  /* 4 MSB*/
  bus4w &= ~(_DATA);    
  bus4w |= (data>>4)&_DATA; 
  _data();
  
  io_delay();
  bus4w |=_EN;  
  _rswe();
  io_delay(); 
  bus4w &=~_EN;   
  _rswe();

  io_delay();
  
  /*send low 4 lsb*/
  bus4w&=~_DATA;         
  bus4w|=(data)&_DATA; 
  _data();
  io_delay();

  bus4w |=_EN;  
  _rswe();
  io_delay(); 
  bus4w &=~_EN;   
  _rswe();
  
  
}

#define send_cmd(data) hd44870_send(data,1)
#define send_data(data) hd44870_send(data,0)

void lcd1602_init(void)    
{
  io_init();

  send_cmd(0x33);
  io_50ms();
  send_cmd(0x32);
  io_50ms();

  send_cmd(0x28); /* ４位数据线，５＊１０字体 */
  io_50ms();
  
  send_cmd(0x0c); /*开显示，无光标， 0xf：开光标并闪烁*/   
  io_50ms();

  send_cmd(0x06);  /* 光标自动右移，整屏不移动 */
  io_50ms();
  

  send_cmd(0x01);  /*清屏*/
  io_50ms();io_50ms();
}


void lcd_cursor(char x, char y)
{
	send_cmd(x+(y?0xc0:0x80));  
}

void lcd_puts(char *s) 
{
    while (*s) 
    {
      send_data( *s );
      s ++;
    }
     
}


char hex2c(char hex)
{

   if(hex<0xa)
      return '0'+hex;
   else
      return 'a'+hex-0xa;
}

static char four[6];
     
void print10(unsigned short n)
{
     //irqoff();
	 four[0]= hex2c( (n/10000) );
	 n = n%10000;
	 four[1]= hex2c(n/1000);
	 n = n%1000;
		   	
	 four[2]= hex2c( (n/100) );
	 n = n%100;
	
	 four[3]= hex2c( (n/10) );
	 four[4]= hex2c( (n%10) );;
	 four[5]= 0;

     if(four[0] == '0')
	    lcd_puts(four+1);
	 else
	 	lcd_puts(four);
}

void print16(unsigned char n)
{
     //irqoff();
	 four[0]= hex2c(n>>4 );
	 four[1]= hex2c(n&0xF);
	 four[2]= 0;

     lcd_puts(four);

     
}
void lcd_clear()
{
   io_50ms();
   send_cmd(0x01);  /*清屏*/
   io_50ms();
   
}

void lcd_putc(char data) //列x=0~15,行y=0,1
{
  send_data( data);
}


#if 0

/*
 * note: char in some C compiled is 'unsigned"
 */
void lcd_scroll(signed char steps)
{
   char cmd = (steps>0)?0x1c:0x18;
   short ssteps= (steps>0)?steps:(-steps);
   /* get negetive is unsafe, char range: -128 to +127, 
    * so -128 get negtive is overflow
	*/

   while(ssteps--)
   {
    	send_cmd((unsigned char)cmd);
		io_delay();  
   }
}


#endif
