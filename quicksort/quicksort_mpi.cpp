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

/*    if (myRank == 0 && (size < pow(numOfProc, 2) || numOfProc == 1))
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


    int indexBegin = I_BEGIN(size, myRank, numOfProc);
    int indexEnd = I_END(size, myRank, numOfProc);

    itVec iterBegin = vec.begin() + indexBegin;
    itVec iterEnd   = vec.begin() + indexEnd;

    std::vector<int> regularSamples(numOfProc);
    std::vector<int> gatherRegSam;
    std::vector<int> privots(numOfProc-1);

    int pSize = privots.size();
    std::vector<std::vector<int>> vecVec(numOfProc);


    std::vector<int> sendLength(numOfProc);
    std::vector<int> sendIndex(numOfProc);
    std::vector<int> recvLength(numOfProc);
    std::vector<int> recvIndex(numOfProc);


    std::sort(iterBegin, iterEnd);


    std::cout << "\n after "<< myRank <<" process sort: \n";
    for (auto iter : vec)
    {
        std::cout << iter << " ";
    }
    std::cout << "\n\n";


    std::cout << "\n regular sample of process: " << myRank << " \n";
    for(int i = 0; i < numOfProc; i++)
    {
        regularSamples[i] = *(iterBegin + (i * size / pow(numOfProc, 2)));
        std::cout << regularSamples[i] << "  ";
    }
    std::cout << "\n\n";

    if(myRank == 0)
    {
        gatherRegSam.resize(pow(numOfProc, 2));
    }

    MPI_Gather(regularSamples.data(), numOfProc, MPI_INT, gatherRegSam.data(), numOfProc, MPI_INT, 0, MPI_COMM_WORLD);

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

    if(myRank == 0)
    {
        for (int i = 0; i < numOfProc-1; i++)
        {
            privots[i] = gatherRegSam[(i+1)*numOfProc];
        }

        std::cout << "\n all the privots : \n";
        for (auto iter : privots)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n";
    }

    MPI_Bcast(privots.data(), numOfProc, MPI_INT, 0, MPI_COMM_WORLD);








    int dupsko = 0;








    if(myRank == dupsko)
    {
        std::cout << "\n full rank " << myRank << ": \n";
        for (itVec j = iterBegin; j != iterEnd; j++)
        {
            std::cout << *j << " ";
        }
        std::cout << "\n";
    }

    {
        auto iterTemp = iterBegin ;

        for (int i = 0; i < pSize; i++)
        {
            if(myRank == dupsko)
            {
                std::cout << "\n\ndivided rank " << dupsko << ": iteration no.:" << i
                          << "\n\tprivot: " << privots[i] << " \n";
            }

            for (itVec j = iterTemp; j != iterEnd; j++)
            {
                if(*j <= privots[i])
                {
                    vecVec[i].push_back(*j);
                }
                else
                {
                    if(myRank == dupsko) std::cout << "\tostatni większy element " << *j << " ";
                    iterTemp = j;
                    break;
                }
            }

            if (i == pSize-1)
            {
                for (itVec j = iterTemp; j != iterEnd; j++)
                {
                    vecVec[i+1].push_back(*j);
                }
            }
        }
        std::cout << "\n";
    }


        std::cout << "\n wszystkie elementy z vectorów dla rank: " << myRank ;
        int vSize = vecVec.size();
        std::cout << "\n\t";
        for(int i = 0; i != vSize; i++)
        {
            for(itVec j = vecVec[i].begin(); j != vecVec[i].end(); j++)
            {
                std::cout << *j << " ";
            }
            std::cout << " || ";
        }


    for(auto i = 0u; i < vecVec.size(); i++)
    {
        sendLength[i] = vecVec[i].size();
        if(i == 0)
        {
            sendIndex[i] = 0;
        }
        else
        {
            sendIndex[i] += sendLength[i-1];
        }
    }


    std::cout << "\n";

    std::cout << "\n\nwyświetlenie rozmiaru każdego z fragmentów tablicy dla procesu "<< myRank <<": \n";
    for(auto iter : sendLength)
    {
        std::cout << iter << " ";
    }
    std::cout << "\n";


    std::cout << "\n\nwyświetlenie indeksu przesunięcia każdego z fragmentów tablicy dla procesu "<< myRank <<": \n";
    for(auto iter : sendIndex)
    {
        std::cout << iter << " ";
    }
    std::cout << "\n";


    MPI_Alltoall(sendLength.data(), 1, MPI_INT,
                 recvLength.data(), 1, MPI_INT,
                 MPI_COMM_WORLD);


    std::cout << "\n\n dla procesu "<< myRank <<": \n";
    // for(auto i = 0u; i < recvLength.size(); i++)
    // {
    //     std::vector<int> temp(recvLength[i]);
    //     // emplace_back
    //     recvVec.push_back(temp); //recvLength[i]);
    //     std::cout << "\n\t Wielkosc recvecVec[" << i << "]: " << recvVec[i].size();
    // }

          auto sum = std::accumulate(recvLength.begin(), recvLength.end(), 0);
             std::vector<int> recvVec(sum);
          // recvVec.reserve(sum);



    // int *recvPartData = new int[sum];



          std::cout << "\n suma recvLength: " << sum;
          std::cout << "\n\t Wielkosc recvec: " << recvVec.size();



        std::cout << "\n\n";

    // if(myRank == dupsko)
    // {
        std::cout << "\n\nwyświetlenie otrzymanych długości każdego z fragmentów tablicy dla procesu "<< myRank <<": \n";
        for (auto iter : recvLength)
        {
            std::cout << iter << " ";
        }
        std::cout << "\n\n";
    // // }

*/

    // MPI_Alltoallv(vecVec.data(), sendIndex.data(),  sendLength.data(), MPI_INT,
    //              recvPartData, recvIndex.data(),  recvLength.data(), MPI_INT,
    //             MPI_COMM_WORLD);














