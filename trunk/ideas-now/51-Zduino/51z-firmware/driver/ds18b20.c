/*
 *  DS18B02 opration lib
 *  hyl 2011.2.21
 */


#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <math.h>  /*abs*/
#include <config.h>
#include <1602.h>
#include <delay.h>
#include <bitops.h>

sbit ds = P1^3;


//初始化DS18B20
 //让DS18B20一段相对长时间低电平, 然后一段相对非常短时间高电平, 即可启动
 void ds18b20_init()
 {
   //对于11.0592MHz时钟, unsigned int型的i, 作一个i++操作的时间大于?us
   unsigned int i;
   ds = 0;
   i = 19000;   //拉低约800us, 符合协议要求的480us以上
   while(i-->0)_nop_;
   ds = 1;    //产生一个上升沿, 进入等待应答状态
   i = 1200;
   while(i>0) i--;
 }
 
 void dsWait()
 {
    unsigned int i=1000;
    while(ds);
	i=1000;
    while((~ds));  //检测到应答脉冲
    i = 4;
    while(i > 0) i--;
}

//向DS18B20读取一位数据
//读一位, 让DS18B20一小周期低电平, 然后两小周期高电平,
//之后DS18B20则会输出持续一段时间的一位数据
bit readBit()
{
    unsigned int i;
   bit b;
   ds = 0;
  i++;   //延时约8us, 符合协议要求至少保持1us
  ds = 1;
  i++; i++;  //延时约16us, 符合协议要求的至少延时15us以上
   b = ds;
    i = 8;
    while(i>0) i--;  //延时约64us, 符合读时隙不低于60us要求
   return b;
}

//读取一字节数据, 通过调用readBit()来实现
unsigned char readByte()
{
   unsigned int i;
     unsigned char j, dat;
    dat = 0;
   for(i=0; i<8; i++)
   {
        j = readBit();
       //最先读出的是最低位数据
       dat = (j << 7) | (dat >> 1);
    }
    return dat;
}

//向DS18B20写入一字节数据
void writeByte(unsigned char dat)
{
 unsigned int i;
   unsigned char j;
    bit b;
  for(j = 0; j < 8; j++)
   {
       b = dat & 0x01;
       dat >>= 1;
       //写"1", 将DQ拉低15us后, 在15us~60us内将DQ拉高, 即完成写1
        if(b) 
       {
            ds = 0;
           i++; i++;  //拉低约16us, 符号要求15~60us内
            ds = 1;  
           i = 8; while(i>0) i--;  //延时约64us, 符合写时隙不低于60us要求
       }
      else  //写"0", 将DQ拉低60us~120us
           ds = 0;
            i = 8; while(i>0) i--;  //拉低约64us, 符号要求
            ds = 1;
           i++; i++;  //整个写0时隙过程已经超过60us, 这里就不用像写1那样, 再延时64us了
     
    }
}

//向DS18B20发送温度转换命令
void sendChangeCmd()
{
   ds18b20_init();    //初始化DS18B20, 无论什么命令, 首先都要发起初始化
   dsWait();   //等待DS18B20应答
   mdelay(1);    //延时1ms, 因为DS18B20会拉低DQ 60~240us作为应答信号
   writeByte(0xcc); //写入跳过序列号命令字 Skip Rom
   writeByte(0x44); //写入温度转换命令字 Convert T
}

//向DS18B20发送读取数据命令
void sendReadCmd()
{
   ds18b20_init();
   dsWait();
   mdelay(1);
   writeByte(0xcc); //写入跳过序列号命令字 Skip Rom
   writeByte(0xbe); //写入读取数据令字 Read Scratchpad
}

//获取当前温度值
int getTmpValue()
{
    unsigned int tmpvalue=0;
    int value; //存放温度数值
    float t;
    unsigned char low, high;
    sendReadCmd();
    //连续读取两个字节数据
    low = readByte();
    high = readByte();
    //将高低两个字节合成一个整形变量
    //计算机中对于负数是利用补码来表示的
    //若是负值, 读取出来的数值是用补码表示的, 可直接赋值给int型的value
    tmpvalue = high;
    tmpvalue <<= 8;
    tmpvalue |= low;
    value = tmpvalue;
 
    //使用DS18B20的默认分辨率12位, 精确度为0.0625度, 即读回数据的最低位代表0.0625度
    t = value * 0.0625;
    //将它放大100倍, 使显示时可显示小数点后两位, 并对小数点后第三进行4舍5入
   //如t=11.0625, 进行计数后, 得到value = 1106, 即11.06 度
   //如t=-11.0625, 进行计数后, 得到value = -1106, 即-11.06 度
    value = t * 100 + (value > 0 ? 0.5 : -0.5); //大于0加0.5, 小于0减0.5
    return value;
}


#define lcd_putc(c) hd44870_send(c,0)
char hex2c(char hex) ;

void display(int n)
{
    
	 int c;
	 char show=0;
     //irqoff();
	 c= hex2c((n/100000));
	 n = n%100000;
     
	 if(c||show){ lcd_putc(c); show =1;}

	 c= hex2c((n/10000));
	 n = n%10000;
     
	 if(c||show){ lcd_putc(c); show =1;}

	 c= hex2c(n/1000);
	 n = n%1000;
	 if(c||show) { lcd_putc(c); show =1;}	 
		   	
	 c= hex2c( (n/100) );
	 n = n%100;
	 if(c||show) { lcd_putc(c); show =1;}	 
	
	 c= hex2c( (n/10) );
     lcd_putc(c);	 

	 c= hex2c( (n%10) );;
	 lcd_putc(c);	 
  
}

ds18b20_demo()
{
    lcd_cursor(0,1);

	display(getTmpValue());
	
	
}

