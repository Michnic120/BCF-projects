#include<algorithm>
#include<iostream>
#include<numeric>
#include<stack>
#include<vector>

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
    std::stack<int> stack;
    stack.push(l);
    stack.push(h);

    while (!stack.empty())
    {
        h = stack.top(); stack.pop();
        l = stack.top(); stack.pop();

        int p = partition(vec, l, h);

        if (p - 1 > l)
        {
            stack.push(l);
            stack.push(p - 1);
        }

        if (p + 1 < h)
        {
            stack.push(p + 1);
            stack.push(h);
        }
    }
}

int main(int argc, char* argv[])
{   
    const int size = 20, m = 1024;  // n = 10
    int numOfProc, myRank;

    // std::vector<double> clock1(n);
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), []() {return rand() % m;});

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0 && (size < pow(numOfProc, 2) || numOfProc == 1))
    {
        std::cout << "Warunek początkowy niespełniony! ";
        return 0;
    }

    if (myRank == 0)
    {
        std::cout << "Ilość wątków: " << numOfProc << "\n" ;

        std::cout << "\n general before sort: \n";
        for (auto iter : vec)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n\n";
    }


    for (int tego = 0; tego < 4; tego++)
    {

    int indexBegin = I_BEGIN(size, myRank, numOfProc);
    int indexEnd = I_END(size, myRank, numOfProc);
    int lenSum = 0;

    itVec iterBegin = vec.begin() + indexBegin;
    itVec iterEnd   = vec.begin() + indexEnd;

    std::vector<int> regularSamples(numOfProc);
    std::vector<int> gatherRegSam;
    std::vector<int> privots(numOfProc-1);

    std::vector<int> sendLength(numOfProc);
    std::vector<int> sendIndex(numOfProc);
    std::vector<int> recvLength(numOfProc);
    std::vector<int> recvIndex(numOfProc);


    std::vector<int> vecVec(iterBegin, iterEnd);

    std::sort(vecVec.begin(), vecVec.end());


    for(int i = 0; i < numOfProc; i++)
    {
        regularSamples[i] = *(iterBegin + (i * size / pow(numOfProc, 2)));
    }

    if(myRank == 0)
    {
        gatherRegSam.resize(pow(numOfProc, 2));
    }

    MPI_Gather(regularSamples.data(), numOfProc, MPI_INT, gatherRegSam.data(), numOfProc, MPI_INT, 0, MPI_COMM_WORLD);

    if(myRank == 0)
    {
        std::sort(gatherRegSam.begin(), gatherRegSam.end());
  
        for (int i = 0; i < numOfProc-1; i++)
        {
            privots[i] = gatherRegSam[(i+1)*numOfProc];
        }
    }

    MPI_Bcast(privots.data(), numOfProc, MPI_INT, 0, MPI_COMM_WORLD);


    auto j = 0u;
    for(int i = 0; i<numOfProc-1; i++) 
    {
        sendIndex[i] = j;
        sendLength[i] = 0;

        while((j < vecVec.size()) && (vecVec[j] <= privots[i])) 
        {
            j++;
            sendLength[i]++;
        }
    }

    sendIndex[numOfProc-1] = j;
    sendLength[numOfProc-1] = vecVec.size() - j;


   if(myRank == tego)
    {
        std::cout << "\n wszystkie vecVec dla rank: " << myRank ;
        std::cout << "\n\t";
    
        for(auto iter : vecVec)
        {
             std::cout << iter << " ";
        }

        std::cout << "\n wszystkie sendIndex dla rank: " << myRank ;
        std::cout << "\n\t";

        for(auto iter : sendIndex)
        {
             std::cout << iter << " ";
        }

        std::cout << "\n wszystkie sendLength dla rank: " << myRank ;
        std::cout << "\n\t";

        for(auto iter : sendLength)
        {
             std::cout << iter << " ";
        }

        std::cout << "\n wszystkie privots dla rank: " << myRank ;
        std::cout << "\n\t";

        for(auto iter : privots)
        {
             std::cout << iter << " ";
        }
    }



    MPI_Alltoall(sendLength.data(), 1, MPI_INT,
                 recvLength.data(), 1, MPI_INT,
                 MPI_COMM_WORLD);


    
    for(int i = 0; i < numOfProc; i++)
    {
        recvIndex[i] = lenSum;
        lenSum += recvLength[i];
    }
    std::vector<int> recvVec(lenSum);


    MPI_Alltoallv(vecVec.data(), sendLength.data(), sendIndex.data(), MPI_INT,
                 recvVec.data(), recvLength.data(), recvIndex.data(), MPI_INT,
                MPI_COMM_WORLD);




 if(myRank == tego)
    {
        std::cout << "\n wszystkie recvLength dla rank: " << myRank ;
        std::cout << "\n\t";

        for(auto iter : recvLength)
        {
             std::cout << iter << " ";
        }

        std::cout << "\n wszystkie recvVec dla rank: " << myRank ;
        std::cout << "\n\t";

        for(auto iter : recvVec)
        {
             std::cout << iter << " ";
        }
        std::cout << "\n";
    }


    std::sort(recvVec.begin(), recvVec.end());


    if(myRank == tego)
    {
        std::cout << "\n posortowane recvVec dla rank: " << myRank ;
        std::cout << "\n\t";

        for(auto iter : recvVec)
        {
             std::cout << iter << " ";
        }
        std::cout << "\n\n\n";
    }


std::vector<>
int* vec = nullptr;

if(myRank == 0)
{   
    vec = new int[size];
}

    



    MPI_Gather(recvVec.data(),
        lenSum,
        MPI_INT,
        vec,
        size,
        MPI_INT,
        0,
        MPI_COMM_WORLD);


// if(myRank == 0)
// {   
//     std::cout <<  "\n\n" ; 
//     for(auto iter : veg)
//     {
//         std::cout << iter << " ";
//     }
// }




}
 
    MPI_Finalize();
    return 0;
}
