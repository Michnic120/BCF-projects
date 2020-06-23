#pragma once
#include"AirConditioner.h"
#include "Room.h"

class CController
{
  private:
    double dTempSet;
  
  public:
    CController(CAirConditioner * condd, CRoom* room, int temtem);
    ~CController();
    CAirConditioner *cond;
    CRoom *room1;
    
    void SetTemp(float temp);
    void IncreaseTemp();
    void DecreaseTemp();
    double GiveTemp();
    void Control();
};
