#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int npes;
    int myrank;

    // Inicjalizacja podsystemu MPI
    MPI_Init(&argc, &argv);

    // Pobierz rozmiar globalnego komunikatora
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // Pobierz numer procesu w globalnym komunikatorze
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    std::cout << "Jestem " << myrank << " procesem z " << npes << "\n";

    MPI_Finalize();

    return 0;
}
