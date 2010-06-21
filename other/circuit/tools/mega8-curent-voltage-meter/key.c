
/*
* Copyright (c) 2005, 广州力源电器公司开发部
* All rights reserved.
* 文件名称：key.c  负责驱动键盘以获得按键状态

* 当前版本： 1.0
* 作者：	 张洪强
* 完成日期： 2005.7.25
*/

#include <avr/io.h>
//#include <avr/iom8.h>
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include "init.h"

extern void CHECK_EDATA_A(void);

unsigned char key_scan(void)	 					//键盘函数
{
 	if(!( PINB & 0x01 ))							//是第一行
 	{
		return POWER_DOWN;  	 	  				//电源键按下
 	}
 	else if(!( PINB & 0x02 ))						//是第二行
 	{
		return A_V_DOWN;  	 	  					//电流/电压切换键按下
 	}
 	else if(!( PINB & 0x04 ))						//是第二行
 	{
		return CC_CV_DOWN;  	 	  				//稳流/稳压切换键按下
 	}
 	else if(!( PINB & 0x08 ))						//是第二行
 	{
		return AUTO_MAN_DOWN;  	 	  			//自动/手动切换键按下
 	}	
	return 0;  	 	  							//返回键值
}

unsigned char key_down(void)                      //判键盘是否活动函数
{    
    if (( PINB & 0x0f) != 0x0f )                   //如果有键按下
    {
        return 1;                 					//返回1
    }
    else
    {
		return 0;                  				//否则返回0
	} 
}

void key_board(void)
{
	unsigned char key_first, key_end, key_press = 0;
	static unsigned int KEY_DOWN_TIME = 0,NUM_CHANGE = FALSE,i = 0;
	
	if ( ( TIMER_NUM > 500 ) && SET_STATU )			
													//如果超时
	{
		SET_STATU = FALSE;
		TIMER_EN = FALSE;
		if ( NUM_CHANGE )
		{
			NUM_CHANGE = FALSE;
			eeprom_busy_wait();
			cli();
			eeprom_write_block( (char*)MAX_A,(char*)0x20,6 );
			sei();
		}
		KEY_DOWN_TIME = 0;
		TIMER_NUM = 0;
		delayMS(100);						//延时10MS
	}
	if ( key_down() )
	{  
		key_first = key_scan();						//得到键值
		delayMS(20);								//延时10MS；
		key_end   = key_scan(); 
		if ( key_end == key_first )
		{
			key_press  = KEY_TEMP ^ key_end;
			if ( key_end == A_V_DOWN )				//如果是电压/电流切换键
			{
				KEY_DOWN_TIME++;					//按键记时
				if ( KEY_DOWN_TIME > 1000 )			//如果持续按下
				{
					SET_STATU = TRUE;
					TIMER_EN = TRUE;
					KEY_DOWN_TIME = 0;
				}
			}
			if (( KEY_INPUT == AUTO_MAN_DOWN ) && SET_STATU )			
													//如果是电压/电流切换键
			{
				SET_STATU = FALSE;
				TIMER_EN = FALSE;
				if ( NUM_CHANGE )
				{
					NUM_CHANGE = FALSE;
					eeprom_busy_wait();
					cli();
					eeprom_write_block( (char*)MAX_A,(char*)0x20,6 );
					sei();
				}
				key_press = 0x00;
				KEY_INPUT = 0x00;
				KEY_DOWN_TIME = 0;
				TIMER_NUM = 0;
				delayMS(100);						//延时10MS
			}
		}
		if ( key_press ) 
		{
			KEY_INPUT = KEY_TEMP = key_end;
			if ( !SET_STATU )
			{
				//处理电源键
				if ( KEY_INPUT == POWER_DOWN )
				{
					POWER_STATE = !POWER_STATE;		//改变电源按键的状态
					KEY_INPUT = 0;
					if ( POWER_STATE )
					{
						PORTB |= ( 1 << POWER );	//开启电源
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x07, 0x01 );
						sei();
					}
					else
					{
						PORTB &= ~( 1 << POWER );	//关闭电源
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x07, 0x00 );
						sei();
					}
				}	
				//处理电流/电压切换，默认状态是显示电压
				else if ( KEY_INPUT == A_V_DOWN )
				{
					KEY_A_V_STATE = !KEY_A_V_STATE;		//切换AV状态
					KEY_INPUT = 0;
					if ( KEY_A_V_STATE )
					{
						PORTB |= ( 1 << OUT_A );		//显示电流
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x01, 0x01 );
						sei();
					}
					else
					{
						PORTB &= ~( 1 << OUT_A );		//显示电压
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x01, 0x00 );
						sei();
					}
				}
				//处理稳流/稳压切换，默认状态是稳流
				if ( KEY_INPUT == CC_CV_DOWN )
				{
					KEY_CC_CV_STATE = !KEY_CC_CV_STATE;
												//改变稳流/稳压按键的状态
					KEY_INPUT = 0;
					if ( KEY_CC_CV_STATE )
					{
						PORTB |= ( 1 << OUT_CV );		//稳压
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x05, 0x01 );		
						sei();
					}
					else
					{
						PORTB &= ~( 1 << OUT_CV );		//稳流
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x05, 0x00 );		
						sei();
					}
				}
				//处理手动/自动切换，默认状态是手动
				if ( KEY_INPUT == AUTO_MAN_DOWN )
				{
					KEY_AUTO_MAN_STATE = !KEY_AUTO_MAN_STATE;
												//改变手动/自动按键的状态
					KEY_INPUT = 0;
					if ( KEY_AUTO_MAN_STATE )
					{
						PORTB |= ( 1 << OUT_AUTO );	//自动
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x03, 0x01 );		
						sei();
					}
					else
					{
						PORTB &= ~( 1 << OUT_AUTO );	//手动
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x03, 0x00 );		
						sei();
					}
				}
			}
		}
	}
	else
	{
		KEY_INPUT = KEY_TEMP = 0x00;
		KEY_DOWN_TIME = 0;
	}
	if ( SET_STATU )
	{
		CHECK_EDATA_A();
		if ( KEY_INPUT == POWER_DOWN )
		{
			i++;
			TIMER_NUM = 0;
			NUM_CHANGE = TRUE;
			if ( ( i % 25 ) == 0 )
			{
				if ( MAX_A[0] > 0 )
				{
					MAX_A[0]--;
					MAX_A[1] = MAX_A[2] = MAX_A[0];
				}
			}
		}	
		else if ( KEY_INPUT == CC_CV_DOWN  )
		{
			i++;
			TIMER_NUM = 0;
			NUM_CHANGE = TRUE;
			if ( ( i % 25 ) == 0 )
			{
				if ( MAX_A[0] < 2000 )
				{
					MAX_A[0]++;
					MAX_A[1] = MAX_A[2] = MAX_A[0];
				}
				else
				{
					MAX_A[1] = MAX_A[2] = MAX_A[0] = 0;
				}
			}
		}
	}
}

