#include <cmath>
#include <cstdlib>
#include <string>
#include "TSP.h"

using namespace std;
using namespace arma;

// total distance done on a specific trajectory, with n_specific_individual which choses for which trajectory i want to find the total distance
double TSP::tot_dist(int n_specific_individual)
{
  // Cities.size() e traj.size() devono coincidere se no c'è un problema
  const trajectory &traj = population[n_specific_individual];

  if (traj.size() != cities.size())
  {
    cerr << "Problema: la length of the array of trajectory doesn't coincide with the number of cities!!" << endl;
    return -1;
  }

  double dist = 0.0;

  for (int i = 0; i < traj.size(); i++)
  {
    int a = traj[i];
    int b = traj[(i + 1) % traj.size()];
    dist += normL1(a, b);
  }
  return dist;
}

// need this for new_pop, since
double TSP::tot_dist_of_trajectory(trajectory &traj)
{
  if (traj.size() != cities.size())
  {
    cerr << "Problema: la length of the array of trajectory doesn't coincide with the number of cities!!" << endl;
    return -1;
  }

  double dist = 0.0;

  for (int i = 0; i < traj.size(); i++)
  {
    int a = traj[i];
    int b = traj[(i + 1) % traj.size()];
    dist += normL1(a, b);
  }

  return dist;
}

// L1 norm distance between two cities
double TSP ::normL1(int a, int b)
{
  // L1 norm
  double distance = sqrt(pow(cities[a].x - cities[b].x, 2.0) + pow(cities[a].y - cities[b].y, 2.0));
  return distance;
}

