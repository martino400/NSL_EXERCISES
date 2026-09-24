/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Random__
#define __Random__

// This class contains functions for generating random numbers using the RANNYU algorithm
class Random
{

private:
  int m1, m2, m3, m4, l1, l2, l3, l4, n1, n2, n3, n4;

protected:
public:
  // Default constructor
  Random();
  // Destructor
  ~Random();
  // Method to set the seed for the RNG
  void SetRandom(int *, int, int);
  // Method to save the seed to a file
  void SaveSeed();
  // Method to generate a random number in the range [0,1)
  double Rannyu(void);
  // Method to generate a random number in the range [min,max)
  double Rannyu(double min, double max);
  // Method to generate a random number with a Gaussian distribution
  double Gauss(double mean, double sigma);
  // Method to generate a number for exercize 2.1.2 for the importance sampling
  double es2punto1importance();
  // Method to generate 1 or -1 for the RandomWalk
  int up_or_down();
  // Method to decide on which dimension take the step of the RW
  int one_two_three();
  // Method to generate a number between 0 a PI, for the thetha coordinate
  double theta() { return Rannyu(0, M_PI); }; //ottimo tra l'altro perchè è esattamente [0,2PI) che a livello matematico è proprio quello che si vuole fare
  // Method to generate a number between 0 a PI, for the thetha coordinate
  double phi() { return Rannyu(0, 2*M_PI); }; //ottimo tra l'altro perchè è esattamente [0,2PI) che a livello matematico è proprio quello che si vuole fare
};

#endif // __Random__

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
