#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<windows.h>

#define ALFA 0.543478
#define BETA 0.456521


void generator(int t0, int N, float fmax, int rozmiar, float* tabsin, float* tabt, float* tabszum,  float* tabsuma,float Asz, float* tabA, float* tabf, float* tabphi)
{
  int i,j;

  for (i=0; i<rozmiar; i++) tabt[i] = t0 + ( i / (10*(fmax)) )  ;  

  for(j=0;j<N; j++)
    for (i=0; i<rozmiar; i++)
    {
      tabsin[i] = tabA[j]*sin((2*M_PI* tabf[j]*tabt[i])+tabphi[j]); tabsuma[i] = tabsuma[i]+tabsin[i]; 
    }   
    szum(tabszum, rozmiar, Asz);
    for (i=0; i<rozmiar; i++) tabsuma[i]=tabsuma[i]+tabszum[i];
}


float *ustawtablice(float *tab, int rozmiar)
{
  tab  = calloc(rozmiar,sizeof(float));
  return tab;
}


void szum(float *tabszum, int rozmiar, float Asz)
{
  int i,tmp=0, ilosc;

  srand(time(NULL));

  ilosc=(rand()%rozmiar);
  int tabsz[ilosc];

  for(i=0; i<ilosc; i++)
  {
     *(tabsz+i) =(rand()%rozmiar); 
     tabszum[tabsz[i]]=rand();
     if(tabszum[tabsz[i]]>tmp)tmp=tabszum[tabsz[i]];
  }

  for(i=0; i<rozmiar; i++)
  {
    if(tabszum[i]!=0)
    tabszum[i]=Asz*((tabszum[i]/tmp)*2-1);
  }
}


void plikzapis(float *tabtf, float *tabsuma, int rozmiar, int p)
{
  int i;

  FILE* plik;
    if(p==1)plik = fopen("C:\\Users\\Asus\\Desktop\\plik.csv", "w");
    if(p==2)plik = fopen("C:\\Users\\Asus\\Desktop\\fourier.csv", "w");

    for (i=0;i<rozmiar;i++)
    {
        fprintf(plik, "%f;%f;", tabtf[i],tabsuma[i]);
        fprintf(plik, "\n");
    }

      if (plik!=NULL)
        {if(p==1) printf("\nZaladowano sygnal\n");
         if(p==2) printf("\nZaladowano fouriera\n");}

    fclose(plik);
}


void plikodczyt(float *tabt, float *tabsuma, int* rozm, float* fmax)
{
  int n=1 , i;
  float tmp, f;
  FILE* plik;

  plik = fopen("C:\\Users\\Asus\\Desktop\\plik.csv", "r");

  for(fscanf(plik, "%f;", &tmp); feof(plik) ==0 ; fscanf(plik, "%f;\n", &tmp))
    n++;
  
  n=0.5*n; 
  rewind(plik);
  
  tabt = calloc(n, sizeof(float)); 
  tabsuma = calloc(n, sizeof(float)); 

  for(i=0; i<n; i++)
    fscanf(plik, "%f; %f; \n", &tabt[i], &tabsuma[i]);  
  for(i=0; i<n; i++)
    printf("%f; %f;\n", tabt[i], tabsuma[i]);

  f = n/(tabt[n-1]-tabt[0]);

  *fmax = f;
  *rozm = n; 
  fclose(plik);
}


void fourier(float *tabsuma, float* tabfourier, float* tabff, int rozmiar,  float fmax)
{
  int i, n, k;

  float tabrefourier[rozmiar],tabimfourier[rozmiar];

  for(i=0;i<rozmiar;i++) 
    tabff[i] = ((i+1)*10*fmax)/rozmiar ; 

  for(k=0; k<rozmiar; k++)
    {
      tabrefourier[k]=0;
      tabimfourier[k]=0;

      for(n=0;n<rozmiar;n++)
      {
        tabrefourier[k] += tabsuma[n]*cos(k*n*2*M_PI/rozmiar);
        tabimfourier[k] -= tabsuma[n]*sin(k*n*2*M_PI/rozmiar);
      }
      
      tabfourier[k] = sqrt((tabrefourier[k]*tabrefourier[k])+(tabimfourier[k]*tabimfourier[k]));
    }
}


void hamming(int N)
{
  int n=1;
  float ham;
  ham = ALFA-BETA*((2*M_PI*n)/N);
}


main()
{
 int   i,w,rozmiar, rozmiarf, p    ,N;
 float t0,tf;
 float fmax=0,Asz;
 float *tabA,*tabf,*tabphi,   
       *tabsin,*tabt,*tabszum,*tabsuma,  
       *tabfourier,*tabff;

 printf("Jesli chcesz generowac sygnal wybierz '1'\nJesli chcesz wczytac plik wybierz '2'\n"); 
 scanf("%d", &w);

 switch(w)
 {
   case 1:
     system("cls");
     printf("Generacja sygnalu.\n\n");
     printf("Podaj ilosc skladowych: ");     
     scanf("%d", &N);
     printf("\nPodaj czas poczatkowy: ");   
     scanf("%f", &t0);
     printf("Podaj czas koncowy: ");         
     scanf("%f", &tf);

     tabA  =  ustawtablice(tabsin, N);
     tabf  =  ustawtablice(tabf, N);
     tabphi=  ustawtablice(tabphi, N);

     for(i=0;i<N;i++)
     {
       printf("\nAmplituda %d: ", i+1);           
       scanf("%f",   (tabA+i));
       printf("Czestotliwosc %d: ", i+1);         
       scanf("%f",   (tabf+i));
       printf("Kat przesuniecia %d: ", i+1);      
       scanf("%f", (tabphi+i));

       if(fmax<*(tabf+i)) fmax = *(tabf+i);
      }

     rozmiar = (tf-t0)*10*(fmax); 

     tabsin =ustawtablice(tabsin, rozmiar);
     tabt   =ustawtablice(tabt,   rozmiar);
     tabszum=ustawtablice(tabszum,rozmiar);
     tabsuma=ustawtablice(tabsuma,rozmiar);

     printf("\nPodaj amplitude szumu: "); 
     scanf("%f", &Asz);

     generator(t0, N, fmax, rozmiar, tabsin, tabt, tabszum, tabsuma, Asz, tabA, tabf, tabphi);

     break;

   case 2:
     system("cls");
     printf("Wczytanie sygnalu z pliku.\n\n");

     plikodczyt(tabt, tabsuma, &rozmiar, &fmax);

     break;

   default:
     system("cls");
     printf("Wybierz ponownie:\n\n");
     
main();
{

  printf("\n\nWcisnij dowolny przycisk  \n\n");
  getch();

  printf("czas   sygnal \n");   for(i=0; i<rozmiar; i++) printf("%f  %f\n", tabt[i]=0,tabsuma[i]=0);
  p=1; plikzapis(tabt, tabsuma, rozmiar, p);


  tabfourier=ustawtablice(tabfourier,rozmiar);
  tabff= ustawtablice(tabff, rozmiar);
  fourier(tabsuma, tabfourier, tabff, rozmiar, fmax);
  p=2; plikzapis(tabff, tabfourier , rozmiar, p);

  return 0;
}