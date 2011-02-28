 #include <AT89X52.H>


/*
 *  run about cyc*2+2 cycle
 */
void _2cycle(unsigned char cyc)
{
   while(--cyc);
   /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/
}

/*小于6us严重不准确, 奇数非常准确,偶数误差约(+ -)1us(仿真) */
/*7-512us*/
#define udelay(t)  _2cycle((t/2)-2)



/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void mdelay(unsigned char ms)
{

/*
    35:    while(--t){     
C:0x0005    1E       DEC      R6               1 cyc
C:0x0006    EE       MOV      A,R6	           1 cyc
C:0x0007    600C     JZ       C:0015           1 cyc
    36:      _2cycle(250); 
C:0x0009    7FFA     MOV      R7,#0xFA	       1 cyc
C:0x000B    120027   LCALL    _2cycle(C:0027)   2 cyc
    37:         _2cycle(250); 
C:0x000E    7FFA     MOV      R7,#0xFA		   1 cyc
C:0x0010    120027   LCALL    _2cycle(C:0027)   2 cyc
    38:    } 
C:0x0013    80F0     SJMP     C:0005           2 cyc
*/
   while(ms--){	  
     udelay(489); //补偿12cyc*2 us
	 udelay(499);
   }
  
}

/*
 * +- 50ms
 */
void sleep(unsigned char t)
{
   while(t--){
   	 mdelay(250);
   	 mdelay(250);
     mdelay(250);
     mdelay(250);
   }
     
}


/***********************************************
程序名称：1602液晶显示程序
简要说明：使用FM1602液晶显示器，数据口接P0口，控制端接P2.0-P2.2
寄存器选择信号端RS-P2.0、读写信号端R/W-P2.1、使能信号端EP-P2.2
编    写：JiangX.net
***********************************************/
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL ;

sbit rs = P1^0;   //RS
sbit rw = P1^1;   //读写选择RW
sbit ep = P1^2;   //使能信号E

BYTE code dis1[] = {"Welcome!"};
BYTE code dis2[] = {"Digital-Toy"};


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
    mdelay(15);      
lcd_wcmd(0x38);    //设定LCD为16*2显示，5*7点阵，8位数据接口
mdelay(11);
lcd_wcmd(0x0c);    //开显示，不显示光标
mdelay(11);
lcd_wcmd(0x06);    //显示光标自动右移，整屏不移动
mdelay(11);
lcd_wcmd(0x01);    //显示清屏
mdelay(11);
}

/*****主函数*****/
void main()
{
  BYTE i=0;
  char pos=0;

  lcd_init();    // 初始化LCD   
  mdelay(200);

again: 
  pos=0; 
  lcd_start(0);    // 设置显示位置为第一行的第5个字符
  mdelay(11);
  while(pos<15){
     lcd_data(i); // 显示第二行字符
     i++;pos++;
     mdelay(11);
  }   

  pos=0;
  lcd_start(0x40);   // 设置显示位置为第二行第一个字符
  while(pos<15){
     lcd_data(i); // 显示第二行字符
      i++;pos++;  
     mdelay(11);
  }   
  
  
  mdelay(255);
  mdelay(255);  
  mdelay(255);
  mdelay(255);
  mdelay(255);
  
  goto again;
}


