#include "mpi.h"
#include <iostream>
using namespace std;
const int n = 100; // try to increase n
int main(int argc, char *argv[])
{
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status stat1, stat2;
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




    /*

    ho per il processo 1 : 
    - imesg[...] = 0, 0, 0, ...
    - imesg2[...] = 1,1,1, ...

    ho per il processo 2:
    - imesg[...] = 1,1,1 ...
    - imesg[...] = 2, 2, 2

    
    
    
    
    
    
    MPI_Send prende in ingresso:
    1. il primo elemento del vettore da passare
    2. la quantità di elementi da passare
    3. il tipo di variabile da passare
    4. a che rank mandare
    5. il tag associato al messaggio di invio 
    6. che communicatore coinvolge questa operazione 


    in questo modo sto copiando quello che c'era dentro a imesg di rank 1 e lo sto mandando in imesg del rank 0 
    e sto copiando imesg2 di rank 0 (1, 1, 1...) in imes2 di rank 0 (2,2,2 ... )

    quindi sto rendendo tutti i vettori per i vari rank = 1,1 1, ...
    
    
    */
    MPI_Send(&imesg[0], n, MPI_INTEGER, 0, itag, MPI_COMM_WORLD);
    MPI_Recv(&imesg2[0], n,MPI_INTEGER, 0, itag2, MPI_COMM_WORLD, &stat2);
    cout << " messaggio ricevuto nel processo 1= " << imesg2[0] << endl;
  }
  else if (rank == 0)
  {
    MPI_Send(&imesg2[0], n,
             MPI_INTEGER, 1, itag2, MPI_COMM_WORLD);
    MPI_Recv(&imesg[0], n, MPI_INTEGER, 1,
             itag, MPI_COMM_WORLD, &stat1);
    cout << "messaggio ricevuto nel processo 0= " << imesg[0] << endl;
  }
  MPI_Finalize();

  cout << " ho quindi rank = " << rank << "mess and mess2 = " << imesg[0] << " " << imesg2[0] << endl;
  
  return 0;
}