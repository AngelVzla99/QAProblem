#include "../../QAP/QAP.hpp"
#include "../../genetic_algorithm_solver/solver/genetic_algorithm.hpp"

/**
 * @brief Implements a local search algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  ans  The initial solution
 * @param[in]  steps  The number of steps before mix the populations\
 * @param[in]  verbose  If true, print the cost of the best solution found
 * 
 * @return     { cost, positions } 
 */
QAP_solution multi_modal_solution(
  QAP instance_qap,
  QAP_solution &ans,
  int number_of_modes,
  int population_size,
  int max_iterations,
  int steps,
  bool verbose
);
