/*
Za slanje ranka procesa 0 upotrjebljen je MPI::Comm.Bcast svim procesima unutar komunikatora.
Nakon primitka procesi vraćaju procesu 0 naziv računala koje je primilo poruku.
Proces 0 prima poruku od ostalih procesa MPI::Comm.Irecv.
Ispisati na zaslon da su primljene poruke na strani procesa 0.
program napisati korištenjem C++ funkcija
*/

#include <mpi.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    MPI::Init(argc, argv);
    int rank, size;
    rank = MPI::COMM_WORLD.Get_rank();            
    size = MPI::COMM_WORLD.Get_size();      

    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length;
    MPI_Get_processor_name(hostname, &hostname_length);     
    string hostname_str(hostname, hostname_length);


    if (rank == 0) {
        MPI::COMM_WORLD.Bcast(&hostname_str[0], hostname_str.length() + 1, MPI::CHAR, 0);
        for (int i = 1; i < size; i++) {
            string recv_hostname(MPI_MAX_PROCESSOR_NAME,' ');
            MPI::COMM_WORLD.Recv(&recv_hostname[0], recv_hostname.length() + 1, MPI::CHAR, i, 0);
            cout << "Proces je dobio poruku od procesa: " << i << " sa host-a " << recv_hostname << endl;
        }
    } else {
        MPI::COMM_WORLD.Bcast(&hostname_str[0], hostname_str.length() + 1, MPI::CHAR, 0);
        MPI::COMM_WORLD.Send(&hostname_str[0], hostname_str.length() + 1, MPI::CHAR, 0, 0);
    }

    MPI::Finalize();
    return 0;
}