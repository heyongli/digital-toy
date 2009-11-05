
#include "ATtiny13prog.h"
//具体的使用说明及内部的定义请见上面的头文件。

#define overtmval 80//×０.２５秒＝２０秒

UCHAR overtm = overtmval;
UCHAR tmloval;
UCHAR temp =0;

SIGNAL(SIG_INTERRUPT0)
{
	UCHAR bak,bak2;
	bak = TCNT1;
	bak2 = OCR1C /2;
	setrx;
	clrint0;
	if(bak >= bak2)
	{
		OCR1A = bak - bak2;
	}
	else
	{
		OCR1A = bak + bak2;
	}
	rxstep = 0;
}

SIGNAL(SIG_OUTPUT_COMPARE1A)//这个中断主要用于进行输入检测
{
	static UCHAR  rxmsk = 1;
	static UCHAR temp = 0;//增加一级缓冲用的移位单元。
	if(testrx)
	{
		rxstep ++;
		switch(rxstep)
		{
			case 1:
			temp = 0;//rxval = 0;
			rxmsk = 1;//在此处接收到的是启动位
			if(testrxd)
			{//此处检测出启动位错误，自动恢复启动检测状态。
				clrintf;
				clrrx;
				setint0;
				break;
			}
			if(testrxok)
				seterr;
			clrrxok;
			break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			if(testrxd)
			{
				temp |= rxmsk;
//				rxval |= rxmsk;
			}
			rxmsk <<= 1;
			break;
			case 10:
			setrxok;
			rxval = temp;
			default:
			clrintf;
			clrrx;
			setint0;
		}
	}
}

INTERRUPT(SIG_OUTPUT_COMPARE1B)//这个中断主要用来进行输出
{
	static unsigned int timehi = 25;
	static UCHAR  timelo = 100;
	static UCHAR txmask = 1;
	if(testtx)
	{
		txstep ++;
		switch(txstep)
		{
			case 1:
			clr1bout;//发送启动位
			txmask = 1;
			break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			if(txval & txmask)
			{
				set1bout;
			}
			else
			{
				clr1bout;
			}
			txmask <<= 1;
			break;
			case 10:
			set1bout;//发送停止位
			break;
			default:
			clrtx;
		}
	}
	wdt_reset();
	if(testtx || testrx || testt0ie)
	{
		setsta;
		timehi = 25;
		timelo = tmloval;
	}
	else
	{
		if(!(tstint0))//如果在进行波特率检测过程中则不要自动恢复。 || testt0ie
		{
			clrintf;
			NOP;
//			setsta;
			setint0;//这里是为了防止程序被外部影响关闭了中断造成接收不正常所加的。
		}
		if(!(timelo--))
		{
			timelo = tmloval;
//			set01s;
			if(!(timehi--))
			{
				timehi = 25;
				if(testpro)
				{
					if(!(overtm--))
					{
						outprog();
					}
				}
				if(!teststa)
				{
					clrsta;
				}
				else
				{
					setsta;
				}
			}
		}
	}
}

SIGNAL(SIG_OVERFLOW0)
{
	seterr;
	TCCR0 = 0;//时钟1关闭。
	clrt0ie;//关闭时钟0溢出中断
	//为了加快检测速度，在此采用强制拉高引脚的方法来退出检测部分。
//	TCCR1A = tc1aval;
	TCCR1B = tc1bval;
	PORTB |= (1 << rxd);
	DDRB |= (1 << rxd);
}


/*SIGNAL(SIG_OVERFLOW1)
{
	step ++;
}*/



void init(void)
{
	//在此进行引脚配置
	PORTA 	= 	paval;
	DDRA	=	daval;//
	PORTB	=	pbval;//
	DDRB 	= 	dbval;//
	tmloval =	100;
//以下开始对定时器进行设置
	TIFR |= 0B01000100;//因为这条指令虽然能够清除标记，但同时也会在下条指令时引起中断。
	OCR1A = 0;
	OCR1B = 0;//rxtime /2 - 5;//因为程序采用了8M的频率，所以正常运行的时候加上了八分频。
	OCR1C = txtimeval;
	TCCR1A = tc1aval;
	TCCR1B = tc1bval;
	TCNT1 = 0;
	clrt0ie;
	sett1ae;
	sett1be;
	setby;
	setint0;
//	sett1ie;//打开时钟1溢出中断
//注意在此打开看门狗并且设置时间为16毫秒
	wdt_enable(WDTO_15MS);//在通讯检测过程中对看门狗进行复位。
	sei();
//	wdt_reset();
}


UCHAR receive(void)//这是用来接收的部分，也可以自己在主程序在来接收
{
	while(!testrxok);
	clrrxok;
	return(rxval);
}

void send(UCHAR sval)
{
	while(testtx);//等待上一个发送的数据完成。
//	sett1ae;
	txval = sval;
	txstep = 0;
	settx;
}

