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
void matrixMullScalar(double mat[N][N], double scalar, double result[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = mat[i][j] * scalar;
        }
    }
}

}
void vectorMullVector() {

}
void scalarMullVector() {

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
void vectorMullVector() {

}
void matrixOddMatrix() {

}
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
        

        //sA = wA * wD;
        //sA = vectorMullVector(wA, wD);
        
        //wC = (sA * wB) + wA;
        //wC = 
        
        // wD = mF * wC;
        matrixMullVector(mF, wC, wD);
        
        //sB += wD * wC;
        //sB += vectorMullVector(wD, wC);
    }
}

int main