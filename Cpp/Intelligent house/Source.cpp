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
	//vector<CSterownik> stery(5);
	
	
	vector<CPokoj>::size_type sz_pok = pokoje.size();

	for(short i=0; i<sz_pok; i++)
        {
            pok_buffer.klima1=&klima[i];
            pok_buffer.minus =i/0.1;
            pok_buffer.plus =i/0.01;
            pok_buffer.numer = i;
            pokoje.push_back(pok_buffer);
        }
        
        
/*

	CPokoj  poddasze (&klima[0], 0.1, 0.01),
            salon    (&klima[1], 0.3, 0.07),
            sypialnia(&klima[2], 0.3, 0.07),
            garaz    (&klima[3], 0.3, 0.02),
            piwnica  (&klima[4], 0.6, 0.02);



cout  << "Podaj temperature w kazdym z pomieszczen" << endl<<endl;
    vector<int>::size_type sz_tem = tem.size();
	for(short i=0; i<sz_tem; i++)
	 {
	    cout << JakisPokoj.nazwa << ":    "; cin >> JakisPokoj.tem; albo jakoś inaczej
	    
	 }

vector<CSterownik>::size_type sz_stery = tem.stery();
for(short i=0; i<sz_stery; i++)
{
    stery[i] = (&klima[i], &pokoj[i],  tem[i]),
              
}

*/


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
