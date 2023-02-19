#define FOR(i, n, m) for (int i = n; i < (int)m; i++)
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include "../../QAP/QAP.hpp"
#include "mutation/mutation.hpp"
#include "crossover/crossover.hpp"

using namespace std;

typedef long long ll;

/**
 * @brief Implements a genetic algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * 
 * @return     { cost, positions } 
 */
QAP_solution genetic_algorithm(QAP instance_qap, int population_size);
