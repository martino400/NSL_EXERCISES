#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "random.h"
#include <algorithm>
#include <functional> // Necessario per std::function

using namespace std;

struct Needle
{
   // x and y coordinate of the front of the needle that indicate it's orientation
   double x, y;
   // x_coordinate of starting point of the vector
   double x_centre;

   // LENGTH OF NEEDLE: L = 1
};

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

//------- BUFFON EXPERIMENT -------

// function returns true if the line was touched by the needle and false if the line wasn't
bool NeedleThrow(Random &rand, double &d)
{
   Needle trial;
   bool check = false;

   // Genera il centro
   trial.x_centre = rand.Rannyu(0.0, d);
   
   // --- METODO ACCEPT-REJECT ---
   double norm2;
   do {
       trial.x = rand.Rannyu(-1.0, 1.0);
       trial.y = rand.Rannyu(-1.0, 1.0);
       norm2 = trial.x * trial.x + trial.y * trial.y;
   } while (norm2 > 1.0 || norm2 == 0.0); // Scarta i punti fuori dal cerchio

   // Ora puoi normalizzare in modo sicuro
   double norm = sqrt(norm2);
   trial.x /= norm;
   trial.y /= norm;

   // Controllo l'intersezione (ricordando che L = 1, la proiezione sull'asse x è trial.x)
   if (trial.x_centre + trial.x > d || trial.x_centre + trial.x < 0.0)
   {
      check = true;
   }
   
   return check;
}
// function returns true if the line was touched by the needle and false if the line wasn't
double experiment(int N_tries, Random &rnd, double d)
{
   int N_hit = 0;
   for (int i = 0; i < N_tries; i++)
   {
      bool check = NeedleThrow(rnd, d);
      if (check == true)
      {
         N_hit++;
      }
   }
   // Ritorna gli hit, non pi greco!
   return double(N_hit); 
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
// Aggiorna la firma per accettare N_tries e d
void datablocking(int M, int L, int N, vector<double> &f_wo_importance, string nomefile, int N_tries, double d)
{
   string percorso = nomefile;
   double somma = 0;
   int cella = 0;
   
   // Questo vettore conterrà le stime di pi greco PER BLOCCO
   vector<double> pi_block(N);
   
   for (int esterno = 0; esterno < N; esterno++)
   {
      somma = 0;
      for (int interno = 0; interno < L; interno++)
      {
         cella = (esterno * L) + interno;
         somma += f_wo_importance[cella]; // Ora questa è la somma degli hit del blocco
      }
      
      // I lanci totali nel blocco sono (N_tries * L)
      double total_throws_in_block = double(N_tries) * double(L);
      
      // Calcoliamo pi greco UNA SOLA VOLTA per l'intero blocco
      pi_block[esterno] = (2.0 / d) * total_throws_in_block / somma;
   }

   // medie cumulative (da qui usi pi_block invece di average)
   vector<double> medie_cum(N);
   vector<double> medie_cum2(N);
   vector<double> errore_progressivo(N);

   for (int esterno = 0; esterno < N; esterno++)
   {
      for (int interno = 0; interno < esterno + 1; interno++)
      {
         // ATTENZIONE QUI: usa pi_block[interno]
         medie_cum[esterno] += pi_block[interno];
         medie_cum2[esterno] += pow(pi_block[interno], 2);
      }
      medie_cum[esterno] /= (esterno + 1);
      medie_cum2[esterno] /= (esterno + 1);
      errore_progressivo[esterno] = errore(medie_cum, medie_cum2, esterno);
   }

   ofstream out;
   out.open(percorso);
   for (int i = 0; i < N; i++)
   {
      out << i + 1 << "\t" << medie_cum[i] << "\t" << errore_progressivo[i] << endl;
   }
   out.close();
}