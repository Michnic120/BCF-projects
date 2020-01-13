#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<ctime>

double timeSum(const std::vector<double>& timeVec, const int& n)
{
    return std::accumulate(timeVec.begin(), timeVec.end(), 0.0000) / (CLOCKS_PER_SEC * n);
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(std::vector<int>& vec, int l, int h)
{
    int pivot = vec[h];
    int i = l - 1;

    for (int j = l; j <= h - 1; j++)
    {
        if (vec[j] <= pivot)
        {
            i++;
            if (i != j)
            {
               swap(&vec[i], &vec[j]);
            }
        }
    }

    swap(&vec[i+1], &vec[h]);
    return i + 1;
}

void quicksortWhile(std::vector<int> vec, int l, int h)
{
    int i = l;
    int j = h;
    int pivot = vec[(l+h) >> 1];

    do
    {
        while(vec[i] < pivot) i++;
        while(vec[j] > pivot) j--;
        if(i <= j)
        {
            swap(&vec[i++], &vec[j--]);
        }
    }while(i <= j);

    if(l < j) quicksortWhile(vec, l, j);
    if(h > i) quicksortWhile(vec, i, h);
}

void quicksortFor(std::vector<int> vec, int l, int h)
{
    if (l < h)
    {
        int p = partition(vec, l, h);
        quicksortFor(vec, l, p - 1);
        quicksortFor(vec, p + 1, h);
    }
}

void quicksortForOpt(std::vector<int> vec, int l, int h)
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

void quicksortParallel(std::vector<int> vec)
{
    int num_threads = 4;
    int div = vec.size() / num_threads;
    std::vector<std::vector<int>> vecVec;

    for(int i = 0; i < num_threads; i++)
    {
        vecVec.emplace_back(vec.begin() + div*i, vec.begin() + div*(i+1));
    }

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            quicksortForOpt(vecVec[0], 0, vecVec[0].size() - 1);
        }
        #pragma omp section
        {
            quicksortForOpt(vecVec[1], 0, vecVec[1].size() - 1);
        }

        #pragma omp section
        {
            quicksortForOpt(vecVec[2], 0, vecVec[2].size() - 1);
        }
        #pragma omp section
        {
            quicksortForOpt(vecVec[3], 0, vecVec[3].size() - 1);
        }
    }

    std::vector<int> temp1(vec.size()/2, 0), temp2(vec.size()/2, 0);

    std::merge(vecVec[0].begin(),  vecVec[0].end(),  vecVec[1].begin(),  vecVec[1].end(),  temp1.begin());
    std::merge(vecVec[2].begin(),  vecVec[2].end(),  vecVec[3].begin(),  vecVec[3].end(),  temp2.begin());
    std::merge(temp1.begin(), temp1.end(), temp2.begin(), temp2.end(), vec.begin());
}

int main()
{
    const int n = 50, s = 500, m = 1024;
    std::vector<double> clock1(n), clock2(n), clock3(n), clock4(n), clock5(n);
    std::vector<int> vec(s);
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
        clock2[i] = clock();
        quicksortWhile(vec, 0, vec.size() - 1);
        clock2[i] = clock() - clock2[i];
    }

    for(int i = 0; i != n; i++)
    {
        clock3[i] = clock();
        quicksortFor(vec, 0, vec.size() - 1);
        clock3[i] = clock() - clock3[i];
    }

    for(int i = 0; i != n; i++)
    {
        clock4[i] = clock();
        quicksortForOpt(vec, 0, vec.size() - 1);
        clock4[i] = clock() - clock4[i];
    }

    for(int i = 0; i != n; i++)
    {
        clock5[i] = clock();
        quicksortParallel(vec, 0, vec.size() - 1);
        clock5[i] = clock() - clock5[i];
    }

    std::cout << "\nIlość powtórzeń: " << n;
    std::cout << "\n1. std::sort               : "<< timeSum(clock1, n);
    std::cout << "\n2. pętla while             : "<< timeSum(clock2, n);
    std::cout << "\n3. pętla for               : "<< timeSum(clock3, n);
    std::cout << "\n4. pętla for_opt           : "<< timeSum(clock4, n) << "\n";
    std::cout << "\n5. pętla for_opt parallel  : "<< timeSum(clock5, n) << "\n";

    return 0;
}
