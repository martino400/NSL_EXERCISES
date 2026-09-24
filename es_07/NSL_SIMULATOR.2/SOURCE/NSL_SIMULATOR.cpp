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
#include "system.h"

using namespace std;

void equilibrate(System &SYS);
void save_data(System &SYS, double current_temp);
void printProgressBar(int currentStep, int totalSteps, int barWidth);

const string PATH_FINDING_TEMP = "../../DATA/finding_temp.dat";


int main(int argc, char *argv[])
{
  int nconf = 1;
  System SYS;

  SYS.initialize();
  SYS.initialize_properties();
  SYS.block_reset(0);
  equilibrate(SYS);

  for (int i = 0; i < SYS.get_nbl(); i++)
  { // loop sui blocchi
    for (int j = 0; j < SYS.get_nsteps(); j++)
    { // loop sugli step nel blocco
      SYS.step();
      SYS.measure();
    }
    SYS.averages(i + 1);
    SYS.block_reset(i + 1);
    printProgressBar(i, SYS.get_nbl(), 50);
  }

  SYS.finalize(); // Chiude i file del blocco corrente
  return 0;
}

// -------------------------------------------------------------
// FUNZIONE equilibrate 
// -------------------------------------------------------------
void equilibrate(System &SYS)
{
  // RIMOSSI initialize(), initialize_properties() e finalize()
  // L'equilibrazione deve solo far evolvere temporalmente il sistema!

  cout << "Equilibrating..." << endl;
  for (int i = 0; i < 10000; i++) // 1000 step di equilibrazione
  {
    SYS.step();
  }
}

// -------------------------------------------------------------
// FUNZIONE save_data (salva i dati nel file in ../DATA) 
// -------------------------------------------------------------
void save_data(System &SYS, double current_temp)
{
  ifstream get_data;
  ofstream save_data_file;

  string riga_corrente;
  string ultima_riga_valida = "";

  get_data.open("../OUTPUT/temperature.dat");
  save_data_file.open(PATH_FINDING_TEMP, ios::app); // Usa lo stesso path definito in alto

  if (get_data.is_open() && save_data_file.is_open())
  {
    // Leggi tutto il file e tieni in memoria solo l'ultima riga
    while (getline(get_data, riga_corrente))
    {
      // Evita di catturare eventuali righe vuote a fine file
      if (!riga_corrente.empty())
      {
        ultima_riga_valida = riga_corrente;
      }
    }

    // Scriviamo l'ultima riga UNA SOLA VOLTA
    if (!ultima_riga_valida.empty())
    {
      save_data_file << setw(12) << current_temp << " " << ultima_riga_valida << endl;
    }
  }
  else
  {
    cout << "Errore: impossibile aprire i file di I/O in save_data" << endl;
  }

  get_data.close();
  save_data_file.close();
}

void printProgressBar(int currentStep, int totalSteps, int barWidth ) {
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
