/*
 * init version author: Unknown.
 *  DS1302 opration lib
 *  hyl 2011.2.21
 */


#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <config.h>
#include <1602.h>
#include <delay.h>
#include <bitops.h>


//#define BURST_OPS

#define C51


/*
 *  C51 oprations 
 */
#ifdef C51

sbit DS1302_SCLK = P0^4; 
sbit DS1302_SDA = P0^5; 
sbit DS1302_RST = P0^3; 

#define bus_nop _nop_()

#define SET_CLK DS1302_SCLK=1
#define CLR_CLK DS1302_SCLK=0
#define SET_SDA DS1302_SDA=1
#define CLR_SDA DS1302_SDA=0
#define SET_RST DS1302_RST=1
#define CLR_RST DS1302_RST=0

#endif

/*
 *
 */

/*
#define SET_CLK DS1302_PORT|=1<<DS1302_SCLK
#define CLR_CLK DS1302_PORT&=~(1<<DS1302_SCLK)
#define SET_SDA DS1302_PORT|=1<<DS1302_SDA
#define CLR_SDA DS1302_PORT&=~(1<<DS1302_SDA)
#define SET_RST DS1302_PORT|=1<<DS1302_RST
#define CLR_RST DS1302_PORT&=~(1<<DS1302_RST)
*/



#define DS1302_SEC 0x80
#define DS1302_MIN 0x82
#define DS1302_HOUR 0x84
#define DS1302_DATE 0x86
#define DS1302_MONTH 0x88
#define DS1302_DAY 0x8a
#define DS1302_YEAR 0x8c
#define DS1302_CONTROL 0x8e
#define DS1302_CHARGER 0x90
#define DS1302_CLKBURST 0xbe
#define DS1302_RAMBURST 0xfe
#define DS1302_RAM_BASE 0xc0		//RAM起始地址
#define DS1302_WP_DIS  ds1302_write(DS1302_CONTROL,0x00)		//enable write
#define DS1302_WP_EN   ds1302_write(DS1302_CONTROL,0x80)		//disable write
#define DS1302_CHG_EN  ds1302_write(DS1302_CHARGER,0xa9)	//enable ticle charge
#define DS1302_CHG_DIS ds1302_write(DS1302_CHARGER,0x0)	//disable ticle charge
#define DS1302_RAM_BURST_WRITE bus_write(DS1302_RAMBURST);
#define DS1302_RAM_BURST_READ  bus_write(DS1302_RAMBURST+1);



static unsigned char bus_read(void)
{
	unsigned char i,val=0;
	SET_SDA;		//set sda to input mode
	bus_nop;
	for(i=0; i<8; i++){
		if (DS1302_SDA) //receive
 		    _set_bit(val,i);	
		else 
	     _clear_bit(val,i);

		SET_CLK;
		bus_nop;
		CLR_CLK;
		bus_nop;
	}
	return  val;
}

static void bus_write(unsigned char val)
{
	unsigned char i;
	for(i=8; i>0; i--)
	{
		(val&0x1)?(SET_SDA):(CLR_SDA);
		bus_nop; 
		SET_CLK;
		bus_nop; 
		CLR_CLK;
		bus_nop; 
		val>>= 1;
	}
}

void ds1302_write(unsigned char addr, unsigned char val)
{
	CLR_RST;
	bus_nop;
	CLR_CLK;
	bus_nop;
	SET_RST;    //enable chip
	bus_nop;
	bus_write(addr); /* 地址，命令 */
	bus_write(val); /* 写1Byte数据*/
	SET_CLK;
	bus_nop;
	CLR_RST;
	bus_nop; 
}

unsigned char ds1302_read(unsigned char addr)
{
	unsigned char val;
	addr|=1;		//读操作，最低bit为1，
	CLR_RST;
	bus_nop;
	CLR_CLK;
	bus_nop;
	SET_RST;
	bus_nop;
	bus_write(addr); /* 地址，命令 */
	val = bus_read(); /* 读1Byte数据 */
	SET_CLK;
	bus_nop;
	CLR_RST;
	bus_nop; 
	return(val);
}


void ds1302_init(void)
{
	CLR_CLK;
	bus_nop; 
	CLR_RST;
	bus_nop; 
	DS1302_WP_DIS;
	//DS1302_CHG_EN;		//允许涓流充电
	ds1302_write(DS1302_SEC,ds1302_read(DS1302_SEC)&0x7f);	//24小时制
	DS1302_WP_EN;
}


