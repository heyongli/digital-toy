#define SET_M8515_OE      PORTC |= 0x04
#define CLR_M8515_OE      PORTC &= ~0x04
#define SET_M8515_WR      PORTC |= 0x02
#define CLR_M8515_WR      PORTC &= ~0x02
#define SET_M8515_BS1     PORTA |= 0x40
#define CLR_M8515_BS1     PORTA &= ~0x40
#define SET_M8515_BS2     PORTB |= 0x01
#define CLR_M8515_BS2     PORTB &= ~0x01
#define SET_M8515_XA0     PORTB |= 0x04
#define CLR_M8515_XA0     PORTB &= ~0x04
#define SET_M8515_XA1     PORTB |= 0x20
#define CLR_M8515_XA1     PORTB &= ~0x20
#define SET_M8515_PAGE    PORTB |= 0x40
#define CLR_M8515_PAGE    PORTB &= ~0x40
#define SET_M8515_XTAL    PORTD |= 0x80
#define CLR_M8515_XTAL    PORTD &= ~0x80
#define SET_M8515_RESET   PORTC |= 0x80;DDRA &= ~0x20;PORTA &= ~0x20
#define CLR_M8515_RESET   DDRA |= 0x20;PORTA &= ~0x20;PORTC &= ~0x80
#define SET_M8515_Vcc     PORTA |= 0x01
#define CLR_M8515_Vcc     PORTA &= ~0x01
#define SET_M8515_GND     PORTB |= 0x02
#define CLR_M8515_GND     PORTB &= ~0x02

#define SET_M8515_D0      PORTA |= 0x80
#define CLR_M8515_D0      PORTA &= ~0x80
#define SET_M8515_D1      PORTC |= 0x40
#define CLR_M8515_D1      PORTC &= ~0x40
#define SET_M8515_D2      PORTB |= 0x08
#define CLR_M8515_D2      PORTB &= ~0x08
#define SET_M8515_D3      PORTB |= 0x10
#define CLR_M8515_D3      PORTB &= ~0x10
#define SET_M8515_D4      PORTA |= 0x02
#define CLR_M8515_D4      PORTA &= ~0x02
#define SET_M8515_D5      PORTD |= 0x02
#define CLR_M8515_D5      PORTD &= ~0x02
#define SET_M8515_D6      PORTA |= 0x04
#define CLR_M8515_D6      PORTA &= ~0x04
#define SET_M8515_D7      PORTA |= 0x08
#define CLR_M8515_D7      PORTA &= ~0x08

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

void HVConnect_mega8515(void);
uchar HVEnterProgrammingMode_mega8515(void);
uchar HVWriteFuse_mega8515(uchar data,uchar pos);
uchar HVReadFuse_mega8515(uchar pos);
uchar HVWriteLockBit_mega8515(uchar data);
uchar HVEraseChip_mega8515(void);
uchar HVReadOscBit_mega8515(uchar address);
uchar HVReadSignature_mega8515(uchar address);
void HVWriteFlash_mega8515(unsigned long prog_address,uchar dataH,uchar dataL);
void HVFlushPage_mega8515(unsigned long address);
uchar HVReadFlash_mega8515(unsigned long prog_address);
void HVWriteEEPROM_mega8515(unsigned int prog_address,uchar data);
uchar HVReadEEPROM_mega8515(unsigned int prog_address);
void HVStartFlashProgramming_mega8515(void);
void HVStartEEPROMProgramming_mega8515(void);
void HVEndProgramming_mega8515(void);
