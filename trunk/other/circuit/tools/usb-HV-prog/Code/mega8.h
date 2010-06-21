#define SET_M8_OE      PORTB |= 0x10
#define CLR_M8_OE      PORTB &= ~0x10
#define SET_M8_WR      PORTA |= 0x02
#define CLR_M8_WR      PORTA &= ~0x02
#define SET_M8_BS1     PORTD |= 0x02
#define CLR_M8_BS1     PORTD &= ~0x02
#define SET_M8_BS2     PORTB |= 0x04
#define CLR_M8_BS2     PORTB &= ~0x04
#define SET_M8_XA0     PORTA |= 0x10
#define CLR_M8_XA0     PORTA &= ~0x10
#define SET_M8_XA1     PORTC |= 0x04
#define CLR_M8_XA1     PORTC &= ~0x04
#define SET_M8_PAGE    PORTC |= 0x02
#define CLR_M8_PAGE    PORTC &= ~0x02
#define SET_M8_XTAL    PORTA |= 0x20
#define CLR_M8_XTAL    PORTA &= ~0x20
#define SET_M8_RESET   PORTD |= 0x01;DDRA &= ~0x80;PORTA &= ~0x80
#define CLR_M8_RESET   DDRA |= 0x80;PORTA &= ~0x80;PORTD &= ~0x01
#define SET_M8_Vcc     PORTA |= 0x04
#define CLR_M8_Vcc     PORTA &= ~0x04
#define SET_M8_GND     PORTA |= 0x08
#define CLR_M8_GND     PORTA &= ~0x08

#define SET_M8_D0      PORTA |= 0x40
#define CLR_M8_D0      PORTA &= ~0x40
#define SET_M8_D1      PORTA |= 0x01
#define CLR_M8_D1      PORTA &= ~0x01
#define SET_M8_D2      PORTB |= 0x01
#define CLR_M8_D2      PORTB &= ~0x01
#define SET_M8_D3      PORTB |= 0x02
#define CLR_M8_D3      PORTB &= ~0x02
#define SET_M8_D4      PORTC |= 0x08
#define CLR_M8_D4      PORTC &= ~0x08
#define SET_M8_D5      PORTC |= 0x10
#define CLR_M8_D5      PORTC &= ~0x10
#define SET_M8_D6      PORTB |= 0x40
#define CLR_M8_D6      PORTB &= ~0x40
#define SET_M8_D7      PORTB |= 0x20
#define CLR_M8_D7      PORTB &= ~0x20

#ifndef uchar
#define	uchar	unsigned char
#endif

#define ERASECHIP     0x80
#define WRITEFUSE     0x40
#define WRITELOCKBIT  0x20
#define WRITEFLASH    0x10
#define WRITEEEPROM   0x11
#define READBYTE      0x08
#define READBIT       0x04
#define READFLASH     0x02
#define READEEPROM    0x03

void HVConnect_mega8(void);
uchar HVEnterProgrammingMode_mega8(void);
uchar HVWriteFuse_mega8(uchar data,uchar pos);
uchar HVReadFuse_mega8(uchar pos);
uchar HVWriteLockBit_mega8(uchar data);
uchar HVEraseChip_mega8(void);
uchar HVReadOscBit_mega8(uchar address);
uchar HVReadSignature_mega8(uchar address);
void HVWriteFlash_mega8(unsigned long prog_address,uchar dataH,uchar dataL);
void HVFlushPage_mega8(unsigned long address);
uchar HVReadFlash_mega8(unsigned long prog_address);
void HVWriteEEPROM_mega8(unsigned int prog_address,uchar data);
uchar HVReadEEPROM_mega8(unsigned int prog_address);
void HVStartFlashProgramming_mega8(void);
void HVStartEEPROMProgramming_mega8(void);
void HVEndProgramming_mega8(void);

