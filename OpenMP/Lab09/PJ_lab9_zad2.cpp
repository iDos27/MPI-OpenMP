#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;


int main() {
    int n;
    
    cout << "Podaj rozmiar wektora n: ";
    cin >> n;

    srand(time(NULL));

    double* a = new double[n];
    double* b = new double[n];
    double* c = new double[n];
    

    for(int i = 0; i < n; i++){
        a[i] = rand() % 11;
    }
    for(int i = 0; i < n; i++){
        b[i] = rand() % 11;
    }
    for(int i = 0; i < n; i++){
        c[i] = rand() % 11;
    }

    // Sekwencyjny
    double start_time = omp_get_wtime();

    for (int i = 0; i < n; ++i) {
        a[i] = c[i] + c[i];
        b[i] = b[i] + c[i];
    }

    double end_time = omp_get_wtime();
    cout << "Czas wykonania sekwencyjnego: " << end_time - start_time << " sekund." << endl;

    // Równoległy
    start_time = omp_get_wtime();
    
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < n; ++i) {
                a[i] = c[i] + c[i];
            }
        }
        #pragma omp section
        {
            for (int i = 0; i < n; ++i) {
                b[i] = b[i] + c[i];
            }
        }
    }
    end_time = omp_get_wtime();
    cout << "Czas wykonania równoległego: " << end_time - start_time << " sekund." << endl;



    // Czas równoległy (for)
    start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        a[i] = c[i] + c[i];
        b[i] = b[i] + c[i];
    }

     end_time = omp_get_wtime();
    cout << "Czas wykonania równoległego (for): " << end_time - start_time << " sekund." << endl;
}