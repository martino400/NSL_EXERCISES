#include "TSP.h"
#include "random.h"
#include <iostream>
// #include "mpi.h"

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

// Function prototypes
void initialize_rand(Random &_rnd, int &rank);
void create_folders(int &rank, int &cartelle);
int find_shortest_MPI(TSP &GA, int rank, int size);
void paste_shortest_rank(TSP &GA, int rank, int shortest_rank);
void save_best_trajectories(TSP &GA, int rank, int size, trajectory &path_to_send, trajectory &path_to_recv, int worst_index, int n_cities);
void exchange_best_trajectories(TSP &GA, int rank, int size, Random &_rnd, int generation_count, trajectory &path_to_send, trajectory &path_to_recv, int worst_index, int n_cities);


// Function to initialize Random Generator parameters from storage files
void initialize_rand(Random &_rnd, int &rank)
{
    int p1, p2; // Read from ../INPUT/Primes a pair of numbers to be used to initialize the RNG
    ifstream Primes("../INPUT/Primes");

    // Salta le righe precedenti in base al rank del processo
    for(int i = 0; i <= rank; i++) {
        Primes >> p1 >> p2;
    }
    Primes.close();
    Primes.close();

    int seed[4]; // Read the seed configuration array of the RNG
    ifstream Seed("../INPUT/seed.in");
    Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];

    // The seed for each rank is different, so I have independent processes
    seed[0] += rank;
    seed[1] += rank;
    _rnd.SetRandom(seed, p1, p2);
}

// Function to create the directory trees for the generation outputs
void create_folders(int &rank, int &n_cartelle)
{
    // Directory generation routines restricted to the Master process to prevent file system access collisions
    if (rank == 0)
    {
        cout << " Terminal input parsed: preparing to create " << n_cartelle << " directories." << endl;

        for (int i = 0; i < n_cartelle; i++)
        {
            string folder_name = "../OUTPUT/GENERATIONS" + to_string(i);
            if (!fs::exists(folder_name))
            {
                fs::create_directory(folder_name);
            }
        }
        cout << "[Master] Output structure built successfully!" << endl;
    }
}

// Function to find shortest MPI process
int find_shortest_MPI(TSP &GA, int rank, int size)
{
    trajectory traj = GA.getSpecTrajectory(0);
    double local_dist = GA.tot_dist_of_trajectory(traj);
    int min;

    vector<double> all_distances;
    if (rank == 0)
    {
        all_distances.resize(size);
    }

    // MPI_Gather function
    /*
    1. the variable that I need to send from each MPI_Process
    2. how many elements the each process has to send
    3. the type of variable each process sends in local_dist
    4. in which variable I need to save the MPI processed
    5. the amount of variables that have to be saved from each MPI process
    6. the type of variables that have to be saved inside
    ...

    IMPORTANT: the interesting thing about Gather is that in the i-cell of the vector local_dist is saved the distance of the i-rank so that there is no ambiguity
    otherwise there would have been a problem when saving the data because otherwise 5th element of all_distance might have been a distance relative to the 8th rank.
    The ambiguity is because the MPI process don't execute the programm in order, but gather saves the variables in order.
    */
    MPI_Gather(&local_dist, 1, MPI_DOUBLE, all_distances.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        ofstream out;
        // save the distance of shortest path for each MPI process on file
        out.open("../OUTPUT/ranking_processes.dat");
        out << "--- PROCESS RANKING (SHORTEST TRAJECTORIES) ---" << endl;
        for (int i = 0; i < size; ++i)
        {
            out << "Process/Continent Rank " << i << " -> Best Distance: " << all_distances[i] << endl;
        }
        out.close();
        min = 0;
        double min_dist = all_distances[0];
        for (int i = 0; i < size; i++)
        {
            if (min_dist > all_distances[i])
            {
                min_dist = all_distances[i];
                min = i;
            }
        }
    }
    //This function sends to all the MPI processes the rank of the process that has the shortest distance
    MPI_Bcast(&min, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return min;
}

//Function to print the rank with the shortest path
void paste_shortest_rank(TSP &GA, int rank, int shortest_rank)
{
    if (rank == shortest_rank)
    {
        cout << "\n=================================================" << endl;
        cout << "  WINNER: MPI Process/Continent Rank " << rank << " has the shortest distance!" << endl;
        cout << "=================================================" << endl;
        GA.cout_trajectory(0);
    }
}

// Function that saves in population of rank == 0 the best trajectories
void save_best_trajectories(TSP &GA, int rank, int size, trajectory &path_to_send, trajectory &path_to_recv, int worst_index, int n_cities){
    // Variable on which I save the best path for each rank
    path_to_send = GA.getSpecTrajectory(0);
    if (rank != 0)
    {
        // Worker processes ship their independent global minimum trajectories over to the Master node
        MPI_Send(path_to_send.data(), n_cities, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        GA.setSpecTrajectory(0, path_to_send);
        // Rank 0 sequentially saves elite paths from every worker to determine the global minimum
        for (int proc = 1; proc < size; proc++)
        {
            // Receive function, I have to change each time from which process I receive
            MPI_Recv(path_to_recv.data(), n_cities, MPI_INT, proc, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Temporarily queue the incoming foreign path at the bottom of the population
            GA.setSpecTrajectory(worst_index, path_to_recv);
            // Sort immediately in order to have the recently save best path immediately shift up
            GA.reorder();
        }
        string send_destination = "../OUTPUT/BestTrajectories.dat";
        GA.paste_population(send_destination);
    }
}

//Function to exchange the best trajectories between MPI processes
void exchange_best_trajectories(TSP &GA, int rank, int size, Random &_rnd, int generation_count, trajectory &path_to_send, trajectory &path_to_recv, int worst_index, int n_cities)
{
    if (generation_count % 100 == 0)
    {
        path_to_send = GA.getSpecTrajectory(0); // Variable in which I save best trajectory for a specific MPI process
        int shift;
        if (rank == 0)
        {
            // Rank ==0 determines a random process displacement value for the ring topology
            shift = int(_rnd.Rannyu(1, size));
        }

        // Broadcast the calculated shift value to all MPI processes
        MPI_Bcast(&shift, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Compute target ranks utilizing modular arithmetic to establish a closed-ring communication topology
        int target_send = (rank + shift) % size;
        int target_recv = (rank - shift + size) % size;

        // Synchronous bilateral data transfer: Executes send and receive operations concurrently to avoid deadlocks
        MPI_Sendrecv(path_to_send.data(), n_cities, MPI_INT, target_send, 0,
                     path_to_recv.data(), n_cities, MPI_INT, target_recv, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Inject the newly imported trajectory by overwriting the local worst individual, then resort
        GA.setSpecTrajectory(worst_index, path_to_recv);
        GA.reorder();
    }
}