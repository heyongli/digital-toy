/*
控制器:
http://www.lcdmk.com/tbpic/pcf8576.pdf
LCD:
http://www.lcdmk.com/tbpic/LCM1603.pdf

*/

#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <timer.h>
#include <bitops.h>
#include <drivers/lcd.h>



sbit   sda = P3^6;
sbit   scl = P3^7;
sbit   back_light_switch = P1^2;

// 0 --f 的笔形码
unsigned char code num_table[16]={0xd7,0x03,0xb6,0xa7,0x63,0xe5,0xf5,0x07,0xf7,0xe7,0x77,0xf1,0xd4,0xb3,0xf4,0x74};



 

/*============写一个字节=======================================================
时序: 上升沿串行移入 ,scl = 0时才能改变
sda 的数据,发送完第八个位后,还要发送一个scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i;
       
    for(i = 0;i < 8; i++)
    {
        scl = 0;
        sda = (dat << i) & 0x80;
        scl = 1;
    }
   
    scl = 0;scl = 0;
    scl = 1;scl = 1;scl = 1;
    scl = 0; 
}
//============================

 


/*======起始信号=============
时序:scl = 1 时sda 出现下降沿
=============================*/
void start(void)
{
    sda = 1;
    scl = 1;
    sda = 0;
    scl = 0;
    write(0x70);
}
//==============================


/*======结束信号================
时序:scl = 1 时 sda 出现上升沿
===============================*/
void stop(void)
{
    sda = 0;
    scl = 1;
    sda = 1;

}
//================================

 

 

/*====数码管液晶显示程序=======
形参: float dat ---带显示的数据
功能:显示 float dat 的数值两位小数数值范围为[-99999.99 , 99999.99];
================================*/
void lcm1630_display(float dat)
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
      //  stop();
     //   start();
          

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

 

void lcm1630_demo()
{
   float i;
   unsigned char k;

 //   while(1)display(-99999.99);
  
   for(i = -1.10;i<1.5;i = i + 0.01)
   {
            lcm1630_display(i);
			_delay_ms(10);
   }
   
    
}




/*====数码管液晶显示程序=======

//数据格式write(0x48)-->stop()-->start()-->write(0xe0)-->write(0)-->
//{17个字节数据进行发送}-->write(0)-->stop()完成整个显示动作

================================*/
// 液晶数据为17个字节，具体段码见资料表格，以下为显示HELLO的断码

//                                 H     E    L    L    0
unsigned char code Hello[17]={0x73,0xf4,0xd0,0xd0,0xd7,0x00,0x00,0x00,
         
      			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 



void lcm1630_demo2(void)
{

    char i;

        
    start();
    write(0x48);    //模式设置命令字，详见中文资料
    stop();
  
    start();
    write(0xe0);    //设备选择
    write(0);        //数据指针设置

    for(i = 0; i <16; i++)
    {
        write(Hello[i]|0x8);
    }  
 
    write(0xff);      
    stop();      //结束
}
 

 
 
