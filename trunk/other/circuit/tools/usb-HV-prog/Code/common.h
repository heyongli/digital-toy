#define SET_COM_OE      PORTB |= 0x40
#define CLR_COM_OE      PORTB &= ~0x40
#define SET_COM_WR      PORTB |= 0x20
#define CLR_COM_WR      PORTB &= ~0x20
#define SET_COM_BS1     PORTB |= 0x04
#define CLR_COM_BS1     PORTB &= ~0x04
#define SET_COM_BS2     PORTD |= 0x80
#define CLR_COM_BS2     PORTD &= ~0x80
#define SET_COM_XA0     PORTB |= 0x02
#define CLR_COM_XA0     PORTB &= ~0x02
#define SET_COM_XA1     PORTB |= 0x01
#define CLR_COM_XA1     PORTB &= ~0x01
#define SET_COM_PAGE    PORTA |= 0x01
#define CLR_COM_PAGE    PORTA &= ~0x01
#define SET_COM_XTAL    PORTC |= 0x20
#define CLR_COM_XTAL    PORTC &= ~0x20
#define SET_COM_RESET   PORTC |= 0x80;DDRA &= ~0x20;PORTA &= ~0x20
#define CLR_COM_RESET   DDRA |= 0x20;PORTA &= ~0x20;PORTC &= ~0x80
#define SET_COM_Vcc     PORTA |= 0x40
#define CLR_COM_Vcc     PORTA &= ~0x40
#define SET_COM_GND     PORTC |= 0x10
#define CLR_COM_GND     PORTC &= ~0x10

#define SET_COM_D0      PORTA |= 0x80
#define CLR_COM_D0      PORTA &= ~0x80
#define SET_COM_D1      PORTC |= 0x40
#define CLR_COM_D1      PORTC &= ~0x40
#define SET_COM_D2      PORTB |= 0x08
#define CLR_COM_D2      PORTB &= ~0x08
#define SET_COM_D3      PORTB |= 0x10
#define CLR_COM_D3      PORTB &= ~0x10
#define SET_COM_D4      PORTA |= 0x02
#define CLR_COM_D4      PORTA &= ~0x02
#define SET_COM_D5      PORTD |= 0x02
#define CLR_COM_D5      PORTD &= ~0x02
#define SET_COM_D6      PORTA |= 0x04
#define CLR_COM_D6      PORTA &= ~0x04
#define SET_COM_D7      PORTA |= 0x08
#define CLR_COM_D7      PORTA &= ~0x08

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

void HVConnect_common(void);
uchar HVEnterProgrammingMode_common(void);
uchar HVWriteFuse_common(uchar data,uchar pos);
uchar HVReadFuse_common(uchar pos);
uchar HVWriteLockBit_common(uchar data);
uchar HVEraseChip_common(void);
uchar HVReadOscBit_common(uchar address);
uchar HVReadSignature_common(uchar address);
void HVWriteFlash_common(unsigned long prog_address,uchar dataH,uchar dataL);
void HVFlushPage_common(unsigned long address);
uchar HVReadFlash_common(unsigned long prog_address);
void HVWriteEEPROM_common(unsigned int prog_address,uchar data);
uchar HVReadEEPROM_common(unsigned int prog_address);
void HVStartFlashProgramming_common(void);
void HVStartEEPROMProgramming_common(void);
void HVEndProgramming_common(void);

