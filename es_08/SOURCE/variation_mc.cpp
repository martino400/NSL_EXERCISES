/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <cmath>
#include <cstdlib>
#include <string>
#include "variation_mc.h"

using namespace std;
using namespace arma;

void Variation_MC ::initialize()
{ // Initialize the System object according to the content of the input files in the ../INPUT/ directory

  int p1, p2; // Read from ../INPUT/Primes a pair of numbers to be used to initialize the RNG
  ifstream Primes("../INPUT/Primes");
  Primes >> p1 >> p2;
  Primes.close();
  int seed[4]; // Read the seed of the RNG
  ifstream Seed("../INPUT/seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  _rnd.SetRandom(seed, p1, p2);

  // ----- prepare file -----

  ofstream couta("../OUTPUT/acceptance.dat"); // Set the heading line in file ../OUTPUT/acceptance.dat
  couta << "#   N_BLOCK:  ACCEPTANCE:" << endl;
  couta.close();

  ofstream coutp("../OUTPUT/energy.dat");
  coutp << "#   BLOCK:  ACTUAL_PE:     PE_AVE:      ERROR:" << endl;
  coutp.close();

  ifstream input("../INPUT/input.dat"); // Start reading ../INPUT/input.dat
  ofstream coutf;
  coutf.open("../OUTPUT/output.dat");
  string property;
  double delta;
  while (!input.eof())
  {
    input >> property;
    if (property == "STEP")
    {
      input >> _step;
    }
    else if (property == "BLOCKS")
    {
      input >> _nblocks;
      coutf << "BLOCKS= " << _nblocks << endl;
    }
    else if (property == "N_STEPS")
    {
      input >> _nsteps;
      coutf << "NSTEPS= " << _nsteps << endl;
    }
    else if (property == "MU")
    {
      input >> _mu;
      coutf << "MU= " << _mu << endl;
    }
    else if (property == "SIGMA")
    {
      input >> _sigma;
      coutf << "SIGMA= " << _sigma << endl;
    }
    else if (property == "NPROP")
    {
      input >> _nprop;
      coutf << "NPROP" << _nprop << endl;
    }
    else if (property == "ENDINPUT")
    {
      coutf << "Reading input completed!" << endl;
      break;
    }
    else
      cerr << "PROBLEM: unknown input" << endl;
  }
  input.close();
  _measurement.resize(_nprop);
  _average.resize(_nprop);
  _block_av.resize(_nprop);
  _global_av.resize(_nprop);
  _global_av2.resize(_nprop);
  _average.zeros();
  _global_av.zeros();
  _global_av2.zeros();
  _nattempts = 0;
  _naccepted = 0;
  // initialize particle positions to a known value to avoid using uninitialized memory
  x_old = 0.0;
  x_new = 0.0;
  //initialize the SA step parameters
  counterSA_step=0;                                   
  _SA_acc=0;                                    
  N_iterations=0;                                

}

bool Variation_MC ::metro()
{ // Metropolis algorithm

  // reset everything
  bool decision = false;
  double p_new = 0, p_old = 0;
  acceptance = 0, _effective_step = 0;

  _effective_step = _step * _rnd.Rannyu(-1, 1);
  p_new = psi(x_old + _effective_step) * psi(x_old + _effective_step);
  p_old = psi(x_old) * psi(x_old);
  acceptance = p_new / p_old;

  // in this way x_old and x_new remain unchanged and will be changed
  // in case the move is accepted
  if (_rnd.Rannyu() < acceptance)
  {
    decision = true; // Metropolis acceptance step
    _naccepted++;
  }
  return decision;
}

// Calculates <H_T>
double Variation_MC ::calculate()
{
  ofstream out;
  bool metro_result = false;
  metro_result = metro();
  _nattempts++;
  H_T = 0;
  // propose the new position from the old one
  x_new = x_old + _effective_step;
  // accept or reject the move: if accepted, update x_old to the proposed x_new
  if (metro_result == true)
  {
    x_old = x_new;
  }
  else
  {
    // rejected: keep x_old (x_new may be ignored)
    x_new = x_old;
  }
  // local energy (do not multiply by acceptance)
  double kinetic = -0.5 * Laplacian(x_old) / psi(x_old);
  double potential = pow(x_old, 4.0) - 5.0 / 2.0 * pow(x_old, 2.0);
  H_T = kinetic + potential;
  this->measure();
  return H_T;
}

double Variation_MC::psi(double x)
{
  return exp(-pow(x - _mu, 2.0) / (2 * pow(_sigma, 2))) + exp(-pow(x + _mu, 2.0) / (2 * pow(_sigma, 2)));
}

double Variation_MC::Laplacian(double x)
{
  return (-1 / pow(_sigma, 2.0)) * (-pow((x - _mu) / _sigma, 2.0) * exp(-pow((x - _mu) / _sigma, 2.0) / 2.0) - pow((x + _mu) / _sigma, 2.0) * exp(-pow((x + _mu) / _sigma, 2.0) / 2.0) + psi(x));
}

void Variation_MC ::block_reset(int blk)
{ // Reset block accumulators to zero
  ofstream coutf;
  if (blk > 0)
  {
    coutf.open("../OUTPUT/output.dat", ios::app);
    coutf << "Block completed: " << blk << endl;
    coutf.close();
  }
  _block_av.zeros();
  return;
}

/// Function helpful for the already initialized data-blocking
void Variation_MC ::measure()
{ // Measure properties
  _measurement.zeros();
  _measurement[0] = H_T;
  _block_av += _measurement; // Update block accumulators
  return;
}

// !!!!! ------- I changed the averages function in this way: -------- !!!!
// it returns the cumulative data blocking average OF THE ENERGY value
// it also returns it relative error
vector<double> Variation_MC ::averages(int blk)
{

  ofstream coutf;
  double average, sum_average, sum_ave2;
  _average = _block_av / double(_nsteps);
  _global_av += _average;
  _global_av2 += _average % _average; // % -> element-wise multiplication

  // ENERGY DATA-BLOCKING //////////////////////////////////////////////////////////
  coutf.open("../OUTPUT/energy.dat", ios::app);
  average = _average(0);
  sum_average = _global_av(0);
  sum_ave2 = _global_av2(0);
  coutf << setw(12) << blk
        << setw(12) << average
        << setw(12) << sum_average / double(blk)
        << setw(12) << this->error(sum_average, sum_ave2, blk) << endl;
  coutf.close();

  // ACCEPTANCE ////////////////////////////////////////////////////////////////
  double fraction;
  coutf.open("../OUTPUT/acceptance.dat", ios::app);
  if (_nattempts > 0)
    fraction = double(_naccepted) / double(_nattempts);
  else
    fraction = 0.0;
  coutf << setw(12) << blk << setw(12) << fraction << endl;
  coutf.close();

  // reset for each block
  _naccepted = 0.0;
  _nattempts = 0.0;

  vector<double> values;
  values.push_back(sum_average / double(blk));
  values.push_back(this->error(sum_average, sum_ave2, blk));

  return values;
}

double Variation_MC ::error(double acc, double acc2, int blk)
{
  if (blk <= 1)
    return 0.0;
  else
    return sqrt(fabs(acc2 / double(blk) - pow(acc / double(blk), 2)) / double(blk));
}

int Variation_MC ::get_nbl()
{
  return _nblocks;
}

int Variation_MC ::get_nsteps()
{
  return _nsteps;
}

void Variation_MC ::global_reset()
{
  _average.zeros();
  _global_av.zeros();
  _global_av2.zeros();
  _nattempts = 0;
  _naccepted = 0;
  // initialize particle positions to a known value to avoid using uninitialized memory
  x_old = 0.0;
  x_new = 0.0;
}
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
