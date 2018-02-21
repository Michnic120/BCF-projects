PROJEKT PRZYCISK 2

;Port A i C do wyświatlacza
;Port B do klawiatury
;Port D do buzzera
;buzzer połączony z PIND0 i GND

.include "m32def.inc"
.def Liczba = R16
.def Wynik = R17
.def Flaga = R18
.def Znak = R21
.def WSegment = R22
.def WKolumna = R23
.def KWiersz = R24
.def KKolumna = R25

.cseg
.org 0x0000
jmp Start


Start:

;KONFIGURACJA: PORT A i C STERUJE SEGMENTAMI i KOLUMNAMI WYŚWIETLACZA
ldi WSegment, 0xFF
out DDRA, WSegment
ldi WKolumna, 0xFF
out DDRC, WKolumna

;konfiguracja portu D do sterowania buzzerem
ldi WSegment, 0x01
out DDRD, WSegment


Klawiatura:

ldi R30, HIGH(RAMEND)	; inicjalicja 16-bitowego stosu
out SPH, R30			
ldi R31, LOW(RAMEND)	
out SPL, R31


ldi KWiersz, 0xF0		;Sprawdzenie, w którym wierszu został wciśnięty przycisk
out DDRB, KWiersz		;w przypadku przyciśnięcia przejdź do obsługi przerwania włączającego buzzer na czas zwarcia 
ldi KWiersz, 0x0F
out PORTB, KWiersz

sbis PINB, 0  ldi KWiersz, 6	sbis PINB, 0 rcall BUZZ_on 
sbis PINB, 1  ldi KWiersz, 3	sbis PINB, 1 rcall BUZZ_on 
sbis PINB, 2  ldi KWiersz, 0	sbis PINB, 2 rcall BUZZ_on
sbis PINB, 3  ldi KWiersz, 9	sbis PINB, 3 rcall BUZZ_on


ldi KKolumna, 0x0F		;Sprawdzenie, w której kolumnie został wciśnięty przycisk
out DDRB, KKolumna
ldi KKolumna, 0xF0
out PORTB, KKolumna

sbis PINB, 4 ldi KKolumna, 1	sbis PINB, 4 rcall BUZZ_on
sbis PINB, 5 ldi KKolumna, 2	sbis PINB, 5 rcall BUZZ_on
sbis PINB, 6 ldi KKolumna, 3	sbis PINB, 6 rcall BUZZ_on
sbis PINB, 7 ldi KKolumna, 13	sbis PINB, 7 rcall BUZZ_on


add KWiersz,KKolumna	; przepisuje do rejestru R24 Wynik klawiatury

cpi KWiersz, 11			; jeśli +,-,/,*  to przepisz
brlt Liczbaprog
cpi KWiersz, 13



Liczbaprog:

mov Liczba, KWiersz
brsh Rownasie


Znakprog:
mov Znak, KWiersz
rjmp Klawiatura


Rownasie:				;jeśli = to do klawiatury 
cpi KWiersz, 12
rjmp Kalkulator

Kalkulator:
Petla0:
brne Petla1
//rjmp Cyfra


Petla1:
cpi Znak, 22		;dodanie
brne Petla2
rjmp Dodaj

Petla2:
cpi Znak, 13		;odejmowanie
brne Petla3
rjmp Odejmij

Petla3:
cpi Znak, 16		;mnożenie
rjmp Mnozenie


Dodaj:
add Wynik, Liczba
rcall Wyswietl

Odejmij:
sub Wynik, Liczba
rcall Wyswietl

Mnozenie:
mul Wynik, Liczba
rcall Wyswietl



Wyswietl:

cpi Wynik,10
brlt Jeden

cpi Wynik,100
brlt Dwa

cpi Wynik,255   // było 100
brlt Trzy

Jeden:
ldi WKolumna, 0b000
rjmp Dalej			

Dwa:
ldi WKolumna,3
rjmp Dalej	

Trzy:
ldi WKolumna,7	

Dalej:

out PORTC,WKolumna		;  portd zamiast (chyba) portu b

push R19

Etykieta1:
subi Wynik,100 // 1000
brlo Etykieta2
inc R19
rjmp Etykieta1

Etykieta2:
subi Wynik,-100  //-1000
mov R26,R19  
ldi R19,0

Etykieta3:
subi Wynik,100
brlo Etykieta4
inc R19
rjmp Etykieta3

Etykieta4:
subi Wynik,-100
mov R27,R19
ldi R19,0
rjmp Wyswietl

Etykieta5:
subi Wynik,10
brlo Etykieta6
inc R19
rjmp Etykieta5

Etykieta6:
subi Wynik,-10
mov R28,R19
ldi R19,0
rjmp Wyswietl

mov R29, Wynik
pop R19


//jeden  

cpi R20, 0		breq  jedenzero
cpi R20, 1		breq  jedenjeden
cpi R20, 2		breq  jedendwa
cpi R20, 3		breq  jedentrzy
cpi R20, 4		breq  jedencztery
cpi R20, 5		breq  jedenpiec
cpi R20, 6		breq  jedenszesc
cpi R20, 7		breq  jedensiedem
cpi R20, 8		breq  jedenosiem
cpi R20, 9		breq  jedendziewiec

