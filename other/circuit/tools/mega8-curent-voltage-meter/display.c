
/*
* Copyright (c) 2005, 广州力源电器公司开发部
* All rights reserved.
* 文件名称：display.c  负责驱动LED发光管以显示数据

* 当前版本： 1.0
* 作者：	 张洪强
* 完成日期： 2005.7.25
*/

#include <avr/io.h>
#include "init.h"

unsigned char LED[13] = 
{ 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0x88,0xc1 };
	  		   			  						//{0123456789 AV} LED段码表
												
void delay_1ms(void)
{
 	unsigned int i;
	for ( i = 0;i < 1422;i++ );					//3.6864M频率时循环数525
}

void delayMS(unsigned char count)
{
 	unsigned char i;
	for ( i = 0;i < count;i++ )
	{
	 	delay_1ms();
	}
}
												
void display(unsigned char DISP_DATA[4],unsigned char DOT)
{	
	unsigned char i;
	for( i = 0;i < 4; i++ )					//4位LED循环
	{
	  	PORTD = LED[DISP_DATA[i]]; 			//取出显示区数据
		if ( DOT == i )						//显示小数点
		{
			PORTD &= 0x7f;
		}
	 	PORTC = ~( 1 << (i+2) ); 				//开显示位
		delayMS(10);							//延时2MS
		//PORTC = 0x3C;						    //关显示
		PORTD = 0xFF;						    //关显示
	}
}