void TSP ::initialize()
{ // Initialize the System object according to the content of the input files in the ../INPUT/ directory

  int p1, p2; // Read from ../INPUT/Primes a pair of numbers to be used to initialize the RNG
  ifstream Primes("../INPUT/Primes");
  Primes >> p1 >> p2;
  Primes.close();
  int seed[4]; // Read the seed of the RNG
  ifstream Seed("../INPUT/seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  _rnd.SetRandom(seed, p1, p2);

  ifstream input("../INPUT/input.dat");
  // check if the file is open
  if (!input.is_open())
  {
    std::cerr << "=========================================================" << std::endl;
    std::cerr << "CRITICAL ERROR: Unable to find or open 'input.dat'!" << std::endl;
    std::cerr << "Attempted path: ../INPUT/input.dat" << std::endl;
    std::cerr << "Please verify that the 'INPUT' directory and the file exist." << std::endl;
    std::cerr << "=========================================================" << std::endl;

    // Immediately terminate the program signaling a failure to the OS
    std::exit(EXIT_FAILURE);
  }
  ofstream coutf("../OUTPUT/info.dat");
  string property;
  while (!input.eof())
  {
    input >> property;
    if (property == "SIM_TYPE")
    {
      input >> simulation_type;
      if (simulation_type == 0)
        coutf << "SIM_TYPE = CIRCLE" << endl;
      else if (simulation_type == 1)
        coutf << "SQUARE" << endl;
      else if (simulation_type == 2)
        coutf << "2-SPHERE" << endl;
    }
    else if (property == "TOT_INDIVID")
    {
      input >> total_individuals;
      coutf << "Total individuals = " << total_individuals << endl;
    }
    else if (property == "N_CITIES")
    {
      input >> n_cities;
      coutf << "Total cities = " << n_cities << endl;
    }
    else if (property == "N_GENERATIONS")
    {
      input >> n_generations;
      coutf << "Total generations = " << n_generations << endl;
    }
    else if (property == "P_VALUE")
    {
      input >> p;
      coutf << "P Value = " << p << endl;
    }
    else if (property == "CROSSOVER_PROB")
    {
      input >> crossover_prob;
      coutf << "CROSSOVER Probability = " << crossover_prob << endl;
    }
    else if (property == "SWAP_PROB")
    {
      input >> swap_prob;
      coutf << "SWAP Probability = " << swap_prob << endl;
    }
    else if (property == "SHIFT_PROB")
    {
      input >> shift_prob;
      coutf << "SHIFT Probability = " << shift_prob << endl;
    }
    else if (property == "PERM_PROB")
    {
      input >> permutation_prob;
      coutf << "PERM Probability = " << permutation_prob << endl;
    }
    else if (property == "REVERSE_PROB")
    {
      input >> reverse_prob;
      coutf << "REVERSE Probability = " << reverse_prob << endl;
    }

    else if (property == "ENDINPUT")
    {
      coutf << "Reading input completed!" << endl;
      break;
    }

    else
      cerr << "PROBLEM: unknown input" << endl;
  }
  if (n_generations == 0 || n_cities == 0 || total_individuals == 0)
  {
    cerr << "PROBLEMA i valori di generazione, città o individui totali non possono essere = 0 !!!" << endl;
  }
}

void TSP ::create_citiesCIRC(string &info)
{
  ofstream out;
  // mi serve questa cosa per ordinare le città usando std library
  double x = 0.0, y = 0.0, theta = 0.0;
  // generate point on a circle
  for (int i = 0; i < n_cities; i++)
  {
    pair<double, double> coordinate = point_on_circle();
    x = coordinate.first;
    y = coordinate.second;
    // siccome so che theta è [-pi, pi], se è negativo devo sommargli 2pi
    theta = atan2(y, x);
    if (theta < 0.0)
    {
      theta += 2.0 * M_PI;
    }
    cities.push_back({0, theta, x, y});
  }
  // having created the struct with the rule to compare two vectors, now i can use sort
  // use the operator '<' that i created
  sort(cities.begin(), cities.end());
  out.open(info, ios::app);
  for (int i = 0; i < n_cities; i++)
  {
    out << i + 1 << "           " << cities[i].x << "   " << cities[i].y << "   " << cities[i].theta << endl;
    cities[i].count = i;
  }
  out.close();
}

//cities in a square
void TSP ::create_citiesSQUARE(string &info)
{
  ofstream out;
  // mi serve questa cosa per ordinare le città usando std library
  double x = 0.0, y = 0.0, theta = 0.0;
  // generate point on a circle
  for (int i = 0; i < n_cities; i++)
  {
    pair<double, double> coordinate = point_in_square();
    x = coordinate.first;
    y = coordinate.second;
    cities.push_back({0, theta, x, y});
  }
  // I don't care about the value of theta, therefore I fix the value of theta = 0.0 for all the cities
  // having created the struct with the rule to compare two vectors, now i can use sort
  out.open(info, ios::app);
  for (int i = 0; i < n_cities; i++)
  {
    out << i << "           " << cities[i].x << "   " << cities[i].y << "   " << cities[i].theta << endl;
    cities[i].count = i;
  }
  out.close();
}


pair<double, double> TSP ::point_on_circle()
{
  double angle = 0.0, x = 0.0, y = 0.0;
  angle = _rnd.Rannyu(0, 2 * M_PI);

  // x coordinate
  x = cos(angle);
  y = sin(angle);

  return {x, y};
}

pair<double, double> TSP ::point_in_square()
{
  double x = 0.0, y = 0.0;
  // x and y coordinate
  x = _rnd.Rannyu(-1.0, 1.0);
  y = _rnd.Rannyu(-1.0, 1.0);
  return {x, y};
}

void TSP ::create_population()
{
  population.resize(total_individuals);
  for (int i = 0; i < total_individuals; i++)
  {
    population[i].resize(n_cities);
    create_trajectory(i);
  }
}

void TSP ::create_trajectory(int &i)
{
  // Use deterministic Fisher-Yates shuffle driven by the class RNG _rnd
  for (int k = 0; k < n_cities; k++)
  {
    population[i][k] = k;
  }

  // Shuffle only the positions from 1..n_cities-1 (leave city 0 fixed)
  for (int idx = 1; idx < n_cities; ++idx)
  {
    // pick j in [idx, n_cities-1]
    int span = n_cities - idx; // >=1
    int offset = int(_rnd.Rannyu(0, (double)span));
    int j = idx + offset;
    std::swap(population[i][idx], population[i][j]);
  }
}

// Use this function to find the distance done by each trajectory and reordering the population once I have done so
void TSP ::reorder()
{
  vector<dist_and_traj> traiettorie_w_distanze;
  for (int i = 0; i < population.size(); i++)
  {
    // PROVA A FARE QUESTO: sostituisci tot_dist(i) con 0.0 temporaneamente
    // double distanza_finta = tot_dist(i); // Commenta questa
    double distanza = tot_dist(i); // Usa questa di test
    traiettorie_w_distanze.push_back({distanza, population[i]});
  }

  // sort algorithm
  sort(traiettorie_w_distanze.begin(), traiettorie_w_distanze.end());

  // rimetto le traiettorie in population
  for (int i = 0; i < total_individuals; i++)
  {
    population[i] = get<1>(traiettorie_w_distanze[i]);
  }
}

void TSP::paste_population(string &info)
{
  ofstream out;
  out.open(info);
  for (int k = 0; k < total_individuals; k++)
  {
    for (int j = 0; j < n_cities; j++)
    {
      out << population[k][j] << " ";
    }
    out << endl;
  }
  out.close();

  // put the information of population in the vector of generations
  // so now I have the population free for new trajectories
  // it is a sort of backup for the information!
  generation.push_back(population);
}

// This is the selection operator, used to see which individual will reproduce at each generation
int TSP::selection()
{
  int index = int(total_individuals * pow(_rnd.Rannyu(), p));

  return index;
}

// 1. SWAP MUTATION (Blindata)
void TSP::mutation_swap(int i, popolazione &new_pop)
{
  trajectory &traj = new_pop[i];

  // Scegliamo indici da 1 a n_cities-1 (l'indice 0 è la prima città e non si tocca)
  int pos1 = int(_rnd.Rannyu(1, n_cities));
  int pos2 = int(_rnd.Rannyu(1, n_cities));

  if (pos1 == pos2)
  {
    // non fare niente
  }
  else
  {
    swap(traj[pos1], traj[pos2]);
  }
}

// 2. SHIFT MUTATION
void TSP::mutation_shift(int i, popolazione &new_pop)
{
  trajectory &traj = new_pop[i];
  if (n_cities <= 3) return;

  // Scegliamo un blocco racchiuso tra l'indice 1 e l'ultimo elemento
  int start = int(_rnd.Rannyu(1, n_cities - 1));
  int end = int(_rnd.Rannyu(start + 1, n_cities + 1)); // estremo escluso

  // Ruota il blocco selezionato verso sinistra di una posizione
  std::rotate(traj.begin() + start, traj.begin() + start + 1, traj.begin() + end);
}
// 3. PERMUTATION MUTATION (La causa principale del bug, ora corretta)
void TSP::mutation_permutation(int i, popolazione &new_pop)
{
  int swap_amount = int(_rnd.Rannyu(1, n_cities / 2)); // Dimensione dei blocchi da scambiare

  // Sicurezza: Serve spazio per: Città_0 (1) + Blocco1 (3) + Blocco2 (3) = Almeno 7 città totali
  if (n_cities < (1 + 2 * swap_amount))
  {
    return;
  }

  // Il primo blocco parte da 1 e può arrivare al massimo dove lascia spazio per sé e per il secondo blocco
  int pos1_swap = int(_rnd.Rannyu(1, n_cities - 2 * swap_amount + 1));

  // Il secondo blocco deve partire TASSATIVAMENTE dopo la fine del primo blocco
  int pos2_swap = int(_rnd.Rannyu(pos1_swap + swap_amount, n_cities - swap_amount + 1));

  trajectory &traj = new_pop[i];
  trajectory::iterator first1 = traj.begin() + pos1_swap;
  trajectory::iterator last1 = traj.begin() + pos1_swap + swap_amount;
  trajectory::iterator first2 = traj.begin() + pos2_swap;

  std::swap_ranges(first1, last1, first2);
}

// 4. REVERSE MUTATION (Blindata)
void TSP::mutation_reverse(int i, popolazione &new_pop)
{
  trajectory &traj = new_pop[i];

  // Scegliamo due punti a caso nel percorso (evitando lo 0)
  int pos1 = int(_rnd.Rannyu(1, n_cities));
  int pos2 = int(_rnd.Rannyu(1, n_cities));

  if (pos1 == pos2)
    return; // Se sono uguali, niente da invertire

  // Assicuriamoci che pos1 sia il più piccolo (l'inizio del taglio)
  if (pos1 > pos2)
  {
    swap(pos1, pos2);
  }

  // pos2 + 1 perché l'estremo destro di std::reverse è escluso!
  // Così invertiamo esattamente il segmento compreso tra pos1 e pos2 (inclusi)
  reverse(traj.begin() + pos1, traj.begin() + pos2 + 1);
}

// Stampa a terminale la traiettoria di un individuo
void TSP::cout_trajectory(int i)
{
  cout << "Distanze = " << tot_dist(i) << endl;
  // for (int j = 0; j < n_cities; j++)
  // {
  //   cout << population[i].at(j) << " ";
  // }
  // cout << endl;
}

void TSP::crossover(popolazione &new_pop, int i, int i1, int i2)
{
  const trajectory &traj1 = population[i1];
  const trajectory &traj2 = population[i2];
  
  trajectory son, daughter;
  son.reserve(n_cities);
  daughter.reserve(n_cities);

  // Scegliamo un punto di taglio (almeno dopo la città 0 fissa)
  int slice = int(_rnd.Rannyu(1, n_cities));

  // 1. Copia la prima parte dai rispettivi genitori
  for(int k = 0; k < slice; ++k) {
      son.push_back(traj1[k]);
      daughter.push_back(traj2[k]);
  }

  // 2. Completa il figlio con l'ordine del secondo genitore
  for(int k = 0; k < n_cities; ++k) {
      int item = traj2[k];
      if (find(son.begin(), son.end(), item) == son.end()) {
          son.push_back(item);
      }
  }

  // 3. Completa la figlia con l'ordine del primo genitore
  for(int k = 0; k < n_cities; ++k) {
      int item = traj1[k];
      if (find(daughter.begin(), daughter.end(), item) == daughter.end()) {
          daughter.push_back(item);
      }
  }

  new_pop[i] = son;
  new_pop[i + 1] = daughter;
}

void TSP::copy_new_population(popolazione &new_pop)
{
  population = new_pop;
  // check delle dimensioni
  for (int i = 0; i < new_pop.size(); i++)
  {
    if (new_pop[i].size() != n_cities)
    {
      cerr << "PROBLEMA: la lunghezza di una traiettoria nella nuova popolazione non coincide con il numero di città!!" << endl;
      return;
    }
  }
  if (new_pop.size() != total_individuals)
  {

    cerr << "PROBLEMA: la lunghezza della nuova popolazione non coincide con il numero di individui!! = " << new_pop.size() << endl;
    return;
  }
}

// returns the latest generation of trajectories, useful for the simulator to print the best trajectory at each generation
vector<trajectory> TSP::get_generation()
{
  return generation.back();
}

double TSP::getProbability(int i)
{
  if (i == 0)
  {
    return swap_prob;
  }
  else if (i == 1)
  {
    return shift_prob;
  }
  else if (i == 2)
  {
    return permutation_prob;
  }
  else if (i == 3)
  {
    return reverse_prob;
  }
  else if (i == 4)
  {
    return crossover_prob;
  }

  else
  {
    cerr << "Index out of range for Mutation Probability! Valid indices: 0 (SWAP), 1 (SHIFT), 2 (PERMUTATION), 3 (REVERSE), 4 (CROSSOVER)" << endl;
    return 0.0;
  }
}

void TSP::find_loss_best()
{
  ofstream out;
  out.open("../OUTPUT/STAT/BestL1.dat");
  out << "# Generation. " << "  Length of best trajectory" << endl;
  for (int i = 0; i < n_generations; i++)
  {
    population = generation[i];
    out << i << "         " << tot_dist(0) << endl;
  }
  out.close();
}

void TSP::find_loss_half()
{
  ofstream out;
  double dist = 0.0;
  out.open("../OUTPUT/STAT/HalfL1.dat");
  out << "# Generation. " << "  Length of best trajectory" << endl;
  for (int i = 0; i < n_generations; i++)
  {
    population = generation[i];
    dist = 0.0;
    for (int k = 0; k < int(total_individuals / 2); k++)
    {
      dist += tot_dist(k);
    }
    // do the average distance
    dist = double(dist / int(total_individuals / 2));
    out << i << "         " << dist << endl;
  }
  out.close();
}