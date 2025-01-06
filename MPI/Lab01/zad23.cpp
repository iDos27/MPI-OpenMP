#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

//wywala blad jak za malo procesow
    if (size < 2) {
        if (rank == 0) {
            cerr << "Program wymaga co najmniej 2 procesów.\n";
        }
        MPI_Finalize();
        return -1;
    }

    const char* filename = "zad2.txt";
    int array_size = 0;
    vector<double> array;

    if (rank == 0) {
        // Proces 0: wczytuje dane z pliku
        ifstream file(filename);

        file >> array_size;
        array.resize(array_size);

        for (int i = 0; i < array_size; ++i) {
            file >> array[i];
        }

        file.close();
    //Proces 0 wysyła rozmiar
        MPI_Send(&array_size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    //Proces 0 wysyła dane
        MPI_Send(array.data(), array_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    } 
    if (rank == 1) 
    {
        // Proces 1: odbiera rozmiar i dane
        MPI_Recv(&array_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        array.resize(array_size);
        MPI_Recv(array.data(), array_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        double sum = 0.0;
        for (const auto& val : array) {
            sum += val;
        }

        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) 
    {

        double sum;
        MPI_Recv(&sum, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Suma elementów tablicy: " << sum << endl;
    }

    MPI_Finalize();
    return 0;
}