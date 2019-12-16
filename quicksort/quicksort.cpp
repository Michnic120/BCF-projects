void quicksort(int tab[], int left, int right){
     int i=left;
     int j=right;
     int x=tab[(left+right)>>1];
     do{
         while(tab[i]<x) i++;
         while(tab[j]>x) j--;
         if(i<=j){                  
             int temp=tab[i];
             tab[i]=tab[j];
             tab[j]=temp;
             i++;
             j--;
         }
     }while(i<=j);
     if(left<j) quicksort(tab,left,j);
     if(right>i) quicksort(tab,i,right);     
}



/*
Zadanie BK16. Sortowanie liczb — metoda quicksort


Algorytm sortowania szybkiego (ang. quicksort) jest bardzo wydajny: jego złożoność jest rzędu
O(n log n). Metoda stosuje podejście rekurencyjne — dziel i rządź. 

Wybieramy pewien element tablicy, tzw. element dzielący, i na początek tablicy przenosimy wszystkie elementy
mniejsze od niego, na koniec wszystkie większe, a w powstałe między tymi obszarami pu-
ste miejsce trafia wybrany element. 

Potem sortujemy osobno początkową i końcową część
tablicy. Rekursja kończy się, gdy kolejny fragment uzyskany z podziału zawiera pojedyn-
czy element, jako że jednoelementowa podtablica jest posortowana. 

Należy zaprojektować
odpowiedni algorytm podziału na P procesorów/maszyn, opracować program, wykonać te-
sty i przedstawić analizę wyników. 

*/