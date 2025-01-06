#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

int main(){
    int n;

    cout << "Podaj rozmiar tablicy: ";
    cin >> n;


    double* a = new double[n];

    for(int i = 0; i<n; i++){
        a[i] = i;
    }

    // Sekwwencyjny
    double sum = 0.0;
    double start_time = omp_get_wtime();

    for (int i = 0; i < n; i++){
        sum += a[i];
    }
    double result = sum/n;
    double end_time = omp_get_wtime();
    cout << "Czas wykonania sekwencyjnego: " << end_time - start_time << " sekund." << endl;
    cout << "Wynik sekwencyjny: " << result << endl;

    // Rownolegly 
    sum = 0.0;
    start_time = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp for
            for (int i = 0; i < n; i++){
            sum += a[i];
        }
    }
    result = sum/n;
    end_time = omp_get_wtime();
    cout << "Czas wykonania rownoleglego: " << end_time - start_time << " sekund." << endl;
    cout << "Wynik rownolegly: " << result << endl;


    // Rownolegly + sekcja krytyczna
    sum = 0.0;
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        double sumr = 0;
    #pragma omp for
        for (int i = 0; i < n; i++)
            sumr += a[i];
    #pragma omp critical
            sum += sumr;
    }
    result = sum/n;
    end_time = omp_get_wtime();
    cout << "Czas wykonania rownoleglego z sekcja krytyczna: " << end_time - start_time << " sekund." << endl;
    cout << "Wynik rownolegly z sekcja krytyczna: " << result << endl;
    
    
    // Rownolegly z klauzula reduction
    sum = 0.0;
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
    #pragma omp for reduction(+: sum)
    for (int i = 0; i < n; i++)
        sum += a[i];
    }
    result = sum/n;
    end_time = omp_get_wtime();
    cout << "Czas wykonania rownoleglego z klauzula reduction: " << end_time - start_time << " sekund." << endl;
    cout << "Wynik rownolegly z klauzula reduction: " << result << endl;


    delete[] a;
}