void inprog(void)
{
	UCHAR i;
	if(!(testpro))
	{
		DDRA  = pdaval;
		PORTA = ppaval;//电源上电，复位置低。
		NOP;
		for(i = 0; i<4;i++)
		{
			setsci;
			NOP;
			clrsci;
		}
		setHV;//打开12V高压！
		_delay_us(2);
		sdoin;//sdo变为输入引脚。
		PORTA |= (1 << sdo);//打开SDO的上拉功能。
		_delay_ms(50);//加长延时等待操作时间为50毫秒。
		overtm = overtmval;
		setpro;//已经进入了编程状态。
	}
}

void outprog(void)
{
	if(testpro)
	{
		clrsci;
		NOP;
		DDRA &= ~(1 << HVout);
		PORTA  &= ~(1 << HVout);
		_delay_us(1);
//		clrvcc;
//		_delay_us(10);
		PORTA 	= 	paval;
		DDRA	=	daval;//
		clrpro;//到此已经退出了编程状态。
	}
}

UCHAR ioopt(UCHAR sdival,UCHAR siival)//在这里对外部所要编程的芯片进行操作，操作时序也在这里
{//在测试在发现如果编程中数据不变的话，sdo会在下个字节中返回上个sdi的内容。
	UCHAR sdoval = 0;
	UCHAR step = 0;
	UCHAR inmask = 0x80,outmask = 0x80;
	if(!(testpro))
	{
		inprog();
//		_delay_ms(100);//额外再延长１００毫秒的时间，以使编程信号稳定。
	}
	overtm = overtmval;
	for(step = 0; step < 11;step++)
	{
		switch(step)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			if(siival & outmask)
			{
				setsii;
			}
			else
			{
				clrsii;
			}
			if(sdival & outmask)
			{
				setsdi;
			}
			else
			{
				clrsdi;
			}
			outmask >>= 1;
			break;
			default:
			clrsdi;
			clrsci;
		}
		_delay_us(1);
		setsci;//上升沿锁存数据
		_delay_us(1);
		clrsci;//下降沿的时候输出稳定，准备读取输出数据
		if(inmask)
		{
			if(testsdo)//这里是读取数据
			{
				sdoval |= inmask;
			}
			inmask >>= 1;//由于在右移过程中高位会写0,所以以后再移位过程中对输入的信号无影响。
		}
	}
	return(sdoval);
}

UCHAR clrchip(void)
{
	ioopt(0x80,0x4c);
	ioopt(0x0,0x64);
	ioopt(0x0,0x6c);
	while(!(testsdo));
	return(ioopt(0x0,0x4c));
//	send(0x0);
//	outprog();
}

UCHAR readfl(void)
{
	ioopt(0x4,0x4c);
	ioopt(0x0,0x68);
	return(ioopt(0x0,0x6c));
//	outprog();
}

UCHAR readfh(void)
{
	ioopt(0x4,0x4c);
	ioopt(0x0,0x7a);
	return(ioopt(0x0,0x7e));
//	outprog();
}

UCHAR readlb(void)
{
	ioopt(0x4,0x4c);
	ioopt(0x0,0x78);
	return(ioopt(0x0,0x7c));
//	outprog();
}

void readsb(void)
{
	UCHAR i;
	ioopt(0x8,0x4c);
	for(i = 0 ;i < 3;i++)
	{
		ioopt(i,0x0c);
		ioopt(0x0,0x68);
		send(ioopt(0x0,0x6c));
	}
//	outprog();
}

UCHAR readcb(void)
{
	ioopt(0x8,0x4c);
	ioopt(0x0,0x0c);
	ioopt(0x0,0x78);
	return(ioopt(0x0,0x7c));
//	outprog();
}


UCHAR writefl(UCHAR sdival)
{
	ioopt(0x40,0x4c);
	ioopt(sdival,0x2c);
	ioopt(0x0,0x64);
	_delay_ms(5);
	temp = ioopt(0x0,0x6c);
	while(!(testsdo));
	return(temp);
//	outprog();
//	send(0);	
}

UCHAR writefh(UCHAR sdival)
{
	ioopt(0x40,0x4c);
	ioopt(sdival & 0x1f,0x2c);
	ioopt(0x0,0x74);
	_delay_ms(5);
	temp = ioopt(0x0,0x7c);
	while(!(testsdo));
	return(temp);
//	outprog();
//	send(0);	
}

UCHAR writelb(UCHAR sdival)
{
	ioopt(0x20,0x4c);
	ioopt(sdival & 0x3,0x2c);
	ioopt(0x0,0x64);
	temp = ioopt(0x0,0x6c);
	while(!(testsdo));
	return(temp);
//	outprog();
//	send(0);	
}

