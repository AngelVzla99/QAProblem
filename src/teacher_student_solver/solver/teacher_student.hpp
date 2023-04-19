#include "../../QAP/QAP.hpp"
#include "../../genetic_algorithm_solver/solver/genetic_algorithm.hpp"

/**
 * @brief Implements a local search algorithm to solve the QAP
 * 
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  ans  The initial solution
 * @param[in]  n_students  The number of students
 * @param[in]  n_teachers  The number of teachers
 * @param[in]  max_iterations  The maximum number of iterations
 * @param[in]  verbose  If true, print the cost of the solution
 * 
 * @return     { cost, positions } 
 */
QAP_solution teacher_student_solution(
  QAP instance_qap,
  QAP_solution &ans,
  int n_students,
  int n_teachers,
  int max_iterations,
  bool verbose
);