;wyświetlenie wyniku
jedenzero:		ldi R20, 0b00111111		ret
jedenjeden:		ldi R20, 0b00000110		ret
jedendwa:		ldi R20, 0b01011011		ret
jedentrzy:		ldi R20, 0b01001111		ret
jedencztery:		ldi R20, 0b01100110		ret
jedenpiec:		ldi R20, 0b01101101		ret
jedenszesc:		ldi R20, 0b01111101		ret
jedensiedem:		ldi R20, 0b00000111		ret
jedenosiem:		ldi R20, 0b11111111		ret
jedendziewiec:		ldi R20, 0b01101111		ret

out PORTC,r20		; zapalanie odpowiednich segmentów


//dwa   // R0 na r26    jeśli równe = ładuj na wyświetlaczu

cpi R26, 0		breq  dwazero 
cpi R26, 1		breq  dwajeden
cpi R26, 2		breq  dwadwa
cpi R26, 3		breq  dwatrzy
cpi R26, 4		breq  dwacztery
cpi R26, 5		breq  dwapiec
cpi R26, 6		breq  dwaszesc
cpi R26, 7		breq  dwasiedem
cpi R26, 8		breq  dwaosiem
cpi R26, 9		breq  dwadziewiec

dwazero:		ldi R26, 0b00111111		ret
dwajeden:		ldi R26, 0b00000110		ret
dwadwa:			ldi R26, 0b01011011		ret
dwatrzy:		ldi R26, 0b01001111		ret
dwacztery:		ldi R26, 0b01100110		ret
dwapiec:		ldi R26, 0b01101101		ret
dwaszesc:		ldi R26, 0b01111101		ret
dwasiedem:		ldi R26, 0b00000111		ret	
dwaosiem:		ldi R26, 0b11111111		ret
dwadziewiec:		ldi R26, 0b01101111		ret

out PORTC,R26	


//trzy   //R1 na R27

cpi R27, 0		breq  trzyzero
cpi R27, 1		breq  trzyjeden
cpi R27, 2		breq  trzydwa
cpi R27, 3		breq  trzytrzy
cpi R27, 4		breq  trzycztery
cpi R27, 5		breq  trzypiec
cpi R27, 6		breq  trzyszesc
cpi R27, 7		breq  trzysiedem
cpi R27, 8		breq  trzyosiem
cpi R27, 9		breq  trzydziewiec

trzyzero:		ldi R27, 0b00111111		ret
trzyjeden:		ldi R27, 0b00000110		ret
trzydwa:		ldi R27, 0b01011011		ret
trzytrzy:		ldi R27, 0b01001111		ret	
trzycztery:		ldi R27, 0b01100110		ret
trzypiec:		ldi R27, 0b01101101		ret
trzyszesc:		ldi R27, 0b01111101		ret	
trzysiedem:		ldi R27, 0b00000111		ret	
trzyosiem:		ldi R27, 0b11111111		ret
trzydziewiec:		ldi R27, 0b01101111		ret

out PORTC,R27		


//cztery		// R2 na R28 

cpi R28, 0		breq  czteryzero
cpi R28, 1		breq  czteryjeden
cpi R28, 2		breq  czterydwa
cpi R28, 3		breq  czterytrzy
cpi R28, 4		breq  czterycztery
cpi R28, 5		breq  czterypiec
cpi R28, 6		breq  czteryszesc
cpi R28, 7		breq  czterysiedem
cpi R28, 8		breq  czteryosiem
cpi R28, 9		breq czterydziewiec

czteryzero:		ldi R28, 0b00111111		ret
czteryjeden:		ldi R28, 0b00000110		ret
czterydwa:		ldi R28, 0b01011011		ret
czterytrzy:		ldi R28, 0b01001111		ret
czterycztery:		ldi R28, 0b01100110		ret
czterypiec:		ldi R28, 0b01101101		ret
czteryszesc:		ldi R28, 0b01111101		ret
czterysiedem:		ldi R28, 0b00000111		ret
czteryosiem:		ldi R28, 0b11111111		ret
czterydziewiec: 	ldi R28, 0b01101111		ret

out PORTC,R28		


//pięć   // R3 na R29

cpi R29, 0		breq  pieczero		
cpi R29, 1		breq  piecjeden
cpi R29, 2		breq  piecdwa
cpi R29, 3		breq  piectrzy
cpi R29, 4		breq  pieccztery
cpi R29, 5		breq  piecpiec
cpi R29, 6		breq  piecszesc
cpi R29, 7		breq  piecsiedem
cpi R29, 8		breq  piecosiem
cpi R29, 9		breq  piecdziewiec

pieczero:       	ldi R29, 0b00111111		ret
piecjeden:      	ldi R29, 0b00000110		ret
piecdwa:        	ldi R29, 0b01011011		ret
piectrzy:       	ldi R29, 0b01100110		ret
piecpiec:       	ldi R29, 0b01101101		ret
piecszesc:      	ldi R29, 0b01111101		ret
piecsiedem:     	ldi R29, 0b00000111		ret
piecosiem:      	ldi R29, 0b11111111		ret
piecdziewiec:   	ldi R29, 0b01101111		ret

out PORTC,R29		
ret


BUZZ_on:			;włączenie buzzera
cbi PORTD, 0


czekaj1: ldi R26, 2
czekaj2: ldi R27, 10
czekaj3: ldi R28, 255
//nop
//nop 
dec R28 brne czekaj3
dec R27 brne czekaj2
dec R26 brne czekaj1

sbi  PORTD, 0
ret
