#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "random.h"
#include <algorithm>

using namespace std;

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
   //se sono al primo step fisso l'errore tra quel blocco e un altro a zero
   //questo perchè non ho discostamento tra il datablocco e sè stesso e questa cosa la devo imporre
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
