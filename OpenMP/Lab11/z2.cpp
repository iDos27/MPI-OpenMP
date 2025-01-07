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
void fillRandom(double** matrix, double* vector, int scalar, int size){
    for(int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            matrix[i][j] = rand() % 10;
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
void matrixMullScalar(double mat[N][N], double scalar, double result[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = mat[i][j] * scalar;
        }
    }
}

// Mnozenie wektorow
void vectorMullVector(double vec1[N], double vec2[N], double result) {
    result = 0.0;
    for (int i = 0; i < N; ++i) {
        result += vec1[i] * vec2[i];
    }
}
// Mnozenie wektora przez skalar
void scalarMullVector(double scalar, double vector[N], double result[N]) {
    for (int i = 0; i < N; i++) {
        result[i] = vector[i] * scalar;
    }
}
// Mnożenie macierzy przez wektor
void matrixMullVector(double mat[N][N], double vec[N], double result[N]) {
    for (int i = 0; i < N; ++i) {
        result[i] = 0.0;
        for (int j = 0; j < N; ++j) {
            result[i] += mat[i][j] * vec[j];
        }
    }
}
// Odejmowanie macierzy
void matrixOddMatrix(double mat1[N][N], double mat2[N][N], double result[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = mat1[i][j] - mat2[i][j];
        }
    }
}
// Dodawanie skalara do wektora
void scalarAddVector() {

}
// Dodawanie dwoch wektorow
void vectorAddVector(double vec1[N], double vec2[N], double result[N]) {
    for (int i = 0; i < N; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
}

void dzialanieSekwencyjne(double** mA,double** mF,double** mTemp, double* wA,double* wB, double* wC, double* wD,double* wTemp,double sA, double sB, double sTemp) {
    for(int i = 0; i < 5; i++) {
        //mF = (sB * mF) - mA;
        matrixMullScalar(mF, sB, mTemp);
        matrixOddMatrix(mTemp, mA, mF);

        //sA = wA * wD;
        vectorMullVector(wA, wD, sA);
        
        //wC = (sA * wB) + wA;
        scalarMullVector(sA, wB, wTemp);
        vectorAddVector(wTemp, wA, wC);
        
        // wD = mF * wC;
        matrixMullVector(mF, wC, wD);
        
        //sB += wD * wC;
        vectorMullVector(wD, wC, wTemp);
        sB += wTemp;
    }
}

void dzialanieRownolegle(double** mA,double** mF,double** mTemp, double* wA,double* wB, double* wC, double* wD,double* wTemp,double sA, double sB, double sTemp) {
    #pragma omp parallel for
    for (int i = 0; i < 5; i++) {
        matrixMullScalar(mF, sB, temp);
        matrixOddMatrix(temp, mA, mF);
        vectorMullVector(wA, wD, sA);
        scalarMullVector(sA, wB, temp);
        vectorAddVector(temp, wA, wC);
        matrixMullVector(mF, wC, wD);
        vectorMullVector(wD, wC, temp);
        sB += temp;
    }
}

int main() {
    int size = 5000;

    // Tworzenie macierzy
    double** matrixA = new double*[size];
    double** matrixF = new double*[size];
    double** matrixTemp = new double*[size];
    // Tworzenie wektorow
    double* vectorA = new double[size];
    double* vectorB = new double[size];
    double* vectorC = new double[size];
    double* vectorD = new double[size];
    double* vectorTemp = new double[size];
    // Tworzenie skalarów
    double scalarA;
    double scalarB;
    double scalarTemp;
    srand(time(0));
    fillRandom(matrixA, vectorA, scalarA, size);
    fillRandom(matrixF, vectorB, scalarB, size);
    fillRandom(matrixTemp, vectorC, scalarTemp, size);
    fillVector(vectorD, vectorTemp, size);
    // Obliczenia sekwencyjne
    double start_time = omp_get_wtime();
    dzialanieSekwencyjne(matrixA, matrixF, matrixTemp, vectorA, vectorB, vectorC, vectorD, vectorTemp, scalarA, scalarB, scalarTemp );
    double end_time = omp_get_wtime();
    double seq_time = end_time - start_time;
    cout << "Czas sekwencyjny: " << seq_time << " ms" << endl;

    string schedules[] = {"static", "dynamic", "guided"};
    for (const auto& schedule: schedules) {
        cout << "\nTest dla: " << schedule << endl;

        start_time = omp_get_wtime();
        dzialanieRownolegle(matrixA, matrixF, matrixTemp, vectorA, vectorB, vectorC, vectorD, vectorTemp, scalarA, scalarB, scalarTemp );
        end_time = omp_get_wtime();
        double row_time = end_time - start_time;
        cout << "Czas rownolegly " << schedule << ": " << row_time << "ms" << endl;
    }
    // Usuwanie 
}