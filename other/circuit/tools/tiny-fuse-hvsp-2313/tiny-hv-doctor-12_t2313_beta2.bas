'------------------------------------------------------------------------------'
'      Program: Attiny fusebit HV-doctor       autor: Manekinen                '
'   kompilator: bascom 1.11.9.0                 data: 09.05.2010               '
'           strona domowa projektu: http://diy.elektroda.eu/                   '
'               WY£¥CZNIE DO U¯YTKU NIEKOMERCYJNEGO                            '
'------------------------------------------------------------------------------'

$regfile = "attiny2313.dat"
$crystal = 4000000
Config Watchdog = 16

Ddra = &B11
Ddrb = &B00000011
Ddrd = &B1111111
Porta = &B00                                                'sii,sdo
Portb = &B11111100                                          'isp,isp,isp,j1,j2,but,led,res
Portd = &B0010000                                           'led,en2,pow,sci,sdi,sci_t15

Sdi Alias Portd.2                                           'serial data input
Sii Alias Porta.1                                           'serial instruction input
Sdo Alias Pina.0                                            'serial data output
Sci Alias Portd.3                                           'serial clock input
Sci_t15 Alias Portd.1                                       'serial clock input for t15
Enable_pin_2 Alias Portd.5                                  'enable_pin(2) for socked2
Pow Alias Portd.4                                           '+5V power
Res Alias Portb.0                                           '+12V reset, enable = 1

Bad_led Alias Portd.6                                       'red led
Ok_led Alias Portb.1                                        'green led
Can_erase Alias Pinb.4                                      'allow erase jumper
No_sign Alias Pinb.2                                        'default fusebits if erased signature
Button Alias Pinb.3                                         'start button

Dim Sdi_data As Byte
Dim Sii_data As Byte
Dim Sdo_data As Byte
Dim Licznik As Byte
Dim Licznik2 As Byte
Dim Proby As Byte
Dim Signature(2) As Byte
Dim Fusebit(3) As Byte
Dim Lock As Bit
Dim Fuse_nr As Byte
Dim Ok As Bit

Declare Sub _init
Declare Sub _read_sig
Declare Sub _read_lock
Declare Sub _verify_fuse
Declare Sub _write_fuse
Declare Sub _chip_erase
Declare Sub _send
Declare Sub _receive
Declare Sub _clock
Declare Sub _wait_sdo
Declare Sub _the_end
Declare Sub _wait

'###############################################################################

Bitwait Button , Reset

_init

_read_sig

Select Case Signature(1)

   Case &H90

      Select Case Signature(2)

         Case &H04
         't11
         Fusebit(1) = &HFC
         Fuse_nr = 1

         Case &H05
         't12
         Fusebit(1) = &H52
         Fuse_nr = 1

         Case &H06
         't15
         Fusebit(1) = &H5C
         Fuse_nr = 1

         Case &H07
         't13
         Fusebit(1) = &H6A
         Fusebit(2) = &HFF
         Fuse_nr = 2

      End Select

   Case &H91

      Select Case Signature(2)

         Case &H02
         'at90s2313/2343
         Fusebit(1) = &HDE
         Fuse_nr = 1

         Case &H06
         'tiny22
         Fusebit(1) = &HDE
         Fuse_nr = 1

         Case Else

            't24,44,84,25,45,85,261,461,861
            Fusebit(1) = &H62
            Fusebit(2) = &HDF
            Fusebit(3) = &HFF
            Fuse_nr = 3

      End Select

   Case &H92 To &H93

      't24,44,84,25,45,85,261,461,861
      Fusebit(1) = &H62
      Fusebit(2) = &HDF
      Fusebit(3) = &HFF
      Fuse_nr = 3

   Case Else
      If No_sign = 0 Then
         Fusebit(1) = 82
         Fusebit(2) = 223
         Fuse_nr = 2
      Else
         'tutaj b³¹d ¿e problem z sygnatur¹, koniec programu
         _the_end
         Set Bad_led
         _wait
         Do
         If Button = 0 Then Start Watchdog
         Loop
      End If

End Select

'pe³ny erejs jeœli u¿ytkownik u¿y³ zworki
If Can_erase = 0 Then _chip_erase

_read_lock

Do
   If Lock = 1 Then _write_fuse
   _verify_fuse

   Incr Proby
   If Proby = 10 Then Exit Do
