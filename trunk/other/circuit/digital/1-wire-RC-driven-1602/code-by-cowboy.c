
//         Drive a LCD1602 with 2 wire
//===================================================
//ICC-AVR application builder : 2010-10-3 19:30:02
// Target : M16
// Crystal: 4.0000Mhz

#include <iom16v.h>
#include <macros.h>

#define Set_E PORTB|=2
#define Clr_E PORTB&=~2
#define Set_D PORTB|=1
#define Clr_D PORTB&=~1
#define Set_xy(y,x) Send(0,(y<<6)|(x&15)|0x80)

//===================================================
void init_devices(void)
{
  CLI(); //disable all interrupts
  DDRB  = 0x03;
  MCUCR = 0x00;
  GICR  = 0x00;
  SEI(); //re-enable interrupts
}

//===================================================
void Delay(unsigned int i)
{
  while(i--);
}

//===================================================
void Send(unsigned char RS, unsigned char dat)
{
  unsigned char i;
  for (i = 2; i > 0; i--)
  {
    if (dat & 0x80) Set_D; else Clr_D;
    Delay(10608);//14520us
    if (RS) Set_E;
    if (dat & 0x40) Set_D; else Clr_D;
    Delay(462);  //660us
    if (dat & 0x20) Set_D; else Clr_D;
    Delay(18);   //30us
    Set_E;
    if (dat & 0x10) Set_D; else Clr_D;
    _NOP();      //0.5us < t < 1.36us
    Clr_E;
    dat <<= 4;
  }
}

//===================================================
void init_1602(void)
{
  unsigned char i = 3;
  Clr_D;
  Clr_E;
  Delay(10608);
  do{
    Clr_D;
    Delay(462);
    Set_D;
    Set_E;
    Delay(18);
    if (i == 0) Clr_D;
    _NOP();_NOP();_NOP();
    Clr_E;
    }while(i--);
  Send(0,0x28);
  Send(0,0x01);
  Send(0,0x0f);
}

//===================================================
void Send_S(unsigned char *p)
{
  while(*p) Send(1,*p++);
}

//===================================================
void main(void)
{
  unsigned char i;
  init_devices();
  init_1602();
  
  Set_xy(0,2);
  Send_S("Hello world!");
  Set_xy(1,3);
  Send_S("I'm COWBOY.");
  for (i=0;i<255;i++) Delay(10000);
  
  Send(0,0x01);
  Set_xy(0,3);
  Send_S("Welcome to");
  Set_xy(1,1);
  Send_S("www.ourdev.cn");
  while(1);
}   
