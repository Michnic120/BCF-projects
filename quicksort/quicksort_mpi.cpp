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


int main(int argc, char* argv[])
{
    const int size = 20, m = 1000; //1024;
    int numOfProc, myRank;

    double clock1;
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), []() {return rand() % m;});

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0 && (size < pow(numOfProc, 2) || numOfProc == 1))
    {
        std::cout << "Initial condition not fulfilled!";
        return 0;
    }

    if (myRank == 0)
    {
        std::cout << "Number of available processors: " << numOfProc << "\n" ;

        // std::cout << "\n General before sort: \n\t";
        // for (const auto& iter : vec)
        // {
        //     std::cout << iter << " ";
        // }
    }

    int indexBegin = I_BEGIN(size, myRank, numOfProc);
    int indexEnd = I_END(size, myRank, numOfProc);
    const int root = 0;
    int lenSum = 0;

    itVec iterBegin = vec.begin() + indexBegin;
    itVec iterEnd   = vec.begin() + indexEnd;

    std::vector<int> regularSamples(numOfProc);
    std::vector<int> privots(numOfProc-1);
    std::vector<int> sendLength(numOfProc);
    std::vector<int> sendIndex(numOfProc);
    std::vector<int> recvLength(numOfProc);
    std::vector<int> recvIndex(numOfProc);
    std::vector<int> gatherVec(size);
    std::vector<int> partVec(iterBegin, iterEnd);
    std::vector<int> gatherRegSam;

    clock1 = MPI_Wtime();

    std::sort(partVec.begin(), partVec.end());

 
    for (int i = 0; i < numOfProc; i++)
    {
        regularSamples[i] = *(iterBegin + (i * size / pow(numOfProc, 2)));

        if (myRank == root) std::cout << "\n\tregularSamples : " << regularSamples[i] ;
    }
   
 
    if (myRank == root)
    {
        gatherRegSam.resize(pow(numOfProc, 2));
    }


    MPI_Gather(regularSamples.data(), numOfProc, MPI_INT,
            gatherRegSam.data(), numOfProc, MPI_INT, root,
            MPI_COMM_WORLD);


    if (myRank == root)
    {
        std::sort(gatherRegSam.begin(), gatherRegSam.end());

        for (int i = 0; i < numOfProc-1; i++)
        {
            privots[i] = gatherRegSam[(i+1)*numOfProc];
        }
    }


    MPI_Bcast(privots.data(), numOfProc, MPI_INT, root, MPI_COMM_WORLD);


    auto j = 0u;
    for (int i = 0; i<numOfProc-1; i++)
    {
        sendIndex[i] = j;
        sendLength[i] = 0;

        while((j < partVec.size()) && (partVec[j] <= privots[i]))
        {
            j++;
            sendLength[i]++;
        }
    }
    sendIndex[numOfProc-1] = j;
    sendLength[numOfProc-1] = partVec.size() - j;



    MPI_Alltoall(sendLength.data(), 1, MPI_INT,
                 recvLength.data(), 1, MPI_INT,
                 MPI_COMM_WORLD);


    for (int i = 0; i < numOfProc; i++)
    {
        recvIndex[i] = lenSum;
        lenSum += recvLength[i];
    }
    std::vector<int> recvVec(lenSum);


    MPI_Alltoallv(partVec.data(), sendLength.data(), sendIndex.data(), MPI_INT,
                 recvVec.data(), recvLength.data(), recvIndex.data(), MPI_INT,
                MPI_COMM_WORLD);



    std::sort(recvVec.begin(), recvVec.end());




    MPI_Gather(recvVec.data(), lenSum, MPI_INT,
            gatherVec.data(), lenSum, MPI_INT, root,
            MPI_COMM_WORLD);



    clock1 = MPI_Wtime() - clock1;

    // if (myRank == root && i == 0)
    // {
    //     std::cout << "\n Gathered after sort: \n\t";
    //     for (const auto& iter : gatherVec)
    //     {
    //          std::cout << iter << " ";
    //     }

    // }


    if (myRank == root)
    {
        std::cout << "\n Tp: " << clock1 << "\n";
    }

    MPI_Finalize();
    return 0;
}
