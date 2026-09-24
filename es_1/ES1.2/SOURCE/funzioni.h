#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "random.h"
#include <algorithm>
#include <functional> // Necessario per std::function

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



//l'idea è quella di fare un'unica funzione per cui in base a quello che gli 
//dico nell'input lui li estrae secondo esponenziale, lorentziana o standard

//da capire
// void distribution_on_files(int (&n_sums)[4], Random& rnd, int& M, string nome_dist)
// {
//    double somma=0;
//    ofstream out;
//    for (int i = 0; i < 4; i++)
//    {
//       string nomefile = "data/"+ nome_dist + to_string(n_sums[i]) + ".dat";
//       out.open(nomefile);
//       for (int j = 0; j < M; j++)
//       {
//          somma = 0;
//          for (int k = 0; k < n_sums[i]; k++)
//          {
//             somma += rnd.Dice();
//          }
//          out << somma / n_sums[i] << endl;
//       }
//       out.close();
//    }
// }
