#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
MPI_Init(&argc, &argv);

int rank, size, n;
double max_random_number, local_max;

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

if (rank == 0) {
cout << "Podaj rozmiar tablicy n: " << endl;
cin >> n;
}

MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


sleep(rank);

double* random_numbers = new double[n];

srand(static_cast<double>(time(0)) + rank);

for (int i = 0; i < n; ++i) {
random_numbers[i] = static_cast<double>(rand() % 11);
}

cout << "Proces numer: " << rank << " wylosował liczby: ";
for (int i = 0; i < n; ++i) {
cout << random_numbers[i] << " ";
}
cout << endl;

local_max = random_numbers[0];
for (int i = 1; i < n; ++i) {
if (random_numbers[i] > local_max) {
local_max = random_numbers[i];
}
}

if (rank == 0) {
max_random_number = local_max;
for (int i = 1; i < size; ++i) {
double received_max;
MPI_Recv(&received_max, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
if (received_max > max_random_number) {
max_random_number = received_max;
}
}
cout << "Maksymalna liczba wylosowana w procesach wynosi: " << max_random_number << endl;
} else {
MPI_Send(&local_max, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}


delete[] random_numbers;

MPI_Finalize();
return 0;
}