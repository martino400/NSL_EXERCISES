#include "mpi.h"
#include <iostream>
#include <string>
#include "TSP.h"
#include "lib.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // VALIDATION: Secure check of terminal arguments
    if (argc < 2)
    {
        if (rank == 0)
            cerr << "Error: You must specify the number of folders/continents as a command-line argument!" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }

    int n_cartelle = stoi(argv[1]);

    // Error in case the the message written on terminal isn't correct
    if (n_cartelle != size)
    {
        if (rank == 0)
            cerr << "Problem: size of MPI_COMM_WORLD is different from the value passed in argv[1]" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }

    // Initialize the pseudo-random number generator with rank-dependent seeds for parallelation
    Random _rnd{};
    initialize_rand(_rnd, rank);

    // Create folders to manage save the information of all MPI_Processes
    create_folders(rank, n_cartelle);

    // Initialize and configure the Genetic Algorithm (GA) environment for the TSP
    TSP GA{};
    GA.initialize();
    GA.load_cities();
    int generation_count = 0;
    int n_cities = GA.get_n_cities();

    // Set up the dynamic file path formatting for logging populations across generations
    string info_base = "../OUTPUT/GENERATIONS" + to_string(rank) + "/PopulationGeneration";
    string info = info_base + to_string(generation_count) + ".dat";

    // Setup initial generation: seed population, sort by fitness, and write generation 0 on file
    GA.create_population(_rnd);
    GA.reorder();
    GA.paste_population(info);

    // MEMORY ALLOCATION: Pre-allocate dedicated data buffers for MPI communication to avoid runtime crashes
    trajectory path_to_send(n_cities);
    trajectory path_to_recv(n_cities);
    int worst_index = GA.get_n_individuals() - 1;

    // Core Evolutionary Loop: Iterates through generations up to the maximum limit specified in GA setup
    for (generation_count = 1; generation_count < GA.getGenerations(); generation_count++)
    {
        info = info_base + to_string(generation_count) + ".dat";
        vector<trajectory> new_pop(GA.get_n_individuals(), trajectory(n_cities));
        GA.reorder();

        // Elitism: Automatically preserve the absolute best historical trajectory into the next generation
        new_pop[0] = GA.getSpecTrajectory(0);

        // BOUNDS PROTECTION: Loop stops at N_INDIVIDUALS - 1 to safeguard against out-of-bounds memory errors on i+1
        for (int i = 1; i < GA.get_n_individuals() - 1; i += 2)
        {
            int i1 = 0, i2 = 0;
            // Select two distinct parents using the GA selection operator (roulette/tournament)
            do
            {
                i1 = GA.selection(_rnd);
                i2 = GA.selection(_rnd);
            } while (i1 == i2);

            // Crossover Phase: Combine genetic information with a specified probability threshold
            if (_rnd.Rannyu() < GA.getProbability(4))
            {
                GA.crossover(new_pop, i, i1, i2, _rnd); // Populates slot i and i+1
            }
            else
            {
                // Direct replication of parents if crossover does not happen
                new_pop[i] = GA.getSpecTrajectory(i1);
                new_pop[i + 1] = GA.getSpecTrajectory(i2);
            }

            // MUTATIONS FOR INDIVIDUAL 'i'
            if (_rnd.Rannyu() < GA.getProbability(0))
            {
                GA.mutation_swap(i, new_pop, _rnd); // Pairwise city swap
            }
            if (_rnd.Rannyu() < GA.getProbability(1))
            {
                GA.mutation_shift(i, new_pop, _rnd); // Shifting a block of cities
            }
            if (_rnd.Rannyu() < GA.getProbability(2))
            {
                GA.mutation_permutation(i, new_pop, _rnd); // Permuting independent blocks
            }
            if (_rnd.Rannyu() < GA.getProbability(3))
            {
                GA.mutation_reverse(i, new_pop, _rnd); // Reversing sequence order
            }

            // MUTATIONS FOR INDIVIDUAL 'i+1'
            if (_rnd.Rannyu() < GA.getProbability(0))
            {
                GA.mutation_swap(i + 1, new_pop, _rnd);
            }
            if (_rnd.Rannyu() < GA.getProbability(1))
            {
                GA.mutation_shift(i + 1, new_pop, _rnd);
            }
            if (_rnd.Rannyu() < GA.getProbability(2))
            {
                GA.mutation_permutation(i + 1, new_pop, _rnd);
            }
            if (_rnd.Rannyu() < GA.getProbability(3))
            {
                GA.mutation_reverse(i + 1, new_pop, _rnd);
            }
        }

        // POPULATION ALIGNMENT: Fill the final unallocated slot if the total population size is an even number
        if (GA.get_n_individuals() % 2 == 0)
        {
            new_pop[worst_index] = GA.getSpecTrajectory(GA.selection(_rnd));
        }

        // Commit the newly generated population array back into the GA object instance and rank them
        GA.copy_new_population(new_pop);
        GA.reorder();

        // EXCHANGE PHASE: Continents interact periodically (every 20 generations) by exchanging elite paths
        exchange_best_trajectories(GA, rank, size, _rnd, generation_count, path_to_send, path_to_recv, worst_index, n_cities);

        // Save current generational telemetry to data files
        GA.paste_population(info);
    }
    // GA.cout_trajectory(0);

    int shortest_rank = find_shortest_MPI(GA, rank, size);

    // DATA FUNCTIONS
    GA.find_loss_best(shortest_rank, rank);
    GA.find_loss_half(shortest_rank, rank);

    paste_shortest_rank(GA, rank, shortest_rank);

    // SAVE ON POPULATION OF RANK == 0 THE BEST PATH OF ALL THE MPI PROCESSES
    // This functions saves in the file "../OUTPUT/BestTrajectories.dat" the best path of each MPI process
    save_best_trajectories(GA, rank, size, path_to_send, path_to_recv, worst_index, n_cities);

    // Safely serialize final random seeds and shut down the parallel execution environment
    _rnd.SaveSeed();
    MPI_Finalize();
    return 0;
}

