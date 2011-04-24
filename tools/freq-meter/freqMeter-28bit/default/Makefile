###############################################################################
# Makefile for the project xfreq-avr
###############################################################################

## General Flags
PROJECT = xfreq-avr
MCU = atmega8
TARGET = xfreq-avr.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99    -DF_CPU=10000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=xfreq-avr.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"Z:\home\digital-toy\tools\freq-meter\freqMeter-28bit\." -I"Z:\home\digital-toy\tools\freq-meter\freqMeter-28bit\..\..\..\metal-cell\include" 

## Objects that must be built in order to link
OBJECTS = freq-meter.o main.o avr-io.o adc.o 74hc595.o 4wire-1602.o timer.o uprint.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) xfreq-avr.hex xfreq-avr.eep xfreq-avr.lss size

## Compile
freq-meter.o: ../freq-meter.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

avr-io.o: ../../../../metal-cell/atmel/avr-io.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../../../../metal-cell/atmel/adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

74hc595.o: ../../../../metal-cell/drivers/74hc595.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

4wire-1602.o: ../../../../metal-cell/drivers/4wire-1602.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../../../../metal-cell/timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uprint.o: ../../../../metal-cell/uprint.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) xfreq-avr.elf dep/* xfreq-avr.hex xfreq-avr.eep xfreq-avr.lss xfreq-avr.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

