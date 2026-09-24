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
#include <iomanip>
#include "system.h"

using namespace std;

void save_data(System &SYS, double current_temp);

void copy_paste();

void equilibrate(System &SYS);

void printProgressBar(int currentStep, int totalSteps, int barWidth = 50);


// ------------------------------ EXERCISE 6.1 ------------------------------ //
int main(int argc, char *argv[])
{
  int nconf = 1;
  System SYS;



  // Loop over temperatures
  double start_temp = 2.0;
  double end_temp = 0.5;
  double delta_temp = 0.1;
  int num_runs = (int)((start_temp - end_temp) / delta_temp) + 1;

  // OUTPUT files
  ofstream files;
  for (int i = 0; i < 4; i++)
  {
    files.open("../OUTPUT/TOTAL/file" + to_string(i) + ".dat");
    files << "#   TEMP:      #BLOCK     ACTUAL_VALUE:    ACC_AVE:   ERROR:" << endl;
    files.close();
  }
  files.open("../OUTPUT/TOTAL/file4.dat");
  files << "TEMP:   # BLOCK TAKEN:   ACCEPTED MC MOVES:" << endl;
  // ----------- LEGENDA -------------
  // 0 = suscettività,
  // 1 = internal,
  // 2 = calore specifico ,
  // 3 = magnetizzazione
  

  //---------------------- Loop over different temperatures --------------------
  for (int run = 0; run < num_runs; run++)
  {
    double current_temp = start_temp - run * delta_temp;
    // in modo da ripartire dalla configurazione precedente e non rifare da 0
    cout << "RUN = " << run << endl;
    if (run > 0) {SYS.set_temp_and_steps(current_temp);}
    //nel primo run è pari a 0 run e quindi coincide con quella di start, non dovrebbe portare a cambi assurdi
    // Change the parameters TEMP and RESTART
  
    SYS.initialize();
    SYS.initialize_properties();
    SYS.block_reset(0);
    equilibrate(SYS);
    for (int i = 0; i < SYS.get_nbl(); i++)
    { // loop over blocks
      printProgressBar(i, SYS.get_nbl());
      for (int j = 0; j < SYS.get_nsteps(); j++)
      { // loop over steps in a block
        SYS.step();
        SYS.measure();
      }
      SYS.averages(i + 1);
      SYS.block_reset(i + 1);
    }
    cout << "NSTEPS = " <<SYS.get_nsteps() << endl;
    SYS.finalize();

    //------------------------ Save measurements with respect to different temperatures in files --------------------
    save_data(SYS, current_temp);


  }

  return 0;
}

void equilibrate(System &SYS)
{
  cout << "------ EQUILIBRATION ----" << endl;
  SYS.initialize();
  SYS.initialize_properties();
  for (int i = 0; i < 10; i++)
  { // loop over blocks
    for (int j = 0; j < 1000; j++)
    { // loop over steps in a block
      SYS.step();
    }
  }
  SYS.finalize();

}

// Void function to save the data on the files
void save_data(System &SYS, double current_temp)
{
  // Nomi dei file di origine
  string dati[] = {"susceptibility.dat", "total_energy.dat", "specific_heat.dat", "magnetization.dat", "acceptance.dat"};
  ifstream get_data;
  ofstream save_data; // Usiamo un singolo oggetto ofstream che riutilizzeremo
  for (int i = 0; i < 5; i++)
  {
    string riga_corrente;
    string dato;
    get_data.open("../OUTPUT/" + dati[i]);
    save_data.open("../OUTPUT/TOTAL/file" + to_string(i) + ".dat", ios::app);
    // Controllo di sicurezza: verifichiamo che ENTRAMBI i file si siano aperti
    if (get_data.is_open() && save_data.is_open())
    {
      // Estraiamo l'ultima riga
      while (getline(get_data, riga_corrente))
      {
        dato = riga_corrente;
      }
      // Scriviamo l'ultima riga
      save_data << setw(12) << current_temp << " " << dato << endl;
    }
    else
    {
      cout << "Errore: impossibile aprire i file per " << dati[i] << endl;
    }
    get_data.close();
    save_data.close();
  }
}


void printProgressBar(int currentStep, int totalSteps, int barWidth) {
    // Calcola la frazione di completamento (da 0.0 a 1.0)
    float progress = static_cast<float>(currentStep) / totalSteps;
    
    // Calcola quanti caratteri della barra devono essere "riempiti"
    int pos = static_cast<int>(barWidth * progress);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) {
            std::cout << "="; // Parte completata
        } else if (i == pos) {
            std::cout << ">"; // Punta della freccia
        } else {
            std::cout << " "; // Parte mancante
        }
    }
    
    // Stampa la percentuale e il carattere \r per tornare all'inizio della riga
    std::cout << "] " << static_cast<int>(progress * 100.0) << " %\r";
    
    // Forza la stampa sulla console (necessario perché non stiamo usando std::endl)
    std::cout.flush();
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
