#include <iostream>
#include <mpi.h>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <queue>
#include <numeric>
#include <string>

using namespace std;

#define BLOCK_LOW(my_rank, comm_sz, T) ((my_rank)*(T)/(comm_sz))
#define BLOCK_HIGH(my_rank, comm_sz, T) (BLOCK_LOW(my_rank+1,comm_sz,T)-1)
#define BLOCK_SIZE(my_rank, comm_sz, T) (BLOCK_HIGH(my_rank,comm_sz,T) - BLOCK_LOW(my_rank,comm_sz,T) + 1)

struct mmdata
{
    // Numer sekwencji tablicy do scalenia
    int stindex;
    // Numer sekwencji w tablicy
    int index;
    unsigned long stvalue;
    mmdata(int st=0, int id=0, unsigned long stv = 0):stindex(st),index(id),stvalue(stv){}
};

bool operator<( const mmdata & One, const mmdata & Two)
{
    return One.stvalue > Two.stvalue;
}

// Dwuwymiarowa tablica próbek regularnych dla każdego procesu,
// długość próbek regularnych dla każdego procesu,
// liczba tablic do scalenia, tablica wyników,
// łączna ilość danych do scalenia

void multiple_merge(unsigned long* starts[], const int lengths[], const int Number, unsigned long newArray[], const int newArrayLength)
{
    priority_queue< mmdata> priorities;

    /*Dodaj pierwszą liczbę każdej tablicy do scalenia do kolejki
    priorytetowej i zapisz numer kolejny tablicy do scalenia
    oraz numer sekwencji liczby w tablicy*/

    for(int i=0; i<Number;i++) {
        if (lengths[i]>0) {
            priorities.push(mmdata(i,0,starts[i][0]));
        }
    }

    int newArrayindex = 0;
    while (!priorities.empty() && (newArrayindex<newArrayLength)) {
        // Weź najmniejsze dane
        mmdata xxx = priorities.top();
        priorities.pop();

        // Dodaj uzyskane dane do tablicy wyników
        newArray[newArrayindex++] = starts[xxx.stindex][xxx.index];

        /*Jeśli usunięte dane nie są ostatnim elementem tablicy do scalenia,
        wepchnij następny element do kolejki priorytetowej*/
        if ( lengths[xxx.stindex]>(xxx.index+1)) {
            priorities.push(mmdata(xxx.stindex, xxx.index+1, starts[xxx.stindex][xxx.index+1]));
        }
    }
}

