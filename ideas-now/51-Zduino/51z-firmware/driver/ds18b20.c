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


#define DS18B20_CONVERT_TEMP     0x44
#define DS18B20_READ_SCRATCHPAD  0xBE

#define DS18B20_SKIPROM    0xCC

sbit ds = P1^3;


void ds_delay(unsigned char i)
{
  while(i--);

}
	  
/*12Mhz for 51, 1us one cycle*/
//#define _udelay _2cycle


//初始化DS18B20
 //让DS18B20一段相对长时间低电平, 然后一段相对非常短时间高电平, 即可启动
 void dallas_reset()
 {
   ds = 0;
   //_udelay(480);
   ds_delay(80);

   ds = 1;    //pull up and prepare receive 
   //_udelay(70);  
   ds_delay(11); //62us
   
   
   if(0==ds);
   else { hd44870_send('!',0);}
    
   // _udelay(410);
   ds_delay(68);

#if 0
	//check bus
	ds=1;

	if(ds==1);//ok
	else  hd44870_send('@',0);
#endif

 }

/*60us for one read time slot*/
bit dallas_read_bit()
{
   unsigned char b=0;
   ds = 0;
   //delay for reading bit
   //_udelay(6);
   _nop_;_nop_; 
   ds = 1;	//relase bus
   
   //delay appropiate time for read
   //_udelay(9);
     _nop_;_nop_; _nop_;_nop_; _nop_;


   if(ds)b=1;

   //finish bit slot
   //_udelay(55);
    ds_delay(7);
	     
    return b;
}

void dallas_write_bit(unsigned char b)
{
   ds = 0;
   //delay for write bit
   //_udelay(6);
   { _nop_;_nop_; _nop_; }
    if(b)
     /// udelay(6);
	 { _nop_;_nop_; _nop_;  }
    else
	   //udelay(60);
	   ds_delay(7);
   	
	ds = 1;	//relase bus
   
    if(b) 
	   //udelay(64);
	   ds_delay(7);
}


unsigned char dallas_read_byte()
{
   unsigned char i;
   unsigned char byte=0;
   for(i=0; i<8; i++)
   {
       if( dallas_read_bit())
	     byte |= 0x01<<i;
    }
    return byte;
}

//向DS18B20写入一字节数据
void writeByte(unsigned char byte)
{
   unsigned char i;
   for(i=0; i<8; i++){
       dallas_write_bit((byte>>i)&0x01);
	   //delay for each wirte
	   _nop_;//udelay(1);
    }
}

//向DS18B20发送温度转换命令
void ds18b20_start()
{
   dallas_reset();    //初始化DS18B20, 无论什么命令, 首先都要发起初始化
   writeByte(DS18B20_SKIPROM);
   writeByte(DS18B20_CONVERT_TEMP); //写入温度转换命令字 Convert T

   while(!dallas_read_bit());//wait it done
}


//获取当前温度值
int getTmpValue()
{
    int value; //存放温度数值
    unsigned char low, high;
    
	dallas_reset();
    writeByte(DS18B20_SKIPROM);
    writeByte(DS18B20_READ_SCRATCHPAD); //写入读取数据令字 Read Scratchpad;
    //连续读取两个字节数据
    low  = dallas_read_byte();
    high = dallas_read_byte();
    //将高低两个字节合成一个整形变量
    //计算机中对于负数是利用补码来表示的
    //若是负值, 读取出来的数值是用补码表示的, 可直接赋值给int型的value
    value = high;
    value <<= 8;
    value |= low;
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
    unsigned int t;
    lcd_cursor(0,1);
	//while(1){ds18b20_start();mdelay(1);};
	ds18b20_start();
	t=getTmpValue();
	display(t>>4);//
	lcd_putc('.');
	display(t&0xF);//
	
	
}

