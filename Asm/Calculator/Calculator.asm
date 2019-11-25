PROJECT CALC

;Port A & C for display
;Port B for keyboard
;Port D for buzzer
;buzzer connected to PIND0 i GND

.include "m32def.inc"
.def Number = R16
.def Result = R17
.def Flag = R18
.def Sign = R21
.def WRow = R22
.def WColumn = R23
.def KRow = R24
.def KColumn = R25

.cseg
.org 0x0000
jmp Start


Start:

;CONFIGURATION: PORT A & C CONTROLS  ROWS AND COLUMNS OF THE DISPLAY
ldi WRow, 0xFF
out DDRA, WRow
ldi WColumn, 0xFF
out DDRC, WColumn

;CONFIGURATION: PORT D to control buzzer
ldi WRow, 0x01
out DDRD, WRow


Keyboard:

ldi R30, HIGH(RAMEND)	; 16-bit stack initialization
out SPH, R30
ldi R31, LOW(RAMEND)
out SPL, R31


ldi KRow, 0xF0
out DDRB, KRow
ldi KRow, 0x0F
out PORTB, KRow

sbis PINB, 0  ldi KRow, 6	sbis PINB, 0 rcall BUZZ_on
sbis PINB, 1  ldi KRow, 3	sbis PINB, 1 rcall BUZZ_on
sbis PINB, 2  ldi KRow, 0	sbis PINB, 2 rcall BUZZ_on
sbis PINB, 3  ldi KRow, 9	sbis PINB, 3 rcall BUZZ_on


ldi KColumn, 0x0F
out DDRB, KColumn
ldi KColumn, 0xF0
out PORTB, KColumn

sbis PINB, 4 ldi KColumn, 1	 sbis PINB, 4 rcall BUZZ_on
sbis PINB, 5 ldi KColumn, 2	 sbis PINB, 5 rcall BUZZ_on
sbis PINB, 6 ldi KColumn, 3	 sbis PINB, 6 rcall BUZZ_on
sbis PINB, 7 ldi KColumn, 13 sbis PINB, 7 rcall BUZZ_on


add KRow,KColumn

cpi KRow, 11
brlt Numberprog
cpi KRow, 13


Numberprog:

mov Number, KRow
brsh Equals


Signprog:
mov Sign, KRow
rjmp Keyboard


Equals:
cpi KRow, 12
rjmp Calculator

Calculator:
Loop0:
brne Loop1
//rjmp Digit


Loop1:
cpi Sign, 22		; Add
brne Loop2
rjmp Add

Loop2:
cpi Sign, 13		; Subtract
brne Loop3
rjmp Subtract

Loop3:
cpi Sign, 16		; Multiply
rjmp Multiply


Add:
add Result, Number
rcall Display

Subtract:
sub Result, Number
rcall Display

Multiply:
mul Result, Number
rcall Display



Display:

cpi Result,10
brlt One

cpi Result,100
brlt Two

cpi Result,255
brlt Three

One:
ldi WColumn, 0b000
rjmp Next

Two:
ldi WColumn,3
rjmp Next

Three:
ldi WColumn,7

Next:

out PORTC, WColumn

push R19

Tag1:
subi Result,100 // 1000
brlo Tag2
inc R19
rjmp Tag1

Tag2:
subi Result,-100  //-1000
mov R26,R19
ldi R19,0

Tag3:
subi Result,100
brlo Tag4
inc R19
rjmp Tag3

Tag4:
subi Result,-100
mov R27,R19
ldi R19,0
rjmp Display

Tag5:
subi Result,10
brlo Tag6
inc R19
rjmp Tag5

Tag6:
subi Result,-10
mov R28,R19
ldi R19,0
rjmp Display

mov R29, Result
pop R19


//One

cpi R20, 0		breq  Onezero
cpi R20, 1		breq  OneOne
cpi R20, 2		breq  OneTwo
cpi R20, 3		breq  OneThree
cpi R20, 4		breq  OneFour
cpi R20, 5		breq  OneFive
cpi R20, 6		breq  OneSix
cpi R20, 7		breq  OneSeven
cpi R20, 8		breq  OneEight
cpi R20, 9		breq  OneNine

;displaying Result

Onezero:		ldi R20, 0b00111111		ret
OneOne:		    ldi R20, 0b00000110		ret
OneTwo:		    ldi R20, 0b01011011		ret
OneThree:		ldi R20, 0b01001111		ret
OneFour:		ldi R20, 0b01100110		ret
OneFive:		ldi R20, 0b01101101		ret
OneSix:		    ldi R20, 0b01111101		ret
OneSeven:		ldi R20, 0b00000111		ret
OneEight:		ldi R20, 0b11111111		ret
OneNine:		ldi R20, 0b01101111		ret

