/*
Svaki procesor ima niz podataka od 100 elemenata (vektor). Svaki od tih lokalnih vektora je dio jednog većeg, globalnog. Potrebno je izračunati 
srednju vrijednost svih elemenata u globalnom vektoru. Preferirani postupak jest taj, da se podaci daju matičnom procesu
pomoću MPI::Comm.Gather funkcije, zbroje elementi dobivenog vektora, podijele s ukupnim brojem elemenata i rezultat se pošalje
svim procesima. Napišite program koji će izvršiti naveden
postupak u C++ programskom jeziku.
*/

#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) 
{
    MPI::Init(argc, argv);

    int size = MPI::COMM_WORLD.Get_size();

    int rank = MPI::COMM_WORLD.Get_rank();

    double local_sum = 0;
    double global_sum = 0;
    vector<double> local_data(100);
    vector<double> global_data(size*100);

    for (int i = 0; i < 100; i++) {
        local_data[i] = i+rank*100;
    }


    for (int i = 0; i < 100; i++) {
        local_sum += local_data[i];
    }

    double local_avg = local_sum / 100;
    cout << "Proces ranka: " << rank << " ima prosjek " << local_avg << endl;


    MPI::COMM_WORLD.Gather(local_data.data(), 100, MPI::DOUBLE, global_data.data(), 100, MPI::DOUBLE, 0);

    if (rank == 0) {
        for(int i = 0; i < global_data.size(); i++ )
            global_sum += global_data[i];
        double global_avg = global_sum / (100 * size);
        cout << "Prosjek globalnog vektora: " << global_avg << endl;
    }

    MPI::Finalize();
    return 0;
}