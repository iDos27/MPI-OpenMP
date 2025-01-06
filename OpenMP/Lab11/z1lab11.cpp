#include <iostream>
#include <ctime>
#include <omp.h>
#include <cstdlib>

using namespace std;

void fillRandom(int** matrix, int* vector, int size){
    for(int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            matrix[i][j] = rand() % 10;
        }
        vector[i] = rand() % 10;
    }
}

void fillTrojkat(int** matrix, int size) {
    for (int i = 0; i < size; ++i){
        for (int j = i; j < size; ++j){
            matrix[i][j] = rand() % 10;
        }
        for (int j = 0; j < i; ++j)
        {
            matrix[i][j] = 0;
        }
    }
}

void mnozenieSekwencyjne(int** matrix, int* vector, int* result, int size) {
    for (int i = 0; i < size; i++) {
        result [i] = 0;
        for (int j = 0; j < size; ++j) {
            result[i] += matrix[i][j] * vector[i];
        }
    } 
}

void mnozenieRownolegle(int** matrix, int* vector, int* result, int size, const string& schedule_type) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++) {
        result [i] = 0;
        for (int j = 0; j < size; ++j) {
            result[i] += matrix[i][j] * vector[i];
        }
    }
}

int main(){
    int size = 5000;

    int** matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
    }

    int* vector = new int[size];
    int* result = new int[size];

    srand(time(0));
    fillRandom(matrix, vector, size);

    double start_time = omp_get_wtime();
    mnozenieSekwencyjne(matrix, vector, result, size);
    double end_time = omp_get_wtime();
    double seq_time_kw = end_time - start_time;
    cout << "Czas sekwencyjny: " << seq_time_kw << " ms" << endl;

    string schedules[] = {"static", "dynamic", "guided"};
    for (const auto& schedule : schedules) {
        cout << "\nTest dla: " << schedule << endl;

        start_time = omp_get_wtime();
        mnozenieRownolegle(matrix, vector, result, size, schedule);
        end_time = omp_get_wtime();
        double row_time_kw = end_time - start_time;
        cout << "Czas rownolegly " << schedule << ": " << row_time_kw << "ms" << endl;
        cout << "Przyspieszenie " << schedule << ": " << seq_time_kw / row_time_kw << endl;
        cout << "Efektywnosc " << schedule << ": " << (seq_time_kw / row_time_kw) / 16 << endl;

    }
////////////////////////////////////////////////////////////////////////
    int** upperMatrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        upperMatrix[i] = new int[size];
    }
    fillTrojkat(upperMatrix, size);

    start_time = omp_get_wtime();
    mnozenieSekwencyjne(upperMatrix, vector, result, size);
    end_time = omp_get_wtime();
    double seq_time_tr = end_time - start_time;

    cout << "\nCzas sekwencyjny tr: " << seq_time_tr << " ms" << endl;

    for (const auto& schedule : schedules) {
        cout << "\nTest dla: " << schedule << endl;

        start_time = omp_get_wtime();
        mnozenieRownolegle(matrix, vector, result, size, schedule);
        end_time = omp_get_wtime();
        double row_time_tr = end_time - start_time;
        cout << "Czas rownolegly tr " << schedule << ": " << row_time_tr << "ms" << endl;
        cout << "Przyspieszenie " << schedule << ": " << seq_time_tr / row_time_tr << endl;
        cout << "Efektywnosc " << schedule << ": " << (seq_time_tr / row_time_tr) / 16 << endl;
    }
    for (int i = 0; i < size; ++i){
        delete[] upperMatrix[i];
    }
    delete[] upperMatrix;
    




    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vector;
    delete[] result;

    return 0;
}