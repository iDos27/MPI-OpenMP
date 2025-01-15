// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ZADANIA ZALEŻNE EGZAMIN WAŻNE 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!`

#include <iostream>
#include <omp.h>
#include <algorithm>

using namespace std;

void fillRandom(double* a, double* b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }
}
double sekwencyjne(double* a, double* b, int size) {
    double x = *min_element(a, a + size);
    cout << "x: " << x << endl;
    double y = *max_element(b, b + size);
    cout << "y: " << y << endl;
    double result = 10 * x + y;
    return result;
}

double rownolegle(double* a, double* b, int size) {
    double x, y, result;
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task depend (in:a, size) depend(out:x)
            {
                x = *min_element(a, a + size);
                cout << "x: " << x << endl;
            }
            #pragma omp task depend (in:b, size) depend(out:y)
            {
                y = *max_element(b, b + size);
                cout << "y: " << y << endl;
            }
        }
        #pragma omp taskwait
        #pragma omp single
        {
            #pragma omp task depend (in:x,y) depend(out:result)
            {
                result = 10 * x + y;
            }
        }
    }
    return result;
}

int main() {
    int size = 20;
    double* a = new double[size];
    double* b = new double[size];

    srand(static_cast<unsigned int>(time(0)));

    fillRandom(a, b, size);

    double start = omp_get_wtime();
    double result = sekwencyjne(a, b, size);
    double end = omp_get_wtime();

    cout << "Sekwencyjnie: " << result << endl;
    cout << "Czas wykonania: " << end - start << "s" << endl;

    start = omp_get_wtime();
    result = rownolegle(a, b, size);
    end = omp_get_wtime();

    cout << "Rownolegle: " << result << endl;
    cout << "Czas wykonania: " << end - start << "s" << endl;

    delete[] a;
    delete[] b;

    return 0;
}