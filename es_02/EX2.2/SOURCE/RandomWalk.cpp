#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "RandomWalk.h"
#include "random.h"

using namespace std;

RandomWalk ::RandomWalk()
{
    for (int i = 0; i < 3; i++)
    {
        punto[i] = 0;
    }
}

// Default constructor, does not perform any action

RandomWalk ::~RandomWalk() {}
// Default destructor, does not perform any action

// Restart the array that says where the RW is
void RandomWalk ::Restart()
{
    for (int i = 0; i < 3; i++)
    {
        punto[i] = 0;
    }
}

void RandomWalk ::RestartContinuos()
{
    for (int i = 0; i < 3; i++)
    {
        punto_continuo[i] = 0;
    }
}

// random walk of a 3D lattice of N_steps --> returns the squared module
double RandomWalk ::RandomWalkCubic(int N_steps, Random &rnd)
{
    const double a = 1.0;
    double Module_squared = 0;
    if (N_steps == 100)
    {
        ofstream out;
        out.open("../DATA/rwsteps.dat");
        for (int i = 0; i < N_steps; i++)
        {
            punto[rnd.one_two_three()] += a * rnd.up_or_down();
            out << punto[0] << " " << punto[1] << " " << punto[2] << endl;
        }
        for (int i = 0; i < 3; i++)
        {
            Module_squared += pow(punto[i], 2);
        }
        out.close();
    }
    return Module_squared;
}

// random walk of a continuos lattice of N_steps --> returns the squared module
double RandomWalk ::RandomWalkContinuous(int N_steps, Random &rnd)
{
    const double a = 1.0;
    double Module_squared = 0;
    double theta = 0, phi = 0;
    if (N_steps == 100)
    {
        ofstream out;
        out.open("../DATA/rwstepscontinous.dat");
        for (int i = 0; i < N_steps; i++)
        {
            theta = rnd.theta();
            phi = rnd.phi();
            punto_continuo[0] += a * sin(theta) * cos(phi);
            punto_continuo[1] += a * sin(theta) * sin(phi);
            punto_continuo[2] += a * cos(theta);
            out << punto_continuo[0] << " " << punto_continuo[1] << " " << punto_continuo[2] << endl;
        }
        for (int i = 0; i < 3; i++)
        {
            Module_squared += pow(punto_continuo[i], 2);
        }

        out.close();
    }
    return Module_squared;
}
