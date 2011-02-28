/*
 * buy from taobao:
 * http://item.taobao.com/item.htm?id=2971207095
 * CopyLeft hyl 2009.12.19
 */

#include "include/avrio.h"
#include <util/delay.h>



#define HT_PORT PORTB
#define SDA 0
#define SC1 1
#define BLS 2


#define ssda  _set_bit(HT_PORT,SDA)	/* 输出低电平，使LED发光 */
#define csda   _clear_bit(HT_PORT,SDA)	/* 输出低电平，使LED发光 */
#define sscl  _set_bit(HT_PORT,SC1)	/* 输出低电平，使LED发光 */
#define cscl   _clear_bit(HT_PORT,SC1)	/* 输出低电平，使LED发光 */

#define io_delay() _delay_us(10)
//#define sback_light_switch  _clear_bit(HT_PORT,BLS)	/* 输出低电平，使LED发光 */
//#define cback_light_switch  _set_bit(HT_PORT,BLS)	/* 输出低电平，使LED发光 */

ht1603_init()
{
   _pins_mode(HT_PORT,SDA,BLS,OUTPUT);//_nm8(0b11,0,1);
}


// 0 --f 的笔形码
#if 1
unsigned char num_table[16]={0xd7,0x03,0xb6,0xa7,0x63,0xe5,0xf5,0x07,0xf7,0xe7,0x77,0xf1,0xd4,0xb3,0xf4,0x74};

#else
unsigned char num_table[17]={0x73,0xf4,0xd0,0xd0,0xd7,0x00,0x00,0x00,
         
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
      0x00}; 
#endif
//延时程序======延时2*t机器周期=====
void delay_char(unsigned char t)
{
    _delay_ms(t);
}
//===================================

 

/*============写一个字节=======================================================
时序: 上升沿串行移入 ,scl = 0时才能改变
sda 的数据,发送完第八个位后,还要发送一个scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i,d;
       
    for(i = 0;i < 8; i++)
    {
        cscl;
		io_delay() ;
        //sda = (dat << i) & 0x80;
		d = (dat << i) & 0x80;
		if(d)
		  ssda;
        else
		  csda;
		  io_delay() ;
		sscl;
    }
   
    cscl ;
	io_delay() ;
	cscl;
	io_delay() ;
    sscl ;
	io_delay() ;
	sscl;
	io_delay() ;
	sscl;
	io_delay() ;
    cscl ; 
}
//============================

 


/*======起始信号=============
时序:scl = 1 时sda 出现下降沿
=============================*/
void start(void)
{
    ssda;
	io_delay() ;
    sscl;
	io_delay() ;
    csda;
	io_delay() ;
    cscl;
	io_delay() ;
    write(0x70);
}
//==============================



/*======结束信号================
时序:scl = 1 时 sda 出现上升沿
===============================*/
void stop(void)
{
    csda;
		io_delay() ;
    sscl;
		io_delay() ;
    ssda;
		io_delay() ;

}
//================================


//==============================

