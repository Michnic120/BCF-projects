#include<iostream> 
#include<vector>
#include<algorithm>
#include<numeric>
#include<ctime>
#include<omp.h>

void swap(int* a, int* b) 
{ 
    int temp = *a;
    *a = *b; 
    *b = temp; 
} 

int partition(std::vector<int>& vec, int l, int h) 
{ 
    int pivot = vec[(vec.size() - 1) / 2]; 
    int i = l - 1; 
    
    for (int j = l; j <= h - 1; j++) // range based loop ? 
    { 
        if (vec[j] <= pivot) 
        { 
            i++; 
            swap(&vec[i], &vec[j]); 
        } 
    } 
    swap(&vec[i + 1], &vec[h]); 
    return i + 1; 
} 

void quicksortIterative(std::vector<int> vec, int l, int h)
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

/*1. podzielenie pivota pomiędzy wątki
2. wykonanie operacji quicksortu
3. zrobić jakiś swap czy coś miedzy częściami
4. zamiast synchronizacji wątków to
    - wykonanie procedury równolegle
    - cośtam cośtam
    - wykonanie następnej procedury równolegle*/




/*
przystosowanie funckji sortiterative, żebu przyjmowała
na wejsciu coś w stylu vec.begin(), vec.end() i pivot jako wartość do której będzie potem porównywał i dzielił 

*/

int partitionParallel(std::vector<int>& vec, int l, int pivot) 
{
    int i = l - 1; 
    int h = vec.size() - 1;

    for (int j = l; j <= h - 1; j++) // range based loop ? 
    { 
        if (vec[j] <= pivot) 
        { 
            i++; 
            swap(&vec[i], &vec[j]); 
        } 
    } 
    swap(&vec[i + 1], &vec[h]); 
    return i + 1; 
} 

std::pair<std::vector<int>, std::vector<int>> quicksortParallelHalve(std::vector<int>::iterator first, std::vector<int>::iterator last, int pivot)
{
    std::vector<int> vec(first, last);
    int l = 0;
    int p = partition(vec, l, vec.size() -1 ); //pivot); 
    
    std::vector<int> vec1(vec.begin(), vec.begin() + p - 1);
    std::vector<int> vec2(vec.begin() + p - 1, vec.end());

    quicksortIterative(vec1, l, vec1.size() - 1); 
    quicksortIterative(vec2, l, vec2.size() - 1); 

    return std::make_pair(vec1, vec2); 
}

void quicksortParallel(std::vector<int> vec)
{ 
    int mid = (vec.size() - 1) / 2;
    int pivot = vec[mid]; // do tego maja porównywać pozostałe
    
    // int threads = 2;

    std::vector<int> vec1(vec.begin(), vec.begin() + mid);
    std::vector<int> vec2(vec.begin() + mid + 1, vec.end());
    
    std::vector<std::pair<std::vector<int>, std::vector<int>>> vecPair;

    // for(int i = 0; i > 2; i++)
    // {

    // }

    #pragma omp parallel sections private(pivot, mid) 
    {
        #pragma omp section
        {
            vecPair.push_back(quicksortParallelHalve(vec1.begin(), vec1.end(), pivot)); 
        }
        #pragma omp section
        {   
            quicksortParallelHalve(vec2.begin(), vec2.end(), pivot); 
        }
    }

    // pętla for ktora będzie sterować ilością wywoływanych sekcji - 
    // zmienna sterująca pętli na podstawie ilości sczytanych wątkow

    /*rezultat sortowania wrzucasz do mapy, identyfikator na podstawie ID wątku
            map<pair<vector, vector>>
            1. vector - mniejszy od pivota
            2. vector - większy od pivota

    */



    for (auto iter : vecPair[0].first) 
    { 
        std::cout << iter << " "; 
    }
    std::cout << " 111111"; 

    for (auto iter : vecPair[0].second) 
    { 
        std::cout << iter << " "; 
    }
}

double timeSum(const std::vector<double>& timeVec, const int& n)
{
    return std::accumulate(timeVec.begin(), timeVec.end(), 0.0000) / (CLOCKS_PER_SEC * n);
}

int main() 
{
    const int n = 1, size = 20, m = 256  ;  // int max = 2147483647
    std::vector<double> clock1(n), clock3(n), clock5(n); 
    std::vector<int> vec(size);
    
    std::generate(vec.begin(), vec.end(), []() {return rand() % m;});
    auto vec1(vec);

    for(int i = 0; i != n; i++)
    {
        clock1[i] = clock();
        std::sort(vec1.begin(), vec1.end());
        clock1[i] = clock() - clock1[i]; 
    }

    for(int i = 0; i != n; i++)
    {
        clock3[i] = clock();
        quicksortIterative(vec, 0, vec.size() - 1); 
        clock3[i] = clock() - clock3[i];
    }

    for(int i = 0; i < n; i++) // zostaw do testowania innych rozwiązań
    {
        clock5[i] = clock();
        quicksortParallel(vec);
        clock5[i] = clock() - clock5[i];
    }

    // for (auto iter : vec) 
    // { 
    //     std::cout << iter << " "; 
    // }
    
    std::cout << "Ilość powtórzeń: " << n;

    std::cout << "\n1. std::sort         : "<< timeSum(clock1, n)
              << "\n3. pętla for_opt     : "<< timeSum(clock3, n)
              << "\n5. for_opt parallel  : "<< timeSum(clock5, n) << "\n";

    // std::cout << "Liczba procków : " << omp_get_thread_limit() << "\n ";
    
    return 0; 
}
