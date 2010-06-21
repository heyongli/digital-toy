#define SET_T2313_OE      PORTD |= 0x02
#define CLR_T2313_OE      PORTD &= ~0x02
#define SET_T2313_WR      PORTA |= 0x04
#define CLR_T2313_WR      PORTA &= ~0x04
#define SET_T2313_BS1     PORTA |= 0x08
#define CLR_T2313_BS1     PORTA &= ~0x08
#define SET_T2313_XA0     PORTA |= 0x20
#define CLR_T2313_XA0     PORTA &= ~0x20
#define SET_T2313_XA1     PORTC |= 0x10
#define CLR_T2313_XA1     PORTC &= ~0x10
#define SET_T2313_BS2     PORTC |= 0x10
#define CLR_T2313_BS2     PORTC &= ~0x10
#define SET_T2313_XTAL    PORTA |= 0x02
#define CLR_T2313_XTAL    PORTA &= ~0x02
#define SET_T2313_RESET   PORTD |= 0x01;DDRA &= ~0x80;PORTA &= ~0x80
#define CLR_T2313_RESET   DDRA |= 0x80;PORTA &= ~0x80;PORTD &= ~0x01
#define SET_T2313_Vcc     PORTA |= 0x01
#define CLR_T2313_Vcc     PORTA &= ~0x01
#define SET_T2313_GND     PORTA |= 0x40
#define CLR_T2313_GND     PORTA &= ~0x40

#define SET_T2313_D0      PORTC |= 0x20
#define CLR_T2313_D0      PORTC &= ~0x20
#define SET_T2313_D1      PORTD |= 0x80
#define CLR_T2313_D1      PORTD &= ~0x80
#define SET_T2313_D2      PORTB |= 0x80
#define CLR_T2313_D2      PORTB &= ~0x80
#define SET_T2313_D3      PORTB |= 0x40
#define CLR_T2313_D3      PORTB &= ~0x40
#define SET_T2313_D4      PORTB |= 0x20
#define CLR_T2313_D4      PORTB &= ~0x20
#define SET_T2313_D5      PORTB |= 0x04
#define CLR_T2313_D5      PORTB &= ~0x04
#define SET_T2313_D6      PORTB |= 0x02
#define CLR_T2313_D6      PORTB &= ~0x02
#define SET_T2313_D7      PORTB |= 0x01
#define CLR_T2313_D7      PORTB &= ~0x01


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

void HVConnect_tiny2313(void);
uchar HVEnterProgrammingMode_tiny2313(void);
uchar HVWriteFuse_tiny2313(uchar data,uchar pos);
uchar HVReadFuse_tiny2313(uchar pos);
uchar HVWriteLockBit_tiny2313(uchar data);
uchar HVEraseChip_tiny2313(void);
uchar HVReadOscBit_tiny2313(uchar address);
uchar HVReadSignature_tiny2313(uchar address);
void HVWriteFlash_tiny2313(unsigned long prog_address,uchar dataH,uchar dataL);
void HVFlushPage_tiny2313(unsigned long address);
uchar HVReadFlash_tiny2313(unsigned long prog_address);
void HVWriteEEPROM_tiny2313(unsigned int prog_address,uchar data);
uchar HVReadEEPROM_tiny2313(unsigned int prog_address);
void HVStartFlashProgramming_tiny2313(void);
void HVStartEEPROMProgramming_tiny2313(void);
void HVEndProgramming_tiny2313(void);
