#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "Controller.h"
#include "AirConditioner.h"
#include "Room.h"

using namespace std;

CController::CController(CAirConditioner * condd, CRoom* room, int temtem)
{
  dTempSet = temtem;
  cond1 = condd;
  room1 = room;
}

CController::~CController()
{
}

void CController::SetTemp(float temp)
{
  dTempSet = temp;
}

void CController::IncreaseTemp()
{
  dTempSet++;
}

void CController::DecreaseTemp()
{
  dTempSet--;
}

double CController::GiveTemp()
{
  return dTempSet;
}

void CController::Control()
{
  if (room1->GiveTem() < dTempSet - 0.5) { cond1->SetAirConditionerOFF(); }
  if (room1->GiveTem() > dTempSet + 0.5) { cond1->SetAirConditionerON(); }
}
