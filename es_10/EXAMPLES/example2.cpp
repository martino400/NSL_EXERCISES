#include "mpi.h"
#include <iostream>
using namespace std;


//tutto quello che io eseguo, lo eseguo per tutti gli *n_size* core

int main(int argc, char *argv[])
{
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int my_values[3];
  for (int i = 0; i < 3; i++){
    if (rank == 0)
      my_values[i] = 5;
    else
      my_values[i] = 0;
  }
  cout << endl << "Prima: " << my_values[0] << " " << my_values[1] << " " << my_values[2] << " per il processo " << rank << endl;

  //COSA FA MPI_Bcast

  /*
  passa i valore dell'oggetto (in questo caso il vettore my_values) in tutti gli altri core
  
  il secondo argomento passa quandi elementi del vettore my_values devono essere tramandati agli altri

  il terzo argomento indica il tipo di dato 

  il quarto è l'etichetta del core che contiene i dati da inviare

  il quinto indica quali core di calcolo sono coinvolti
  */
  MPI_Bcast(my_values, 2, MPI_INTEGER, 0, MPI_COMM_WORLD);
  cout << "Dopo: " << my_values[0] << " " << my_values[1] << " "<< my_values[2]<< " per il processo "<< rank<< endl;
      MPI_Finalize();
  return 0;
}