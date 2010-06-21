#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "HVProg.h"
#include "mega8.h"
#include "mega16.h"
#include "tiny2313.h"
#include "tiny26.h"
#include "s5x.h"
#include "mega8515.h"
#include "common.h"

extern uchar chip_id;

void HWSwitch(void){
	switch(chip_id){
		
	case 20 : // mega8/48/88/168	
		HVConnect = HVConnect_mega8;
		HVEnterProgrammingMode = HVEnterProgrammingMode_mega8;
		HVWriteFuse = HVWriteFuse_mega8;
		HVReadFuse = HVReadFuse_mega8;
		HVWriteLockBit = HVWriteLockBit_mega8;
		HVEraseChip = HVEraseChip_mega8;
		HVReadOscBit = HVReadOscBit_mega8;
		HVReadSignature = HVReadSignature_mega8;
		HVReadFlash = HVReadFlash_mega8;
		HVWriteFlash = HVWriteFlash_mega8;
		HVFlushPage = HVFlushPage_mega8;
		HVWriteEEPROM = HVWriteEEPROM_mega8;
		HVReadEEPROM = HVReadEEPROM_mega8;
		HVStartFlashProgramming = HVStartFlashProgramming_mega8;
		HVStartEEPROMProgramming = HVStartEEPROMProgramming_mega8;
		HVEndProgramming = HVEndProgramming_mega8;
		break;
		
	case 21 :	// mega16/32/8535
		HVConnect = HVConnect_mega16;
		HVEnterProgrammingMode = HVEnterProgrammingMode_mega16;
		HVWriteFuse = HVWriteFuse_mega16;
		HVReadFuse = HVReadFuse_mega16;
		HVWriteLockBit = HVWriteLockBit_mega16;
		HVEraseChip = HVEraseChip_mega16;
		HVReadOscBit = HVReadOscBit_mega16;
		HVReadSignature = HVReadSignature_mega16;
		HVReadFlash = HVReadFlash_mega16;
		HVWriteFlash = HVWriteFlash_mega16;
		HVFlushPage = HVFlushPage_mega16;
		HVWriteEEPROM = HVWriteEEPROM_mega16;
		HVReadEEPROM = HVReadEEPROM_mega16;
		HVStartFlashProgramming = HVStartFlashProgramming_mega16;
		HVStartEEPROMProgramming = HVStartEEPROMProgramming_mega16;
		HVEndProgramming = HVEndProgramming_mega16;
		break;
	
	case 22 : // tiny2313
		HVConnect = HVConnect_tiny2313;
		HVEnterProgrammingMode = HVEnterProgrammingMode_tiny2313;
		HVWriteFuse = HVWriteFuse_tiny2313;
		HVReadFuse = HVReadFuse_tiny2313;
		HVWriteLockBit = HVWriteLockBit_tiny2313;
		HVEraseChip = HVEraseChip_tiny2313;
		HVReadOscBit = HVReadOscBit_tiny2313;
		HVReadSignature = HVReadSignature_tiny2313;
		HVReadFlash = HVReadFlash_tiny2313;
		HVWriteFlash = HVWriteFlash_tiny2313;
		HVFlushPage = HVFlushPage_tiny2313;
		HVWriteEEPROM = HVWriteEEPROM_tiny2313;
		HVReadEEPROM = HVReadEEPROM_tiny2313;
		HVStartFlashProgramming = HVStartFlashProgramming_tiny2313;
		HVStartEEPROMProgramming = HVStartEEPROMProgramming_tiny2313;
		HVEndProgramming = HVEndProgramming_tiny2313;
		break;	
		
	case 24 :	// tiny26
		HVConnect = HVConnect_tiny26;
		HVEnterProgrammingMode = HVEnterProgrammingMode_tiny26;
		HVWriteFuse = HVWriteFuse_tiny26;
		HVReadFuse = HVReadFuse_tiny26;
		HVWriteLockBit = HVWriteLockBit_tiny26;
		HVEraseChip = HVEraseChip_tiny26;
		HVReadOscBit = HVReadOscBit_tiny26;
		HVReadSignature = HVReadSignature_tiny26;
		HVReadFlash = HVReadFlash_tiny26;
		HVWriteFlash = HVWriteFlash_tiny26;
		HVFlushPage = HVFlushPage_tiny26;
		HVWriteEEPROM = HVWriteEEPROM_tiny26;
		HVReadEEPROM = HVReadEEPROM_tiny26;
		HVStartFlashProgramming = HVStartFlashProgramming_tiny26;
		HVStartEEPROMProgramming = HVStartEEPROMProgramming_tiny26;
		HVEndProgramming = HVEndProgramming_tiny26;
		break;
				
	case 25 : // s5x
		HVConnect = ispConnect_s5x;
		HVEnterProgrammingMode = ispEnterProgrammingMode_s5x;
		HVWriteFuse = ispWriteFuse_s5x;
		HVReadFuse = ispReadFuse_s5x;
		HVWriteLockBit = ispWriteLockBit_s5x;
		HVEraseChip = ispEraseChip_s5x;
		HVReadOscBit = ispReadOscBit_s5x;
		HVReadSignature = ispReadSignature_s5x;
		HVReadFlash = ispReadFlash_s5x;
		HVWriteFlash = ispWriteFlash_s5x;
		HVFlushPage = ispFlushPage_s5x;
		HVWriteEEPROM = ispWriteEEPROM_s5x;
		HVReadEEPROM = ispReadEEPROM_s5x;
		HVStartFlashProgramming = ispStartFlashProgramming_s5x;
		HVStartEEPROMProgramming = ispStartEEPROMProgramming_s5x;
		HVEndProgramming = ispEndProgramming_s5x;
		break;
		
	case 26 :	// mega8515
		HVConnect = HVConnect_mega8515;
		HVEnterProgrammingMode = HVEnterProgrammingMode_mega8515;
		HVWriteFuse = HVWriteFuse_mega8515;
		HVReadFuse = HVReadFuse_mega8515;
		HVWriteLockBit = HVWriteLockBit_mega8515;
		HVEraseChip = HVEraseChip_mega8515;
		HVReadOscBit = HVReadOscBit_mega8515;
		HVReadSignature = HVReadSignature_mega8515;
		HVReadFlash = HVReadFlash_mega8515;
		HVWriteFlash = HVWriteFlash_mega8515;
		HVFlushPage = HVFlushPage_mega8515;
		HVWriteEEPROM = HVWriteEEPROM_mega8515;
		HVReadEEPROM = HVReadEEPROM_mega8515;
		HVStartFlashProgramming = HVStartFlashProgramming_mega8515;
		HVStartEEPROMProgramming = HVStartEEPROMProgramming_mega8515;
		HVEndProgramming = HVEndProgramming_mega8515;
		break;
		
	default : // common
		HVConnect = HVConnect_common;
		HVEnterProgrammingMode = HVEnterProgrammingMode_common;
		HVWriteFuse = HVWriteFuse_common;
		HVReadFuse = HVReadFuse_common;
		HVWriteLockBit = HVWriteLockBit_common;
		HVEraseChip = HVEraseChip_common;
		HVReadOscBit = HVReadOscBit_common;
		HVReadSignature = HVReadSignature_common;
		HVReadFlash = HVReadFlash_common;
		HVWriteFlash = HVWriteFlash_common;
		HVFlushPage = HVFlushPage_common;
		HVWriteEEPROM = HVWriteEEPROM_common;
		HVReadEEPROM = HVReadEEPROM_common;
		HVStartFlashProgramming = HVStartFlashProgramming_common;
		HVStartEEPROMProgramming = HVStartEEPROMProgramming_common;
		HVEndProgramming = HVEndProgramming_common;
	}
}

void HVDisconnect(void){
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD &= 0xFC;
	DDRA = 0;
	DDRB = 0;
	DDRC = 0;
	DDRD &= 0xFC;
}
