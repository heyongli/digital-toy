###############################################################################
# Makefile for the project i-uC-startup
###############################################################################

## General Flags
PROJECT = i-uC-startup
MCU = atmega88
TARGET = i-uC-Mega88-startup.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99           -DF_CPU=10000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=i-uC-Mega88-startup.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = 4wire-1602.o 74hc595.o adc.o avrio.o charger.o led.o main.o pwm.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) i-uC-Mega88-startup.hex i-uC-Mega88-startup.eep i-uC-Mega88-startup.lss size

## Compile
4wire-1602.o: ../4wire-1602.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

74hc595.o: ../74hc595.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

avrio.o: ../avrio.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

charger.o: ../charger.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

led.o: ../led.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pwm.o: ../pwm.c
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
	-rm -rf $(OBJECTS) i-uC-Mega88-startup.elf dep/* i-uC-Mega88-startup.hex i-uC-Mega88-startup.eep i-uC-Mega88-startup.lss i-uC-Mega88-startup.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

