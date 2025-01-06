#include <iostream>
#include <cmath>

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

int main() {
    int n;
    std::cout << "Podaj liczbę przedziałów: ";
    std::cin >> n;

    double a = -1.0, b = 1.0;
    double pi = trapezoidal_rule(a, b, n);

    std::cout << "Obliczona wartość Pi: " << pi << std::endl;

    return 0;
}