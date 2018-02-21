#pragma once
#include "Pokoj.h"
#include"Sterownik.h"

class CSched
{
  private:
    int iCyklCzasu;

  public:
    CPokoj* pokoj1, *pokoj2, *pokoj3, *pokoj4, *pokoj5;
    CSterownik *st1, *st2, *st3, *st4, *st5;
    CSched(CPokoj *pokoik1, CSterownik *ster1, CPokoj *pokoik2, CSterownik *ster2, 
           CPokoj *pokoik3, CSterownik *ster3, CPokoj *pokoik4, CSterownik *ster4, 
           CPokoj *pokoik5, CSterownik *ster5, int cykl);
    ~CSched();
  
    void uruchamiacz();
};
