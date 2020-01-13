#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

#include "Room.h"
#include "Sched.h"
#include "Controller.h"

using namespace std;
VOID WINAPI Sleep(DWORD dwMilliseconds);

int main()
{
	CRoom room_buffer;
	vector<CAirConditioner> conds(5);
   	vector<CRoom> rooms(5);
	vector<int> tem(5);
	vector<CRoom>::size_type sz_room = rooms.size();

	for(short i=0; i<sz_pok; i++)
        {
            room_buffer.cond1=&cond[i];
            room_buffer.minus =i/0.1;
            room_buffer.plus =i/0.01;
            room_buffer.number = i;
            rooms.push_back(room_buffer);
        }
        

	cout  << "Set the temperature in every room" << endl<<endl;
	cout  << "Attic:  "; cin >> tem[0];
	cout  << "Living room:     "; cin >> tem[1];
	cout  << "Bed room: "; cin >> tem[2];
	cout  << "Garage:     "; cin >> tem[3];
	cout  << "Cellar:   "; cin >> tem[4];


	CSterownik s1(&cond[0], &attic,   tem[0]),
                   s2(&cond[1], &livroom, tem[1]),
                   s3(&cond[2], &bedroom, tem[2]),
                   s4(&cond[3], &garage,  tem[3]),
                   s5(&cond[4], &cellar,  tem[4]);

	CSched shed(&attic, &s1, &livroom, &s2, &bedroom, &s3, &garage, &s4, &cellar, &s5, 1000);

	shed.Starter();

	return 0;
}
