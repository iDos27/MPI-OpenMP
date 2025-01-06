#include <iostream>
#include <mpi.h>

using namespace std;

int main (int argc, char *argv[])
{
     MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        int a = 10;
        MPI_Send(&a, 1, MPI_INT, 1, 102, MPI_COMM_WORLD);
    }
    if (rank == 1)
    {
        int b;
        MPI_Recv(&b, 1, MPI_INT, 0, 102, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
        cout << "Proces 1 odebral liczbe: " << b << endl;
    }
    MPI_Finalize();
    return 0;
}