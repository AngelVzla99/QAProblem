#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP
#include "../../QAP/QAP.hpp"

/**
 * @brief Generates a random permutation of the numbers from 0 to N-1
 * 
 * @param[in]  N     The number of elements in the permutation
 * 
 * @return     A random permutation of the numbers from 0 to N-1
 */
vector<int> generate_random_positions(int N);

/**
 * @brief Finds the best neighbour of a given permutation
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  positions     The permutation
 * 
 * @return     The best neighbour of the given permutation
 */
vector<int> find_best_neighbour(QAP instance_qap, vector<int> positions);

/**
 * @brief Implements a local search algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * 
 * @return     { cost, positions } 
 */
QAP_solution local_search_solution(QAP instance_qap);

/**
 * @brief Implements a local search algorithm to solve the QAP starting in 
 * a specific position in the space
 * 
 * @param[in]  start First position in the space to make local search
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * 
 * @return     { cost, positions } 
 */
QAP_solution start_local_search_solution(vector<int> start, QAP instance_qap);


#endif // LOCAL_SEARCH_HPP
