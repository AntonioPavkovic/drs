/*
Razmak između uzastopnih prostih brojeva 2 i 3 je samo 1, dok je razmak između uzastopnih prostih brojeva 7 i 11 je 4. Napiši paralelni 
program za određivanje svih cijelih brojeva manjih od 1.000.000, sa najvećim razmakom između para uzastopnih prostih
brojeva. Napišite program koji će izvršiti naveden postupak u C++
programskom jeziku.
*/

#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;


vector<int> func(int num)
{
    vector<int> primes;
    vector<bool> isPrime(num + 1, true);

    for (int i = 2; i <= num; i++) 
    {
        if (isPrime[i]) 
        {
            primes.push_back(i);
            for (int j = 2 * i; j <= num; j += i) 
            {
                isPrime[j] = false;
            }
        }
    }

    return primes;
}

int main(int argc, char* argv[]) 
{
    int rank, size;

    MPI::Init(argc, argv);
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

 
    int n = 1000000;
    vector<int> primes = func(n);

    int maxGap = 0;
    for (int i = 1; i < primes.size(); i++) 
    {
        int gap = primes[i] - primes[i - 1];
        maxGap = max(maxGap, gap);
    }

    int globalMaxGap;
    MPI::COMM_WORLD.Reduce(&maxGap, &globalMaxGap, 1, MPI::INT, MPI::MAX, 0);

    if (rank == 0) 
    {
        cout << "Najveci razmak izmedju dva uzastopna prosta broja manja od: " << n << " je " << globalMaxGap << endl;
    }
    MPI::Finalize();
    return 0;
}
