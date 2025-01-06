#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char**argv){

    srand(time( NULL));

    int n;
    cout << "Podaj n" << endl;
    cin >> n;

    double* x = new double[n];
    double* y = new double[n];

    // Losu losu
    for(int i = 0; i < n; i++){
        x[i] = rand() % 11;
    }

    cout << "Koniec losowania tablicy x" << endl;
    for(int i = 0; i < n; i++){
        cout << x[i] << " ";
    }
    cout << endl;
    #pragma omp parallel sections
    {
        // Sekcja 1 - zapisywanie do pliku
        #pragma omp section
        {
            ofstream outFile("out.txt");
            
            if (outFile.is_open()) {
                for (int i = 0; i < n; i++) {
                    outFile << x[i] << " ";
                }
                outFile.close();
                cout << "Zapisano wektor x do pliku " << endl;
            } else {
                cerr << "Nie udało się otworzyć pliku do zapisu!" << endl;
            }
        }
        // Sekcja 2 - Obliczenia
        #pragma omp section
        {
            for (int i = 0; i < n; i++)
            {
                y[i] = sin(x[i]) + log(x[i] + 1);
            }
            cout << "Obliczenia dla wektora y zakonczone!" << endl;
        }
    }
    cout << "Wektor y:" << endl;
    for (int i = 0; i < n; i++) {
        cout << y[i] << " ";
    }
    cout << endl;

    // Zwalnianie pamięci
    delete[] x;
    delete[] y;

    return 0;
}