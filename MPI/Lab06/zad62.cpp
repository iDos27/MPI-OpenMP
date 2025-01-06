#include <iostream>
#include <cmath>
#include <mpi.h>

using namespace std;

double f(double x) {
    return sqrt(1 - x * x);
}

double trapezoidalPartial(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += f(x);
    }
    return h * sum;
}

double trapezoidalSequential(double a, double b, int n) {
    return trapezoidalPartial(a, b, n); // Sekwencyjna wersja jest taka sama jak równoległa, ale na jednym procesie
}

int main(int argc, char* argv[]) {
    int rank, size, n, local_n;
    double a = -1.0, b = 1.0;
    double local_a, local_b;
    double local_result, total_result;
    double tBegin, tEnd;
    double totalResultSeq;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Wczytywanie liczby przedziałów tylko w procesie 0
    if (rank == 0) {
        cout << "Podaj liczbę przedziałów całkowania: ";
        cin >> n;
        tBegin = MPI_Wtime();  // Start pomiaru czasu dla równoległego
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Sekwencyjne obliczenie na procesie 0
    if (rank == 0) {
        double tSeqBegin = MPI_Wtime(); // Start pomiaru czasu dla wersji sekwencyjnej
        totalResultSeq = trapezoidalSequential(a, b, n);
        double tSeqEnd = MPI_Wtime();
        cout << "Sekwencyjny wynik: " << totalResultSeq << endl;
        cout << "Czas obliczeń sekwencyjnych: " << (tSeqEnd - tSeqBegin) << " sekund" << endl;
    }

    // Podział zadań na procesy w wersji równoległej
    local_n = n / size;
    local_a = a + rank * local_n * ((b - a) / n);
    local_b = local_a + local_n * ((b - a) / n);

    local_result = trapezoidalPartial(local_a, local_b, local_n);

    MPI_Reduce(&local_result, &total_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Zakończenie pomiaru czasu dla równoległej wersji
    if (rank == 0) {
        total_result *= 2;
        tEnd = MPI_Wtime();  // Czas zakończenia pomiaru dla równoległego

        cout << "Przybliżona wartość liczby π: " << total_result << endl;
        cout << "Liczba procesów: " << size << endl;
        cout << "Liczba przedziałów: " << n << endl;
        cout << "Czas obliczeń równoległych: " << (tEnd - tBegin) << " sekund" << endl;

        // Wyliczenie przyspieszenia i efektywności
        double speedup = (tEnd - tBegin) / (tEnd - tBegin);
        double efficiency = speedup / size;
        
        cout << "Przyspieszenie: " << speedup << endl;
        cout << "Efektywność: " << efficiency << endl;
    }

    MPI_Finalize();
    return 0;
}