int main(int argc, char* argv[])
{
    int comm_sz, my_rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // 1. Łączna liczba danych do posortowania to indeks 2 i wskaźnik pliku
    int power = strtol(argv[1], NULL, 10);
    unsigned long dataLength = pow(2, power);
    // unsigned long allDataLength;
    ifstream fin(argv[2], ios::binary);

     /*2. Każdy proces uzyskuje pozycję, w której zaczyna czytać dane,
     a ocenia się go na podstawie mocy, a nie długości danych.*/
    unsigned long myDataStart = BLOCK_LOW(my_rank, comm_sz, dataLength);
    unsigned long myDataLength = BLOCK_SIZE(my_rank, comm_sz, dataLength);
    fin.seekg((myDataStart+1)*sizeof(unsigned long), ios::beg);

    // 3. Każdy proces otrzymuje dane
    unsigned long *myData = new unsigned long[myDataLength];
    for(unsigned long i=0; i<myDataLength; i++)
        fin.read((char*)&myData[i], sizeof(unsigned long));
    fin.close();

    // Nagraj T_p
    double startTime, endTime;
    startTime = MPI_Wtime();

    // 4. Procesy sortują swoje dane
    sort(myData, myData+myDataLength);

    // 5. Uzyskaj regularne próbki, każdy proces wyodrębnia comm_sz
    unsigned long *regularSamples = new unsigned long[comm_sz];
    for(int index=0; index<comm_sz; index++)
        regularSamples[index] = myData[(index*myDataLength)/comm_sz];

    // 6. Proces 0 otrzymuje wszystkie regularSamples, łącznie comm_sz * comm_sz
    unsigned long *gatherRegSam;
    if(my_rank == 0)
    {
        gatherRegSam = new unsigned long[comm_sz*comm_sz];
    }

    // sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm
    MPI_Gather(regularSamples, comm_sz, MPI_UNSIGNED_LONG, gatherRegSam, comm_sz, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    // 7.
    /*Proces 0 scala i sortuje regularne Próbki wysyłane przez
    każdy proces i wyodrębnia liczbę comm_sz-1 dla programu prywatnego*/

    unsigned long *privots = new unsigned long[comm_sz];
    if(my_rank == 0)
    {
        /*start służy do przechowywania indeksów RegularSamples
        w gatherRegSam, co jest równoważne dwuwymiarowej tablicy*/
        unsigned long **starts = new unsigned long* [comm_sz];
        //
        /*Długość regularSamples
        każdego procesu w gatherRegSam jest taka sama jak comm_sz*/
        int *lengths = new int[comm_sz];
        for(int i=0; i<comm_sz; i++) {
            starts[i] = &gatherRegSam[i*comm_sz];
            lengths[i] = comm_sz;
        }

        //
        /*Ponieważ randomSamples każdego procesu są w porządku,
         wystarczy scalić dane każdego procesu w gatherRegSam.*/
        unsigned long *sortedGatRegSam = new unsigned long[comm_sz*comm_sz];
        multiple_merge(starts, lengths, comm_sz, sortedGatRegSam, comm_sz*comm_sz);

        // Ekstraktor
        for(int i=0; i<comm_sz-1; i++)
            privots[i] = sortedGatRegSam[(i+1)*comm_sz];

        delete []starts;
        delete []lengths;
        delete []sortedGatRegSam;
    }

    // 8.Główny nadawca
    MPI_Bcast(privots, comm_sz-1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    // 9.
    /*Każdy proces dzieli swoje dane według głównego elementu,
     a partStartIndex przechowuje indeks początkowy każdego segmentu.
     // Proces jest również zagrożony przepełnieniem*/
    int *partStartIndex = new int[comm_sz];

    /*Gdy jest tylko jeden proces, int ma ryzyko
    przepełnienia, ponieważ istnieją 2 ^ 32 liczby.
    Jeśli są dwa procesy,
    nie przepełni się, dopóki nie będzie długości (2 ^ 31) -1*/

    int *partLength = new int[comm_sz];

    unsigned long dataIndex = 0;
    for(int partIndex = 0; partIndex<comm_sz-1; partIndex++) {
        partStartIndex[partIndex] = dataIndex;
        partLength[partIndex] = 0;

        while((dataIndex<myDataLength) && (myData[dataIndex]<=privots[partIndex])) {
            dataIndex++;
            partLength[partIndex]++;
        }
    }
    //
    /*Ostatnia część danych jest
    zakończona (zleceniodawcy przepełnią się)*/
    partStartIndex[comm_sz-1] = dataIndex;
    partLength[comm_sz-1] = myDataLength - dataIndex;

    // 9.
    /*Operacja ALLTOALL, proces i zna długość segmentu i
    wszystkich procesów
     // przepełnienie oznacza, że element przekracza zakres int*/
    int *recvRankPartLen = new int[comm_sz];
    MPI_Alltoall(partLength, 1, MPI_INT, recvRankPartLen, 1, MPI_INT, MPI_COMM_WORLD);

    /*10. Operacja ALLTOALLV, proces i gromadzi wszystkie dane procesu i
     // Oblicz całkowitą liczbę odebranych danych i symboliczne
     przesunięcie pozycji początkowej od pozycji odebranej
     // Ryzyko przepełnienia dwóch procesów jest bardzo wysokie,
      więc możliwe jest wyeliminowanie dwóch operacji procesu
     w jak największym stopniu, a operacja liniowa jest w porządku.*/

    int rankPartLenSum = 0;
    int *rankPartStart = new int[comm_sz];
    for(int i=0; i<comm_sz; i++)
    {
        rankPartStart[i] = rankPartLenSum;
        rankPartLenSum += recvRankPartLen[i];
    }

    // Odbierz tablicę i segmentów dla każdego procesu
    unsigned long *recvPartData = new unsigned long[rankPartLenSum];
    MPI_Alltoallv(myData, partLength, partStartIndex, MPI_UNSIGNED_LONG,
                    recvPartData, recvRankPartLen, rankPartStart, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);

    // 11.
    /*Scal segmenty comm_sz
     // utwórz tablicę dwuwymiarową*/
    unsigned long **mulPartStart = new unsigned long*[comm_sz];
    for(int i=0; i<comm_sz; i++)
        mulPartStart[i] = &recvPartData[rankPartStart[i]];

    // Tablica wyników
    unsigned long *sortedRes = new unsigned long[rankPartLenSum];
    multiple_merge(mulPartStart, recvRankPartLen, comm_sz, sortedRes, rankPartLenSum);

    endTime = MPI_Wtime();

    // 12.
    /*Każdy proces ocenia, czy jego dane są w porządku*/
    bool sorted = true;
    for(int i=0; i<rankPartLenSum-1; i++)
        if(!(sortedRes[i] <= sortedRes[i+1]))
        {
            sorted = false;
            break;
        }
    string state = sorted ? "success" : "fail";
    cout << "rank " << my_rank << " sort " << state << "!!!!!!!!!!!!!!" << endl;

    // 13.
    /*Każda ranga bezpiecznie przesyła dane do następnej rangi.
    Numery parzyste są wysyłane jako pierwsze, a liczby  nieparzyste są odbierane jako pierwsze,
    a następnie liczby nieparzyste są wysyłane jako ostatnie, a liczby parzyste - jako pierwsze.*/

    unsigned long preMaxData;
    if(my_rank % 2 == 0)
    {
        if(my_rank != comm_sz-1)
        {
            MPI_Send(&sortedRes[rankPartLenSum-1], 1, MPI_UNSIGNED_LONG, my_rank+1, 0, MPI_COMM_WORLD);
        }
        if(my_rank != 0)
        {
            MPI_Recv(&preMaxData, 1, MPI_UNSIGNED_LONG, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        MPI_Recv(&preMaxData, 1, MPI_UNSIGNED_LONG, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(my_rank != comm_sz-1)
        {
            MPI_Send(&sortedRes[rankPartLenSum-1], 1, MPI_UNSIGNED_LONG, my_rank+1, 0, MPI_COMM_WORLD);
        }
    }

    if(my_rank > 0 && preMaxData <= sortedRes[0])
    {
        printf("rank: %d, rank %d is small, success\n", my_rank-1, my_rank);
    }
    else
    {
        printf("rank: %d, rank %d is big, fail........................\n", my_rank-1, my_rank);
    }


    if(my_rank == 0)
    {
        ofstream fout(argv[3], ios::app);
        fout << "processors:" << comm_sz << " power:" << power;
        fout << " Tp:" << endTime-startTime << endl;
    }

    // Końcowe przetwarzanie
    delete []myData;
    delete []regularSamples;
    if(my_rank==0)
        delete []gatherRegSam;
    delete []partStartIndex;
    delete []partLength;
    delete []recvRankPartLen;
    delete []rankPartStart;
    delete []recvPartData;
    delete []mulPartStart;
    delete []sortedRes;
    MPI_Finalize();

    return 0;
}
