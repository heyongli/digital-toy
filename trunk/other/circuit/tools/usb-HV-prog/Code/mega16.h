#define SET_M16_OE      PORTB |= 0x20
#define CLR_M16_OE      PORTB &= ~0x20
#define SET_M16_WR      PORTB |= 0x40
#define CLR_M16_WR      PORTB &= ~0x40
#define SET_M16_BS1     PORTB |= 0x80
#define CLR_M16_BS1     PORTB &= ~0x80
#define SET_M16_BS2     PORTA |= 0x01
#define CLR_M16_BS2     PORTA &= ~0x01
#define SET_M16_XA0     PORTD |= 0x80
#define CLR_M16_XA0     PORTD &= ~0x80
#define SET_M16_XA1     PORTB |= 0x02
#define CLR_M16_XA1     PORTB &= ~0x02
#define SET_M16_PAGE    PORTB |= 0x01
#define CLR_M16_PAGE    PORTB &= ~0x01
#define SET_M16_XTAL    PORTC |= 0x02
#define CLR_M16_XTAL    PORTC &= ~0x02
#define SET_M16_RESET   PORTC |= 0x80;DDRA &= ~0x20;PORTA &= ~0x20
#define CLR_M16_RESET   DDRA |= 0x20;PORTA &= ~0x20;PORTC &= ~0x80
#define SET_M16_Vcc     PORTA |= 0x40
#define CLR_M16_Vcc     PORTA &= ~0x40
#define SET_M16_GND     PORTA |= 0x10
#define CLR_M16_GND     PORTA &= ~0x10

#define SET_M16_D0      PORTA |= 0x80
#define CLR_M16_D0      PORTA &= ~0x80
#define SET_M16_D1      PORTC |= 0x40
#define CLR_M16_D1      PORTC &= ~0x40
#define SET_M16_D2      PORTB |= 0x08
#define CLR_M16_D2      PORTB &= ~0x08
#define SET_M16_D3      PORTB |= 0x10
#define CLR_M16_D3      PORTB &= ~0x10
#define SET_M16_D4      PORTA |= 0x02
#define CLR_M16_D4      PORTA &= ~0x02
#define SET_M16_D5      PORTD |= 0x02
#define CLR_M16_D5      PORTD &= ~0x02
#define SET_M16_D6      PORTA |= 0x04
#define CLR_M16_D6      PORTA &= ~0x04
#define SET_M16_D7      PORTA |= 0x08
#define CLR_M16_D7      PORTA &= ~0x08

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

void HVConnect_mega16(void);
uchar HVEnterProgrammingMode_mega16(void);
uchar HVWriteFuse_mega16(uchar data,uchar pos);
uchar HVReadFuse_mega16(uchar pos);
uchar HVWriteLockBit_mega16(uchar data);
uchar HVEraseChip_mega16(void);
uchar HVReadOscBit_mega16(uchar address);
uchar HVReadSignature_mega16(uchar address);
void HVWriteFlash_mega16(unsigned long prog_address,uchar dataH,uchar dataL);
void HVFlushPage_mega16(unsigned long address);
uchar HVReadFlash_mega16(unsigned long prog_address);
void HVWriteEEPROM_mega16(unsigned int prog_address,uchar data);
uchar HVReadEEPROM_mega16(unsigned int prog_address);
void HVStartFlashProgramming_mega16(void);
void HVStartEEPROMProgramming_mega16(void);
void HVEndProgramming_mega16(void);
