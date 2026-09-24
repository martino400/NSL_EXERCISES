#include "random.h"

#ifndef __RandomWalk__
#define __RandomWalk__

// This class contains functions for generating random numbers using the RANNYU algorithm
class RandomWalk
{

protected:
  int punto[3];
  double punto_continuo[3];

public:
  // Default constructor
  RandomWalk();
  // Destructor
  ~RandomWalk();
  // Method to create a Random Walk on a cubic square lattice of N_steps
  double RandomWalkCubic(int N_steps, Random& rnd);
  // Restart RW
  void Restart();
  // Restart RW continuo
  void RestartContinuos();
  // Method to create a continuos RW of N_steps
  double RandomWalkContinuous(int N_steps, Random& rnd);
};
#endif