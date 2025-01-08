#include <iostream>
#include <ctime>
#include <cstdlib>
#include <omp.h>

using namespace std;
/*
 Zalozenia
 Wykonaj 5 razy:
    Macierz F = (skalar B * macierz F) - macierz A
    skalar a = wektor a * wektor d
    wektor c = skalar a * wektor b + wektor a
    wektor d = Macierz F * wektor c
    skalar B += wektor d * c
*/
// Uzupelnienie macierzy, wektora i skalara
void fillRandom(double** matrix, double* vector, double& scalar, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() & 10;
        }
        vector[i] = rand() % 10;
    }
    scalar = rand() % 10;
}
// Uzupelnienie 2 vectorow
void fillVector(double* vec1, double* vec2, int size) {
    for (int i = 0; i < size; i++) {
        vec1[i] = rand() % 10;
        vec2[i] = rand() % 10;
    }
}


// Sekwencyjne
void dzialanieSekwencyjne(double** mA, double** mF, double* wA, double* wB, double* wC,
                          double* wD, double& sA, double& sB, int size) {
    for (int iter = 0; iter < 5; iter++) {
        // Operacja 1: F = (skalar B * F) - A
        for (int i = 0; i < size; i++) {
            //mTemp[i] = new double[size];
            //mResult[i] = new double[size];
            for (int j = 0; j < size; j++) {
                //mTemp[i][j] = mF[i][j] * sB;
                mF[i][j] = (mF[i][j] * sB) - mA[i][j];
            }
        }

        // Operacje na wektorach i skalarach
        for (int i = 0; i < size; i++) {
            sA += wA[i] * wD[i];
        }
        for (int i = 0; i < size; i++) {
            wC[i] = sA * wB[i] + wA[i];
        }

        for (int i = 0; i < size; i++) {
            wD[i] = 0.0;
            for (int j = 0; j < size; j++) {
                wD[i] += mF[i][j] * wC[j];
            }
        }
        for (int i = 0; i < size; i++) {
            sB += wD[i] * wC[i];
        }
    }
}

// Rownolegle dzialanie
void dzialanieRownolegle(double** mA, double** mF, double* wA, double* wB, double* wC, double* wD,
                         double& sA, double& sB, int size) {
    for (int iter = 0; iter < 5; iter++) {
        // Operacja 1: F = (skalar B * F) - A
        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                mF[i][j] = (mF[i][j] * sB) - mA[i][j];
            }
        }
        // Operacje na wektorach i skalarach
        #pragma omp parallel
        {
            #pragma omp for reduction(+:sA)
            for (int i = 0; i < size; i++) {
                sA += wA[i] * wD[i];
            }

            #pragma omp for
            for (int i = 0; i < size; i++) {
                wC[i] = sA * wB[i] + wA[i];
            }

            #pragma omp for
            for (int i = 0; i < size; i++) {
                wD[i] = 0.0;
                for (int j = 0; j < size; j++) {
                    wD[i] += mF[i][j] * wC[j];
                }
            }

            #pragma omp for reduction(+:sB)
            for (int i = 0; i < size; i++) {
                sB += wD[i] * wC[i];
            }
        }
    }
}


int main() {
    int size = 5000;
    // Tworzenie macierzy
    double** matrixA = new double*[size];
    double** matrixF = new double*[size];
    for (int i = 0; i < size; i++) {
        matrixA[i] = new double[size];
        matrixF[i] = new double[size];
    }
    // Tworzenie wktorow
    double* vectorA = new double[size];
    double* vectorB = new double[size];
    double* vectorC = new double[size];
    double* vectorD = new double[size];
    // Tworzenie skalarow
    double scalarA, scalarB;
    // Inicjalizacja
    srand(time(0));
    fillRandom(matrixA, vectorA, scalarA, size);
    fillRandom(matrixF, vectorB, scalarB, size);
    fillVector(vectorC, vectorD, size);
    // Sekwencyjne
    double start_time = omp_get_wtime();
    dzialanieSekwencyjne(matrixA, matrixF, vectorA, vectorB, vectorC, vectorD, scalarA, scalarB, size);
    double end_time = omp_get_wtime();
    cout << "Czas sekwencyjny: " << (end_time - start_time) << " s" << endl;
    // Rownolegle obliczenia
    start_time = omp_get_wtime();
    dzialanieRownolegle(matrixA, matrixF, vectorA, vectorB, vectorC, vectorD, scalarA, scalarB, size);
    end_time = omp_get_wtime();
    cout << "Czas rownolegly: " << (end_time - start_time) << " s" << endl;
    // Czyszczenie pamieci
    for (int i = 0; i < size; i++) {
        delete[] matrixA[i];
        delete[] matrixF[i];
    }
    delete[] matrixA;
    delete[] matrixF;

    delete[] vectorA;
    delete[] vectorB;
    delete[] vectorC;
    delete[] vectorD;

    return 0;
}