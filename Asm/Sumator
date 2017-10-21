.include "m32def.inc"
.def Wynik = R16
.def Liczba = R20
.cseg
.org 0x0000
jmp Start


Start:
ldi R18, HIGH(RAMEND)		//zainicjowanie 16-bitowego stosu
out SPH, R18
ldi R17, LOW(RAMEND)
out SPL, R17


Przycisk:			//podprogram sprawdzający, czy został wciśnięty przycisk na klawiaturze 4x4 
ldi R21, 0x0F			//kierunek rejestru wejściowy na 4 ostatnich bitach
out DDRD, R21
ldi R21, 0xF0			//podciągnięcie rezystorów pull up, przez załadowanie 4 ostatnich bitów na stan wysoki
out PORTD, R21


sbis PIND, PIND4	rjmp Dalej      //skip if bit is set, program pracuje w pętli, póki żaden przycisk nie zostanie wciśnięty
sbis PIND, PIND5	rjmp Dalej	//jeśli będzie wciśnięty, na pinie będzie stan niski i program przejdzie dalej, do programu Dalej
sbis PIND, PIND6	rjmp Dalej
sbis PIND, PIND7	rjmp Dalej
rjmp Przycisk


Dalej:
rcall Klawiatura			//przejdź do programu Klawiatura sprawdzającego, który przycisk został wciśnięty

add Wynik,Liczba			//dodaj do Wyniku liczbę która została wczytana przez klawiaturę		


rcall Wyswietl				//przejdź do procedury wyświetlenia wyniku


rjmp Start				//wróć na początek programu, do pierwszej pętli czekającej na wciśnięcie przycisku


Klawiatura:				//by określić, który przycisk został wciśnięty ustala się jego współrzędne czyli numer wiersza i numer kolumny w którym sie znajduje	
ldi R24, 0xF0 				//konfiguracja portuD na odebranie informacji z pierwszych czterech bitów
out DDRD, R24	
ldi R24, 0x0F				
out PORTD, R24

sbis PIND, PIND0 	ldi R24, 0	//jeśli pin będzie w stanie niskim załaduj do rejestru R24 odpowiednią liczbę, z wiersza 
sbis PIND, PIND1 	ldi R24, 4
sbis PIND, PIND2	ldi R24, 8
sbis PIND, PIND3	ldi R24, 12

ldi R25, 0x0F				//konfiguracja portuD na odebranie informacji z ostatnich czterech bitów
out DDRD, R25
ldi R25, 0xF0
out PORTD, R25

sbis PIND, PIND4	ldi R25, 1	//jeśli pin będzie w stanie niskim załaduj do rejestru R25 odpowiednią liczbę, z kolumny
sbis PIND, PIND5	ldi R25, 2
sbis PIND, PIND6	ldi R25, 3
sbis PIND, PIND7	ldi R25, 4

add R24,R25				//zsumuj rejestry 
mov Liczba,R24				//przenieś wynik do rejestru Liczba
ret					//wróć do ostatniego miejsca sprzed przerwania



Wyswietl:			
push R17
push R18

ldi R24, 0xFF			//porty A i B jako wyjściowe (A segmenty, B kolumny)
out DDRA, R24
ldi R25, 0xFF
out DDRB, R25

ldi R24, 0x00			// port A na stan niski, B na wysoki
out PORTA, R24
ldi R25, 0xFF
out PORTB, R25

Sprawdzenie:			//program sprawdzający czy liczba do wyświetlenia zawiera setki, dziesiątki i jedności
ldi R17, 0

cpi Wynik,100			
brsh Trzecia_kolumna		//sprawdzenie flagi po porównaniu, jeśli większe lub równe to przejdź do podprogramu sprawdzającego ile jest setek w liczbie

cpi Wynik,10			
brsh Druga_kolumna	

cpi Wynik,1
brsh Pierwsza_kolumna1

cpi Wynik,0			//jeśli zero to również przejdź do Pierwsza kolumna
breq Pierwsza_kolumna1

Trzecia_kolumna:  		

Etykieta1:			//sprawdzenie ile setek jest w liczbie
subi Wynik,100 			//odejmij 100
brlo Etykieta2			//jeśli wartość Wynik zejdzie poniżej zera przejdź do etykieta2
inc R17				//zwiększ o 1, zapisując ile jest setek w liczbie
rjmp Etykieta1			

