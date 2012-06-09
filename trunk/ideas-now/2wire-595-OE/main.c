

#include <at89x52.h>
#include <intrins.h>



// _2cycle(1)-> move r7, 1, lcall _2cycle, total 3cycle
//so _2cycle(n)=> 2n+5 cycles, n from 1...255;
#pragma REGPARMS
void _2cycle(unsigned char cyc);


/*12Mhz : at least 7us (7-5=2 2/2=1)   + - 1us if above 8us*/
#define _delay_us(t)  _2cycle((t-5)>>1) //_2cycle can't accept 0


void _delay_ms( unsigned short ms)
{
  char i=ms;
  while(ms){
   _delay_us(100);
   _delay_us(100);
   _delay_us(100);
   _delay_us(100);
   _delay_us(100);
     
   ms--;
  }

}

/* 3-wire Serial LCD using 74HC595
Rajendra Bhatt, Sep 6, 2010
*/
 
sbit Data_Pin=P0^1;
sbit Clk_Pin=P0^0;
sbit Enable_Pin=P0^2;
 
// Always mention this definition statement
unsigned short Low_Nibble, High_Nibble, p, q,  Mask, N,t, RS, Flag, temp;
 


void io_delay()
{
   _delay_us(15);
}
void io_50ms()
{
   _delay_ms(100);
}
 
void Write_LCD_Nibble(unsigned short N)
{

  		Enable_Pin = 0;
		_delay_us(5);
		Clk_Pin=0;
		Data_Pin = RS;

		Clk_Pin = 1;
		_delay_us(5);
		Clk_Pin = 0;
		_delay_us(5);
		
 		// ****** Write RS *********
 		
 		// Shift in 8 bits
 		Mask=0x8;
		
		for (t=0; t<4; t++){
   				Flag = N & Mask;
   				if(Flag==0)
					 Data_Pin = 0;
   				else 
					 Data_Pin = 1;
				_delay_us(5);
				Clk_Pin = 1;
				_delay_us(5);
				Clk_Pin = 0;
				_delay_us(5);
				Mask = Mask>>1;
  	     }
 		// One more clock because SC and ST clks are tied
		Clk_Pin = 1;
		_delay_us(5);
		Clk_Pin = 0;
		_delay_us(5);

  		Enable_Pin = 1;
		_delay_ms(1);
  		Enable_Pin = 0;
		_delay_ms(1);

 }
// ******* Write Nibble Ends
static void hd44870_send(unsigned char c, char is_cmd) 
{
   io_delay();
   if(is_cmd)
   		RS=0;
	else RS=1; // It is Data, not command
   
   Low_Nibble =  c&0xF;
   High_Nibble = (c>>4)&0xF;
   
   Write_LCD_Nibble(High_Nibble);
   
   Write_LCD_Nibble(Low_Nibble);

}

#define send_cmd(data) hd44870_send(data,1)
#define send_data(data) hd44870_send(data,0)
 
void lcd1602_init(void)    
{
  
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

void lcd_putc(char c) //列x=0~15,行y=0,1
{
  send_data( c);
}
void lcd_puts(char *s) 
{
    while (*s) 
    {
      send_data( *s );
      s ++;
    }
     
} 
void main() {
  lcd1602_init();
 //  while(1);
  lcd_puts("INIT...DONE");
  while(1); 
}





