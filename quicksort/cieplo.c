#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{	
	long startclock; //Zmienna wykorzystywane do mierzenia czasu
	startclock = (double)clock(); //Przypisanie czasu poczatkowego
	
	//********* WARTOSCI PODSTAWOWE *********
	int X = 100; // Wielkosc tablicy elementow skonczonych X-Y
	int Y = X; //Zakładam, ze tablica będzie kwadratowa
	int ITER = 10000; //Ilosc iteracji
	float *T1; //Wskaznik na dane typu float
	float *T2; //Wskaznik na dane typu float
	T1 = (float*) malloc (sizeof(float)*X*Y); //Alokacja pamieci
	T2 = (float*) malloc (sizeof(float)*X*Y); //Alokacja pamieci
	
	//Zerowanie tablicy T1
	for (int i = 0; i < X; i++) 
	{
		for (int j = 0; j < Y; j++) 
		{
			int index = i + j * X;
			T1[index] = 0.0;
		}
	}
	//Na potrzeby projektu zakladam, ze cieplo bedzie transferowane z jednego boku, a brzegi będą zimne
	
	for (int j = 0; j < Y; j++) 
	{
		int index = j * X;
		T1[index] = 99.0; //Ustawiam temperature na 99
	}
	
	int iter = 0; //Ustawiam licznik iteracji na 0
	while(iter<ITER) //Glowna petla wykonujaca zadana ilosc przebiegow
	{
		//Obliczanie temperatury i zapisaanie wyniku w T2
		for(int j=1; j<Y-1; j++)
		{
			for(int i=1; i<X-1; i++)
			{
				float T_SR = T1[i + X*j]; //Element srodkowy
				float T_L = T1[(i+1) + X*j]; //Element Lewy
				float T_P = T1[(i-1) + X*j]; //Element Prawy
				float T_G = T1[i + X*(j+1)]; //Element Gorny
				float T_D = T1[i + X*(j-1)]; //Element Dolny
				T2[i+X*j] = 0.125*(4*T_SR+T_L + T_P + T_G + T_D); //Obliczenie temperatury i zapisanie jej do tablicy T2
			}
		}
		//Obliczanie temperatury i zapisanie wyniku w T1
		for(int j=1; j<Y-1; j++)
		{
			for(int i=1; i<X-1; i++)
			{
				float T_SR = T1[i + X*j];
				float T_L = T1[(i+1) + X*j];
				float T_P = T1[(i-1) + X*j];
				float T_G = T1[i + X*(j+1)];
				float T_D = T1[i + X*(j-1)];
				T1[i+X*j] = 0.125*(4*T_SR+T_L + T_P + T_G + T_D);
			}
		}
		iter+=2; //Poniewaz wykorzystane sa zamiennie 2 tablice inkrementuje co 2
	}

	startclock = (double)clock() - startclock; //Obliczenie czasu trwania programu
	printf("\nPrezentacja wyniku co: %i komorka tabeli\n\n",(X/20));
	//Pokazanie wynikow na ekranie, wyświetlane jest tylko 20x20 komorek (ograniczonych do typu int)
	for(int j=0; j<20; j++)
	{
		for(int i=0; i<20; i++)
		{
			int index = i*(X/20) + j*(X*(X/20));
			if ((int)T1[index] < 10) printf(" ");
			printf(" %i",(int)T1[index]);
		}
		printf("\n",j);
	}
	printf("\nIlosc iteracji: %i Wielkosc tablicy X=Y: %i",ITER,X);
	printf("\nCzas wykonania: %f \n",(float)startclock/CLOCKS_PER_SEC);
	free(T1);
	free(T2);
	return 0;
}