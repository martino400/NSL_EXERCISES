#include "mpi.h"
#include <iostream>
using namespace std;
const int n = 10000000; // try to increase n
int main(int argc, char *argv[])
{
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status stat1, stat2;
  MPI_Request req;
  int *imesg = new int[n];
  int *imesg2 = new int[n];
  int itag = 1;
  int itag2 = 2;
  for (int i = 0; i < n; i++)
  {
    imesg[i] = rank;
    imesg2[i] = rank + 1;
  }
  if (rank == 1)
  {
    MPI_Isend(&imesg[0], n,
              MPI_INTEGER, 0, itag, MPI_COMM_WORLD, &req);
    MPI_Recv(&imesg2[0], n, MPI_INTEGER, 0, itag2,
             MPI_COMM_WORLD, &stat2);

    // la variabile req serve a Wait per capire se il programma è andato a buon fine o meno
    MPI_Wait(&req, &stat1);
    cout << "messaggio = " << imesg2[0] << endl;

    // ha ricevuto il valore di imesg2 dal processo 0
  }
  else if (rank == 0)
  {
    MPI_Send(&imesg2[0], n,
             MPI_INTEGER, 1, itag2, MPI_COMM_WORLD);
    MPI_Recv(&imesg[0], n, MPI_INTEGER,
             1, itag, MPI_COMM_WORLD, &stat1);
    cout << "messaggio = " << imesg[0] << endl;

    // ha ricevuto in imesg[0] il valore contenuto nel processo 1
  }
  /*
  è necessario usare la funzione MPI_Wait perchè se no ci sarebbe il rischio di far 
  finire il programma e svuotare la memoria mentre sta ancora avvenendo l'invio dei dati

  
  */
  MPI_Finalize();
  return 0;
}