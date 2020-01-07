#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
int main()
{
	long startclock; // Zmienna przechowujaca czas startu programu
	startclock = (double)clock(); // Pomiar czasu
	
	//********* WARTOSCI PODSTAWOWE *********
	int X = 4000; // Wielkosc tablicy elementow skonczonych X-Y
	int Y = X; //Zakładam, ze tablica będzie kwadratowa
	int ITER = 1000; //Ilosc iteracji
	int num_threads;
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
		#pragma omp parallel for
		for(int j=1; j<Y-1; j++) //Zmiana tylko wnętrza tablicy "Zimne brzegi"
		{
			num_threads=omp_get_num_threads();
			for(int i=1; i<X-1; i++)
			{
				float T_SR = T1[i + X*j]; //Element srodkowy
				float T_L = T1[(i+1) + X*j]; //Element Lewy
				float T_P = T1[(i-1) + X*j]; //Element Prawy
				float T_G = T1[i + X*(j+1)]; //Element Gorny
				float T_D = T1[i + X*(j-1)]; //Element Dolny
				T2[i+X*j] = 0.125*(4*T_SR+T_L + T_P + T_G + T_D); //Obliczenie temperatury i zapisanie
				jej do tablicy T2
			}
		}
		//Obliczanie temperatury i zapisanie wyniku w T1
		#pragma omp parallel for
		for(int j=1; j<Y-1; j++)
		{
			for(int i=1; i<X-1; i++)
			{
				float T_SR = T1[i + X*j]; //Element srodkowy
				float T_L = T1[(i+1) + X*j]; //Element Lewy
				float T_P = T1[(i-1) + X*j]; //Element Prawy
				float T_G = T1[i + X*(j+1)]; //Element Gorny
				float T_D = T1[i + X*(j-1)]; //Element Dolny

				T1[i+X*j] = 0.125*(4*T_SR+T_L + T_P + T_G + T_D); //Obliczenie temperatury i zapisanie jej do tablicy T2
			}
		}
		iter+=2; //Poniewaz wykorzystane sa zamiennie 2 tablice inkrementuje co 2
	}
	
	startclock = (double)clock() - startclock; //Obliczenie czasu trwania programu
	//Pokazanie wynikow na ekranie, wyświetlane jest tylko 20x20 komorek (ograniczonych do typu int)
	printf("\nPrezentacja wyniku co: %i komorka tabeli\n\n",(X/20));
	
	for(int j=0; j<20; j++) //Zmiana tylko wnętrza tablicy "Zimne brzegi"
	{
		for(int i=0; i<20; i++)
		{
			int index = i*(X/20) + j*(X*(X/20));
			if ((int)T1[index] < 10) printf(" ");
			printf(" %i",(int)T1[index]);
		}
		printf("\n");
	}
	
	printf("\nIlosc iteracji: %i Wielkosc tablicy X=Y: %i",ITER,X);
	printf("\nCzas wykonania: %f ",(float)startclock/CLOCKS_PER_SEC);
	printf("\nIlosc uzytych rdzeni: %i\n\n",num_threads);
	free(T1); // Zwolnienie pamięci
	free(T2); // Zwolnienie pamięci
	return 0;
}