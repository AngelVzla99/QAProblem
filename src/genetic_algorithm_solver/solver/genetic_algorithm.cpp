#include "genetic_algorithm.hpp"

/**
 * @brief This function build the initial population of the genetic algorithm
 * In this version we are building random permutations for the QAP problem
 * 
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[in]   amount The size of the initial population
 * 
 * @return     List of QAP_solutions with random permutations
 */
vector<QAP_solution> buildInitialPopulation(QAP instance_qap, int amount){
  vector<QAP_solution> ans = vector<QAP_solution>(amount);

  vector<int> facilities(instance_qap.N);
  iota(facilities.begin(), facilities.end(), 0);

  FOR(i,0,amount){
    QAP_solution sol;
    random_shuffle(facilities.begin(), facilities.end());
    sol.positions = facilities;
    sol.cost = get_cost(instance_qap,facilities);
    ans[i] = sol;
  }

  return ans;
}

/**
 * @brief This function checks if the genetic algorithm has to stop
 * 
 * @param[in]   iterations  The current number of iterations of the genetic algorithm
 * @param[in]   population  The current population of the genetic algorithm
 * 
 * @return     True if the genetic algorithm has to stop, false otherwise
 */
bool end_genetic_algorithm( int iterations, vector<QAP_solution> population, int max_iterations ){
  return iterations>max_iterations;
}

/**
 * @brief This function selects the parents of the genetic algorithm, in this
 * version each parent have 50% of probability to be selected as parent
 * 
 * @param[in]   population  The current population of the genetic algorithm
 * 
 * @return     List of QAP_solutions with the parents
 */
vector<QAP_solution> select_parents( vector<QAP_solution> population ){
  vector<QAP_solution> ans;
  for(auto ind : population) if(rand()%2==0) ans.push_back(ind);
  return ans;
}

/**
 * @brief This function selects the survivors of the genetic algorithm, in this
 * version we will take the top 50% of the population, ordered by cost
 * 
 * @param[in]   population  The current population of the genetic algorithm
 * @param[in]   children  The children of the genetic algorithm
 * @param[in]   population_size  The size of the population
 * 
 * @return     List of QAP_solutions with the survivors
 */
vector<QAP_solution> select_survivors( vector<QAP_solution> population, vector<QAP_solution> children, int population_size ){
  vector<QAP_solution> ans;
  for(auto indv: children) population.push_back(indv);
  sort(
    population.begin(), 
    population.end(), 
    [](QAP_solution a, QAP_solution b){ return a.cost < b.cost; }
  ); 
  FOR(i,0,min(population_size,int(population.size()))) 
    ans.push_back(population[i]);
  return ans;
}

/**
 * @brief This function implements the genetic algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * 
 * @return     { cost, positions } 
 */
QAP_solution genetic_algorithm(QAP instance_qap, int population_size, int max_iterations){
  QAP_solution ans = {};
  vector<QAP_solution> population = buildInitialPopulation( instance_qap, population_size );

  int iteration = 0;
  while( !end_genetic_algorithm(iteration,population,max_iterations) ){
    vector<QAP_solution> parents = select_parents( population );
    vector<QAP_solution> children = crossover( instance_qap, parents );
    children = mutation( instance_qap, iteration, children );
    population = select_survivors( population, children, population_size );

    // save the best solution
    if( population[0].cost < ans.cost || iteration==0 ) 
      ans = population[0];
    
    iteration++;
  }

  return ans;
}