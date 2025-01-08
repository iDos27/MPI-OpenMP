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
// Mnozenie macierzy przez skalar
double** matrixMullScalar(double** matrix, double scalar, int size) {
    double** result = new double*[size];
    for (int i = 0; i < size; i++) {
        result[i] = new double[size];
        for (int j = 0; j < size; j++) {
            result[i][j] = matrix[i][j] * scalar;
        }
    }
    return result;
}
// Odejmowanie macierzy
double** matrixOddMatrix(double** matrix1, double** matrix2, int size) {
    double** result = new double*[size];
    for (int i = 0; i < size; i++) {
        result[i] = new double[size];
        for (int j = 0; j < size; j++) {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
    return result;
}
// Mnozenie wektorow (iloczyn skalarny)
double vectorMullVector(double* vec1, double* vec2, int size) {
    double result = 0.0;
    for (int i = 0; i < size; i++) {
        result += vec1[i] * vec2[i];
    }
    return result;
}
// Mnozenie wektora przez skalar
double* scalarMullVector(double scalar, double* vector, int size) {
    double* result = new double[size];
    for (int i = 0; i < size; i++) {
        result[i] = vector[i] * scalar;
    }
    return result;
}
// Mnozenie macierzy przez wektor
double* matrixMullVector(double** mat, double* vec, int size) {
    double* result = new double[size];
    for (int i = 0; i < size; i++) {
        result[i] = 0.0;
        for (int j = 0; j < size; j++) {
            result[i] += mat[i][j] * vec[j];
        }
    }
    return result;
}
// Dodawanie dwoch wektorow
double* vectorAddVector(double* vec1, double* vec2, int size) {
    double* result = new double[size];
    for (int i = 0; i < size; i++) {
        result[i] = vec1[i] + vec2[i];
    }
    return result;
}

// Sekwencyjne
void dzialanieSekwencyjne(double** mA, double** mF, double* wA, double* wB, double* wC,
                          double* wD, double& sA, double& sB, int size) {
    double* wTemp;
    for (int i = 0; i < 5; i++) {
        // Operacja 1
        mF = matrixMullScalar(mF, sB, size);
        mF = matrixOddMatrix(mF, mA, size);

        // Operacja 2
        sA = vectorMullVector(wA, wD, size);
        wTemp = scalarMullVector(sA, wB, size);
        wC = vectorAddVector(wTemp, wA, size);
        //delete[] wC;
        //wC = wResult;

        // Operacja 3
        wD = matrixMullVector(mF, wC, size);

        // Operacja 4
        sB += vectorMullVector(wD, wC, size);
    }
    delete[] wTemp;
}

// Rownolegle dzialanie
void dzialanieRownolegle(double** mA, double** mF, double* wA, double* wB, double* wC, double* wD,
                         double& sA, double& sB, int size) {
    for (int iter = 0; iter < 5; iter++) {
        // Operacja 1: F = (skalar B * F) - A
        //double** mTemp = new double*[size];
        //double** mResult = new double*[size];
        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            //mTemp[i] = new double[size];
            //mResult[i] = new double[size];
            for (int j = 0; j < size; j++) {
                //mTemp[i][j] = mF[i][j] * sB;
                mF[i][j] = (mF[i][j] * sB) - mA[i][j];
            }
        }

        // Zwolnienie starego mF
        //#pragma omp parallel for
        //for (int i = 0; i < size; i++) {
        //    delete[] mF[i];
        //}
        //delete[] mF;
        //mF = mResult;

        // Czyszczenie tymczasowej macierzy mTemp
        //#pragma omp parallel for
        //for (int i = 0; i < size; i++) {
        //    delete[] mTemp[i];
        //}
        //delete[] mTemp;

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
    int size = 5;
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