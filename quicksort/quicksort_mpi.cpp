#include<algorithm>
#include<iostream>
#include<numeric>
#include<vector>
#include<queue>

#include<cmath>
#include<ctime>

#include<mpi.h>

#define I_BEGIN(vecSize, myRank, numOfProc) (vecSize*myRank/numOfProc)
#define I_END(vecSize, myRank, numOfProc)   (vecSize*(myRank+1)/numOfProc)

using itVec = std::vector<int>::iterator;

inline double timeSum(const std::vector<double>& timeVec, const int& n)
{
    return std::accumulate(timeVec.begin(), timeVec.end(), 0.0000) / (CLOCKS_PER_SEC * n);
}

inline void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int getMedian(int vecSize, itVec begin, itVec end)
{
    return 1;
}

struct mmdata
{
    int stindex;
    int index;
    unsigned long stvalue;

    mmdata(int st=0, int id=0, unsigned long stv = 0):stindex(st),index(id),stvalue(stv){}
};

bool operator<(const mmdata & One, const mmdata & Two)
{
    return One.stvalue > Two.stvalue;
}

void multiple_merge(unsigned long* starts[], const std::vector<int>& lengths,
                    const int Number, unsigned long newArray[], const int newArrayLength)
{
    std::priority_queue<mmdata> priorities;

    for(int i = 0; i < Number; i++)
    {
        if (lengths[i] > 0)
        {
            priorities.push(mmdata(i,0,starts[i][0]));
        }
    }

    int newArrayindex = 0;
    while (!priorities.empty() && (newArrayindex < newArrayLength))
    {
        mmdata xxx = priorities.top();
        priorities.pop();

        newArray[newArrayindex++] = starts[xxx.stindex][xxx.index];

        if (lengths[xxx.stindex]>(xxx.index+1))
        {
            priorities.push(mmdata(xxx.stindex, xxx.index+1, starts[xxx.stindex][xxx.index+1]));
        }
    }
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

int main(int argc, char* argv[])
{
    const int  n = 10, size = 10, m = 1024;
    int numOfProc, myRank;

    std::vector<double> clock1(n);
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), []() {return rand() % m;});

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int indexBegin = I_BEGIN(size, myRank, numOfProc);
    int indexEnd = I_END(size, myRank, numOfProc);

    itVec iterBegin = vec.begin() + indexBegin;
    itVec iterEnd   = vec.begin() + indexEnd;

    if (myRank == 0)
    {
        std::cout << "\n general before sort: \n";
        for (auto iter : vec)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n\n";
    }

    std::sort(iterBegin, iterEnd);

    std::cout << "\n after "<< myRank <<" process sort: \n";
    for (auto iter : vec)
    {
        std::cout << iter << " ";
    }
    std::cout << "\n\n";


    std::vector<unsigned long> regularSamples(numOfProc);


    for(int i = 0; i < numOfProc; i++)
    {
        regularSamples[i] = vec[i*size / pow(numOfProc, 2)];

        std::cout << "\n regular sample of process: " << myRank << " \n";
        std::cout << regularSamples[i] << "  ";
    }
    std::cout << "\n\n";


    std::vector<unsigned long> gatherRegSam;
    if(myRank == 0)
    {
        gatherRegSam.resize(pow(numOfProc, 2));
    }


    MPI_Gather(
        regularSamples.data(), numOfProc, MPI_UNSIGNED_LONG,
        gatherRegSam.data(), numOfProc, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);


    if(myRank == 0)
    {
        std::cout << "\n all regular samples before sorting : \n";
        for (const auto& iter : gatherRegSam)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n";


        std::sort(gatherRegSam.begin(), gatherRegSam.end());

        std::cout << "\n all regular samples after sorting : \n";
        for (auto iter : gatherRegSam)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n";
    }


    std::vector<unsigned long> privots(numOfProc-1);

    if(myRank == 0)
    {
        for (int i = 0; i < numOfProc-1; i++)
        {
            privots[i] = 106;//gatherRegSam[(i+1)*numOfProc];
        }

        privots.push_back(115);


        std::cout << "\n all the privots : \n";
        for (auto iter : privots)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n";
    }

    MPI_Bcast(privots.data(), numOfProc, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);


    // sortowanie fragmentów tablic przez każdy proces na podstawie n pivotów

    // 1. mamy posortowane tablice już
    // 2. trzeba znaleźć gdzie można wcisnąć te pivoty
    // 3. po znalezieniu można podzielić tablicę na P tablic


// wyświetlenie elementów mniejszych od podanego pivota wszystkich
// wyświetlenie elementów mniejszych od podanego pivota tylko dla tego procesu
    // od danych pivotów

    if(myRank == 0)
    {
        std::cout << "\n full rank 0: \n";
        for (itVec j = iterBegin; j != iterEnd; j++)
        {
            std::cout << *j << " ";
        }
        std::cout << "\n";


        for (int i = 0; i < privots.size(); i++)
        {
            std::cout << "\ndevided rank 0: iteration no.:" << i <<"\n"
                      << "privot: " << privots[i] << " \n\n";
            for (itVec j = iterBegin; j != iterEnd; j++)
            {
                if(*j <= privots[i])
                {
                    std::cout << *j << " ";
                }
                else
                {
                     std::cout << "\n\n" << *j << " ";
                     iterBegin = j;
                     break;
                }
            }

            std::cout << "\n";
        }

    }


    // std::cout << "\n after sort: \n";
    // for (auto iter : vec)
    // {
    //     std::cout << iter << " ";
    // }
    // std::cout << "\n\n";
    // std::cout << "\n Ilość powtórzeń: " << n << " ID: " << myRank
    //           << "\nnum of processes: " << numOfProc << "\n" ;

    MPI_Finalize();
    return 0;
}
