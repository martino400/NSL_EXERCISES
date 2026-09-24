#include "funzioni.h"
#include "random.h"

using namespace std;

//----------- Exercises 1.2 -----------
int main(int argc, char *argv[])
{
   // --------- Excercise 1.2 ----------
   Random rnd{};
   inizializza_generatore_random(rnd);

   // -------- Parameters for experiment -------
   int N_tries = 100000;        // 10^5
   int N_experiments = 100000; // 10^5

   // -------- Parameters for data blocking ------
   int N_blocks = 100;
   int L = int(N_experiments / N_blocks);

   double d = 5; // Length of the vector
   vector<double> hit_values;
   for (int i = 0; i < N_experiments; i++)
   {
      hit_values.push_back(experiment(N_tries, rnd, d));
      cout << "Experiment n " << i + 1 << endl;
   }

   // Ora passiamo N_tries e d per calcolare pi greco dentro la funzione
   datablocking(N_experiments, L, N_blocks, hit_values, "../DATA/Buffon_5.dat", N_tries, d);

   return 0;
}
