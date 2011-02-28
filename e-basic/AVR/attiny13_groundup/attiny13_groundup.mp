NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0026   011A =    282. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0013  __start
       0013  __text_start
       0032  _exit
       0033  _Delay
       003E  _LED_On
       004E  _main
       0082  push_gset1
       0085  pop_gset1
       0086  pop
       0099  lsl16
       00A0  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0002 =      2. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\icc\lib\crtAVR.o	[ crtavr.s ]
main.o	[ main.c ]
<library>	[ gpush.s, lsl16.s ]

User Global Definitions

ram_end = 0xdf
hwstk_size = 0x10

User Base Address Definitions

func_lit = 0x26
data = 0x60
eeprom:1.128

