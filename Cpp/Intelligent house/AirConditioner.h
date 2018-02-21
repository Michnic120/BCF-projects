#pragma once

class CAirConditioner
{
  private:
      int iMyStatus;
  public:
      AirConditioner();
      ~AirConditioner();
      void SetAirConditionerON();
      void SetAirConditionerOFF();
      int GiveStatus();
};
