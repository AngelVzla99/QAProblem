#include "ant_colony_search.hpp"
#include <set>
#include <random>

typedef long double ld;
typedef vector<vector<ld>> pheromones_t;

struct ant{
  vector<int> positions;
  ll cost;
  set<int> validPositions;
  set<int> validCities;
  QAP instance_qap;

  ant(QAP instance_qap){
    int n = instance_qap.N;
    positions = vector<int>(n,-1);
    cost = 0;
    FOR(i,0,n) validPositions.insert(i), validCities.insert(i);
    this->instance_qap = instance_qap;
  }

  void assign(int pos, int city){
    positions[city] = pos;
    validPositions.erase(pos);
    validCities.erase(city);
    cost = 0;
    FOR(i,0,instance_qap.N){
      FOR(j,0,instance_qap.N){
        if( positions[i] == -1 || positions[j] == -1 ) continue;

        ll weight = instance_qap.W[i][j];
        ll dist = instance_qap.D[ positions[i] ][ positions[j] ];
        cost += weight*dist;
      }
    }
  }
};

struct change{
  int pos, city;
  ld cost;
  change(int pos, int city, ll cost){
    this->pos = pos;
    this->city = city;
    this->cost = cost;
  }
};

default_random_engine generator;
void move_ant( ant &a,pheromones_t &pheromones, pheromones_t &new_pheromone ){
  vector<int> availablePositions = vector<int>(a.validPositions.begin(), a.validPositions.end());
  vector<int> availableCities = vector<int>(a.validCities.begin(), a.validCities.end());

  // get possible changes and their probabilities
  vector<change> probabilities;
  for( int city : availableCities ){
    for( int pos : availablePositions ){
      probabilities.push_back({pos,city,pheromones[pos][city]});
    }
  }  

  // get values of probabilities
  vector<ld> values;
  for( change c : probabilities ) values.push_back( round(c.cost*1000000) );
  random_shuffle(values.begin(), values.end());

  // get random change
  discrete_distribution<int> distribution(values.begin(), values.end());
  int index = distribution(generator);
  change c = probabilities[index];

  // make change
  a.assign(c.pos, c.city);

  // update pheromones
  new_pheromone[c.pos][c.city] += 1.0/a.cost;
}

QAP_solution ant_colony_search_solution(
  QAP instance_qap, 
  QAP_solution &current_solution,
  long double p = 0.5,
  int n_ants = 100,
  int n_iterations = 10
){
  cout << "ant colony search solver\n";

  // init pheromones
  int n = instance_qap.N;
  pheromones_t pheromones = pheromones_t(n, vector<ld>(n, 1.0));

  // create and update colony
  vector<ant> ants = vector<ant>(n_ants, ant(instance_qap));
  FOR(i,1,n_iterations*n+1){
    //  move n_ants
    pheromones_t new_pheromone = pheromones_t(n, vector<ld>(n, 0.0));
    FOR(j,0,n_ants){
      move_ant(ants[j], pheromones, new_pheromone);
    }

    // new pheromones matrix
    FOR(position,0,n){
      FOR(city,0,n){
        pheromones[position][city] = (1-p)*pheromones[position][city] + new_pheromone[position][city];
      }
    }

    // print each ant 
    // FOR(j,0,n_ants){
    //   QAP_solution sol = { ants[j].cost, ants[j].positions };
    //   cout << QAP_solution_to_string(sol) << '\n';
    // }

    if( i%n==0 ){
      // get best ant
      int best_ant = 0;
      FOR(j,0,n_ants){
        if( ants[j].cost < ants[best_ant].cost ) best_ant = j;
      }

      // update current solution
      if( ants[best_ant].cost < current_solution.cost ){
        current_solution.cost = ants[best_ant].cost;
        current_solution.positions = ants[best_ant].positions;
      }

      // resets n_ants 
      FOR(j,0,n_ants) ants[j] = ant(instance_qap);
    }
  }
  
  return current_solution;
}