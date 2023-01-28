/*
Uporabom MPI izraditi simulaciju raspodjeljenog problema n filozofa. Pri svakoj promjeni program mora vizualno prikazati za sve flozofe
što oni rade. Npr. kada filozof 4 ide jesti tada bi ispis trebao izgledati ovako "Stanje filozofa: X o O X o" X - jede, O- razmišlja, o - ceka na vilice

Problem pet filozofa. filozofi obavljaju dvije različite aktivnosti: misle ili jedu. to rade na poseban način.
na jednom okruglom stolu nalazi se pet tanjura te pet vilica (izmedju svaka dva po jedan). filozof prilazi stolu, uzima lijevu vilicu 
pa desni te jede. Zatim vraća štapiće i odlazi misliti.

Program napisati korištenjem C++ funkcija.
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <mpi.h>

using namespace std;

vector<mutex> forks;
vector<string> states;

void philosopher(int id, unique_ptr<mutex[]> &forks, vector<string> &states)
{
    int count = 0;
    while(count < 10)
    {
        states[id] = "O";

        cout << "stanje filozofa " << id << ": ";
        for (int i = 0; i < states.size(); i++)
        {
            cout << states[i] << " ";
        }

        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));

        forks[id].lock();
        forks[(id + 1) % states.size()].lock();

        states[id] = "X";
        cout << "stanje filozofa: " << id << ": ";
        for (int i = 0; i < states.size(); i++)
        {
            cout << states[i] << " ";
        }

        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));

        forks[id].unlock();
        forks[(id+1) % states.size()].unlock();
        count++;
    }
}

int main(int argc, char* argv[])
{
    MPI::Init(argc, argv);

    int rank, size;
    rank = MPI::COMM_WORLD.Get_rank();
    size = MPI::COMM_WORLD.Get_size();

    // dynamically allocate array of mutexes
    std::unique_ptr<std::mutex[]> forks(new std::mutex[size]);
    vector<string> states(size, "0");

    vector<thread> threads;

    for (int i = rank; i < size; i += size)
    {
        threads.push_back(thread(philosopher, i, std::ref(forks), std::ref(states)));
    }

    for (auto& t : threads) 
    {
        t.join();
    }

    MPI::Finalize();
    return 0;
}