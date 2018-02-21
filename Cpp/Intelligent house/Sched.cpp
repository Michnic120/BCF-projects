#define _CRT_SECURE_NO_WARNINGS
#include "Sched.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include "Sterownik.h"
#include <windows.h>
#include <conio.h>
#include "Sterownik.h"
#include <string>

using namespace std;
VOID WINAPI Sleep(DWORD dwMilliseconds);

CSched::CSched(CPokoj *pokoik1, CSterownik *ster1, CPokoj *pokoik2, CSterownik *ster2, CPokoj *pokoik3, CSterownik *ster3, CPokoj *pokoik4, CSterownik *ster4, CPokoj *pokoik5, CSterownik *ster5, int cykl)
{
	iCyklCzasu = cykl;
	
	pokoj1 = pokoik1;
	st1 = ster1;
	
	pokoj2 = pokoik2;
	st2 = ster2;
	
	pokoj3 = pokoik3;
	st3 = ster3;
	
	pokoj4 = pokoik4;
	st4 = ster4;
	
	pokoj5 = pokoik5;
	st5 = ster5;
}

CSched::~CSched()
{
}

void CSched::uruchamiacz()
{
	for (int i=0;i<=iCyklCzasu;i++) {
		
		cout << "           Aktualna temperatura:       Temperatura na sterowniku:" << endl;
		cout << "Poddasze:                        " << pokoj1->DajTem() << endl << "                                                                  " << st1->DajTemperature() << endl;
		cout << "Salon:                           " << pokoj2->DajTem() << endl << "                                                                  " << st2->DajTemperature() << endl;
		cout << "Sypialnia:                       " << pokoj3->DajTem() << endl << "                                                                  " << st3->DajTemperature() << endl;
		cout << "Garaz:                           " << pokoj4->DajTem() << endl << "                                                                  " << st4->DajTemperature() << endl;
		cout << "Piwnica:                         " << pokoj5->DajTem() << endl << "                                                                  " << st5->DajTemperature() << endl;
		
		pokoj1->ZmienTem(); //dać stan od klimy
		pokoj2->ZmienTem();
		pokoj3->ZmienTem();
		pokoj4->ZmienTem();
		pokoj5->ZmienTem();
		
		if (_kbhit()) {
			string a;
			cin >> a;

			if (a == "poddasze+")st1->ZwiekszTemperatur(); 
			if (a == "poddasze-")st1->ZmiejszTemperature();
			
			if (a == "salon+")st2->ZwiekszTemperatur(); 
			if (a == "salon-")st2->ZmiejszTemperature();
			
			if (a == "sypialnia+")st3->ZwiekszTemperatur(); 
			if (a == "sypialnia-")st3->ZmiejszTemperature();
			
			if (a == "garaz+")st4->ZwiekszTemperatur(); 
			if (a == "garaz-")st4->ZmiejszTemperature();
			
			if (a == "piwnica+")st5->ZwiekszTemperatur(); 
			if (a == "piwnica-")st5->ZmiejszTemperature();

			if (a == "poddaszeokno+") { pokoj1->plus *= 1.5; cout << "Okno na poddaszu otwarte."; }
			if (a == "poddaszeokno-") { pokoj1->plus /= 1.5; cout << "Okno na poddaszu zamkniete."; }
		
			if (a == "salonokno+"){pokoj2->plus *= 1.5; cout << "Okno w salonie  otwarte.";}
			if (a == "salonokno-"){pokoj2->plus /= 1.5; cout << "Okno w salonie zamkniete.";}

			if (a == "sypialniaokno+"){pokoj3->plus *= 1.5; cout << "Okno w sypialni otwarte.";}
			if (a == "sypialniaokno-"){pokoj3->plus /= 1.5; cout << "Okno w sypialni zamkniete.";}
		
		}
		
		st1->Kontrola();
		st2->Kontrola();
		st3->Kontrola();
		st4->Kontrola();
		st5->Kontrola();
		
		Sleep(500);
		system("cls");
	}

}
