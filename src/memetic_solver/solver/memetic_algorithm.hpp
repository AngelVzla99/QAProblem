#ifndef MEMETIC_ALGORITHM_HPP
#define MEMETIC_ALGORITHM_HPP

#include <set>

#include "../../genetic_algorithm_solver/solver/genetic_algorithm.hpp"
#include "../../local_search_solver/solver/local_search.hpp"

/**
 * @brief Implements a memetic algorithm to solve the QAP
 *
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  population_size  The size of the population
 * @param[in]  max_iterations  The maximum number of iterations
 *
 * @return QAP_solution { cost, positions }
 */
QAP_solution memetic_algorithm(QAP instance_qap, int population_size,
                               int max_iterations);

#endif  // MEMETIC_ALGORITHM_HPP