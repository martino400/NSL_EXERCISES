#include "mpi.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (size != 4)
  {
    cout << "Servono 4 processi, non " << size << " !!" << endl;
    return 1;
  }
  int icolor, ikey;
  if (rank == 0)
  {
    icolor = 1;
    ikey = 2;
  }
  if (rank == 1)
  {
    icolor = 1;
    ikey = 1;
  }
  if (rank == 2)
  {
    icolor = 2;
    ikey = 1;
  }
  if (rank == 3)
  {
    icolor = 2;
    ikey = 2;
  }
  MPI_Comm nuovocom;
  MPI_Comm_split(MPI_COMM_WORLD, icolor, ikey, &nuovocom);

  /*
  
  mi permette di raggruppare i processi MPI prendendo i processi di COMM_WORLD per cui ho che 
  icolor mi dice in quale gruppo deve andare mentre key dice con che ordine si deve mettere nel gruppo


  color = 1 (rank = 0, 1)
  color = 2 (rank = 2, 3)

  ora però serve ordinare le informazioni dentro a ciascun color per cui ho 

  color = 1 ho --> rank = 0 che ha key = 2 --> nuovo rank = 1
  color = 1 ho --> rank = 1 che ha key = 1 --> nuovo rank = 0

  color = 2 ho --> rank = 2 che ha key = 1 --> nuovo rank = 0
  color = 2 ho --> rank = 3 che ha key = 2 --> nuovo rank = 1

  il nuovo communicatore è strettamente collegato a quello vecchio
  
  */
  int newsize, newrank;
  MPI_Comm_size(nuovocom, &newsize);
  MPI_Comm_rank(nuovocom, &newrank);
  cout << "MPI process: " << rank << " di " << size << " " << newrank << " di " << newsize << endl;


  MPI_Finalize();
  return 0;
}