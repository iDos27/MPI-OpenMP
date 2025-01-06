#include <iostream>
#include <mpi.h>
using namespace std;

int main (int argc, char *argv[])
{
     MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        int a;
        cout << "Podaj liczbe do pobrania" << endl;
        cin >> a;
        MPI_Send(&a, 1, MPI_INT, 1, 102, MPI_COMM_WORLD);
        int b;
        MPI_Recv(&b, 1, MPI_INT, size-1, 102, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
        cout << "Proces " << rank << " odebral liczbe: " << b << endl;
    }
    if (rank == 1)
    {
        int b;
        MPI_Recv(&b, 1, MPI_INT, 0, 102, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
        cout << "Proces 1 odebral liczbe: " << b << endl;
        MPI_Send(&b, 1, MPI_INT, rank+1, 102, MPI_COMM_WORLD);
    }
    if (rank == size-1)
    {
        int b;
        MPI_Recv(&b, 1, MPI_INT, rank-1 ,102, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        cout << "Proces " << rank << " odebral liczbe: " << b << endl;
        MPI_Send(&b, 1, MPI_INT, 0, 102, MPI_COMM_WORLD);
    }
    if (rank < size-1 && rank > 1)
    {
        int b;
        MPI_Recv(&b, 1, MPI_INT, rank-1 ,102, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        cout << "Proces " << rank << " odebral liczbe: " << b << endl;
        MPI_Send(&b, 1, MPI_INT, rank+1, 102, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}