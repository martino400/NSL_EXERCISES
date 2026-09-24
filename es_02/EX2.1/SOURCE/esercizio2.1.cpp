#include "funzioni.h"
#include "random.h"

using namespace std;

int main(int argc, char *argv[])
{

    Random rnd{};
    inizializza_generatore_random(rnd);
    const int M = 100000;
    const int N = 100;

    int L = (M / N);

    // vectors that contain respectively the values of the integral calculated with and without importance sampling
    vector<double> f_wo_importance;
    vector<double> integral_importance;

    double random = 0;
    for (int i = 0; i < M; i++)
    {
        f_wo_importance.push_back(coseno_esercizio2punto1(rnd.Rannyu()));
        random = rnd.es2punto1importance();
        integral_importance.push_back(coseno_esercizio2punto1(random) / function_importance(random));
    }
    rnd.SaveSeed();
    datablocking(M, L, N, f_wo_importance, "../DATA/Data2.1.1.dat");
    datablocking(M, L, N, integral_importance, "../DATA/Data2.1.2.dat");

    return 0;
}
 