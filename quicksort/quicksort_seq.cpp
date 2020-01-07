#include<iostream> 
#include<vector>
#include<algorithm>
#include<numeric>
#include<ctime>

void swap(int* a, int* b) 
{ 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 

int partition(std::vector<int>& vec, int l, int h) 
{ 
    int x = vec[h]; 
    int i = l - 1; 

    for (int j = l; j <= h - 1; j++) 
    { 
        if (vec[j] <= x) 
        { 
            i++; 
            swap(&vec[i], &vec[j]); 
        } 
    } 
    
    swap(&vec[i + 1], &vec[h]); 
    return (i + 1); 
} 

void quicksort(std::vector<int> vec, int left, int right)  // bez refki 
{   
    int i = left;
    int j = right;
    int pivot = vec[(left+right) >> 1];

    do
    {
        while(vec[i] < pivot) i++;
        while(vec[j] > pivot) j--;
        if(i <= j)
        {                  
            swap(&vec[i++], &vec[j--]);
        }
    }while(i <= j);

    if(left < j)  quicksort(vec, left, j);
    if(right > i) quicksort(vec, i, right);
}

void quickSort(std::vector<int> vec, int l, int h)   // bez refki 
{
    if (l < h)
    {
        int p = partition(vec, l, h); 
        quickSort(vec, l, p - 1); 
        quickSort(vec, p + 1, h); 
    }
}

void quickSortIterative(std::vector<int> vec, int l, int h)  // bez refki 
{
    int stack[h - l + 1]; 
    int top = -1; 

    stack[++top] = l; 
    stack[++top] = h; 

    while (top >= 0)
    {
        h = stack[top--]; 
        l = stack[top--]; 

        int p = partition(vec, l, h); 

        if (p - 1 > l)
        {
            stack[++top] = l; 
            stack[++top] = p - 1; 
        }

        if (p + 1 < h)
        {
            stack[++top] = p + 1; 
            stack[++top] = h; 
        }
    }
} 

double timeSum(const std::vector<double>& timeVec, const int& n)
{
    return std::accumulate(timeVec.begin(), timeVec.end(), 0.0000) / (CLOCKS_PER_SEC * n);
}

int main() 
{
    const int n = 10, s = 10000, m = 10000;
    // srand( time( NULL ) );
    std::vector<double> clock1(n), clock2(n), clock3(n), clock4(n); 
    std::vector<int> vec(s);
    std::generate(vec.begin(), vec.end(), []() {return rand() % m;});
    // std::vector<uint> można też sprobować
    auto vec1(vec);

    for(int i = 0; i != n; i++)
    {
        clock1[i] = (double)clock(); //Przypisanie czasu poczatkowego
        std::sort(vec1.begin(), vec1.end());
        clock1[i] = (double)clock() - clock1[i]; //Obliczenie czasu trwania programu

        clock2[i] = (double)clock();
        quicksort(vec, 0, vec.size() - 1); 
        clock2[i] = (double)clock() - clock2[i];

        clock3[i] = (double)clock();
        quickSort(vec, 0, vec.size() - 1);  
        clock3[i] = (double)clock() - clock3[i];

        clock4[i] = (double)clock();
        quickSortIterative(vec, 0, vec.size() - 1); 
        clock4[i] = (double)clock() - clock4[i];

        // for (auto iter : vec) 
        // { 
        //     std::cout << iter << " "; 
        // }
    }

    std::cout << "\nIlość powtórzeń: " << n;
    std::cout << "\n1. std::sort                 : "<< timeSum(clock1, n);
    std::cout << "\n2. pętla while               : "<< timeSum(clock2, n);
    std::cout << "\n3. pętla for                 : "<< timeSum(clock3, n);
    std::cout << "\n4. pętla for zoptymalizowana : "<< timeSum(clock4, n) << "\n";

    return 0; 
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

Należy zaprojektować:
odpowiedni algorytm podziału na P procesorów/maszyn, opracować program, wykonać te-
sty i przedstawić analizę wyników. 

*/