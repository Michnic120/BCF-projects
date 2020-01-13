#pragma once
#include "Room.h"
#include "Controller.h"

class CSched
{
  private:
    int iTimeCycle;

  public:
    CRoom* room1, *room2, *room3, *room4, *room5;
    CController *con1, *con2, *con3, *con4, *con5;
     CSched(CRoom *rom1, CController *cont1, CRoom *rom2, CController *cont2, 
            CRoom *rom3, CController *cont3, CRoom *rom4, CController *cont4, 
            CRoom *rom5, CController *cont5, int cycle)
    ~CSched();
  
    void Starter();
};
