
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

/*===============================================*/
/* 常量*/
#define F_CPU 8000000UL
#define	ftime	F_CPU/800


#define sign01s 0//10毫秒时间到
#define busy	 1//程序忙
#define error	 2//如果通讯过程中发生错误置位
#define nowtx	 3//正在进行串行输出
#define nowrx	 4//正在进行串行输入
#define rxok	 5//串行接收完成
#define sign1s	 6//时间到1秒
#define isprog	 7//已经进入编程状态

/*===============================================*/
/*功能端口定义*/

// 串行 编程端口
#define HVSPI_PORT     PORTA
#define HVSPI_PIN        PINA 
#define HVSPI_DDR       DDRA    

#define keyin	          PA7//在这里可以接一个按键，
					//当不进行通讯的时候，则按下此键自动恢复出厂值
#define testkey	(HVSPI_PIN & (1 << keyin))

//UART 通讯端口
#define  UART_PORT  PORTB
#define  UART_PIN     PINB
#define  UART_DDR   DDRB


/*HVSPI 端口引脚定义*/
#define VCC		5//连接被编程芯片的VCC
#define sci		4//连接被编程芯片的PB3
#define sdo		3//连接被编程芯片的PB2
#define sii		2//连接被编程芯片的PB1
#define  sdi		1//连接被编程芯片的PB0
#define	HVout	0//高压控制输出，高电平输出0V，低电平输出12V，输入输出5V。

#define testvcc	HVSPI_PIN & (1 << VCC)
#define setvcc		HVSPI_PORT |= (1 << VCC)
#define clrvcc		HVSPI_PORT &= ~(1 << VCC)

#define testsii		HVSPI_PIN & (1 << sii)
#define setsii		HVSPI_PORT |= (1 << sii)
#define clrsii		HVSPI_PORT &= ~(1 << sii)

#define testsci	HVSPI_PIN & (1 << sci)
#define setsci		HVSPI_PORT |= (1 << sci)
#define clrsci		HVSPI_PORT &= ~(1 << sci)


#define testsdo	HVSPI_PIN & (1 << sdo)
#define setsdo	HVSPI_PORT |= (1 << sdo)
#define clrsdo		HVSPI_PORT &= ~(1 << sdo)
#define sdoin		HVSPI_DDR &= ~(1 << sdo)

#define testsdi	HVSPI_PIN & (1 << sdi)
#define setsdi		HVSPI_PORT |= (1 << sdi)
#define clrsdi		HVSPI_PORT &= ~(1 << sdi)



#define testHV	(~(HVSPI_PORT & (1 << HVout)))
#define clrHV		(HVSPI_PORT |= (1 << HVout))
#define setHV		(HVSPI_PORT &= ~(1 << HVout))


/*UART 端口功能定义*/
#define rxd		6//串行接收端子
#define txd		3//串行发送端子。
#define sta		4//工作状态指示。

#define teststa	(UART_PIN & (1 << sta))
#define clrsta		(UART_PORT |= (1 << sta))
#define setsta		(UART_PORT &= ~(1 << sta))
#define testrxd	(UART_PIN & (1 << rxd))
/*===============================================*/
/*初始化值*/
#define SPI_PORT_INIT			0B11000001//
#define SPI_PORT_INIT_POWER   	0B11100001//供电，复位为0V，其余引脚置为0
#define SPI_DDR_INIT				0B00111111//暂时测试过程中将所有的A口置为输入,0B00001111
#define SPI_DDR_INIT_POWER		0B00111111

#define UART_PORT_INIT	0B11101111//B口打开上拉
#define UART_DDR_INIT	0B00011000//初始化的时候先将所有的B口置为输入


/*timer resource , time 1*/
#define TMC_PRESCALER        0B0100    //  8M /8, 8分频
#define TMC_CTC		          0B10000000
#define TMC_PRESCALER_RESET            0B01000000

#define tc1aval	0B00110100;//当发生定时1匹配B的时候，输出置位。在此直接进行一次强制匹配。
#define tc1bval	TMC_CTC|TMC_PRESCALER_RESET|TMC_PRESCALER




#define T0val	0//在此准备让定时器0。
#define TCCR0val	0B00001011//在此复位预分频器，并且分频为64.这要求上位机发送的值是0X80
#define txtimeval	100//在出现错误的时候自动将内部的计数定为此值。
#define mintm		30//定义最小的接收时间周期，也是定义最快的通讯速度。





