pragma once
#include"AirConditioner.h"

class CRoom
{
  private:
    double dActualTem;
  public:
    CRoom();
    CRoom(CAirConditioner *condd, float dec, float inc, short num);
    ~CRoom();
   
  CAirConditioner *cond1;
  short number;
  float plus, minus;
  void ChangeTem();
  double GiveTem();

};
