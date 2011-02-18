/**
   DS1302 opration lib
*/


#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <config.h>
#include <1602.h>
#include <delay.h>
#include <bitops.h>


sbit DS1302_SCLK = P0^4; /*实时时钟时钟线引脚 */
sbit DS1302_SDA = P0^5; /*实时时钟数据线引脚 */
sbit DS1302_RST = P0^3; /*实时时钟复位线引脚 */

#define DS1302_WAIT udelay(10);_nop_()

#define Set_CLK DS1302_SCLK=1
#define Clr_CLK DS1302_SCLK=0
#define Set_SDA DS1302_SDA=1
#define Clr_SDA DS1302_SDA=0
#define Set_RST DS1302_RST=1
#define Clr_RST DS1302_RST=0

typedef unsigned char UI8_T;

/*********************************************************************
DS1302 C51 Driver
*********************************************************************/

#define DS1302_SEC_Reg 0x80
#define DS1302_MIN_Reg 0x82
#define DS1302_HR_Reg 0x84
#define DS1302_DATE_Reg 0x86
#define DS1302_MONTH_Reg 0x88
#define DS1302_DAY_Reg 0x8a
#define DS1302_YEAR_Reg 0x8c
#define DS1302_CONTROL_Reg 0x8e
#define DS1302_CHARGER_Reg 0x90
#define DS1302_CLKBURST_Reg 0xbe
#define DS1302_RAMBURST_Reg 0xfe
#define DS1302_RAM_BASE_ADDR 0xc0		//RAM起始地址
#define DS1302_WP_DIS  RTC_Write(DS1302_CONTROL_Reg,0x00)		//enable write
#define DS1302_WP_EN RTC_Write(DS1302_CONTROL_Reg,0x80)		//disable write
#define DS1302_CHG_EN RTC_Write(DS1302_CHARGER_Reg,0xa9)	//使能涓流充电
#define DS1302_CHG_DIS RTC_Write(DS1302_CHARGER_Reg,0x0)	//禁止涓流充电
#define DS1302_RAM_BURST_WRITE RTC_Write_Byte(DS1302_RAMBURST_Reg);
#define DS1302_RAM_BURST_READ RTC_Write_Byte(DS1302_RAMBURST_Reg+1);
//



/*
#define Set_CLK RTC_PORT|=1<<DS1302_SCLK
#define Clr_CLK RTC_PORT&=~(1<<DS1302_SCLK)
#define Set_SDA RTC_PORT|=1<<DS1302_SDA
#define Clr_SDA RTC_PORT&=~(1<<DS1302_SDA)
#define Set_RST RTC_PORT|=1<<DS1302_RST
#define Clr_RST RTC_PORT&=~(1<<DS1302_RST)
*/

/********************************************************************
*
* 名称: RTC_Write_Byte
* 说明:
* 功能: 往DS1302写入1Byte数据
* 调用:
* 输入: Dat 写入的数据
* 返回值: 无
***********************************************************************/
void RTC_Write_Byte(UI8_T Dat)
{
	UI8_T i;
	for(i=8; i>0; i--)
	{
		(Dat&0x1)?(Set_SDA):(Clr_SDA);
		DS1302_WAIT; 
		Set_CLK;
		DS1302_WAIT; 
		Clr_CLK;
		DS1302_WAIT; 
		Dat>>= 1;
	}
}
/********************************************************************
*
* 名称: RTC_Write
* 说明: 先写地址，后写命令/数据
* 功能: 往DS1302写入数据
* 调用: RTC_Write_Byte()
* 输入: Addr: DS1302地址, Dat: 要写的数据
* 返回值: 无
***********************************************************************/
void RTC_Write(UI8_T Addr, UI8_T Dat)
{
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;    //enable chip
	DS1302_WAIT;
	RTC_Write_Byte(Addr); /* 地址，命令 */
	RTC_Write_Byte(Dat); /* 写1Byte数据*/
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WAIT; 
}
/********************************************************************
*
* 名称: UI8_T RTC_Read_Byte
* 说明:
* 功能: 从DS1302读取1Byte数据
* 调用:
* 输入:
* 返回值: dat
***********************************************************************/
UI8_T RTC_Read_Byte(void)
{
	UI8_T i,dat=0;
	Set_SDA;		//set sda to input mode
	DS1302_WAIT;
	for(i=8; i>0; i--)
	{
		if (DS1302_SDA)
		     _set_bit(dat,7);		//load into MSB
		else 
		     _clear_bit(dat,7);
		dat>>=1;
		DS1302_WAIT; 
		Set_CLK;
		DS1302_WAIT;
		Clr_CLK;
		DS1302_WAIT;
		
	}
	return  dat;
}
/********************************************************************
*
* 名称: RTC_Read
* 说明: 先写地址，后读命令/数据
* 功能: 读取DS1302某地址的数据
* 调用: RTC_Write_Byte() , RTC_Read_Byte()
* 输入: ucAddr: DS1302地址
* 返回值: ucDa :读取的数据
***********************************************************************/
UI8_T RTC_Read(UI8_T Addr)
{
	UI8_T Dat;
	Addr|=1;		//读操作，最低bit为1，
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	RTC_Write_Byte(Addr); /* 地址，命令 */
	Dat = RTC_Read_Byte(); /* 读1Byte数据 */
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WAIT; 
	return(Dat);
}


