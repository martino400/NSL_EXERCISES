/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include "variation_mc.h"

using namespace std;
// Simulated annealing step
void SA_step(Variation_MC &SYS, vector<double> &results);
// Function to initialize files
void file_initialize(ofstream &couts);
// Write data on files
void write_data(Variation_MC &SYS, double &initial_energy, double &initial_error, ofstream &couts);
// Percentage of the SA simulation done
void percentage(double &frac, double &frac_old, Variation_MC &SYS);
// Write on file Simulated Annealing informations
void SA_info(double &final_temp, double &initial_temp, Variation_MC &SYS, double &alfa);

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    cout << "Insert  '--- EXECUTABLE ---' ; '--- Final Temperature --- ' ; '--- Initial Temperature --- '" << endl;
    return 1;
  }
  // Temperatura finale da raggiungere e iniziale
  double final_temp;
  double initial_temp;
  final_temp = stod(argv[1]);
  initial_temp = stod(argv[2]);
  cout << endl
       << "------ TEMPERATURES SAVED --------" << endl;
  cout << "Final temperature =       " << final_temp << endl;
  cout << "Initial temperature =     " << initial_temp << endl;

  // ------------------  EXERCISE 8 ------------------
  int nconf = 1;
  Variation_MC SYS;

  // initial configuration of the parameters sigma and mu from 'input.dat'
  SYS.initialize();
  SYS.block_reset(0);

  // initialize streams
  ofstream couts;
  ofstream coutp;

  //-- -- -- -SIMULATED ANNEALING-- -- -- -
  // variabili di appoggio per il Simulated Annealing
  double sigma = 0, mu = 0;
  // result vector for confronting the energy
  vector<double> results = {0.0, 0.0};
  // variabili di appoggio
  double initial_energy = 0, initial_error = 0;
  // Initialize the SA counters
  SYS._SA_acc = 0, SYS.counterSA_step = 0;
  // Variation parameters for sigma and mu (control how much MU and SIGMA can change)
  double variation_mu = 1.0, variation_sigma = 1.0;
  // Decrease constant
  double alfa = 0.997;
  cout << endl
       << "Freezing constant =   " << alfa << endl;
  // Rapporto per quantificare il progresso della simulazione
  double frac = 0.0;
  double frac_old = 0.0;

  // Calculate the number of cycles necessary for the simulation
  SYS.N_iterations = log(final_temp / initial_temp) / log(alfa);
  cout << endl
       << "Number of iterations =   " << SYS.N_iterations << endl;

  // create files of simulation
  file_initialize(coutp);

  double new_mu = 0.0;
  SYS.counterSA_step = 0;
  SA_info(final_temp, initial_temp, SYS, alfa);
  SA_step(SYS, results);

  while (initial_temp > final_temp)
  {
    if (SYS.counterSA_step == 0)
    {
      SA_step(SYS, results);
      initial_energy = results[0];
      initial_error = results[1];
    }
    // Tiro giù i valori di mu e sigma
    sigma = SYS.get_sigma();
    mu = SYS.get_mu();
    new_mu = mu + variation_mu * SYS._rnd.Gauss(0, initial_temp);

    if (new_mu <= 0)
    {
      SYS.set_mu(mu);
    }
    else
    {
      SYS.set_mu(new_mu);
    }
    SYS.set_sigma(abs(sigma + variation_sigma * SYS._rnd.Gauss(0, initial_temp)));

    // Calcolo la nuova energia
    SA_step(SYS, results);
    // 4. Metropolis
    if (SYS._rnd.Rannyu() < exp(-(1.0 / initial_temp) * (results[0] - initial_energy)))
    {
      SYS._SA_acc++;
      initial_energy = results[0];
      initial_error = results[1];
    }
    else
    {
      // save the old variables
      SYS.set_sigma(sigma);
      SYS.set_mu(mu);
    }
    write_data(SYS, initial_energy, initial_error, coutp);
    percentage(frac, frac_old, SYS);

    // Raffreddamento (in questo modo diventa sempre più piccolo ma sempre più lentamente)
    initial_temp = double(initial_temp * alfa);
    SYS.counterSA_step++;
  }
  ofstream info;
  info.open("../OUPUT/SA_info.dat");
  info << endl
       << "Number of iterations =   " << SYS.N_iterations << endl;
  info << "Number accepted moves = " << SYS._SA_acc << endl;
  cout << "Number accepted moves = " << SYS._SA_acc << endl;
  return 0;
}

//  Simulated annealing step --> to see if this is a good configuration
void SA_step(Variation_MC &SYS, vector<double> &results)
{
  ofstream histo;
  histo.open("../OUTPUT/histo.dat");
  SYS.global_reset();
  SYS.block_reset(0);
  histo << "X of particles" << endl;
  for (int i = 0; i < SYS.get_nbl(); i++)
  { // loop over blocks
    for (int j = 0; j < SYS.get_nsteps(); j++)
    { // loop over steps in a block
      histo << setw(12) << SYS.get_posizione() << setw(12) <<  endl;
      SYS.calculate();
    }
    // sovrascrivo a ogni ciclo in modo che alla fine salva quelli dell'ultimo blocco

    // remember : results[0] = cumulative energy value
    // remember : results[1] = cumulative energy error

    results = SYS.averages(i + 1);
    SYS.block_reset(i + 1);
    histo.close();
  }
}
#include <iomanip> // Assicurati di includere questa libreria per setw

void file_initialize(ofstream &couts)
{
  couts.open("../OUTPUT/energy_sigma_mu.dat");
  
  // Usa setw anche per l'intestazione per allinearla perfettamente ai dati
  couts << setw(15) << "# Block" 
        << setw(15) << "VALUE" 
        << setw(15) << "ERROR_VALUE" 
        << setw(15) << "MU" 
        << setw(15) << "SIGMA" << endl;
        
  couts.close();
  cout << "\n---------- All files initialized -------" << endl;
  cout << "\nYou will find the data in \n  -) '../OUTPUT/energy_sigma_mu.dat\n"
       << endl;
}

void write_data(Variation_MC &SYS, double &initial_energy, double &initial_error, ofstream &couts)
{
  // Metti ios::app altrimenti sovrascrivi ad ogni step!
  // Nota: ho uniformato il nome del file a energy_sigma_mu.dat come nell'inizializzazione
  couts.open("../OUTPUT/energy_sigma_mu.dat", ios::app); 
  
  // Applica setw() immediatamente prima di OGNI variabile, senza inserire spazi manuali
  couts << setw(15) << SYS.counterSA_step 
        << setw(15) << initial_energy 
        << setw(15) << initial_error  
        << setw(15) << SYS.get_mu() 
        << setw(15) << SYS.get_sigma() << endl;
        
  couts.close();
}
void percentage(double &frac, double &frac_old, Variation_MC &SYS)
{
  frac = double(SYS.counterSA_step * 100.0) / double(SYS.N_iterations);

  if (int(frac) % 10 == 0 && int(frac_old) != int(frac))
  {
    cout << int(frac) << "% done!" << endl;
  }
  frac_old = frac;
}
void SA_info(double &final_temp, double &initial_temp, Variation_MC &SYS, double &alfa)
{
  ofstream info;
  info.open("../OUTPUT/SA_info.dat");
  info << endl
       << "------ TEMPERATURES SAVED --------" << endl;
  info << "Final temperature =       " << final_temp << endl;
  info << "Initial temperature =     " << initial_temp << endl;
  info << endl
       << "Freezing constant =   " << alfa << endl;
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
