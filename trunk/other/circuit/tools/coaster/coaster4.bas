'Program "elektronicznej podstawki". autor: Manekinen (manekinen@gmail.com). kompilator: bascom 1.11.9.0. data: 06.08.2009. Wy³¹cznie do u¿ytku niekomercyjnego. http://diy.elektroda.pl
$regfile = "attiny13.dat"
$crystal = 4800000
'$sim
'$tiny
'ustawienia stosu i ramki
$hwstack = 16
$swstack = 8
$framesize = 24

'ustawiam piny jako wyjœcia
Config Portb.0 = Output
Config Portb.1 = Output
Config Portb.2 = Output

'pin pomiaru adc z LM35
'Config Portb.4 = Input


Config Timer0 = Timer , Prescale = 1
Enable Timer0
'Start Timer0                                                'BASCOM LECI SOBIE W CH..CHINY, PO POLECENIU "START TIMER" ZATRZYMUJE GO, A BEZ TEGO POLECENIA TIMER DZIA£A.
On Timer0 Software_pwm

Enable Interrupts

'zmienne kolorów
Dim R As Byte
Dim G As Byte
Dim B As Byte

'zmienne pomocnicze
Dim Reference As Byte
Dim Przejscie As Byte
Dim Temp As Word
Dim Wyl As Bit

'napiêcie wyjœciowe LM35 to 0V-1V (0*C-100*C), wewnêtrzne napiêcie odniesienia Vbg to ok.1V (ok, czyli wg dokumentacji 1.0V do 1.2V - ³adny rozrzut, pogratulowaæ)
Config Adc = Single , Prescaler = Auto , Reference = Internal
Start Adc

'powitanie "R-G-B", mo¿na zaremowaæ jeœli komuœ siê nie podoba.
R = 255
Waitms 10
R = 0
Waitms 150
G = 255
Waitms 10
G = 0
Waitms 150
B = 255

'_______________________________________________________________________________
Do

Waitms 20                                                   'przy 50ms ca³a pêtla kolorów trwa oko³o 5 minut

If Wyl = 0 Then


   Select Case Przejscie

      Case 0:
         'zapalam B
         Incr B
         'przy 35*C i powy¿ej ca³kowicie zaniknie
         Temp = Temp + B
         If Temp > 320 Then Przejscie = 1

      Case 1:
         'gaszê R
         If R < 6 Then
            Przejscie = 2
         Else
            Decr R
         End If

      Case 2:
         'zapalam G
         Incr G
         If G = 150 Then Przejscie = 3

      Case 3:
         'gaszê B
         If B < 6 Then
            Przejscie = 4
         Else
            Decr B
         End If

      Case 4:
         'zapalam R
         Incr R
         'przy 20*C i poni¿ej ca³kowicie zaniknie
         Temp = 1000 - Temp
         Temp = Temp + R
         If Temp > 900 Then Przejscie = 5


      Case 5:
         'gaszê G
         If G < 6 Then
            Przejscie = 0
         Else
            Decr G
         End If

   End Select

End If


'pomiar temperatury
Temp = Getadc(2)
If Temp > 350 Then Temp = 350
If Temp < 100 Then Temp = 100


'jeœli temperatura pokojowa (temperatury za³¹czania dobierane eksperymentalnie, ró¿ni¹ siê niewiele od wartoœci ni¿ej, (240=ok.22*C)
If Temp > 250 And Temp < 300 Then
   If R > 0 Then Decr R                                     'wygaszam kolory
   If G > 0 Then Decr G
   If B > 0 Then Decr B
   Wyl = 1
End If

'jeœli temperatura napoju
If Temp > 300 Or Temp < 250 Then Wyl = 0


Loop
'_______________________________________________________________________________
Software_pwm:

Incr Reference                                              'zmienna referencyjna

If Reference > R Then : Set Portb.1 : Else : Reset Portb.1 : End If       'red

If Reference > G Then : Set Portb.0 : Else : Reset Portb.0 : End If       'green

If Reference > B Then : Set Portb.2 : Else : Reset Portb.2 : End If       'blue

Return