Loop Until Ok = 1

'wyjdŸ z programowania i odetnij zasilanie
_the_end

If Ok = 1 Then
   Set Ok_led
Else
   Do
      Set Bad_led
      _wait
      Reset Bad_led
      _wait

      If Lock = 0 Then
      'tutaj b³¹d ¿e problem z lockbitami, koniec programu
          Set Ok_led
          _wait
          Reset Ok_led
          _wait

      End If

   Loop Until Button = 0
End If
_wait
Do
If Button = 0 Then Start Watchdog
Loop

'###############################################################################
'###############################################################################
_wait:
Waitms 250
Return

_init:
'Set Prog_enable pins listed in Table ## to “000”, RESET pin to “0” and Vcc to 0V
' (set low on startup)

'Apply 4.5 - 5.5V between VCC and GND. Ensure that Vcc reaches at least 1.8V within the next 20µs.
Reset Pow                                                   'w³¹czany stanem L

'Wait 20 - 60µs, and apply 11.5 - 12.5V to RESET
Waitus 40
Set Res                                                     'w³¹czany stanem H

'Keep the Prog_enable pins unchanged for at least 10µs after the High-voltage has been applied to ensure the Prog_enable Signature has been latched.
Waitus 20

'Release the Prog_enable[2] pin to avoid drive contention on the Prog_enable[2]/SDO pin.
Reset Portd.5                                               'reset enable2(t44)
Ddrd = &B1011111                                            'enable t44 jako wejœcie
Reset Porta.0                                               'reset enable2(sdo)
Ddra = &B10                                                 'SDO jako wejœcie

'Wait at least 300µs before giving any serial instructions on SDI/SII.
Waitus 500
Return

'_______________________________________________________________________________

_clock:
'jeden takt zegara trwa 2us, prawie 10 razy dluzej niz wymagane
'1us przerwy
Waitus 1
Set Sci
Set Sci_t15
'1us trwania
Waitus 1
Reset Sci
Reset Sci_t15
Return
'_______________________________________________________________________________

_wait_sdo:
'oczekujemy na gotowoœæ pacjenta, czyli SDO=1
Waitus 1
Bitwait Sdo , Set
Return
'_______________________________________________________________________________

_read_sig:

Sdi_data = &B00001000
Sii_data = &B01001100
_send

'(pomijam pierwszy (00) bajt sygnatury - producenta, zawsze bêdzie taki sam
Sdi_data = &B00000000                                       '00 - bajt 1 sygnatury - producent
Sii_data = &B00001100
_send

Sdi_data = &B00000000
Sii_data = &B01101000
_send

Sdi_data = &B00000000
Sii_data = &B01101100
_receive

Signature(x) = Sdo_data
')

'Sdi_data = &B00001000
'Sii_data = &B01001100
'_send

Sdi_data = &B00000001                                       '01 - bajt 2 sygnatury - iloœæ pamiêci
Sii_data = &B00001100
_send

Sdi_data = &B00000000
Sii_data = &B01101000
_send

'Sdi_data = &B00000000
Sii_data = &B01101100
_receive

'Lcd Hex(sdo_data) ; " "
Signature(1) = Sdo_data

'instr1 pominiêta
Sdi_data = &B00000010                                       '10 - bajt 3 sygnatury - rodzaj procka (bajtu 00 - producenta - nie potrzebujemy)
Sii_data = &B00001100
_send

Sdi_data = &B00000000
Sii_data = &B01101000
_send

'Sdi_data = &B00000000
Sii_data = &B01101100
_receive

'Lcd Hex(sdo_data) ; " "
Signature(2) = Sdo_data

Return
'_______________________________________________________________________________

_read_lock:

Sdi_data = &B00000100
Sii_data = &B01001100
_send

Sdi_data = &B00000000
Sii_data = &B01111000
_send

'Sdi_data = &B00000000
Sii_data = &B01111100
_receive

Lock = 1
If Sdo_data.1 = 0 Or Sdo_data.0 = 0 Then Lock = 0

Return
'_______________________________________________________________________________

