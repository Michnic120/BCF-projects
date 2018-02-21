#include "AirConditioner.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

CAirConditioner::CAirConditioner()
{
  iMyStatus = 0; 
}

CAirConditioner::~CAirConditioner()
{
}

void CAirConditioner::SetAirConditionerON()
{
  MyStatus = 1;
}

void CAirConditioner::SetAirConditionerOFF()
{
  iMyStatus = 0;
}

int CAirConditioner::GiveStatus()
{
  return iMyStatus;
}
