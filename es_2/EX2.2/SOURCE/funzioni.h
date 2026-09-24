#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

void inizializza_generatore_random(Random &rnd);

double errore(vector<double> AV, vector<double> AV2, int n);

void inizializza_generatore_random(Random &rnd)
{
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes.txt");
   if (Primes.is_open())
   {
      Primes >> p1 >> p2;
   }
   else
      cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open())
   {
      while (!input.eof())
      {
         input >> property;
         if (property == "RANDOMSEED")
         {
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed, p1, p2);
         }
      }
      input.close();
   }
   else
      cerr << "PROBLEM: Unable to open seed.in" << endl;
}

double errore(vector<double> AV, vector<double> AV2, int n)
{
   if (n == 0)
   {
      return 0;
   }
   else
   {
      return sqrt((AV2[n] - AV[n] * AV[n]) / n);
   }
}

// --------------- DATA BLOCKING for RANDOM WALK ------------ //
// I only need the last value, so I don't need to put the values on the file
void datablocking(int M, int L, int N, int N_steps, vector<double> &RandomWalk, ofstream &out)
{
   // calcolo di A_i
   double somma = 0;
   int cella = 0;
   vector<double> average(N);

   for (int esterno = 0; esterno < N; esterno++)
   {
      somma = 0;
      for (int interno = 0; interno < L; interno++)
      {
         cella = esterno * L + interno;
         somma += RandomWalk[cella];
      }
      average[esterno] = sqrt(somma / L);
   }

   vector<double> medie_cum(N, 0.0);
   vector<double> medie_cum2(N, 0.0);
   vector<double> errore_progressivo(N, 0.0);

   for (int esterno = 0; esterno < N; esterno++)
   {
      for (int interno = 0; interno < esterno + 1; interno++)
      {
         medie_cum[esterno] += average[interno];
         medie_cum2[esterno] += pow(average[interno], 2);
      }
      medie_cum[esterno] /= (esterno + 1);
      medie_cum2[esterno] /= (esterno + 1);
      errore_progressivo[esterno] = errore(medie_cum, medie_cum2, esterno);
   }

   // stampa su file l'ultimo blocco siccome è quello che tiene conto della media di tutti i blocchi
   out << N_steps << "\t" << medie_cum[medie_cum.size() - 1] << "\t" << errore_progressivo[errore_progressivo.size() - 1] << endl;
}
