#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#define FOR(i, n, m) for (int i = n; i < (int)m; i++)
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "../../QAP/QAP.hpp"
#include "crossover/crossover.hpp"
#include "mutation/mutation.hpp"

using namespace std;

typedef long long ll;

/**
 * @brief This function build the initial population of the genetic algorithm
 * In this version we are building random permutations for the QAP problem
 *
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[in]   amount The size of the initial population
 *
 * @return     List of QAP_solutions with random permutations
 */
vector<QAP_solution> buildInitialPopulation(QAP instance_qap, int amount);

/**
 * @brief This function checks if the genetic algorithm has to stop
 *
 * @param[in]   iterations  The current number of iterations of the genetic
 * algorithm
 * @param[in]   population  The current population of the genetic algorithm
 *
 * @return     True if the genetic algorithm has to stop, false otherwise
 */
bool end_genetic_algorithm(int iterations, vector<QAP_solution> population,
                           int max_iterations);

/**
 * @brief This function selects the parents of the genetic algorithm, in
 * this version each parent have 50% of probability to be selected as parent
 *
 * @param[in]   population  The current population of the genetic algorithm
 *
 * @return     List of QAP_solutions with the parents
 */
vector<QAP_solution> select_parents(vector<QAP_solution> population);

/**
 * @brief This function selects the survivors of the genetic algorithm, in
 * this version we will take the top 50% of the population, ordered by cost
 *
 * @param[in]   population  The current population of the genetic algorithm
 * @param[in]   children  The children of the genetic algorithm
 * @param[in]   population_size  The size of the population
 *
 * @return     List of QAP_solutions with the survivors
 */
vector<QAP_solution> select_survivors(vector<QAP_solution> population,
                                      vector<QAP_solution> children,
                                      int population_size);

/**
 * @brief Implements a genetic algorithm to solve the QAP
 *
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  population_size  The size of the population
 * @param[in]  max_iterations  The maximum number of iterations
 *
 * @return     { cost, positions }
 */
QAP_solution genetic_algorithm(QAP instance_qap, int population_size,
                               int max_iterations);

#endif  // GENETIC_ALGORITHM_HPP