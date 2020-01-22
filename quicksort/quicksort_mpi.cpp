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

int main(int argc, char* argv[])
{
    const int root = 0, size = 20, m = 1000;
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
        std::cout << "Available processors: " << numOfProc << "\n" 
                  << "\n General before sort: \n\t";
        for (const auto& iter : vec)
        {
            std::cout << iter << " ";
        }
    }

    int indexBegin = I_BEGIN(size, myRank, numOfProc);
    int indexEnd = I_END(size, myRank, numOfProc);
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
    std::vector<int> gatherRegSamples;

    clock1 = MPI_Wtime();
    
    std::sort(partVec.begin(), partVec.end());

    for (int i = 0; i < numOfProc; i++)
    {
        regularSamples[i] = *(iterBegin + (i * size / pow(numOfProc, 2))-1);
    }

    if (myRank == root)
    {   
        gatherRegSamples.resize(pow(numOfProc, 2));
    }

    MPI_Gather(regularSamples.data(), numOfProc, MPI_INT,
            gatherRegSamples.data(), numOfProc, MPI_INT, 
            root, MPI_COMM_WORLD);

    if (myRank == root)
    {
        std::sort(gatherRegSamples.begin(), gatherRegSamples.end());

        for (int i = 0; i < numOfProc-1; i++)
        {
            privots[i] = gatherRegSamples[(i+1)*numOfProc];
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

    auto count = lenSum;
    MPI_Gather(&count, 1, MPI_INT,
            recvLength.data(), 1, MPI_INT, 
            root, MPI_COMM_WORLD);
    
    lenSum = 0;
    for (int i = 0; i < numOfProc; i++)
    {
        recvIndex[i] = lenSum;
        lenSum += recvLength[i];
    }

    MPI_Gatherv(recvVec.data(), count, MPI_INT,
            vec.data(), recvLength.data(), recvIndex.data(), MPI_INT, 
            root, MPI_COMM_WORLD);

    clock1 = MPI_Wtime() - clock1;

    if (myRank == root)
    {
        std::cout << "\n\n Gathered after sort: \n\t";
        for (const auto& iter : vec)
        {
             std::cout << iter << " ";
        }
        std::cout << "\n\n Processing time: \n\t" << clock1 << "\n";
    }

    MPI_Finalize();
    return 0;
}
