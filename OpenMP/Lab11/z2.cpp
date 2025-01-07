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

// Wielkosc
const int N = 500;

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

void dzialanieSekwencyjne() {
    for(int i = 0; i < 5; i++) {
        //mF = (sB * mF) - mA;
        matrixMullScalar(mF, sB, temp);
        matrixOddMatrix(temp, mA, mF);

        //sA = wA * wD;
        vectorMullVector(wA, wD, sA);
        
        //wC = (sA * wB) + wA;
        scalarMullVector(sA, wB, temp);
        vectorAddVector(temp, wA, wC);
        
        // wD = mF * wC;
        matrixMullVector(mF, wC, wD);
        
        //sB += wD * wC;
        vectorMullVector(wD, wC, temp);
        sB += temp;
    }
}

int main() {

}