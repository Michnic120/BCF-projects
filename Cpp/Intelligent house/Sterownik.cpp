#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "Sterownik.h"
#include "Klima.h"
#include "Pokoj.h"

using namespace std;

CSterownik::CSterownik(CKlima * klimka, CPokoj* pokoj, int temtem)
{
  dTemperaturaZadana = temtem;
  klima1 = klimka;
  pokoj1 = pokoj;
}

CSterownik::~CSterownik()
{
}

void CSterownik::UstawTemperature(float temp)
{
  dTemperaturaZadana = temp;
}

void CSterownik::ZwiekszTemperatur()
{
  dTemperaturaZadana++;
}

void CSterownik::ZmiejszTemperature()
{
  dTemperaturaZadana--;
}

double CSterownik::DajTemperature()
{
  return dTemperaturaZadana;
}

void CSterownik::Kontrola()
{
  if (pokoj1->DajTem() < dTemperaturaZadana - 0.5) { klima1->UstawKlimeOFF(); }
  if (pokoj1->DajTem() > dTemperaturaZadana + 0.5) { klima1->UstawKlimeON(); }
}