void RTC_Init(void)
{
	Clr_CLK;
	DS1302_WAIT; 
	Clr_RST;
	DS1302_WAIT; 
	DS1302_WP_DIS;
	//DS1302_CHG_EN;		//允许涓流充电
	RTC_Write(DS1302_SEC_Reg,RTC_Read(DS1302_SEC_Reg)&0x7f);	//24小时制
	DS1302_WP_EN;
}

/********************************************************************
*
* 名称: RTC_Write_Burst
* 说明: 先写地址，后写数据(时钟多字节方式)
* 功能: 往DS1302写入时钟数据(多字节方式)
* 调用: RTC_Write_Byte()
* 输入: pDat: 时钟数据地址 格式为: 秒 分 时 日 月 星期 年 控制
* 8Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
#if 0
void RTC_Write_Burst(UI8_T *pDat)
{
	UI8_T i;
	DS1302_WP_DIS;/* 控制命令,WP=0,写操作?*/
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	RTC_Write_Byte(DS1302_CLKBURST_Reg); /* 0xbe:时钟多字节写命令 */
	for (i=8;i>0;i--) /*8Byte = 7Byte 时钟数据 + 1Byte 控制*/
	{
		RTC_Write_Byte(*pDat);/* 写1Byte数据*/
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WP_EN;
}
#endif
/********************************************************************
*
* 名称: RTC_Time_Get_Burst
* 说明: 先写地址，后读命令/数据(时钟多字节方式)
* 功能: 读取DS1302时钟数据
* 调用: RTC_Write_Byte() , RTC_Read_Byte()
* 输入: pDat: 时钟数据地址 格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: ucDa :读取的数据
***********************************************************************/
#if 0
void RTC_Time_Get_Burst(UI8_T *pDat)
{
	UI8_T i;
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	RTC_Write_Byte(DS1302_CLKBURST_Reg+1); /* 0xbf:时钟多字节读命令 */
	for (i=8; i>0; i--)
	{
		*pDat = RTC_Read_Byte(); /* 读1Byte数据 */
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
}
#endif
/********************************************************************
*
* 名称: RTC_RAM_Write_Burst
* 说明: 先写地址，后写数据(寄存器多字节方式)
* 功能: 往DS1302寄存器数写入数据(多字节方式)
* 调用: RTC_Write_Byte()
* 输入: pDat: 寄存器数据地址
* 返回值: 无
***********************************************************************/
#if 0
void RTC_RAM_Write_Burst(UI8_T *pDat)
{
	UI8_T i;
	DS1302_WP_DIS; //解除写保护
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	DS1302_RAM_BURST_WRITE; /* 0xfe:RAM多字节写命令 */
	for (i=31;i>0;i--) /*31Byte 寄存器数据 */
	{
		RTC_Write_Byte(*pDat); /* 写1Byte数据*/
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WP_EN;
}
#endif
/********************************************************************
*
* 名称: RTC_RAM_Read_Burst
* 说明: 先写地址，后读命令/数据(寄存器多字节方式)
* 功能: 读取DS1302寄存器数据
* 调用: RTC_Write_Byte() , RTC_Read_Byte()
* 输入: pDat: 寄存器数据地址
* 返回值: 无
***********************************************************************/
#if 0
void RTC_RAM_Read_Burst(UI8_T *pDat)
{
	UI8_T i;
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	DS1302_RAM_BURST_READ;/* 0xff:RAM字节读命令 */
	for (i=31; i>0; i--) /*31Byte 寄存器数据 */
	{
		*pDat = RTC_Read_Byte(); /* 读1Byte数据 */
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
}
#endif
/********************************************************************
*
* 名称: RTC_IniTime_Set
* 说明:
* 功能: 设置初始时间
* 调用: v_W1302()
* 输入: pDat: 初始时间地址。初始时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
#if 0
void RTC_IniTime_Set(UI8_T *pDat)
{
	UI8_T i;
	UI8_T Addr = DS1302_SEC_Reg;
	DS1302_WP_DIS;
	for(i =7;i>0;i--)
	{
		RTC_Write(Addr,*pDat); /* 秒 分 时 日 月 星期 年 */
		pDat++;
		Addr +=2;
	}
	DS1302_WP_EN;
}
#endif

/********************************************************************
*
* 名称: RTC_CurTime_Get
* 说明:
* 功能: 读取DS1302当前时间
* 调用: RTC_Read()
* 输入: Curtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
UI8_T Curtime[8];
#define SEC    (Curtime[0])
#define MIN    (Curtime[1])
#define HOUR   (Curtime[2])
#define YEAR   (Curtime[3])
#define DATE   (Curtime[4])
#define WEEK   (Curtime[5])


void RTC_CurTime_Get(UI8_T Curtime[])
{
	UI8_T i;
	UI8_T Addr = DS1302_SEC_Reg|1;	 //读操作
//	for (i=0;i<7;i++){
		Curtime[i] = RTC_Read(Addr); /*格式为: 秒 分 时 日 月 星期 年 */
		Addr += 2;
//	}
}

unsigned char sec,min,hour;

void lcd_showhex(unsigned char x);

ds1302_test()
{
    
//	RTC_CurTime_Get(Curtime);
	lcd_cursor(0,1);
	
	
	//sec = SEC&0x0f+(SEC>>4)*10;
	lcd_puts("s:");
	print10(RTC_Read(DS1302_SEC_Reg));

	lcd_puts("s:");
	print10(RTC_Read(DS1302_SEC_Reg+2));
	lcd_puts("s:");
	print10(RTC_Read(DS1302_SEC_Reg+4));
	
	
}


