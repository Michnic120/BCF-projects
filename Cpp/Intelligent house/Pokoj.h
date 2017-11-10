pragma once
#include"Klima.h"

class CPokoj
{
private:
  double dAktualnaTem;
public:
  CPokoj();
  CPokoj(CKlima *klimka, float zmn, float zw);
  ~CPokoj();
   CKlima *klima1;
  float plus, minus;
  void ZmienTem();
  double DajTem();
};
