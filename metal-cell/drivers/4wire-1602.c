/*
 * ref:
 * 	http://www.bpcd.net/teacher/electrical/web/PIC_MICRO.html
 *  http://en.wikipedia.org/wiki/HD44780_Character_LCD
 *
 *  ������������� MAXIM i2c תHD44780 ����
 *  http://china.maxim-ic.com/app-notes/index.mvp/id/3658
 *
 * CopyLeft hyl 2009.12.19
 */

#include <config.h>
#include <timer.h>  //uC system files
#include <drivers/74hc595.h>  
#include <io.h>
#include <bitops.h>


#ifdef METAL_4W1602_VIA_74HC595  
  #define io_init() init_74hc595()
#else
  #define io_init() \
      _pins_mode(METAL_4W1602_DATA_PORT,0,3,OUTPUT); \
	  _pins_mode(METAL_4W1602_RSWE_PORT,5,7,OUTPUT)
#endif

static char bus4w = 0;// 0xC0;

#ifdef METAL_4W1602_VIA_74HC595  
  	#define  _rswe()  write_74hc595(bus4w)
  	#define  _data()  write_74hc595(bus4w)
#endif

#ifdef METAL_4W1602_BUS4W
  #define  _rswe()  METAL_4W1602_RSWE_PORT = bus4w
  #define  _data()  METAL_4W1602_RSWE_PORT = bus4w    
#endif 


/*************�������������޸ģ���ֲ���޸���������******************/

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



/*************�������������޸ģ���ֲ���޸���������******************/

#ifndef _TINY_ROM
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
   _delay_ms(50);
}
#else
#define io_delay() 
#define io_50ms() 
#endif

static void hd44870_send(unsigned char c, char is_cmd) 
{
  io_delay();
  
  bus4w &=~(_EN|_RW);       
  _rswe();
  //io_delay();
  
  if(is_cmd)
  	 bus4w&=~_RS;  //RS=0��command
  else
  	 bus4w|=_RS;  //RS=1, data
  _rswe();
  io_delay();
  
  /* 4 MSB*/
  bus4w &= ~(_DATA);    
  bus4w |= (c>>4)&_DATA; 
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
  bus4w|=(c)&_DATA; 
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

  send_cmd(0x28); /* ��λ�����ߣ������������� */
  io_50ms();
  
  send_cmd(0x0c); /*����ʾ���޹�꣬ 0xf������겢��˸*/   
  io_50ms();

  send_cmd(0x06);  /* ����Զ����ƣ��������ƶ� */
  io_50ms();
  

  send_cmd(0x01);  /*����*/
  io_50ms();io_50ms();
}


void lcd_cursor(char x, char y)
{
	if(y == 0){
		send_cmd(x|0x80);  
	}
	if(y == 1){
		send_cmd(x|0xc0);  
	}
	//lcd2004 support 
	if(y == 2){
		send_cmd((x+0x14)|0x80);  
	}
	if(y == 3){
		send_cmd((x+0x54)|0x80);  
	}



}


void lcd_putc(char c) //��x=0~15,��y=0,1
{
  send_data( c);
}



#ifndef _TINY_ROM
void lcd_puts(char *s) 
{
    while (*s) 
    {
      send_data( *s );
      s ++;
    }
     
}


void lcd_clear()
{
   io_50ms();
   send_cmd(0x01);  /*����*/
   io_50ms();
   
}
#endif



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
