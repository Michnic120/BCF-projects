#define _CRT_SECURE_NO_WARNINGS
#include "Sched.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include "Controller.h"
#include <windows.h>
#include <conio.h>
#include <string>

using namespace std;
VOID WINAPI Sleep(DWORD dwMilliseconds);

CSched::CSched(CRoom *rom1, CController *cont1, CRoom *rom2, CController *cont2, CRoom *rom3, CController *cont3, CRoom *rom4, CController *cont4, CRoom *rom5, CController *cont5, int cycle)
{
	iTimeCycle = cycle;
	
	room1 = rom1;
	con1 = cont1;
	
	room2 = rom2;
	con2 = cont2;
	
	room3 = rom3;
	con3 = cont3;
	
	room4 = rom4;
	con4 = cont4;
	
	room5 = rom5;
	con5 = cont5;
}

CSched::~CSched()
{
}

void CSched::Starter()
{
	for (int i=0;i<=iTimeCycle;i++) 
	{
		
		cout << "           Current temperature:       Temperature on controller:" << endl;
		cout << "Attic:                             " << room1->GiveTem() << endl << "                                                                  " << st1->DajTemperature() << endl;
		cout << "Living room:                       " << room2->GiveTem() << endl << "                                                                  " << st2->DajTemperature() << endl;
		cout << "Bed room:                          " << room3->GiveTem() << endl << "                                                                  " << st3->DajTemperature() << endl;
		cout << "Garage:                            " << room4->GiveTem() << endl << "                                                                  " << st4->DajTemperature() << endl;
		cout << "Cellar:                            " << room5->GiveTem() << endl << "                                                                  " << st5->DajTemperature() << endl;
		
		room1->ChangeTem(); 
		room2->ChangeTem();
		room3->ChangeTem();
		room4->ChangeTem();
		room5->ChangeTem();
		
		if (_kbhit()) {
			string a;
			cin >> a;

			if (a == "attic+")con1->IncreaseTemp(); 
			if (a == "attic-")con1->DecreaseTemp();
			
			if (a == "livroom+")con2->IncreaseTemp(); 
			if (a == "livroom-")con2->DecreaseTemp();
			
			if (a == "bedroom+")con3->IncreaseTemp(); 
			if (a == "bedroom-")con3->DecreaseTemp();
			
			if (a == "garage+")con4->IncreaseTemp(); 
			if (a == "garage-")con4->DecreaseTemp();
			
			if (a == "cellar+")con5->IncreaseTemp(); 
			if (a == "cellar-")con5->DecreaseTemp();

			if (a == "atticwindow+") { room1->plus *= 1.5; cout << "Window in attic opened."; }
			if (a == "atticwindow-") { room1->plus /= 1.5; cout << "Window in attic closed."; }
		
			if (a == "livroomwindow+"){room2->plus *= 1.5; cout << "Window in living room opened.";}
			if (a == "livroomwindow-"){room2->plus /= 1.5; cout << "Window in living room closed.";}

			if (a == "bedroomwindow+"){room3->plus *= 1.5; cout << "Window in bed room opened.";}
			if (a == "bedroomwindow-"){room3->plus /= 1.5; cout << "Window in bed room closed.";}
		
		}
		
		con1->Control();
		con2->Control();
		con3->Control();
		con4->Control();
		con5->Control();
		
		Sleep(500);
		system("cls");
	}

}