/********************************************************************
* 输入: val: 初始时间地址。初始时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
*/
#if 0
void ds1302_initime_Set(unsigned char *val)
{
	unsigned char i;
	unsigned char addr = DS1302_SEC_Reg;
	DS1302_WP_DIS;
	for(i =7;i>0;i--)
	{
		ds1302_write(addr,*val); /* 秒 分 时 日 月 星期 年 */
		val++;
		addr +=2;
	}
	DS1302_WP_EN;
}
#endif

/********************************************************************
*
* 名称: ds1302_CurTime_Get
* 说明:
* 功能: 读取DS1302当前时间
* 调用: ds1302_read()
* 输入: Curtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
unsigned char Curtime[8];
#define SEC    (Curtime[0])
#define MIN    (Curtime[1])
#define HOUR   (Curtime[2])
#define YEAR   (Curtime[3])
#define DATE   (Curtime[4])
#define WEEK   (Curtime[5])


void ds1302_CurTime_Get(unsigned char Curtime[])
{
	unsigned char i;
	unsigned char addr = DS1302_SEC|1;	 //读操作
//	for (i=0;i<7;i++){
		Curtime[i] = ds1302_read(addr); /*格式为: 秒 分 时 日 月 星期 年 */
		addr += 2;
//	}
}

unsigned char sec,min,hour;

void lcd_showhex(unsigned char x);

ds1302_test()
{
    
    char sec; 
//	ds1302_CurTime_Get(Curtime);
	lcd_cursor(0,0);
	
	
	//year/month/date
	lcd_showhex(0x20);
	lcd_showhex(ds1302_read(DS1302_YEAR));
	lcd_puts("/");
	lcd_showhex(ds1302_read(DS1302_MONTH));
	lcd_puts("/");
	lcd_showhex(ds1302_read(DS1302_DATE));
	lcd_puts(" ");
	
	/*hour:min*/
	sec=ds1302_read(DS1302_SEC);
	
	lcd_showhex(ds1302_read(DS1302_HOUR));
	if(sec%2)
		lcd_puts(":");
	else
		lcd_puts(" ");
	lcd_showhex(ds1302_read(DS1302_MIN));
	

	
}


#ifdef BURST_OPS
/*
 * 时钟数据地址 格式为: 秒 分 时 日 月 星期 年 控制
 * 8Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B 1B
 */
void ds1302_write_burst(unsigned char *data)
{
	unsigned char i;
	DS1302_WP_DIS;/* 控制命令,WP=0,写操作?*/
	CLR_RST;
	bus_nop;
	CLR_CLK;
	bus_nop;
	SET_RST;
	bus_nop;
	bus_write(DS1302_CLKBURST_Reg); /* 0xbe:时钟多字节写命令 */
	for (i=8;i>0;i--) /*8Byte = 7Byte 时钟数据 + 1Byte 控制*/
	{
		bus_write(*val);/* 写1Byte数据*/
		val++;
	}
	SET_CLK;
	bus_nop;
	CLR_RST;
	DS1302_WP_EN;
}

void ds1302_time_get_burst(unsigned char *val)
{
	unsigned char i;
	CLR_RST;
	bus_nop;
	CLR_CLK;
	bus_nop;
	SET_RST;
	bus_nop;
	bus_write(DS1302_CLKBURST_Reg+1); /* 0xbf:时钟多字节读命令 */
	for (i=8; i>0; i--)
	{
		*val = bus_read(); /* 读1Byte数据 */
		val++;
	}
	SET_CLK;
	bus_nop;
	CLR_RST;
}

void ds1302_ram_write_burst(unsigned char *val)
{
	unsigned char i;
	DS1302_WP_DIS; //解除写保护
	CLR_RST;
	bus_nop;
	CLR_CLK;
	bus_nop;
	SET_RST;
	bus_nop;
	DS1302_RAM_BURST_WRITE; /* 0xfe:RAM多字节写命令 */
	for (i=31;i>0;i--) /*31Byte 寄存器数据 */
	{
		bus_write(*val); /* 写1Byte数据*/
		val++;
	}
	SET_CLK;
	bus_nop;
	CLR_RST;
	DS1302_WP_EN;
}

void ds1302_ram_read_burst(unsigned char *val)
{
	unsigned char i;
	CLR_RST;
	bus_nop;
	CLR_CLK;
	bus_nop;
	SET_RST;
	bus_nop;
	DS1302_RAM_BURST_READ;/* 0xff:RAM字节读命令 */
	for (i=31; i>0; i--) /*31Byte 寄存器数据 */
	{
		*val = bus_read(); /* 读1Byte数据 */
		val++;
	}
	SET_CLK;
	bus_nop;
	CLR_RST;
}
#endif
