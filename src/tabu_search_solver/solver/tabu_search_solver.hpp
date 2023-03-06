#ifndef TABU_SEARCH_SOLVER_HPP
#define TABU_SEARCH_SOLVER_HPP
#include "../../QAP/QAP.hpp"
#include "../../local_search_solver/solver/local_search.hpp"
#include "../../utils/circular_list/circular_list.hpp"

const ll INFLL = 1e18;

// new type to define
typedef pair<int, int> movement;
typedef vector<int> element;

QAP_solution tabu_search_solution(QAP instance_qap);

#endif  // TABU_SEARCH_SOLVER_HPP