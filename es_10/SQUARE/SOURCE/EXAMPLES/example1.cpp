#include "mpi.h"
#include <iostream>
using namespace std;

/* passa per tutti i core e li esegue tutti uno a uno, i messaggi vengono messi fuori in base a quale finisce prima (li faccio partire in ordine
 ma non è detto che finiscano in ordine di esecuzione*/
int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << " Sono il nodo " << rank << " dei " << size << " che hai
        utilizzato !"<<endl;
        MPI_Finalize();
    return 0;
}