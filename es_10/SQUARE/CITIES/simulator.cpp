#include <iostream>
#include "TSP.h"
using namespace std;


/*THE PURPOSE OF THIS CODE IS TO CREATE THE CITIES ONCE FOR ALL MPI PROCESSES SO THAT THEY ARE IDENTICAL BETWEEN MPI PROCESSES*/

int main(int argc, char *argv[])
{
    TSP GA{};
    GA.initialize();
    string info = "../OUTPUT/CitiesInfo.dat";
    // create file for saving cities
    if (GA.getSim_Type() == 0)
    {
        GA.create_citiesCIRC(info);
    }
    else if (GA.getSim_Type() == 1)
    {
        GA.create_citiesSQUARE(info);
    }

    GA._rnd.SaveSeed();
    return 0;
}
