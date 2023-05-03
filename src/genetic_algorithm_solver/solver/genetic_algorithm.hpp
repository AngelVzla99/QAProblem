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
#include "../../local_search_solver/solver/local_search.hpp"
#include "crossover/crossover.hpp"
#include "mutation/mutation.hpp"

using namespace std;

typedef long long ll;

vector<QAP_solution> buildInitialPopulation(QAP instance_qap, int amount);

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

/**
 * @brief This function implements the genetic algorithm to solve the QAP, used
 * only for the multi-modal solver
 *
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[out] population  The current population
 * @param[in]  start_iteration  The start iteration
 * @param[in]  max_iterations  The maximum number of iterations
 * @param[out] ans  The best solution found
 */
void genetic_algorithm_multi_modal(QAP instance_qap,
                                   vector<QAP_solution> &population,
                                   int start_iteration, int max_iterations,
                                   QAP_solution &ans);
