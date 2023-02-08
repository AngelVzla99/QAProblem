#ifndef ITERATIVE_LOCAL_SEARCH_HPP
#define ITERATIVE_LOCAL_SEARCH_HPP
#define FOR(i, n, m) for (int i = n; i < (int)m; i++)
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>

#include "../../local_search_solver/solver/local_search.hpp"

using namespace std;

typedef long long ll;
// const ll INFLL = 1e18;
const double PER_SIMILARITY = 0.85;

/**
 * @brief Implements an iterative local search algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * 
 * @return     { cost, positions } 
 */
QAP_solution iterative_local_search_solution(QAP instance_qap);

#endif // ITERATIVE_LOCAL_SEARCH_HPP
