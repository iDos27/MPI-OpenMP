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

int main(int argc, char* argv[]) {
    int rank, size, n, local_n;
    double a = -1.0, b = 1.0;
    double local_a, local_b;
    double local_result, total_result;
    double tBegin, tEnd;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Podaj liczbę przedziałów całkowania: ";
        cin >> n;
        tBegin = MPI_Wtime(); 
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_n = n / size;
    local_a = a + rank * local_n * ((b - a) / n);
    local_b = local_a + local_n * ((b - a) / n);

    local_result = trapezoidalPartial(local_a, local_b, local_n);

    MPI_Reduce(&local_result, &total_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        total_result *= 2;
        tEnd = MPI_Wtime();  

        cout << "Przybliżona wartość liczby π: " << total_result << endl;
        cout << "Liczba procesów: " << size << endl;
        cout << "Liczba przedziałów: " << n << endl;
        cout << "Czas obliczeń: " << (tEnd - tBegin) << " sekund" << endl;
    }

    MPI_Finalize();
    return 0;
}
