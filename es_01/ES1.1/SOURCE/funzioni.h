#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "random.h"
#include <algorithm>

using namespace std;

void init_gen(Random &rnd)
{
   int seed[4];
   int p1, p2;
   
   // 1. Lettura dei Primes
   ifstream Primes("Primes.txt");
   if (Primes.is_open())
   {
      Primes >> p1 >> p2;
   }
   else {
      cerr << "PROBLEM: Unable to open Primes" << endl;
      return; // Meglio fermarsi se manca un pezzo fondamentale
   }
   Primes.close();

   // 2. Lettura del Seed
   ifstream input("seed.in");
   string property;
   if (input.is_open())
   {
      // CORREZIONE BUG: Il while corretto in C++ per leggere file
      while (input >> property) 
      {
         if (property == "RANDOMSEED")
         {
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            
            // STAMPA DI DEBUG: Ti mostra cosa sta realmente leggendo
            cout << "DEBUG - Inizializzo RNG con:" << endl;
            cout << "Primes: " << p1 << ", " << p2 << endl;
            cout << "Seed: " << seed[0] << " " << seed[1] << " " << seed[2] << " " << seed[3] << endl;
            
            rnd.SetRandom(seed, p1, p2);
         }
      }
      input.close();
   }
   else {
      cerr << "PROBLEM: Unable to open seed.in" << endl;
   }
}

double errore(vector<double> AV, vector<double> AV2, int n)
{
   // se sono al primo step fisso l'errore tra quel blocco e un altro a zero
   // questo perchè non ho discostamento tra il datablocco e sè stesso e questa cosa la devo imporre
   if (n == 0)
   {
      return 0;
   }
   else
   {
      return sqrt((AV2[n] - AV[n] * AV[n]) / n);
   }
}

// --------------- DATA BLOCKING ------------
void datablocking(int M, int L, int N, vector<double> &f_wo_importance, string nomefile)
{
   // calcolo di A_i

   string percorso = nomefile;

   double somma = 0;
   int cella = 0;
   vector<double> average(N);
   vector<double> average2(N);
   for (int esterno = 0; esterno < N; esterno++)
   {
      somma = 0;
      for (int interno = 0; interno < L; interno++)
      {
         cella = (esterno * L) + interno;
         somma += f_wo_importance[cella];
      }
      average[esterno] = (somma / L);
   }

   // medie cumulative
   vector<double> medie_cum(N);
   vector<double> medie_cum2(N);
   vector<double> errore_progressivo(N);

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

   // sulla prima colonna ho il N di blocco - sulla seconda colonna ho le medie cumulative - sulle terza colonna ho gli errori

   ofstream out;
   out.open(percorso);
   for (int i = 0; i < N; i++)
   {
      out << i + 1 << "\t" << medie_cum[i] << "\t" << errore_progressivo[i] << endl;
   }
   out.close();
}

// Fill a vector with M numbers each found randomly in [0; 1)
void Fill_Casual(vector<double> &r, Random &rnd, const int &M)
{
   for (int i = 0; i < M; i++)
   {
      r.push_back(rnd.Rannyu());
   }
   rnd.SaveSeed();
}

// Conta quanti numeri ci sono nell'intervallino [i*h ; (i+1)*h]
double count_interval(const double &h, int j, vector<double> &random_number)
{
   int conta = 0;
   for (int i = 0; i < random_number.size(); i++)
   {
      if (j * h <= random_number[i] && random_number[i] <= (j + 1) * h)
      {
         conta++;
      }
   }
   return conta;
}

//-------------- Chi-test -------------
void Chi_test(Random &rnd, vector<double> &Chi_value, int n_trials)
{
   // come funziona la funzione

   // 1. divido il mio intervallo da 0 a 1 in 100 intervallini uguali più piccoli e distinti

   // 2. estraggo 10000 numeri tra 0 e 1 e poi faccio un ciclo che mi conta quanti numeri sono caduti dentro ciascun intervallino.

   // 3. faccio questa cosa per 100 volte

   // 4. returna il vettore con le 100 Chi

   // Parameters for Chi-function
   const int N_blocks = 100;              // number of blocks
   const double h = 1.0 / 100.0;              // larghezza intervallo
   const int N_of_random_numbers = 10000; // Number of random numbers per vector
   double expected = (double)N_of_random_numbers / (double)N_blocks;


   vector<double> random_number; // random number vector
   double sum_for_Chi = 0;
   for (int j = 0; j < n_trials; j++)
   {
      random_number.clear();
      Fill_Casual(random_number, rnd, N_of_random_numbers);
      sum_for_Chi = 0;
      for (int i = 0; i < N_blocks; i++)
      {
         sum_for_Chi += pow(count_interval(h, i, random_number) - expected, 2) / expected;      
      }
      Chi_value.push_back(sum_for_Chi);
   }
}
