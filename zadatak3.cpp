/*
Generirati proizvoljnu matricu. Distribuirati pojednin redak pojedinom procesu.
Formirati donju trokutastu matricu sa ispisom naziva procesa(računala) koji je ispisao 
pojedini redak. Program napisati korištenjem C++ funkcija.
*/

#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
    int rank, size;

    MPI::Request request;
    MPI::Status status;

    MPI::Init(argc, argv);

    rank = MPI::COMM_WORLD.Get_rank();      
    size = MPI::COMM_WORLD.Get_size();      

    int matrix[size][size];
    int a[size];

    
    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                matrix[i][j] = i*j;
            }
        }
    }

    MPI::COMM_WORLD.Scatter(matrix, size, MPI::INT, a, size, MPI::INT, 0); 
    
    if (rank != 0)
    {
        MPI::COMM_WORLD.Recv(matrix, 1, MPI::INT, rank-1, 10);
    }

    cout << "Proces " << rank << ": ";

    for (int i = 0; i <= rank; i++)
    {
        cout << a[i] << " ";
    }

    cout << endl;

    if (rank < size - 1)
    {
        MPI::COMM_WORLD.Send(matrix, 1, MPI::INT, rank + 1, 10);
    }

    MPI::Finalize();
    return 0;
}