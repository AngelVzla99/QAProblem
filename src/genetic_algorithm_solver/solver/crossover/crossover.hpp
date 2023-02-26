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
 * @brief Implements a crossover function for the QAP 
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  parents  List of parents to be merged
 * 
 * @return   List of childrens after the crossover
 */
vector<QAP_solution> crossover( QAP instance_qap, vector<QAP_solution> parents );
