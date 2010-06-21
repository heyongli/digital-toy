#define SET_S5x_MOSI      PORTD |= 0x02
#define CLR_S5x_MOSI      PORTD &= ~0x02
#define SET_S5x_SCK     	PORTA |= 0x08
#define CLR_S5x_SCK    	 	PORTA &= ~0x08
#define SET_S5x_RST     	PORTA |= 0x20
#define CLR_S5x_RST     	PORTA &= ~0x20
#define SET_S5x_Vcc     	PORTA |= 0x01
#define CLR_S5x_Vcc     	PORTA &= ~0x01
#define SET_S5x_GND     	PORTB |= 0x02
#define CLR_S5x_GND     	PORTB &= ~0x02

#ifndef uchar
#define	uchar	unsigned char
#endif

void ispConnect_s5x(void);
uchar ispEnterProgrammingMode_s5x(void);
uchar ispWriteFuse_s5x(uchar data,uchar pos);
uchar ispReadFuse_s5x(uchar pos);
uchar ispWriteLockBit_s5x(uchar data);
uchar ispEraseChip_s5x(void);
uchar ispReadOscBit_s5x(uchar address);
uchar ispReadSignature_s5x(uchar address);
void ispWriteFlash_s5x(unsigned long prog_address,uchar dataH,uchar dataL);
void ispFlushPage_s5x(unsigned long address);
uchar ispReadFlash_s5x(unsigned long prog_address);
void ispWriteEEPROM_s5x(unsigned int prog_address,uchar data);
uchar ispReadEEPROM_s5x(unsigned int prog_address);
void ispStartFlashProgramming_s5x(void);
void ispStartEEPROMProgramming_s5x(void);
void ispEndProgramming_s5x(void);
