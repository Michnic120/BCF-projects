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
	vector<CKlima> klima(5);
	
	CPokoj  poddasze (&klima[0], 0.1, 0.01),
            salon    (&klima[1], 0.3, 0.07),
            sypialnia(&klima[2], 0.3, 0.07),
            garaz    (&klima[3], 0.3, 0.02),
            piwnica  (&klima[4], 0.6, 0.02);

	vector<int> temp(5);

	//vector<int>::size_type sz_temp = temp.size();
	//for(short i=0; i<sz_temp; i++){}

	cout  << "Podaj temperature w kazdym z pomieszczen" << endl<<endl;
	cout  << "Poddasze:  "; cin >> temp[0];
	cout  << "Salon:     "; cin >> temp[1];
	cout  << "Sypialnia: "; cin >> temp[2];
	cout  << "Garaz:     "; cin >> temp[3];
	cout  << "Piwnica:   "; cin >> temp[4];

	CSterownik  s1(&klima[0], &poddasze,  temp[0]),
                s2(&klima[1], &salon,     temp[1]),
                s3(&klima[2], &sypialnia, temp[2]),
                s4(&klima[3], &garaz,     temp[3]),
                s5(&klima[4], &piwnica,   temp[4]);

	CSched shed(&poddasze, &s1, &salon, &s2, &sypialnia, &s3, &garaz, &s4, &piwnica, &s5, 1000);

	shed.uruchamiacz();

	return 0;
}
