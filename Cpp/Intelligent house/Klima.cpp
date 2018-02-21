#include "Klima.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

CKlima::CKlima()
{
  iMojStan = 0; 
}

CKlima::~CKlima()
{
}

void CKlima::UstawKlimeON()
{
  iMojStan = 1;
}

void CKlima::UstawKlimeOFF()
{
  iMojStan = 0;
}

int CKlima::DajStan()
{
  return iMojStan;
}