out PORTC,r20		; displaying rows


//Two   // R0 -> r26   if equal = display

cpi R26, 0		breq  Twozero
cpi R26, 1		breq  TwoOne
cpi R26, 2		breq  TwoTwo
cpi R26, 3		breq  TwoThree
cpi R26, 4		breq  TwoFour
cpi R26, 5		breq  TwoFive
cpi R26, 6		breq  TwoSix
cpi R26, 7		breq  TwoSeven
cpi R26, 8		breq  TwoEight
cpi R26, 9		breq  TwoNine

Twozero:		ldi R26, 0b00111111		ret
TwoOne:		    ldi R26, 0b00000110		ret
TwoTwo:			ldi R26, 0b01011011		ret
TwoThree:		ldi R26, 0b01001111		ret
TwoFour:		ldi R26, 0b01100110		ret
TwoFive:		ldi R26, 0b01101101		ret
TwoSix:		    ldi R26, 0b01111101		ret
TwoSeven:		ldi R26, 0b00000111		ret
TwoEight:		ldi R26, 0b11111111		ret
TwoNine:		ldi R26, 0b01101111		ret

out PORTC,R26


//Three   //R1 -> R27

cpi R27, 0		breq  Threezero
cpi R27, 1		breq  ThreeOne
cpi R27, 2		breq  ThreeTwo
cpi R27, 3		breq  ThreeThree
cpi R27, 4		breq  ThreeFour
cpi R27, 5		breq  ThreeFive
cpi R27, 6		breq  ThreeSix
cpi R27, 7		breq  ThreeSeven
cpi R27, 8		breq  ThreeEight
cpi R27, 9		breq  ThreeNine

Threezero:		ldi R27, 0b00111111		ret
ThreeOne:		ldi R27, 0b00000110		ret
ThreeTwo:		ldi R27, 0b01011011		ret
ThreeThree:		ldi R27, 0b01001111		ret
ThreeFour:		ldi R27, 0b01100110		ret
ThreeFive:		ldi R27, 0b01101101		ret
ThreeSix:		ldi R27, 0b01111101		ret
ThreeSeven:		ldi R27, 0b00000111		ret
ThreeEight:		ldi R27, 0b11111111		ret
ThreeNine:		ldi R27, 0b01101111		ret

out PORTC,R27


//Four		// R2 -> R28

cpi R28, 0		breq  Fourzero
cpi R28, 1		breq  FourOne
cpi R28, 2		breq  FourTwo
cpi R28, 3		breq  FourThree
cpi R28, 4		breq  FourFour
cpi R28, 5		breq  FourFive
cpi R28, 6		breq  FourSix
cpi R28, 7		breq  FourSeven
cpi R28, 8		breq  FourEight
cpi R28, 9		breq FourNine

Fourzero:		ldi R28, 0b00111111		ret
FourOne:		ldi R28, 0b00000110		ret
FourTwo:		ldi R28, 0b01011011		ret
FourThree:		ldi R28, 0b01001111		ret
FourFour:		ldi R28, 0b01100110		ret
FourFive:		ldi R28, 0b01101101		ret
FourSix:		ldi R28, 0b01111101		ret
FourSeven:		ldi R28, 0b00000111		ret
FourEight:		ldi R28, 0b11111111		ret
FourNine: 	    ldi R28, 0b01101111		ret

out PORTC,R28


//five   // R3 -> R29

cpi R29, 0		breq  Fivezero
cpi R29, 1		breq  FiveOne
cpi R29, 2		breq  FiveTwo
cpi R29, 3		breq  FiveThree
cpi R29, 4		breq  FiveFour
cpi R29, 5		breq  FiveFive
cpi R29, 6		breq  FiveSix
cpi R29, 7		breq  FiveSeven
cpi R29, 8		breq  FiveEight
cpi R29, 9		breq  FiveNine

Fivezero:       	ldi R29, 0b00111111		ret
FiveOne:      	    ldi R29, 0b00000110		ret
FiveTwo:        	ldi R29, 0b01011011		ret
FiveThree:       	ldi R29, 0b01100110		ret
FiveFive:       	ldi R29, 0b01101101		ret
FiveSix:      	    ldi R29, 0b01111101		ret
FiveSeven:     	    ldi R29, 0b00000111		ret
FiveEight:      	ldi R29, 0b11111111		ret
FiveNine:   	    ldi R29, 0b01101111		ret

out PORTC,R29
ret


BUZZ_on:			; buzzer on
cbi PORTD, 0


wait1: ldi R26, 2
wait2: ldi R27, 10
wait3: ldi R28, 255
//nop
//nop
dec R28 brne wait3
dec R27 brne wait2
dec R26 brne wait1

sbi  PORTD, 0
ret
