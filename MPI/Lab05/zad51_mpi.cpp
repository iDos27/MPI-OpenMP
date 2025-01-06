#include <iostream>
#include <cmath>
#include <mpi.h>

double f(double x) {
    return 2 * sqrt(1 - x * x);
}

double trapezoidal_rule(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        double x_i = a + i * h;
        sum += f(x_i);
    }
    return h * sum;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    if (rank == 0) {
        std::cout << "Podaj liczbę przedziałów: ";
        std::cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double a = -1.0, b = 1.0;
    int local_n = n / size;
    double local_a = a + rank * local_n * (b - a) / n;
    double local_b = local_a + local_n * (b - a) / n;

    double local_result = trapezoidal_rule(local_a, local_b, local_n);

    double global_result;
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Obliczona wartość Pi: " << global_result << std::endl;
    }

    MPI_Finalize();
    return 0;
}
