CC = iccavr
CFLAGS =  -IC:\icc\include\ -e  -l -g 
ASFLAGS = $(CFLAGS)  -Wa-g
LFLAGS =  -O -LC:\icc\lib\ -g -bfunc_lit:0x26.0x800 -dram_end:0xdf -bdata:0x60.0xdf -dhwstk_size:16 -beeprom:1.128 -fihx_coff -S2
FILES = main.o 

attiny13_groundup:	$(FILES)
	$(CC) -o attiny13_groundup $(LFLAGS) @attiny13_groundup.lk  
main.o: C:/icc/include/iot13v.h
main.o:	Z:\dev-prj\digital-toy\AVR\attiny13_groundup\main.c
	$(CC) -c $(CFLAGS) Z:\dev-prj\digital-toy\AVR\attiny13_groundup\main.c
