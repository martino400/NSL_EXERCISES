#include "mpi.h"
#include <iostream>
using namespace std;

// tutto quello che io eseguo, lo eseguo per tutti gli *n_size* core

int main(int argc, char *argv[])
{
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (size > 3)
  {
    cout << "Hai scelto troppi processi" << endl;
    return 1;
  }
  int irecv[3];
  for (int i = 0; i < 3; i++)
    irecv[i] = 0;
  int isend = rank + 1;
  MPI_Gather(&isend, 1, MPI_INTEGER, irecv, 1, MPI_INTEGER, 0,
             MPI_COMM_WORLD);

  /*
  il primo argomento mi dice da che grandezza devo inviare

  il secondo mi dice quanti dati  del primo argomento devo inviare 

  il terzo il tipo di dato 

  il quarto argomento mi dice dove devo inviare
  
  il quinto argomento mi dice quanti dati devono essere messi nel nuovo vettore 

  il sesto argomento mi dice il tipo di dato in arrivo (devo pensare che posso fare operazioni tra l'invio e la ricevizone)

  il settimo argomenot mi dice a che processo MPI devo inviare i dati 

  il MPI_COMM_WORLD mi dice quali processi MPI devo coinvolgere con questa funzione
  
  */

  // invece per il caso in cui rank != 0 allora ho recieve fissato ancora a 0
  if (rank == 0)
    cout << "irecv: " << irecv[0] << " "
         << irecv[1] << " " << irecv[2] << endl;
  MPI_Finalize();
  return 0;
}