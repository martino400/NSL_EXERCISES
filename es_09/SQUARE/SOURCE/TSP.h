#ifndef __System__
#define __System__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <armadillo>
#include <utility>  // Richiesto per std::pair
#include <stdlib.h> //exit
#include "random.h"

using namespace std;
using namespace arma;

struct city
{
    int count;
    double theta, x, y;

    // come in tnds in cui insegno a fare operazioni interne all struct
    bool operator<(const city &other) const
    {
        return theta < other.theta;
    }

    city();
    city(int count, double theta, double x, double y) : count(count), theta(theta), x(x), y(y) {}
};

using trajectory = vector<int>;                  // need this for popoulation
using popolazione = vector<trajectory>;          // need this for generations
using dist_and_traj = tuple<double, trajectory>; // i need this to reorder the vector using the std library function

class TSP
{
private:
    // Potrebbe servire poi per salvare le popolazioni ogni modifica
    vector<popolazione> generation; // Generation: a vector of population
    vector<trajectory> population;  // POPULATION: vector that contains vectors of int
    vector<city> cities;            // Vector that contains the information for all the cities
    int n_cities;                   // Total cities
    int total_individuals;          // Total number of individuals
    int simulation_type;            // Type of simulation (Circle =0 , Square =1, 2-sphere... =2)
    int n_generations;              // Total number of populations, and therefore generations
    double p;                       // Exponent for selection operator
    double swap_prob;               // Swap probability
    double shift_prob;              // Shift probability
    double permutation_prob;        // Permutation probability
    double reverse_prob;            // Reverse probability
    double crossover_prob;          // Crossover probability

public:
    Random _rnd;
    // ## Functions for calculating the distance
    double tot_dist(int n_specific_individual);      // Function to find the total distance between cities.
    double tot_dist_of_trajectory(trajectory &traj); // Function to find the total distance between cities for a specific trajectory, useful for the mutation functions
    double normL1(int a, int b);                     // L1 Norm function between two cities

    // ## Functions for the start of the simulation
    int get_n_cities() { return n_cities; };               // Returns the total number of cities
    int get_n_individuals() { return total_individuals; }; // Returns total number of individuals
    int getGenerations() { return n_generations; };        // Returns total number of generations
    int getSim_Type() { return simulation_type; };         // Returns simulation type
    double getProbability(int prob_index);                 // Returns the probability of  doing a mutation --> Legend: SWAP (INDEX = 0); SHIFT (INDEX = 1); PERM (INDEX = 2); REVERSE (INDEX = 3); CROSSOVER (INDEX = 4);
    void initialize();                                     // Initialize function for Random Generator
    int selection();                                       // Function used for selection of the best trajectory
    // use these functions to create points on a circle
    void create_citiesCIRC(string &info); // Function to create cities on the circumfrence
    void create_citiesSQUARE(string &info); //Function to create cities in a square (x is [0;1) and y is [0;1))
    pair <double, double> point_in_square(); //Retruns x and y of points in a square
    pair<double, double> point_on_circle(); // Returns x and y of the city

    // use these functions to create trajectories
    void create_population();       // Function that creates a population of different possible trajectories
    void create_trajectory(int &i); // Function that creates a trajectory all made up of different integers from 1 to 32

    void reorder();                      // Use this function to find the distance done by each trajectory and reordering the population once I have done so
    void paste_population(string &info); // Function to paste population on file

    // MUTATION FUNCTIONS
    void mutation_swap(int i, popolazione &new_pop);        // Function that swaps two cities in the trajectory of the i-th individual
    void mutation_shift(int i, popolazione &new_pop);       // Function that shifts all elements except the first in a vector
    void mutation_permutation(int i, popolazione &new_pop); // Function that permutes a subset of the trajectory of the i-th individual
    void mutation_reverse(int i, popolazione &new_pop);     // Function to flip an entire part of trajectory;

    // CROSSOVER FUNCTION
    void crossover(popolazione &new_pop, int i, int i1, int i2);

    void cout_trajectory(int i); // Cout function of a certain trajectory

    // Get specific trajectory of the population
    trajectory getSpecTrajectory(int i) { return population[i]; };

    void copy_new_population(popolazione &new_pop); // COPIA NEW_POP in POPULATION

    vector<trajectory> get_generation(); // returns the latest generation of trajectories

    // Data analysis functions, functions to write values on files so that then I can analyse them
    void find_loss_best(); // function that finds L1 of the best trajectory
    void find_loss_half(); // function that finds L1 of the best half of the population
};

#endif