_write_fuse:

      'fuse-LOW
      Sdi_data = &B01000000
      Sii_data = &B01001100
      _send
      '          Fuse   LOW
      Sdi_data = Fusebit(1)
      Sii_data = &B00101100
      _send

      Sdi_data = &B00000000
      Sii_data = &B01100100
      _send

      'Sdi_data = &B00000000
      Sii_data = &B01101100
      _send

      'czekamy a¿ SDO pójdzie w górê
      _wait_sdo

   If Fuse_nr > 1 Then
      'fuse-HIGH
      'Sdi_data = &B01000000
      'Sii_data = &B01001100
      '_send
      '          Fuse  HIGH
      Sdi_data = Fusebit(2)
      Sii_data = &B00101100
      _send

      Sdi_data = &B00000000
      Sii_data = &B01110100
      _send

      'Sdi_data = &B00000000
      Sii_data = &B01111100
      _send

      _wait_sdo
   End If

   If Fuse_nr = 3 Then
      'fuse-EXT
      'Sdi_data = &B01000000
      'Sii_data = &B01001100
      '_send
      '          Fuse EXTENDED
      Sdi_data = Fusebit(3)
      Sii_data = &B00101100
      _send

      Sdi_data = &B00000000
      Sii_data = &B01100110
      _send

      'Sdi_data = &B00000000
      Sii_data = &B01101110
      _send

     _wait_sdo
   End If

Return
'_______________________________________________________________________________

_verify_fuse:

Ok = 1

      'fuse-LOW
      Sdi_data = &B00000100
      Sii_data = &B01001100
      _send

      Sdi_data = &B00000000
      Sii_data = &B01101000
      _send

      'Sdi_data = &B00000000
      Sii_data = &B01101100
      _receive

      If Sdo_data <> Fusebit(1) Then Reset Ok


   If Fuse_nr > 1 Then
      'fuse-HIGH
      'Sdi_data = &B00000100
      'Sii_data = &B01001100
      '_send

      Sdi_data = &B00000000
      Sii_data = &B01111010
      _send

      'Sdi_data = &B00000000
      Sii_data = &B01111110
      _receive

      If Sdo_data <> Fusebit(2) Then Reset Ok
   End If

   If Fuse_nr = 3 Then
      'fuse-EXT
      'Sdi_data = &B00000100
      'Sii_data = &B01001100
      '_send

      Sdi_data = &B00000000
      Sii_data = &B01101010
      _send

      'Sdi_data = &B00000000
      Sii_data = &B01101110
      _receive

      If Sdo_data <> Fusebit(3) Then Reset Ok
   End If
Return
'_______________________________________________________________________________

_chip_erase:

Sdi_data = &B10000000
Sii_data = &B01001100
_send

Sdi_data = &B00000000
Sii_data = &B01100100
_send

Sdi_data = &B10000000
Sii_data = &B01101100
_send

_wait_sdo

Return
'_______________________________________________________________________________

_send:


Reset Sdi
Reset Sii
_clock

Licznik = 8

Do

Decr Licznik
Sdi = Sdi_data.licznik
Sii = Sii_data.licznik
_clock
Loop Until Licznik = 0

Reset Sdi
Reset Sii
_clock
_clock

Return
'_______________________________________________________________________________

_receive:

Reset Sdi
Reset Sii
_clock
Sdo_data.7 = Sdo

Licznik = 8
Licznik2 = 7

Do
Decr Licznik
Decr Licznik2
Sdi = Sdi_data.licznik
Sii = Sii_data.licznik
_clock
Sdo_data.licznik2 = Sdo
Loop Until Licznik2 = 0

Sdi = Sdi_data.licznik
Sii = Sii_data.licznik
_clock

Reset Sdi
Reset Sii
_clock
_clock

Return
'_______________________________________________________________________________

_the_end:
Reset Res
Waitms 10
Set Pow
Return
'_______________________________________________________________________________


'(
Signatures And Fusebits:
_____________
Attiny11
   1e = 00011110 = 30
   90 = 10010000 = 144
   04 = 00000100 = 4

At00 - Low = 252
         1
         1
         1
Fstrt  = 1
Rstdisbl=1
Cksel2 = 1
Cksel1 = 0
Cksel0 = 0
_____________
Attiny12
   1e = 00011110 = 30
   90 = 10010000 = 144
   05 = 00000101 = 5