Etykieta2:
subi Wynik,-100  		//dodaj 100 do Wynik, by nie był poniżej zera, ale nie miał setek

ldi R18, 0b00001111  //0b11111011      //aktywacja wszystkich kolumn (początkowo: wyświetl wynik na kolumnie wyświetlacza nr 3)
out PORTB, R18				


cpi R17, 0		brne l0		ldi R24, 0b11000000		l0:    //porównaj liczby, jeśli nie są równe, przejdź do następnego porównania
cpi R17, 1		brne l1		ldi R24, 0b11111001		l1:
cpi R17, 2		brne l2		ldi R24, 0b10100100		l2:
cpi R17, 3		brne l3		ldi R24, 0b10110000		l3:
cpi R17, 4		brne l4		ldi R24, 0b10011001		l4:
cpi R17, 5		brne l5		ldi R24, 0b10010010		l5:
cpi R17, 6		brne l6 	ldi R24, 0b10000010		l6:
cpi R17, 7		brne l7		ldi R24, 0b11111000		l7:
cpi R17, 8		brne l8		ldi R24, 0b00000000		l8:
cpi R17, 9		brne l9		ldi R24, 0b10010000		l9:

out PORTA, R24
rjmp Sprawdzenie		


Pierwsza_kolumna1:
rjmp Pierwsza_kolumna2


Druga_kolumna:

Etykieta3:			
subi Wynik,10			//odejmij 10
brlo Etykieta4			//jeśli wartość Wynik zejdzie poniżej zera przejdź do etykieta4
inc R17				//zwiększ o 1, zapisując ile jest dziesiątek w liczbie
rjmp Etykieta3			

Etykieta4:
subi Wynik,-10
			

ldi R18, 0b00001111 // 0b11111101 	//aktywacja wszystkich kolumn (początkowo: wyświetl wynik na kolumnie wyświetlacza nr 2)
out PORTB, R18	

cpi R17, 0		brne l10		ldi R24, 0b11000000		l10:    
cpi R17, 1		brne l11		ldi R24, 0b00000110		l11:
cpi R17, 2		brne l12		ldi R24, 0b10100100		l12:
cpi R17, 3		brne l13		ldi R24, 0b10110000		l13:
cpi R17, 4		brne l14		ldi R24, 0b10011001		l14:
cpi R17, 5		brne l15		ldi R24, 0b10010010		l15:
cpi R17, 6		brne l16 		ldi R24, 0b10000010		l16:
cpi R17, 7		brne l17		ldi R24, 0b11111000		l17:
cpi R17, 8		brne l18		ldi R24, 0b00000000		l18:
cpi R17, 9		brne l19		ldi R24, 0b10010000		l19:

out PORTA, R24
rjmp Sprawdzenie


Pierwsza_kolumna2:

Etykieta5:
subi Wynik,1			//odejmij 1
brlo Etykieta6			//jeśli wartość Wynik zejdzie poniżej zera przejdź do etykieta6
inc R17				//zwiększ o 1, zapisując ile jest jedności w liczbie
rjmp Etykieta5

Etykieta6:
subi Wynik,-1

ldi R18, 0b00001111 //0b11111110		//aktywacja wszystkich kolumn (początkowo: wyświetl wynik na kolumnie wyświetlacza nr 1)
out PORTB, R18	

cpi R17, 0		brne l20		ldi R24, 0b11000000  		l20:
cpi R17, 1		brne l21		ldi R24, 0b11111001		l21:
cpi R17, 2		brne l22		ldi R24, 0b10100100		l22:
cpi R17, 3		brne l23		ldi R24, 0b10110000		l23:
cpi R17, 4		brne l24		ldi R24, 0b10011001		l24:
cpi R17, 5		brne l25		ldi R24, 0b10010010		l25:
cpi R17, 6		brne l26 		ldi R24, 0b10000010		l26:
cpi R17, 7		brne l27		ldi R24, 0b11111000		l27:
cpi R17, 8		brne l28		ldi R24, 0b00000000		l28:
cpi R17, 9		brne l29		ldi R24, 0b10010000		l29:

out PORTA, R24

Koniec:
pop R18
pop R17
ret

