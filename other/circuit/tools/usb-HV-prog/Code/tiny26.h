#define SET_T26_OE      PORTA |= 0x08
#define CLR_T26_OE      PORTA &= ~0x08
#define SET_T26_WR      PORTA |= 0x80
#define CLR_T26_WR      PORTA &= ~0x80
#define SET_T26_BS1     PORTB |= 0x10
#define CLR_T26_BS1     PORTB &= ~0x10
#define SET_T26_BS2     PORTB |= 0x08
#define CLR_T26_BS2     PORTB &= ~0x08
#define SET_T26_XA0     PORTC |= 0x40
#define CLR_T26_XA0     PORTC &= ~0x40
#define SET_T26_XA1     PORTB |= 0x08
#define CLR_T26_XA1     PORTB &= ~0x08
#define SET_T26_PAGE    PORTB |= 0x10
#define CLR_T26_PAGE    PORTB &= ~0x10
#define SET_T26_XTAL    PORTA |= 0x04
#define CLR_T26_XTAL    PORTA &= ~0x04
#define SET_T26_RESET   PORTC |= 0x01;DDRA &= ~0x40;PORTA &= ~0x40
#define CLR_T26_RESET   DDRA |= 0x40;PORTA &= ~0x40;PORTC &= ~0x01
#define SET_T26_Vcc     PORTA |= 0x02
#define CLR_T26_Vcc     PORTA &= ~0x02
#define SET_T26_GND     PORTD |= 0x02
#define CLR_T26_GND     PORTD &= ~0x02

#define SET_T26_D0      PORTA |= 0x01
#define CLR_T26_D0      PORTA &= ~0x01
#define SET_T26_D1      PORTB |= 0x01
#define CLR_T26_D1      PORTB &= ~0x01
#define SET_T26_D2      PORTB |= 0x02
#define CLR_T26_D2      PORTB &= ~0x02
#define SET_T26_D3      PORTB |= 0x04
#define CLR_T26_D3      PORTB &= ~0x04
#define SET_T26_D4      PORTB |= 0x80
#define CLR_T26_D4      PORTB &= ~0x80
#define SET_T26_D5      PORTD |= 0x80
#define CLR_T26_D5      PORTD &= ~0x80
#define SET_T26_D6      PORTC |= 0x20
#define CLR_T26_D6      PORTC &= ~0x20
#define SET_T26_D7      PORTC |= 0x10
#define CLR_T26_D7      PORTC &= ~0x10


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

void HVConnect_tiny26(void);
uchar HVEnterProgrammingMode_tiny26(void);
uchar HVWriteFuse_tiny26(uchar data,uchar pos);
uchar HVReadFuse_tiny26(uchar pos);
uchar HVWriteLockBit_tiny26(uchar data);
uchar HVEraseChip_tiny26(void);
uchar HVReadOscBit_tiny26(uchar address);
uchar HVReadSignature_tiny26(uchar address);
void HVWriteFlash_tiny26(unsigned long prog_address,uchar dataH,uchar dataL);
void HVFlushPage_tiny26(unsigned long address);
uchar HVReadFlash_tiny26(unsigned long prog_address);
void HVWriteEEPROM_tiny26(unsigned int prog_address,uchar data);
uchar HVReadEEPROM_tiny26(unsigned int prog_address);
void HVStartFlashProgramming_tiny26(void);
void HVStartEEPROMProgramming_tiny26(void);
void HVEndProgramming_tiny26(void);
