#include "funzioni.h"
#include "random.h"

using namespace std;

//----------- Exercises 1.2 -----------
int main(int argc, char *argv[])
{

   // --------- Excercise 1.2 ----------

   Random rnd{};
   inizializza_generatore_random(rnd);

   // number of random variables found
   const int M = 100000;
   //allocate array of the number of sums
   int n_sums[4] = {1, 2, 10, 100};

   double somma = 0;


   //For Dice distribution
   ofstream out;
   for(int i=0; i<4; i++)
   {
      string nomefile = "../DATA/Dice" + to_string(n_sums[i]) + ".dat";
      out.open(nomefile);
      for(int j=0; j<M; j++)
      {
         somma=0;
         for(int k=0; k<n_sums[i]; k++)
         {
            somma += rnd.Dice();
         }
         out << somma/n_sums[i] << endl;
      }
      out.close();
   }

   //For Exponential

   for(int i=0; i<4; i++)
   {
      string nomefile = "../DATA/Exponential" + to_string(n_sums[i]) + ".dat";
      out.open(nomefile);
      for(int j=0; j<M; j++)
      {
         somma=0;
         for(int k=0; k<n_sums[i]; k++)
         {
            somma += rnd.Exp(1.0);
         }
         out << somma/n_sums[i] << endl;
      }
      out.close();
   }

   //For Lorentian
   for(int i=0; i<4; i++)
   {
      string nomefile = "../DATA/Lorentian" + to_string(n_sums[i]) + ".dat";
      out.open(nomefile);
      for(int j=0; j<M; j++)
      {
         somma=0;
         for(int k=0; k<n_sums[i]; k++)
         {
            somma += rnd.Cauchy_Lorentz(1.0,0);
         }
         out << somma/n_sums[i] << endl;
      }
      out.close();
   }
   
   



   return 0;
}
