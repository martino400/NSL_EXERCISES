#include "funzioni.h"
#include "random.h"

using namespace std;

//----------- Exercises 1.1.1 and 1.1.2 and 1.1.3 -----------
int main(int argc, char *argv[])
{

   // --------- Excercise 1.1.1 ----------

   Random rnd{};
   init_gen(rnd);

   // number of random variables found
   const int M = 100000;
   // number of blocks
   const int N = 100;
   // Number of variables per block
   int L = (M / N);

   // vector with numbers from [0,1)
   vector<double> r{};

   // we are calculating the definition of average value from 0 to 1
   // with uniform distribution from 0 to 1
   Fill_Casual(r, rnd, M);
   datablocking(M, L, N, r, "../DATA/Data1.1.1.dat");

   // --------- Excercise 1.1.2 ----------

   r.clear();

   for (int i = 0; i < M; i++)
   {
      r.push_back(pow(rnd.Rannyu() - 0.5, 2));
   }
   rnd.SaveSeed();

   datablocking(M, L, N, r, "../DATA/Data1.1.2.dat");
   datablocking(M, int(M/N), N, r, "../DATA/db_try.dat");
   

   // --------- Excercise 1.1.3 ----------

   vector<double> Chi_value;

   int n_trials = 100; 
   Chi_test(rnd, Chi_value, n_trials);

   ofstream out; 
   out.open("../DATA/Chivalue.dat");
   
   for(int i=0; i<Chi_value.size(); i++)
   {
      //i-esimo Chi valore 
      out << i << "\t" << Chi_value[i] << endl;
   }
   out.close();

   //faccio di nuovo per N_trials = 1000

   n_trials = 1000;

   //clear del vettore se no ne prendo 1100 anzichè 1000
   Chi_value.clear();

   Chi_test(rnd, Chi_value, n_trials);

   out.open("../DATA/Chivalue_more.dat");

   for(int i=0; i<Chi_value.size(); i++)
   {
      //i-esimo Chi valore 
      out << i << "\t" << Chi_value[i] << endl;
   }
   out.close();

   n_trials = 10000;

   //clear del vettore se no ne prendo 1100 anzichè 1000
   Chi_value.clear();

   Chi_test(rnd, Chi_value, n_trials);

   out.open("../DATA/Chivalue_10K.dat");

   for(int i=0; i<Chi_value.size(); i++)
   {
      //i-esimo Chi valore 
      out << i << "\t" << Chi_value[i] << endl;
   }
   out.close();

   return 0;
}

// per usare il python ogni volta che mi connetto devo fare

// module load python3/anaconda

// a questo punto ho caricato tutto quello che mi serve compreso il comando jupyter notebook
