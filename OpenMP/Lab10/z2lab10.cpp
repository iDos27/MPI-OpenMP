#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

double f(double x) {
    return sqrt(1 - x * x);
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

int main() {
    int n;
    cout << "Podaj liczbę przedziałów całkowania: ";
    cin >> n;

    if (n <= 0) {
        cerr << "Liczba przedziałów musi być dodatnia." << std::endl;
        return 1;
    }

  
    omp_set_num_threads(4); 

    double pi_approximation = calculate_pi(n);
    cout << "Przybliżenie liczby π: " << pi_approximation << std::endl;

    return 0;
}
