#include "funzioni.h"
#include "random.h"
#include "data_analysis.h"
#include <cmath>
using namespace std;

int main(int argc, char *argv[])
{

    Random rnd{};
    inizializza_generatore_random(rnd);
    // data blocking data
    const int M = 100000; // number of data
    const int N = 100;    // number of blocks
    int L = (M / N);

    // Call and Put
    vector<double> C; // call
    vector<double> P; // put

    Black_Scholes_Direct(C, P, rnd, M);

    datablocking(M, L, N, C, "../DATA/CallDirect.dat");
    datablocking(M, L, N, P, "../DATA/PutDirect.dat");

    cout << "Black-Schole Put & Call Direct -------- √" << endl;

    // svuoto i vettori per poter poi fare Step by Step anzichè fare in modo diretto
    C.clear();
    P.clear();

    Black_Scholes_Step_by_Step(C, P, rnd, M);

    datablocking(M, L, N, C, "../DATA/CallStepStep.dat");
    datablocking(M, L, N, P, "../DATA/PutStepStep.dat");

    cout << "Black-Schole Put & Call Direct -------- √" << endl;

    return 0;
}
