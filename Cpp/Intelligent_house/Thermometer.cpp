#include "Thermometer.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

CThermometer::CThermometer()
{
  dMyTemp = 18;
}

CThermometer::~CThermometer()
{
}
 
double CThermometer::GiveTemp()
{
  return dMyTemp;
}
