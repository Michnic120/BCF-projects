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

int partitionParallel_dumb(std::vector<int>& vec, int l, int h) 
{ 
    int pivot = vec[h]; 
    int i = l - 1; 
    
    #pragma omp parallel // private(i)
        #pragma omp for schedule(dynamic) nowait
            for (int j = l; j <= h - 1; j++) 
            { 
                if (vec[j] <= pivot) 
                { 
                    i++; 
                    swap(&vec[i], &vec[j]); 
                } 
            } 
    
    swap(&vec[i + 1], &vec[h]); 
    return (i + 1); 
} 

int partitionParallel(std::vector<int>& vec, int l, int h) 
{ 
    int pivot = vec[h]; 
    int i = l - 1; 
    
    #pragma omp parallel // private(i)
        #pragma omp for schedule(dynamic) nowait
            for (int j = l; j <= h - 1; j++) 
            { 
                if (vec[j] <= pivot) 
                { 
                    i++; 
                    swap(&vec[i], &vec[j]); 
                } 
            } 
    
    swap(&vec[i + 1], &vec[h]); 
    return (i + 1); 
} 

void quickSortIterative(std::vector<int> vec, int l, int h)
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

void quickSortIterativeParallel_dumb(std::vector<int> vec, int l, int h)
{
    int stack[h - l + 1]; 
    int top = -1; 

    stack[++top] = l; 
    stack[++top] = h; 

    while (top >= 0)
    {
        h = stack[top--]; 
        l = stack[top--]; 

        int p = partitionParallel_dumb(vec, l, h); 

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


void quickSort_parallel_dumb2(std::vector<int> array, int left, int right) 
{
    int i = left, j = right;
    int tmp;
    int pivot = array[(left + right) / 2];

    int cutoff = 1000;

    /* PARTITION PART */
    while (i <= j) 
    {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        
        if (i <= j) 
        {
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            i++;
            j--;
        }
    }
    
    if ( ((right-left)<cutoff) )
    {
        if (left < j)
            { 
                quickSort_parallel_dumb2(array, left, j); 
            }           
        if (i < right)
            { 
                quickSort_parallel_dumb2(array, i, right); 
            }
    }
    else
    {
        #pragma omp task    
            quickSort_parallel_dumb2(array, left, j); 
        #pragma omp task    
            quickSort_parallel_dumb2(array, i, right);      
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
void quickSort_parallel(std::vector<int> vec)
{ 
    int pivot = vec[(vec.size() - 1) / 2 ]; // do tego maja orównywać pozostałe

    #pragma omp parallel sections private(pivot)
    {
        #pragma omp section
        {
            auto vec1(vec);
            quickSortIterative(vec1, 0, vec1.size() -1); 
        }
        #pragma omp section
        {   
            auto vec2(vec);
            quickSortIterative(vec, 0, vec2.size() -1); 
        }
    }
}


void quickSort_parallel_internal(std::vector<int> vec, int l, int h)
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
    // srand( time( NULL ) );
    const int n = 5, s = 1000, m = 256  ;  // int max = 2147483647
    std::vector<double> clock1(n), clock2(n), clock3(n), clock4(n),  clock5(n), clock6(n) , clock9(n); 
    std::vector<int> vec(s);
    
    // int lenArr = 262144000; // 262144000 = 1000 MBytes = 1 GByte
    int numthreads = 2;

    std::generate(vec.begin(), vec.end(), []() {return rand() % m;});

    // std::vector<uint> można też sprobować
    
    auto vec1(vec);

    for(int i = 0; i != n; i++)
    {
        clock9[i] = clock();
        std::sort(vec1.begin(), vec1.end());
        clock9[i] = clock() - clock9[i]; 
    }

    for(int i = 0; i != n; i++)
    {
        clock3[i] = clock();
        quickSortIterative(vec, 0, vec.size() - 1); 
        clock3[i] = clock() - clock3[i];
    }

    for(int i = 0; i < n; i++)
    {
        clock4[i] = clock();
        quickSortIterativeParallel_dumb(vec, 0, vec.size() - 1); 
        clock4[i] = clock() - clock4[i];
    }


    for(int i = 0; i < n; i++) // zostaw do testowania innych rozwiązań
    {
        clock6[i] = clock();
        quickSort_parallel_dumb2(vec, 0, vec.size() - 1);
        clock6[i] = clock() - clock6[i];
    }

    for(int i = 0; i < n; i++) // zostaw do testowania innych rozwiązań
    {
        clock5[i] = clock();
        quickSort_parallel(vec);
        clock5[i] = clock() - clock5[i];
    }


    // for (auto iter : vec) 
    // { 
    //     std::cout << iter << " "; 
    // }
    


    std::cout << "\nIlość powtórzeń: " << n;
    std::cout << "\n1. std::sort              : "<< timeSum(clock9, n);
    std::cout << "\n3. pętla for_opt          : "<< timeSum(clock3, n) << "\n";
    
    std::cout << "\n5. q _ p parallel         : "<< timeSum(clock5, n) << "\n";

    std::cout << "\n6. q _ p dumb2            : "<< timeSum(clock6, n);
    std::cout << "\n4. for_opt parallel_dumb  : "<< timeSum(clock4, n) << "\n";
    std::cout << "Liczba procków : " << omp_get_thread_limit() << "\n ";
    
    return 0; 
}
