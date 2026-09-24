#include <iostream>
#include "TSP.h"

using namespace std;

void file_for_cities(string &info);

int main(int argc, char *argv[])
{
  TSP GA{};
  GA.initialize();

  // create file for saving cities
  string info = "../OUTPUT/CitiesInfo.dat";
  file_for_cities(info);
  if (GA.getSim_Type() == 0)
  {
    GA.create_citiesCIRC(info);
  }
  if (GA.getSim_Type() == 1)
  {
    GA.create_citiesSQUARE(info);
  }
  // else
  // {
  //   // square cities, to do
  // }
  info = "../OUTPUT/GENERATIONS/PopulationGeneration" + to_string(0) + ".dat";
  GA.create_population();
  GA.reorder();
  GA.paste_population(info);
  GA.cout_trajectory(0);

  for (int generation_count = 1; generation_count < GA.getGenerations(); generation_count++)
  {
    info = "../OUTPUT/GENERATIONS/PopulationGeneration" + to_string(generation_count) + ".dat";
    vector<trajectory> new_pop(GA.get_n_individuals(), trajectory(GA.get_n_cities()));
    int i1, i2;

    new_pop[0] = GA.getSpecTrajectory(0); // Elitism: the best trajectory is copied to the new population

    // Il ciclo parte da 2 e va fino alla fine
    for (int i = 1; i < GA.get_n_individuals(); i += 2)
    {
      i1 = 0, i2 = 0;
      do
      {
        i1 = GA.selection();
        i2 = GA.selection();
      } while (i1 == i2);
      if (GA._rnd.Rannyu() < GA.getProbability(4))
      {
        GA.crossover(new_pop, i, i1, i2); // Riempie i e i+1
      }
      else
      {
        new_pop[i] = GA.getSpecTrajectory(i1);
        new_pop[i + 1] = GA.getSpecTrajectory(i2);
      }
      // MUTATIONS for i that change based on the generation number;
      if (GA._rnd.Rannyu() < GA.getProbability(0))
      {
        GA.mutation_swap(i, new_pop);
      }
      if (GA._rnd.Rannyu() < GA.getProbability(1))
      {
        GA.mutation_shift(i, new_pop);
      }
      if (GA._rnd.Rannyu() < GA.getProbability(2))
      {
        GA.mutation_permutation(i, new_pop);
      }
      if (GA._rnd.Rannyu() < GA.getProbability(3))
      {
        GA.mutation_reverse(i, new_pop);
      }

      // MUTATIONS for i+1
      if (GA._rnd.Rannyu() < GA.getProbability(0))
      {
        GA.mutation_swap(i + 1, new_pop);
      }
      if (GA._rnd.Rannyu() < GA.getProbability(1))
      {
        GA.mutation_shift(i + 1, new_pop);
      }
      if (GA._rnd.Rannyu() < GA.getProbability(2))
      {
        GA.mutation_permutation(i + 1, new_pop);
      }
      if (GA._rnd.Rannyu() < GA.getProbability(3))
      {
        GA.mutation_reverse(i + 1, new_pop);
      }
    }
    // don't need that much elitsm
    GA.copy_new_population(new_pop);
    GA.reorder();
    GA.paste_population(info);
  }
  GA.find_loss_best();
  GA.find_loss_half();
  GA.cout_trajectory(0);
  GA._rnd.SaveSeed();
  return 0;
}

void file_for_cities(string &info)
{
  ofstream out;
  out.open(info);
  out << "# of city " << "  X position  " << "  Y position  " << " THETA " << endl;
}