At00 - Low = 82
Odlevel = 0
Boden = 1
Spien = 0
Rstdisbl = 1
Cksel3 = 0
Cksel2 = 0
Cksel1 = 1
Cksel0 = 0
_____________
Attiny13
   1e = 00011110 = 30
   90 = 10010000 = 144
   07 = 00000111 = 7

At00 - Low = 106
Spien = 0
Eesave = 1
Wdton = 1
Ckdiv8 = 0
Sut1 = 1
Sut0 = 0
Cksel1 = 1
Cksel0 = 0

At01 - Hig = 255
Selfprgen = 1
Dwen = 1
Bodlevel1 = 1
Bodlevel0 = 1
Rstdisbl = 1
           1
           1
           1
_____________
Attiny15
   1e = 00011110 = 30
   90 = 10010000 = 144
   06 = 00000110 = 6

At00 - Low = 92
Bodlevel = 0
Boden = 1
Spien = 0
Rstdisbl = 1
         1
         1
Cksel1 = 0
Cksel0 = 0
____________
Attiny25
   1e = 00011110 = 30
   91 = 10010001 = 145
   08 = 00001000 = 8
Attiny45
   1e = 00011110 = 30
   92 = 10010010 = 146
   06 = 00000110 = 6
Attiny85
   1e = 00011110 = 30
   93 = 10010011 = 147
   0b = 00001011 = 11

At00 - Low = 98
Ckdiv8 = 0
Ckout = 1
Sut1 = 1
Sut0 = 0
Cksel3 = 0
Cksel2 = 0
Cksel1 = 1
Cksel0 = 0

At01 - Hig = 223
Rstdisbl = 1
Dwen = 1
Spien = 0
Wdton = 1
Eesave = 1
Bodlevel2 = 1
Bodlevel1 = 1
Bodlevel0 = 1

At10 - Ext = 255
            1
            1
            1
            1
            1
            1
            1
Selfprgen = 1
________________
Attiny24
   1e = 00011110 = 30
   91 = 10010001 = 145
   0b = 00001011 = 11
Attiny44
   1e = 00011110 = 30
   92 = 10010010 = 146
   07 = 00000111 = 7
Attiny84
   1e = 00011110 = 30
   93 = 10010011 = 147
   0c = 00001100 = 12

At00 - Low = 98
Ckdiv8 = 0
Ckout = 1
Sut1 = 1
Sut0 = 0
Cksel3 = 0
Cksel2 = 0
Cksel1 = 1
Cksel0 = 0

At01 - Hig = 223
Rstdisbl = 1
Dwen = 1
Spien = 0
Wdton = 1
Eesave = 1
Bodlevel2 = 1
Bodlevel1 = 1
Bodlevel0 = 1

At10 - Ext = 255
            1
            1
            1
            1
            1
            1
            1
Selfprgen = 1
_______________
At90s2323
   1e = 00011110 = 30
   91 = 10010001 = 145
   02 = 00000010 = 2

At00 - Low = 222
        1
        1
Spien = 0
        1
        1
        1
        1
Fstrt = 0

_______________
At90s2343
   1e = 00011110 = 30
   91 = 10010001 = 145
   03 = 00000011 = 3

At00 - Low = 222
        1
        1
Spien = 0
        1
        1
        1
        1
Rcen =  0
'______________
Attiny22
   1e = 00011110 = 30
   91 = 10010001 = 145
   06 = 00000110 = 6

At00 - Low = 222
        1
        1
SPIEN = 0
        1
        1
        1
        1
RCEN  = 0
'_____________
Attiny261
   1e = 00011110 = 30
   91 = 10010001 = 145
   0c = 00001100 = 12
Attiny461
   1e = 00011110 = 30
   92 = 10010010 = 146
   08 = 00001000 = 8
Attiny861
   1e = 00011110 = 30
   93 = 10010011 = 147
   0d = 00001101 = 13

At00 - Low = 98
CKDIV8 = 0
CKOUT  = 1
SUT1   = 1
SUT0   = 0
CKSEL3 = 0
CKSEL2 = 0
CKSEL1 = 1
CKSEL0 = 0

At01 - Hig = 223
RSTDISBL=1
DWEN   = 1
SPIEN  = 0
WDTON  = 1
EESAVE = 1
BODLVL2= 1
BODLVL1= 1
BODLVL0= 1

At10 - Ext = 255
         1
         1
         1
         1
         1
         1
         1
SELFPRGE=1


')