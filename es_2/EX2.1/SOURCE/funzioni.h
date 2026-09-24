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

double coseno_esercizio2punto1(double x)
{
   double f = (M_PI / 2.0) * cos((M_PI * x / 2.0));
   return f;
}



double function_importance(double x)
{
   double d_x = 2.0*(1.0-x);
   return d_x;
}




// --------------- DATA BLOCKING ------------ //
void datablocking(int M, int L, int N, vector<double> &f_wo_importance, string nomefile)
{
   // calcolo di A_i
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
   out.open(nomefile);
   for (int i = 0; i < N; i++)
   {
      out << i + 1 << "\t" << medie_cum[i] << "\t" << errore_progressivo[i] << endl;
   }
   out.close();
}
