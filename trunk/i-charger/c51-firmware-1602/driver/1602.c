#include "delay.h"
#include "1602.h"

#include <AT89X52.H>			 

#define io_delay mdelay(8)
/***********************************************
程序名称：1602液晶显示程序
简要说明：使用FM1602液晶显示器，数据口接P0口，控制端接P2.0-P2.2
寄存器选择信号端RS-P2.0、读写信号端R/W-P2.1、使能信号端EP-P2.2
编    写：JiangX.net
***********************************************/
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL ;

sbit rs = P2^0;   //RS
sbit rw = P2^1;   //读写选择RW
sbit ep = P2^2;   //使能信号E

BYTE code dis1[] = {"Digital-Toy"};
BYTE code dis2[] = {"i-Charger V1.0"};


/*****判别LCD忙碌状态*****/
BOOL lcd_bz()
{
	BOOL result;
	rs = 0;
	rw = 1;
	ep = 1;
	result = (BOOL)(P0 & 0x80);
	ep = 0;
	return result;
}
/*****写入指令数据到LCD*****/
void lcd_wcmd(BYTE cmd)
{
  while(lcd_bz());
  rs = 0;
  rw = 0;
  ep = 0;
  udelay(100);
  P0 = cmd;

  ep = 1;
  ep = 0;  
}

/*****设定显示位置*****/
void lcd_start(BYTE start)
{       
   lcd_wcmd(start | 0x80);
}

/*****写入字符显示数据到LCD*****/
void lcd_data(BYTE dat)
{       
  while(lcd_bz());
  rs = 1;
  rw = 0;
  ep = 0;
  P0 = dat;
  ep = 1;
  ep = 0;
}

/*****LCD初始化设定*****/
void lcd_init()
{
    io_delay;      
	lcd_wcmd(0x38);    //设定LCD为16*2显示，5*7点阵，8位数据接口
	io_delay;
	lcd_wcmd(0x0c);    //开显示，不显示光标
	io_delay;
	lcd_wcmd(0x06);    //显示光标自动右移，整屏不移动
	io_delay;
	lcd_wcmd(0x01);    //显示清屏
	io_delay;

	
}


void lcd_logo()
{
  int pos=0,i=0;
  lcd_start(0);    // 设置显示位置为第一行的第5个字符
  io_delay;
  while(pos < (sizeof(dis1)-1)){
     lcd_data(dis1[i]); // 显示第二行字符
     i++;pos++;
     io_delay;
  }   

  pos=0;i=0;
  lcd_start(0x40);   // 设置显示位置为第二行第一个字符
  while(pos<(sizeof(dis2)-1)){
     lcd_data(dis2[i]); // 显示第二行字符
     i++;pos++;
     io_delay;
  }   


}
#if 0

/*pos: 0-15, 16-31*/
void lcd_puts(char pos, char *str)
{
   if(pos>15)
		lcd_start(0x40|(pos-16));
   else 
		lcd_start(pos);

   pos=0;
   while(str[pos]){
     lcd_data(str[pos]);
	 pos++; 
   }
    
}


void demo_1602()
{
  BYTE i=0;
  char pos=0;

  lcd_init();    // 初始化LCD   
  mdelay(200);

again: 
  pos=0; 
  lcd_start(0);    // 设置显示位置为第一行的第5个字符
  io_delay;
  while(pos<15){
     lcd_data(i); // 显示第二行字符
     i++;pos++;
     io_delay;
  }   

  pos=0;
  lcd_start(0x40);   // 设置显示位置为第二行第一个字符
  while(pos<15){
     lcd_data(i); // 显示第二行字符
      i++;pos++;  
     io_delay;
  }   
  
  
  
  goto again;
}

#endif

