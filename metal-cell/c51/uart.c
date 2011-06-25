 /*
  *  hyl 2011.2.28
  */


#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <bitops.h>
#include <drivers/lcd.h>
#include <io.h> /*cli sti*/



/* 11M or 11.0592M, BaduRate 9600	*/
void uart_init()
{
  
                    	  //初始化串行口和波特率发生器
	SCON =0x50;           //选择串口工作方式1，打开接收允许
	TMOD =0x20;           //定时器1工作在方式2，定时器0工作在方式1
	TH1 =TL1 =0xFA; //2400 //0xfA;            //实现波特率9600（系统时钟11.0592MHZ）
	PCON = 0x80;

    
	RI=TI=0; //clear tx/rx flag

	
	ES=1; //enable serial interrupt
	PS=1; //serial int priority 
	
	TR1 = 1; //enable timer 1
	
}


char banner[] = {"Welcom to Z51!"};
void test_sent()
{
   char i;
   for(i=0;i<sizeof(banner); i++){
     SBUF=banner[i];
	 while(!TI);
	 TI=0;
   }
	
}

void uart_receiver_intrrupt()  interrupt 4 using 3
{

   unsigned char  buffer;
   
   if(RI){
   		RI=0; //clear int flag
		buffer = SBUF; //get data
		if(buffer == 's')
		{
		 	SCON=0x40; //disable receive
			test_sent();
			
			lcd_puts("===");
			SCON = 0x50; //eanble receive
		}
    }	

}

void uart_demo()
{
   cli();
   uart_init();
   test_sent();
   lcd_cursor(0,0);
   lcd_puts("UART demo here!");
   sti();
   while(1);

}
