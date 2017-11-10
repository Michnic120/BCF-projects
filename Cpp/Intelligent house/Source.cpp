#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include "Pokoj.h"
#include "Sched.h"
#include "Sterownik.h"

using namespace std;

VOID WINAPI Sleep(DWORD dwMilliseconds);

int main()
{
  int t1, t2, t3, t4, t5;
  cout << "Podaj temperature w kazdym z pomieszczen" << endl;
  cout << "Poddasze: "; cin >> t1;
  cout << "Salon: "; cin >> t2;
  cout << "Sypialnia: "; cin >> t3;
  cout << "Garaz: "; cin >> t4;
  cout << "Piwnica: "; cin >> t5;
  
  CKlima k1,k2,k3,k4,k5;
  CPokoj poddasze(&k1, 0.1, 0.1), salon(&k2, 0.3, 0.07), sypialnia(&k3, 0.3, 0.07),garaz(&k4, 0.3, 0.02), piwnica(&k5, 0.6, 0.02);
  CSterownik s1(&k1, &poddasze, t1), s2(&k2, &salon, t2), s3(&k3, &sypialnia, t3),s4(&k4, &garaz, t4), s5(&k5, &piwnica, t5);
  CSched shed(&poddasze, &s1, &salon, &s2, &sypialnia, &s3, &garaz, &s4, &piwnica, &s5,1000);
  
  shed.uruchamiacz();
  return 0;
}
