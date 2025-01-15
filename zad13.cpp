// 10.2
// 11.3

// ZADANIE 10.2
#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

double f(double x) {
    return sqrt(1 - x * x);
}

double trapezoidal_integrationSeq(double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        integral += f(a + i * h);
    }

    integral *= h;
    return integral;
}

double trapezoidal_integration(double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.5 * (f(a) + f(b));

    #pragma omp parallel for reduction(+:integral)
    for (int i = 1; i < n; ++i) {
        integral += f(a + i * h);
    }

    integral *= h;
    return integral;
}

double calculate_pi(int n) {
    double a = -1.0;
    double b = 1.0;
    double area = trapezoidal_integration(a, b, n);
    return 2 * area;
}
double calculate_piSeq(int n) {
    double a = -1.0;
    double b = 1.0;
    double area = trapezoidal_integrationSeq(a, b, n);
    return 2 * area;
}
int main() {
    int n;
    cout << "Podaj liczbę przedziałów całkowania: ";
    cin >> n;

    if (n <= 0) {
        cerr << "Liczba przedziałów musi być dodatnia." << endl;
        return 1;
    }
    //omp_set_num_threads(4); 

    double tBegin = omp_get_wtime();
    double pi_seq = calculate_piSeq(n);
    double tEnd = omp_get_wtime();
   // cout << "Przybliżenie liczby π: " << pi_seq << endl;
// Obliczenie przyspieszenia i efektywności
    double tSeq = tEnd - tBegin;
    cout << "Czas sekwencyjny: " << tSeq << endl;

    tBegin = omp_get_wtime();
    double pi_approximation = calculate_pi(n);
    tEnd = omp_get_wtime();
   // cout << "Przybliżenie liczby π: " << pi_approximation << endl;
    double tPar = tEnd - tBegin;
    cout << "Czas rownolegly: " << tPar << endl;
    //tseq/rpar
    double speedup = (tSeq) / (tPar);
    // speedup / threads
    double efficiency = speedup / 8;   
    cout << "Przyspieszenie row: " << speedup << endl;
    cout << "Efektywność row: " << efficiency << endl;

    return 0;
}
