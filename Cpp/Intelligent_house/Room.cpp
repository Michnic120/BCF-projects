#include "Room.h"
#include "AirConditioner.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

CRoom::CRoom()
{
  dActualTem = 18;
}

CRoom::CRoom(CAirConditioner * condd, float dec, float inc, short num)
{
  dActualTem = 18;
  cond1 = condd;
  plus = inc
  minus = dec;
  number = num;
  
}

CRoom::~CRoom()
{
}


void CRoom::ChangeTem()
{
  dActualTem+=plus;
  
  if (0 == cond1->GiveStatus()) 
    dActualTem=dActualTem;
  if (1 == cond1->GiveStatus()) 
    dActualTem-=minus;
  if (0 != cond1->GiveStatus() && 1 != cond1->GiveStatus()) 
    cout << "Error!\n";
}

double CRoom::GiveTem()
{
  return dActualTem;
}