UCHAR autounld(void)//自动解锁功能
{
	UCHAR readval;
	readval = readlb();//先读取锁定位
	if(~(readval | 0xfc))
	{//如果芯片被锁定则要先对芯片进行擦除操作。
		clrchip();
	}
	//写入熔丝位的低位数据
	if(readfl() != 0b01101010)
	{
		writefl(0b01101010);
	}
	//写入熔丝位的高位数据
	if(readfh() != 0xff)
	{//将熔丝位恢复出厂值
		writefh(0xff);
	}
	outprog();
	return(0);
}

void option(void)
{
	UCHAR sdival;
	UCHAR siival;
	UCHAR sdoval;
	sdival = receive();
	siival = receive();
	//以下将接收到的数据送到编程芯片中去，同时将返回的数据读回来。
	if(siival < 0x80)
	{
		sdoval = ioopt(sdival,siival);
		send(sdoval);
	}
	else
	{//如果指令字节最高位为1表示要进行特殊操作功能。
		switch(siival)
		{
			case 0x81://进编程状态
			inprog();
			send(sign);
			break;
			case 0x82://退出编程状态
			outprog();
			send(sign);
			break;
			case 0x83://返回当前状态
			send(sign);
			break;
			case 0x84://直接控制端口A的方向
			send(DDRA);
			DDRA = sdival;
			break;
			case 0x85://直接控制端口A的数据
			send(PORTA);
			PORTA = sdival;
			break;
			case 0x86://
			send(PINA);
			break;
			case 0x87://
			send (PORTA);
			break;
			case 0x88://
			send(DDRA);
			break;
			case 0x90:
			send(readfl());
			break;
			case 0x91:
			send(readfh());
			break;
			case 0x92:
			send(readlb());
			break;
			case 0x93:
			send(readcb());
			break;
			case 0x94:
			readsb();
			break;
			case 0xa0:
			send(writefl(sdival));
			break;
			case 0xa1:
			send(writefh(sdival));
			break;
			case 0xa2:
			send(writelb(sdival));
			break;
			case 0xa3:
			send(clrchip());
			break;
			case 0xa4:
			send(autounld());//自动对熔丝位恢复出厂值，如果芯片被锁定则会自动执行擦除操作。
			break;
			default://如果出现非法数据则退出到重新检测通讯参数的状态。
			seterr;
		}
		if(siival != 0x81)
		{//这是保证可以进行手动测试的部分，其余的特殊操作都会将编程模式关闭，手动操作功能只能在２０秒内完成
			outprog();
		}
	}

}

void chkrx(void)
{
//	setbusy;
//	step = 1;
	//在引自以下部分开始对外部的串行输入信号进行检测。
	//在自动检测完成后程序会将检测出的参数发送给上位机，如果未能正常检测，则不会发送数据。
	int val = ftime;//2500;
	TCCR0 = 0;
	TIFR |= (1 << TOV0);
	TCNT0 = T0val;
	clrint0;
	clrerr;
	sett0ie;//打开时钟0溢出中断
	if(testrxd)
	{
		while(testrxd);
		TCCR0 = TCCR0val;
		TCCR1B = 0;
		TCNT1 = 0;
		wdt_reset();
		while(!(testrxd));
		TCCR0 = 0;//时钟0关闭
		TCCR1B = tc1bval;
		wdt_reset();
		if(testerr)
		{
			DDRB &= ~(1 << rxd);
			rxtime = txtimeval;
		}
		else
		{
			if(TCNT0 > mintm)
			{
				rxtime = TCNT0;
				temp = 0;
				while(val > 0)//芯片没有乘除法指令，所以用减法来代替
				{
					temp ++;
					val -= rxtime;
				}
				tmloval = temp;
				send(rxtime);
			}
			else
			{
				seterr;
				rxtime = txtimeval;
			}
		}
		OCR1C = rxtime;
	}
	else
	{
		seterr;
	}
	clrintf;
	clrt0ie;//关闭定时0中断
	setint0;
	if(!(testerr))//如果通讯自动校正波特率成功的话，则进行一次通讯试验。以保证通讯正确。
	{
		send(receive());
		if( rxval != 0x55)
		{
//			send(0xff);//通讯错误的时候发送的数据。
			while(testtx);//等待发送完成后参数恢复。
			seterr;
			rxtime = txtimeval;
			OCR1C = rxtime;
			TCNT1 = 0;
		}
	}
//	clrbusy;
}

int main(void)
{
	init();//在初始化过程中会将外部的灯关闭掉的。
	while(1)
	{
		seterr;
		while(testerr)
		{
			//以下部分代码是准备给没有通讯时候只接一个按键做自动恢复用的。
			if(!testkey)//操作键按下
			{
				_delay_ms(20);
				if(!testkey)
				{
					autounld();
					while(!testkey);
					_delay_ms(800);
				}
			}
			chkrx();
		}
		while(!(testerr))
		{
			option();
		}
	}
//	return(0);
}

