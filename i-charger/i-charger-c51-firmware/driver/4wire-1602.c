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
#include <AT89X52.H>
#include <config.h>
#include <1602.h>
#include <delay.h>

unsigned  char bus4w=0;


/*  4wire direct to IO */

#define _RS          (1<<6) 
#define _RW          (1<<5)
#define _EN          (1<<4) 
#define _RSWE        (_RS|_RW|_EN)
#define _DATA        (0x0f)


/*abcd1234 => 4321dcba */
unsigned char _swap4(unsigned char x)
{
    /*abcd1234 => 1234abcd*/
	x= (x>>4) | (x<<4);
	/*1234abcd  => 3412cdab*/
    	   /*1200ab00*/         /*003400cd*/
	x= ((x&0xcc)>>2) | ((x&0x33)<<2);
                 /*0xcc*/            /*0x33*/
	/*3412cdab => 4321dcba*/
	x= ((x&0xAA)>>1) | ((x&0x55)<<1);
            /*0xAA*/            /*0x55*/
    return x>>4;
}



#define  _rswe()   P2=( (P2&(~_RSWE))|(bus4w&_RSWE))
#define  _data()   P2=( (P2&(~_DATA))|_swap4((bus4w&_DATA)) )


/*************以下内容无需修改，移植请修改以上内容******************/

/*
 #define  io_delay()  _delay_us(11);
	15us tested, 12us occation failed, 11us not stable, 10us failed
 for safe and reserve space, use function and 12us	
*/
void io_delay()
{
   _delay_us(7); //atleast 7cycle
}
void io_50ms()
{
   _delay_ms(50);
}

void hd44870_send(unsigned char datax , char is_cmd) 
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
  bus4w |= (datax>>4)&_DATA; 
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
  bus4w|=(datax)&_DATA; 
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

  bl_on();

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

void lcd_puts(unsigned char *s) 
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
void print10(unsigned short n)
{
     static char four[6];
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

void showVA(unsigned short n)
{
     static char four[6];
     //irqoff();
	 four[0]= hex2c( (n/1000) );
	 n = n%1000;
	 four[1]= hex2c(n/100);
	 n = n%100;
		   
	 four[2] = '.';	   	
	 four[3]= hex2c( (n/10) );
	 n = n%10;
	
	 four[4]= hex2c( (n) );
	 four[5]= 0;

     if(four[0] == '0')
	    lcd_puts(four+1);
	 else
	 	lcd_puts(four);
}



#if 0

void lcd_putc(char datax) //列x=0~15,行y=0,1
{
  send_data( datax);
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


void lcd_clear()
{
   io_50ms();
   send_cmd(0x01);  /*清屏*/
   io_50ms();
   
}
#endif
