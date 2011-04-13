###############################################################################
# Makefile for the project i-uC-startup
###############################################################################

## General Flags
PROJECT = i-uC-startup
MCU = atmega8
TARGET = i-uC-lib-demo.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                        -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=i-uC-lib-demo.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"Z:\home\digital-toy\Hrduino-V0\i-uC-lib\include" -I"Z:\home\digital-toy\Hrduino-V0\i-uC-lib\." 

## Objects that must be built in order to link
OBJECTS = main.o freq-meter.o uc-print.o timer0.o adc.o avr-io.o pwm.o timer.o 4wire-1602.o 74hc595.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) i-uC-lib-demo.hex i-uC-lib-demo.eep i-uC-lib-demo.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

freq-meter.o: ../freq-meter.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uc-print.o: ../uc-print.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer0.o: ../atmel/timer0.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../atmel/adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

avr-io.o: ../atmel/avr-io.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pwm.o: ../atmel/pwm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

4wire-1602.o: ../drivers/4wire-1602.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

74hc595.o: ../drivers/74hc595.c
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
	-rm -rf $(OBJECTS) i-uC-lib-demo.elf dep/* i-uC-lib-demo.hex i-uC-lib-demo.eep i-uC-lib-demo.lss i-uC-lib-demo.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

