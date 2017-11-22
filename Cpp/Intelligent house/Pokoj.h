pragma once
#include"Klima.h"

class CPokoj
{
private:
  double dAktualnaTem;
public:
  CPokoj();
  CPokoj(CKlima *klimka, float zmn, float zw, short num);
  ~CPokoj();
   
  CKlima *klima1;
  short numer;
  float plus, minus;
  void ZmienTem();
  double DajTem();
};
