#include "mutation.hpp"

// 1/10 mutation probability
const int MUTATION_PROBABILITY = 1;

// =========================
//   Mutation functions   //
// =========================

/**
 * @brief This mutation swaps two random positions of the individual in a permutation
 * 
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[out]   sol  The individual that will be mutated
 * 
 * @return    QAP_solution mutated
*/
QAP_solution two_swap( QAP &instance_qap, QAP_solution sol ){
  int a = rand()%sol.positions.size();
  int b = rand()%sol.positions.size();
  swap(sol.positions[a],sol.positions[b]);
  sol.cost = get_cost(instance_qap,sol.positions);
  return sol;
}

/**
  * @brief This mutations make a cycle permutations of an individual in the qap
  * 
  * @param[in]   instance_qap  The instance of a quadratic assignment problem
  * @param[out]  sol  The individual that will be mutated
  * 
  * @return    QAP_solution mutated
*/
QAP_solution insertion( QAP &instance_qap, QAP_solution sol ){
  int a = rand()%sol.positions.size();
  int b = rand()%sol.positions.size();
  int aux = sol.positions[a];
  sol.positions.erase(sol.positions.begin()+a);
  sol.positions.insert(sol.positions.begin()+b,aux);
  sol.cost = get_cost(instance_qap,sol.positions);
  return sol;
}

/**
 * @brief This mutations select two points in the permutation and make a random shuffle
 * 
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[out]  sol  The individual that will be mutated
 * 
 * @return    QAP_solution mutated
*/
QAP_solution mess( QAP &instance_qap, QAP_solution sol ){
  int a = rand()%sol.positions.size();
  int b = rand()%sol.positions.size();
  if(a>b) swap(a,b);
  random_shuffle( sol.positions.begin()+a, sol.positions.begin()+b );
  return sol;
}

/**
 * @brief This mutations select two points in the permutation and make a reverse
 * 
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[out]  sol  The individual that will be mutated
 * 
 * @return    QAP_solution mutated
*/
QAP_solution inversion( QAP &instance_qap, QAP_solution sol){
  int a = rand()%sol.positions.size();
  int b = rand()%sol.positions.size();
  reverse( sol.positions.begin()+a, sol.positions.begin()+b );
  return sol;
}

// =========================
// Main mutation function //
// =========================

vector<QAP_solution> mutation( QAP &instance_qap, int iteration, vector<QAP_solution> population ){
  // answer of the problem 
  vector<QAP_solution> ans;
  // Array of mutation functions 
  vector<QAP_solution (*)(QAP&,QAP_solution)> mutations = {two_swap,insertion,mess,inversion};

  // number of times the same mutation will be applied to the same individual
  int amount_of_mutations = 1 + iteration/4;
  for(auto individual : population){ 
    if( rand()%MUTATION_PROBABILITY == 0 ){
      int mutation = rand()%mutations.size();
      FOR(i,0,amount_of_mutations)
        ans.push_back( mutations[mutation](instance_qap,individual) );
    }
  }

  return ans;
}