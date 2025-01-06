#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Funkcja dla wersji sekwencyjnej sumy dwóch wektorów
void vectorSumSequential(double* A, double* B, double* C, int vector_size) {
    for (int i = 0; i < vector_size; i++) {
        C[i] = A[i] + B[i];
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double tBegin, tEnd;
    int vector_size;
    double *A = nullptr;
    double *B = nullptr;
    double *C = nullptr;

    double *local_A = nullptr;
    double *local_B = nullptr;
    double *local_C = nullptr;

    if (rank == 0) {
        cout << "Podaj rozmiar wektorów: ";
        cin >> vector_size;

        A = new double[vector_size];
        B = new double[vector_size];
        C = new double[vector_size];

        srand(time(0));

        // Inicjalizacja wektorów A i B
        for (int i = 0; i < vector_size; i++) {
            A[i] = 10.0 + (rand() / ((double)RAND_MAX) * 10.0);
        }

        for (int i = 0; i < vector_size; i++) {
            B[i] = 100.0 + (rand() / ((double)RAND_MAX) * 100.0);
        }

        // Pomiar czasu dla wersji sekwencyjnej
        double seqStartTime = MPI_Wtime();
        vectorSumSequential(A, B, C, vector_size);
        double seqEndTime = MPI_Wtime();
        cout << "Czas obliczeń sekwencyjnych: " << (seqEndTime - seqStartTime) << " sekund" << endl;
    }

    // Broadcast rozmiaru wektorów do wszystkich procesów
    MPI_Bcast(&vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int elements_per_process = vector_size / size;

    local_A = new double[elements_per_process];
    local_B = new double[elements_per_process];
    local_C = new double[elements_per_process];

    // Scatter danych wektorów A i B do procesów
    MPI_Scatter(A, elements_per_process, MPI_DOUBLE, local_A, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, elements_per_process, MPI_DOUBLE, local_B, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Równoległe obliczenie sumy wektorów
    for (int i = 0; i < elements_per_process; i++) {
        local_C[i] = local_A[i] + local_B[i];
    }

    // Gather wyników w procesie 0
    MPI_Gather(local_C, elements_per_process, MPI_DOUBLE, C, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Pomiar czasu dla wersji równoległej
        double parallelStartTime = MPI_Wtime();

        // Wersja równoległa już została wykonana (w wyniku MPI_Gather)

        double parallelEndTime = MPI_Wtime();
        cout << "Czas obliczeń równoległych: " << (parallelEndTime - parallelStartTime) << " sekund" << endl;

        // Obliczenie przyspieszenia i efektywności
        double speedup = (tEnd - tBegin) / (tEnd - tBegin);
        double efficiency = speedup / size;
        
        cout << "Przyspieszenie: " << speedup << endl;
        cout << "Efektywność: " << efficiency << endl;

    /*
        cout << "Wektor A: ";
        for (int i = 0; i < vector_size; i++) {
            cout << A[i] << " ";
        }
        cout << endl;

        cout << "Wektor B: ";
        for (int i = 0; i < vector_size; i++) {
            cout << B[i] << " ";
        }
        cout << endl;

        cout << "Wektor C (A + B): ";
        for (int i = 0; i < vector_size; i++) {
            cout << C[i] << " ";
        }
        cout << endl;
        */
    }

    // Zwolnienie pamięci
    delete[] local_A;
    delete[] local_B;
    delete[] local_C;
    if (rank == 0) {
        delete[] A;
        delete[] B;
        delete[] C;
    }

    MPI_Finalize();
    return 0;
}
