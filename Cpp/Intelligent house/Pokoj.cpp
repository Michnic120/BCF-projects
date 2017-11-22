#include "Pokoj.h"
#include "Klima.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

CPokoj::CPokoj()
{
  dAktualnaTem = 18;
}

CPokoj::CPokoj(CKlima * klimka, float zmn, float zw, short num)
{
  dAktualnaTem = 18;
  klima1 = klimka;
  plus = zw;
  minus = zmn;
  numer = num;
  
}

CPokoj::~CPokoj()
{
}

void CPokoj::ZmienTem()
{
  dAktualnaTem+=plus;
  if (0 == klima1->DajStan()) {dAktualnaTem=dAktualnaTem;}
  if (1 == klima1->DajStan()) {dAktualnaTem-=minus;}
  if (0 != klima1->DajStan() && 1 != klima1->DajStan()) {cout << "Cos sie popsulo\n";}
}

double CPokoj::DajTem()
{
  return dAktualnaTem;
}
