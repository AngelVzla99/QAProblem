#include "../../QAP/QAP.hpp"

/**
 * @brief Implements a local search algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  current_solution  The current best solution found
 * @param[in]  p  The pheromone evaporation rate
 * @param[in]  n_ants  The number of ants
 * @param[in]  n_iterations  The number of iterations
 * 
 * @return     { cost, positions } 
 */
QAP_solution ant_colony_search_solution(
  QAP instance_qap, 
  QAP_solution &current_solution,
  long double p,
  int n_ants,
  int n_iterations
);

