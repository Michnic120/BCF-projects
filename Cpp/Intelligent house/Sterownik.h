#pragma once
#include"Klima.h"
#include "Pokoj.h"

class CSterownik
{
private:
  double dTemperaturaZadana;
  
public:
  CSterownik(CKlima *klimka, CPokoj* pokoj, int temtem);
  ~CSterownik();
  CKlima *klima1;
  CPokoj *pokoj1;
  void UstawTemperature(float temp);
  void ZwiekszTemperatur();
  void ZmiejszTemperature();
  double DajTemperature();
  void Kontrola();
};
