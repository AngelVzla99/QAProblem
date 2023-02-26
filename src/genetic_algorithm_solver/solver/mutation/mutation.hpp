#define FOR(i, n, m) for (int i = n; i < (int)m; i++)
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>
#include "../../../QAP/QAP.hpp"

using namespace std;

typedef long long ll;

/**
 * @brief Implements a genetic algorithm to solve the QAP
 * 
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[in]   iterations The number of iterations of the genetic algorithm
 * @param[in]   parents The individuals that will be mutated 
 * 
 * @return    List of QAP_solutions with the mutated individuals
*/
vector<QAP_solution> mutation( QAP &instance_qap, int iteration, vector<QAP_solution> population );