std::vector<int> vecvecdupa;
if(myRank == 0)
{
    vecvecdupa = {1,2};
}
else if(myRank == 1)
{
    vecvecdupa = {2,1};

}
std::vector<int> dupaDataRec(2);


std::vector<int> sendDupaL = {1,1}, sendDupaI = {1,1};
std::vector<int> rDupaL = {1,1}, rDupaI = {1,1};


    MPI_Alltoallv(vecvecdupa.data(), sendDupaI.data(),  sendDupaL.data(), MPI_INT,
                  dupaDataRec.data(), rDupaI.data(),  rDupaL.data(), MPI_INT,
                MPI_COMM_WORLD);



if(myRank == 0)
{
   std::cout << "pierwszy element: " << dupaDataRec[0] << " drugi element: " << dupaDataRec[1] ;
}








//     // Odbierz tablicę i segmentów dla każdego procesu
//     int recvPartData[2];
//     int myData[2];

// if(myRank == 0)
// {
//     myData[0] = 1;
//     myData[1] = 2;
// }
// else if(myRank == 1)
// {
//     myData[0] = 2;
//     myData[1] = 1;

// }



// int sendDupaL[2], sendDupaI[2], rDupaL[2], rDupaI[2];




//     sendDupaL[0] = 1;
//     sendDupaL[1] = 1;
//     sendDupaI[0] = 1;
//     sendDupaI[1] = 1;
//     rDupaL[0] = 1;
//     rDupaL[1] = 1;
//     rDupaI[0] = 1;
//     rDupaI[1] = 1;





//     MPI_Alltoallv(myData, sendDupaI,  sendDupaL, MPI_INT,
//                   recvPartData, rDupaI,  rDupaL, MPI_INT,
//                 MPI_COMM_WORLD);



    // MPI_Alltoallv(myData, partLength, partStartIndex, MPI_UNSIGNED_LONG,
    //                 recvPartData, recvRankPartLen, rankPartStart, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);












    // if(myRank == 0)
    // {

    //     std::cout << "\n chuj wie:  \n";
    //     int vSize = recvVecVec.size();
    //     for(int i = 0; i != vSize; i++)
    //     {
    //         std::cout << "\n";
    //         for(itVec j = vecVec[i].begin(); j != vecVec[i].end(); j++)
    //         {
    //             std::cout << *j << " ";
    //         }
    //     }
    //     std::cout << "\n";
    // }








    // 2. merge wszystkich elementów vecVec do tempVec

    // std::vector<int> tempVec;

    // int vSize = vecVec.size();
    // for(int i = 0; i != vSize; i++)
    // {
    //      tempVec.insert(tempVec.end(), vecVec[i].begin(), vecVec[i].end());
    // }


    //3. każdy z procesów po otrzymaniu swojego przydziału sortuje elementy

    // std::sort(tempVec.begin(), tempVec.end());




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
