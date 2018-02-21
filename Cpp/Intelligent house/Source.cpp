#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

#include "Pokoj.h"
#include "Sched.h"
#include "Sterownik.h"

using namespace std;
VOID WINAPI Sleep(DWORD dwMilliseconds);

int main()
{
	CPokoj pok_buffer;
	vector<CKlima> klima(5);
   	vector<CPokoj> pokoje(5);
	vector<int> tem(5);
	vector<CPokoj>::size_type sz_pok = pokoje.size();

	for(short i=0; i<sz_pok; i++)
        {
            pok_buffer.klima1=&klima[i];
            pok_buffer.minus =i/0.1;
            pok_buffer.plus =i/0.01;
            pok_buffer.numer = i;
            pokoje.push_back(pok_buffer);
        }
        

	cout  << "Podaj temperature w kazdym z pomieszczen" << endl<<endl;
	cout  << "Poddasze:  "; cin >> tem[0];
	cout  << "Salon:     "; cin >> tem[1];
	cout  << "Sypialnia: "; cin >> tem[2];
	cout  << "Garaz:     "; cin >> tem[3];
	cout  << "Piwnica:   "; cin >> tem[4];


	CSterownik  s1(&klima[0], &poddasze,  tem[0]),
                s2(&klima[1], &salon,     tem[1]),
                s3(&klima[2], &sypialnia, tem[2]),
                s4(&klima[3], &garaz,     tem[3]),
                s5(&klima[4], &piwnica,   tem[4]);

	CSched shed(&poddasze, &s1, &salon, &s2, &sypialnia, &s3, &garaz, &s4, &piwnica, &s5, 1000);

	shed.uruchamiacz();

	return 0;
}
