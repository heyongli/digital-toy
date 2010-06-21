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

#define NULL 0

void (*HVConnect)(void);
uchar (*HVEnterProgrammingMode)(void);
uchar (*HVWriteFuse)(uchar data,uchar pos);
uchar (*HVReadFuse)(uchar pos);
uchar (*HVWriteLockBit)(uchar data);
uchar (*HVEraseChip)(void);
uchar (*HVReadOscBit)(uchar address);
uchar (*HVReadSignature)(uchar address);
void (*HVWriteFlash)(unsigned long prog_address,uchar dataH,uchar dataL);
void (*HVFlushPage)(unsigned long address);
uchar (*HVReadFlash)(unsigned long int prog_address);
void (*HVWriteEEPROM)(unsigned int prog_address,uchar data);
uchar (*HVReadEEPROM)(unsigned int prog_address);
void (*HVStartFlashProgramming)(void);
void (*HVStartEEPROMProgramming)(void);
void (*HVEndProgramming)(void);

void HWSwitch();
void HVDisconnect(void);