/*===============================================*/
/*program flag*/
#define testpro	sign & (1 << isprog)
#define	setpro	sign |= (1 << isprog)
#define	clrpro	sign &= ~(1 << isprog)





#define testt0ie 	(TIMSK & (1 << TOIE0))
#define sett0ie	TIMSK |= (1 << TOIE0)
#define clrt0ie	TIMSK &= ~(1 << TOIE0)

#define sett1ie	TIMSK |= (1 << TOIE1)
#define clrt1ie	TIMSK &= ~(1 << TOIE1)

#define sett1ae	TIMSK |= (1 << OCIE1A)
#define clrt1ae	TIMSK &= ~(1 << OCIE1A)

#define sett1be	TIMSK |= (1 << OCIE1B)
#define clrt1be	TIMSK &= ~(1 << OCIE1B)

#define setint0	GIMSK |= (1 << INT0)
#define clrint0	GIMSK &= ~(1 << INT0)
#define tstint0	(GIMSK & (1 << INT0))
#define clrintf		GIFR |= (1 << INTF0)


#define setby		MCUCR |= (1 << ISC01)

#define set1bout	TCCR1A |= (1 << COM1B0)
#define clr1bout	TCCR1A &= ~(1 << COM1B0)


#define UCHAR unsigned char


/*-------------------------------------------------------------------*/

#define NOP		asm("nop")

extern UCHAR volatile sign;
extern UCHAR rxtime;//此处保存的是通讯的时间长度。
extern UCHAR step ;//在程序运行过程中用来指示当前正在执行的步序，这样程序各模块之间能够相互配合工作
extern UCHAR txval,rxval,txstep,rxstep;


#define nowbusy	sign & (1 << busy)
#define setbusy	sign |= (1 << busy)
#define clrbusy sign &= ~(1 << busy)

#define test01s	(sign & ( 1 << sign01s))
#define set01s	sign |= ( 1 << sign01s)
#define clr01s	sign &= ~( 1 << sign01s)

#define testrxok	(sign & (1 << rxok))
#define setrxok	sign |= (1 << rxok)
#define clrrxok	sign &= ~(1 << rxok)


#define testerr	(sign & (1 << error))
#define seterr	sign |= (1 << error)
#define clrerr	sign &= ~(1 << error)

#define testtx	(sign & (1 << nowtx))
#define settx	sign |= (1 << nowtx)
#define clrtx	sign &= ~(1 << nowtx)

#define testrx	(sign & (1 << nowrx))
#define setrx	sign |= (1 << nowrx)
#define clrrx	sign &= ~(1 << nowrx)

void outprog(void);
//在通讯过程中，上位机设置好参数后发送0X80,如果检测成功下位机会发送一个检测出的参数，否则没有任何数据返回，上位机就要重复发送0X80；
//在接收到数据后，上位机要发送0X55,以检验通讯正确性，下位机在检测正确后会返回0X55，否则返回的值不是0X55。要重新返回发送0X80.
//此程序采用了自动设置波特率的方式，并且实现了全双工通讯，实际检测实验在4800bps--19200bps连续收发完全无误！
//测试过程中传送的数据是15KBytes。
//控制方式改变如下：
//主要控制放在定时器1中，且定时器1不停止，看门狗的复位也放在这里面。
//检测串行波特率的时候，用定时器0来定时。其余时候定时器0不工作。
//将周期时间放在OCR1C中
//不用移位，也就是不用USI功能，移位由软件来实现，输入放在INT0上，输出放在OC1B处，保证输出时钟准确。
//由OCR1B来输出串行输出部分，当发生匹配的时候自动改变输出电平，同时引发中断，在中断中设置下次匹配时的输出电平。
//并且能够检测出是否发送完成，形成相应的标志，OCR1A设置在接收到启动位时自动设置。看门狗可以在这里清除。
//OCR1A不对外输出，主要用来检测外部的输入。但是启动位由程序主动来检测，当检测到启动位后会对OCR1A进行自动计算设置，
//单片机的工作时钟进行提高，一般要达到4M以上，现在决定采用8M。
//状态指示部分由OCR1B来实现，如果没有串行通讯就闪烁，否则就常亮。并且闪亮的速度与实际同步后的速度相成比例。
//实际工作过程中，上位机发送0x80后，单片机会进行检测并在同步后向上位机发送一个内部检测出来的值。
//我自己在试验中发现，采用内部振荡器时，实际的振荡频率比标称值大，所以所测出的值会大于我们理论计算出来的值。