/*====数码管液晶显示程序=======
形参: float dat ---带显示的数据
功能:显示 float dat 的数值两位小数数值范围为[-99999.99 , 99999.99];
================================*/
void display(float dat)
{
    unsigned long  temp;
    char i,sum;
    unsigned char num[8]= {0};  //num[]存储7位十进制数，num[7]为符号位,length为有效整数位数
  
  
    if(dat < 0)
    {
        dat = - dat;
        num[7] = 0x20;  //符号位为 -
    }

    else
    {
        num[7] = 0;     //符号位为 +
    }


       
        start();
        write(0x48);    //模式设置
        stop();
   
        start();
        write(0xe0);    //设备选择
        write(0);        //数据指针设置
     //   stop();
       // start();
          

    if(dat < 100000 ) //只能显示2位小数,5位整数
    {
       
        temp = 100 * dat;       //取得各个位数上的数值
        num[6] = temp / 1000000;
        num[5] = temp / 100000 %10;
        num[4] = temp / 10000 %10;
        num[3] = temp / 1000 %10;
        num[2] = temp / 100 %10;
        num[1] = temp / 10 %10;
        num[0] = temp %10;
       
        sum = 0;
        for(i = 6;i >2;i--) //查找整数部分有效位的起始位
        {
            sum = num[i] + sum;
            if(sum > 0) break;
        }
        sum = i;    //sum是有效位的起始位显示时从num[sum]开始显示数字
       
        for(i = 0;i <= sum;i++)   //查找每位有效位数字的笔形码
        {
            num[i] = num_table[num[i]];
        }
            num[sum + 1] = num[7];  //符号位
            num[2] = num[2] | 0x08; //添加小数点
           
       
        for(i = 0; i < 14-sum; i++)  //前1至15-sum个位置不显
        {
            write(0);
        }
      
        for(i = sum +1; i >= 0 ;i--) //从num[sum + 1]开始显示
        {
            write(num[i]);
        }
    }

    else    //数据超出显示范围则所有为显示负号
    {
        for(i = 0; i < 16; i++)
        {
            write(0x20);
        }   
    }
    write(0);           //第17个字节写0,即不显示右上角的符号
    stop();      //结束
}
//================================



#if 0
/*============写一个字节=======================================================
时序: 上升沿串行移入 ,scl = 0时才能改变
sda 的数据,发送完第八个位后,还要发送一个scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i,d;
      
    for(i = 0;i < 8; i++)
    {
        cscl; //= 0;
        //sda = (dat << i) & 0x80;
		d = (dat << i) & 0x80;
		if(d)
		  ssda;
        else
		  csda;
		HT_PORT |=  (  (((dat << i) & 0x80)>>7) <<SDA);
        sscl ;//= 1;
    }
  
    cscl;// = 0;
	cscl ;//= 0;
    sscl ;//= 1;
	sscl;// = 1;
	sscl ;//= 1;
    cscl;// = 0;
}
//============================

 


/*======起始信号=============
时序:scl = 1 时sda 出现下降沿
=============================*/
void start(void)
{
    ssda;// = 1;
    sscl ;//= 1;
    csda;// = 0;
    cscl ;//= 0;
    write(0x70);           //命令字，详见中文资料
}
//==============================
  

 

/******************************************************/


//参照叶春勇先生的程序进行移植,只要更改一下IO口配置
//即可移植到多种芯片如AVR PIC 51 ARM HT STC C8051F等

//以I2C总线形式发送数据


// 液晶数据为17个字节，具体段码见资料表格，以下为显示HELLO的断码

//                                 H     E    L    L    0

unsigned char num_table[17]={0x73,0xf4,0xd0,0xd0,0xd7,0x00,0x00,0x00,
         
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
      0x00}; 

void stop(void);
void start(void);
void display(void);
void write(unsigned char  dat);
void delay_char(unsigned char t);


void main()
{


   while(1)display();
 
}



 

/*============写一个字节=======================================================
时序: 上升沿串行移入 ,scl = 0时才能改变
sda 的数据,发送完第八个位后,还要发送一个scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i,d;
      
    for(i = 0;i < 8; i++)
    {
        cscl; //= 0;
        //sda = (dat << i) & 0x80;
		d = (dat << i) & 0x80;
		if(d)
		  ssda;
        else
		  csda;
		HT_PORT |=  (  (((dat << i) & 0x80)>>7) <<SDA);
        sscl ;//= 1;
    }
  
    cscl;// = 0;
	cscl ;//= 0;
    sscl ;//= 1;
	sscl;// = 1;
	sscl ;//= 1;
    cscl;// = 0;
}
//============================

 


/*======起始信号=============
时序:scl = 1 时sda 出现下降沿
=============================*/
void start(void)
{
    ssda;// = 1;
    sscl ;//= 1;
    csda;// = 0;
    cscl ;//= 0;
    write(0x70);           //命令字，详见中文资料
}


#endif
