#include "Termometr.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

CTermometr::CTermometr()
{
  dMojaTemperatura = 18;
}

CTermometr::~CTermometr()
{
}
 
double CTermometr::DajTemperature()
{
  return dMojaTemperatura;
}
