#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //sleep(rank);

    srand(time(0) + rank);

    double random_number = static_cast<double>(rand()) / RAND_MAX * 10;
    cout << "Proces " << rank << " wylosowal liczbe: " << random_number << endl;

    double max_number = rank;
    MPI_Reduce(&random_number, &max_number, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 1) {
        cout << "Maksymalna liczba to: " << max_number << endl;
    }

    MPI_Finalize();
    return 0;
}