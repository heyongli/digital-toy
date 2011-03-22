//频率测量源代码:


unsigned long counter;
unsigned char countlow;
unsigned char fre[]="00.000000";
unsigned char time[]="00:00:00";

#pragma interrupt_handler timer2_ovf_isr:iv_TIMER2_OVF
void timer2_ovf_isr(void)
{
unsigned char i;
//TCCR2=0;

PORTB &= ~0X02; //关闭闸门

//读取计数值
counter = TCNT1; //读取T/C1计数值（频率值的高16位）
counter = counter*256; //计算频率值的高16位
countlow = (PIND & 0b11000000)|(PINC & 0b00111111);
//读取计数值的低8位
counter = counter + countlow; //计算频率值

PORTD |= 0X01; //74LS393清零
TCNT1 = 0;

PORTD &= ~0X01; //关闭清零
TCNT2 = 0X80; //定时时间1秒

PORTB |= 0X02; //打开闸门

//实时时钟显示
time[7]++;

fre[0]=counter /10000000+0x30;
counter=counter %10000000;
fre[1]=counter /1000000+0x30;
counter=counter %1000000;
fre[3]=counter /100000+0x30;
counter=counter %100000;
fre[4]=counter /10000+0x30;
counter=counter %10000;
fre[5]=counter /1000+0x30;
counter=counter %1000;
fre[6]=counter /100+0x30;
counter=counter %100;
fre[7]=counter /10 + 0x30;
fre[8]=counter %10 + 0x30;

LCD_write_String(0,2,fre);
LCD_draw_map(14,2,Bmp003,7,14); //显示小数点
LCD_draw_map(62,2,MHz,22,14); //显示频率单位

if ( time[7] > (9+0x30) )
{
time[6] ++;
time[7] -= 10;
}
if ( time[6] > (5+0x30) )
{
time[4] ++;
time[6] -= 6;
}
if (time[4]> (9+0x30))
{
time[3]++;
time[4]-=10;
}
if (time[3] > (5+0x30))
{
time[1]++;
time[3]-=6;
}
if (time[1]>(9+0x30))
{
time[0]++;
time[1]-=10;
}
LCD_write_String(2,4,time);
LCD_draw_map(16,4,Bmp002,7,14); //显示“：”字符
LCD_draw_map(37,4,Bmp002,7,14);

}

/******************************************************************************/
void main(void)
{
unsigned char i;
OSCCAL=0xA3; // 8M系统内部时钟校准

//设置MCU的I/O口
DDRB |= LCD_RST | LCD_DC | LCD_CE | SPI_MOSI | SPI_CLK ;

DDRB |= 0x02; //计数闸门
DDRD |= 0X01; //74LS393清零
//DDRD |= 0X08; //LED
//DDRD &= ~0X20;
DDRC &= 0B11000000;
DDRD &= 0B00111111;

SPSR |= BIT(SPI2X); // 设置SPI时钟倍速
SPCR |= BIT(SPE)|BIT(MSTR); // 使能SPI接口，主机模式，4M时钟

LCD_init();
//LCD_clear(); // 清屏
LCD_draw_map(12,0,Bmp001,60,13);

ASSR = 1 <
TCNT2 = 0X80; //定时时间1秒
TCCR2 = (1<<
TIMSK |= BIT(TOIE2); //允许TC2溢出中断

TCCR1B |= BIT(CS12)|BIT(CS11)|BIT(CS10); //时钟外部T1输入，上升源驱动

SEI();

while(1)
{

}
}

