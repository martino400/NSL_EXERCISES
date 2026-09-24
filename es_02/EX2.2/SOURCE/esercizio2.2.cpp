#include "funzioni.h"
#include "random.h"
#include "RandomWalk.h"

using namespace std;

int main(int argc, char *argv[])
{

    Random rnd{};
    inizializza_generatore_random(rnd);

    // for the throws and later on data blocking
    const int M = 10000; // M throws
    const int N = 100;   // N_blocks
    int L = int(M / N);

    const int tot_length = 100;

    cout << "PARAMETERS FOR RANDOM WALK" << endl;
    cout << "Number of single values  = " << M << endl << "Number of blocks  = " << N << endl << "Total distance = " << tot_length << endl;

    RandomWalk RW{};

    vector<double> measurements;
    ofstream out;
    out.open("data/ValuesRWCubic.dat");
    for (int j = 1; j <= tot_length; j++)
    {
        measurements.clear();
        for (int i = 0; i < M; i++)
        {
            RW.Restart();
            measurements.push_back(RW.RandomWalkCubic(j, rnd));
        }

        // gli passo l'array di tutti i moduli quadri per una stessa lunghezza (dimensione M)
        // fa il data blocking e fa l'ultimo progress del data blocking

        datablocking(M, L, N, j, measurements, out);
    }

    cout << " RandomWalk for descrete lattice completed ----------- √ " << endl;
    out.close();
    out.open("data/ValuesRWContinuos.dat");

    RW.RandomWalkContinuous(10, rnd);
    for (int j = 1; j <= 100; j++)
    {
        measurements.clear();
        for (int i = 0; i < M; i++)
        {
            RW.RestartContinuos(); // FIX fondamentale
            measurements.push_back(RW.RandomWalkContinuous(j, rnd));
        }

        // gli passo l'array di tutti i moduli quadri per una stessa lunghezza (dimensione M)
        // fa il data blocking e fa l'ultimo progress del data blocking

        datablocking(M, L, N, j, measurements, out);
    }
    cout << " RandomWalk for continous lattice completed ------------ √ " << endl;

    return 0;
}
