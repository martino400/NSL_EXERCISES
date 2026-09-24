/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __System__
#define __System__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <armadillo>
#include <stdlib.h> //exit
#include "random.h"

using namespace std;
using namespace arma;

class Variation_MC
{
private:
  const int _ndim = 1;    // Dimensionality of the system
  double x_old;           // Old particle position
  double x_new;           // New particle position
  int _nblocks;           // Number of blocks for block averaging
  int _nsteps;            // Number of simulation steps in each block
  int _nattempts;         // Number of attempted moves
  int _naccepted;         // Number of accepted moves
  double acceptance;      // Probability of accepting the MC move
  double _step;           // Step for the Metropolis
  double _effective_step; // Effective step used for Metropolis
  double _mu, _sigma;     // Parameters of the $/psi$ function
  double H_T;             // H_T single value obtained from the metropolis step
  int _nprop;             // Number of properties to measure

  // if _nprop = 1 --> measure only H_T
  // else ... boh vedo se ci sarà bisogno di fare altro

  // Properties
  int prop = 1;     // Number of properties being measured
  vec _block_av;    // Block averages of properties
  vec _global_av;   // Global averages of properties
  vec _global_av2;  // Squared global averages of properties
  vec _average;     // Average values of properties
  vec _measurement; // Measured values of properties

public:                             // Function declarations
  void step_metropolis(double step) // set step for the metropolis simulation
  {
    _step = step;
  };
  double get_step() { return _step; }             // return _step
  void initialize();                              // Initialize step, RNG, N_block and N_steps
  int get_nbl();                                  // Get the number of blocks
  int get_nsteps();                               // Get the number of steps in each block
  void block_reset(int blk);                      // Reset block averages
  void measure();                                 // Measure properties of the system
  vector<double> averages(int blk);               // Compute averages of properties
  double error(double acc, double acc2, int blk); // Compute error
  bool metro();                                   // Perform Metropolis acceptance-rejection step
  double psi(double x);                           // Wave - function value
  double calculate();                             // Calculates <H_T>
  double Laplacian(double x);                     // Calculates the Laplacian for the trial Wave-function (psi... look above)
  double get_posizione() { return x_old; };       // Gets the value of the position
  int counterSA_step=0;                                   // Number of simulated annealing steps, change
  int _SA_acc=0;                                    // Number of simulated annealing accepted steps
  int N_iterations=0;                               // Number of total iterations


  //L'ho promosso a public in modo da poter accedervi dal main
  Random _rnd;                                    // Random number generator

  // FUNCTIONS FOR MU AND SIGMA
  void set_mu(double a) { _mu = a; };       // Initializes the value of mu for the Gaussian Distributions
  void set_sigma(double a) { _sigma = a; }; // Initializes the value of sigma for the Gaussian Distributions
  double get_mu() { return _mu; };          // Returns value for mu;
  double get_sigma() { return _sigma; };    // Returns value for sigma;

  // Generate casuale number Gaussian
  double Gauss(double t) { return _rnd.Gauss(0, t); };
  double casual() { return _rnd.Rannyu(-1.0, 1.0); }; // generate a casual number between [-1.0; 1.0]
  void global_reset();                                // Reset all the accumulators
};

#endif // __System__

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
