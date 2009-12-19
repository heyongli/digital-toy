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

#include "include/avrio.h"
#include <util/delay.h>

#define LCD_CTR_PORT    PORTD 
#define LCD_DATA_PORT   PORTC  
#define LCD_RS          (1<<PD5) //0x02  portA1       out
#define LCD_RW          (1<<PD6)
#define LCD_EN          (1<<PD7) //0x08   portA3       out
#define LCD_DATA        0xf /*portC 0..3 for DATA*/

#define io_init() \
    _port_modenm(LCD_DATA_PORT,0,3,OUTPUT); \
    _port_modenm(LCD_CTR_PORT,5,7,OUTPUT)

/*************以下内容无需修改，移植请修改以上内容******************/

/*
 #define  io_delay()  _delay_us(11);
	15us tested, 12us occation failed, 11us not stable, 10us failed
 for safe and reserve space, use function and 12us	
*/
#if 0 /*gcc optimized for size is better than defin function by self!!*/
void io_delay()
{
   _delay_us(14);
}
#else
#define io_delay()   _delay_us(14)
#endif

static void hd44870_send(unsigned char data, char is_cmd) 
{
  io_delay();
  
  LCD_CTR_PORT &=~(LCD_EN|LCD_RW);       
  
  //io_delay();
  
  if(is_cmd)
  	 LCD_CTR_PORT&=~LCD_RS;  //RS=0，command
  else
  	 LCD_CTR_PORT|=LCD_RS;  //RS=1, data
  
  io_delay();
  
  /* 4 MSB*/
  LCD_DATA_PORT&=0Xf0;    
  LCD_DATA_PORT|=(data>>4)&0xf; 
  
  io_delay();
  LCD_CTR_PORT |=LCD_EN;  
  io_delay(); 
  LCD_CTR_PORT &=~LCD_EN;   

  io_delay();
  
  /*send low 4 lsb*/
  LCD_DATA_PORT&=0xf0;         
  LCD_DATA_PORT|=data&0xf; 
  io_delay();

  LCD_CTR_PORT |=LCD_EN;   
  io_delay(); 
  LCD_CTR_PORT &=~LCD_EN;  
  
  
}

#define send_cmd(data) hd44870_send(data,1)
#define send_data(data) hd44870_send(data,0)

void lcd1602_init(void)    
{
  io_init();

  send_cmd(0x33);
  _delay_ms(50);
  send_cmd(0x32);
  _delay_ms(50);

  send_cmd(0x28); /* ４位数据线，５＊１０字体 */
  _delay_ms(50);
  
  send_cmd(0x0c); /*开显示，无光标， 0xf：开光标并闪烁*/   
  _delay_ms(50);

  send_cmd(0x06);  /* 光标自动右移，整屏不移动 */
  _delay_ms(50);
  

  send_cmd(0x01);  /*清屏*/
  _delay_ms(100);
}



void lcd_cursor(char x, char y)
{
	send_cmd(x+(y?0xc0:0x80));  
}

void lcd_puts(unsigned char *s) 
{
    while (*s) 
    {
      send_data( *s );
      s ++;
    }
     
}


void lcd_putc(char data) //列x=0~15,行y=0,1
{
  send_data( data);
}


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
