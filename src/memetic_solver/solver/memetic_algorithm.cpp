#include "./memetic_algorithm.hpp"

/**
 * @brief Generate n random numbers in the range [a, b] without repetition
 *
 * @param[in]  n     The number of random numbers
 * @param[in]  a     The minimum value
 * @param[in]  b     The maximum value
 *
 * @return     A vector with n random numbers in the range [a, b]
 */
vector<int> generate_random_numbers(int n, int a, int b) {
  set<int> s;
  while (s.size() < n) {
    s.insert(rand() % (b - a + 1) + a);
  }
  vector<int> ans(s.begin(), s.end());
  return ans;
}

// recombination methods:
//    1. most common feature
//    2. random feature from each parent

/**
 * @brief This function implements the most common feature recombination
 * method, we take the most common feature of each position and we create
 * the childrens with that feature, if there is a tie we take the first,
 *
 * @param[in]  parents  The parents of the genetic algorithm
 *
 * @return     The childrens of the genetic algorithm
 */
QAP_solution most_common_feature(vector<QAP_solution> parents,
                                 QAP instance_qap) {
  int n = parents.size();
  int m = parents[0].positions.size();
  QAP_solution sol;
  sol.positions = vector<int>(m);
  set<int> used;
  for (int j = 0; j < m; j++) {
    map<int, int> freq;
    for (int k = 0; k < n; k++) {
      freq[parents[k].positions[j]]++;
    }
    int max_freq = 0;
    int max_freq_value = 0;
    for (auto p : freq) {
      if (p.second > max_freq) {
        max_freq = p.second;
        max_freq_value = p.first;
      }
    }
    while (used.count(max_freq_value)) {
      max_freq_value = (max_freq_value + 1) % m;
    }
    used.insert(max_freq_value);
    sol.positions[j] = max_freq_value;
  }
  sol.cost = get_cost(instance_qap, sol.positions);
  return sol;
}

/**
 * @brief This function implements the random feature recombination method,
 * we take a random feature of each position and we create the childrens
 * with that feature, with take each parent in a random order
 *
 * @param[in]  parents  The parents of the genetic algorithm
 *
 * @return     Three childrens of the genetic algorithm
 */
vector<QAP_solution> random_feature(vector<QAP_solution> parents,
                                    QAP instance_qap, int n_childrens) {
  int n = parents.size();
  int m = parents[0].positions.size();
  vector<int> indexes = generate_random_numbers(m, 0, m - 1);
  vector<QAP_solution> childrens;
  FOR(i, 0, n_childrens) {
    QAP_solution sol;
    sol.positions = vector<int>(m);
    set<int> used;
    for (auto k : indexes) {
      int parent_index = rand() % n;
      int value = parents[parent_index].positions[k];
      int max_tries = 10;
      while (used.count(value) && max_tries--) {
        parent_index = rand() % n;
        value = parents[parent_index].positions[k];
      }
      if (max_tries == -1) {
        while (used.count(value)) {
          value = (value + 1) % m;
        }
      }
      used.insert(value);
      sol.positions[k] = value;
    }
    sol.cost = get_cost(instance_qap, sol.positions);
    childrens.push_back(sol);
  }
  return childrens;
}

/**
 * @brief This function implements the main recombination of the memetic
 * algorithm
 *
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  parents  The parents of the genetic algorithm
 *
 * @return     The childrens of the genetic algorithm, the size of this vector
 * is the same as the size of the parents vector
 */
vector<QAP_solution> recombination(QAP instance_qap,
                                   vector<QAP_solution> parents) {
  int n = parents.size();
  int m = parents[0].positions.size();
  vector<QAP_solution> childrens;
  // we will separate the parents in groups of 4 and we will apply the
  // recombination method to each group
  int n_groups = n / 4;
  for (int i = 0; i < n_groups; i++) {
    vector<QAP_solution> group;
    for (int j = 0; j < 4; j++) {
      group.push_back(parents[i * 4 + j]);
    }
    // we apply the first recombination method to the group
    vector<QAP_solution> childrens_group =
        random_feature(group, instance_qap, 3);
    for (int j = 0; j < 3; j++) {
      childrens.push_back(childrens_group[j]);
    }
    // we apply the second recombination method to the group
    QAP_solution children = most_common_feature(group, instance_qap);
    childrens.push_back(children);
  }
  return childrens;
}

/**
 * @brief This function implements the main solution improvement of the
 * memetic algorithm
 *
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 * @param[in]  childrens  The childrens of the genetic algorithm
 * @param[in]  percentage  The percentage of the childrens that will be
 * improved
 *
 * @return     The same vector children after apply the solution improvement
 * method to each one
 */
vector<QAP_solution> solution_improvement(QAP instance_qap,
                                          vector<QAP_solution> childrens,
                                          float percentage) {
  int n = childrens.size();

  // We will improve the top 50% of the childrens
  // we select 50% of the childrens
  int n_improve = int(n * percentage);
  vector<QAP_solution> childrens_to_improve;
  vector<int> indexes = generate_random_numbers(n_improve, 0, n - 1);
  for (int i = 0; i < n_improve; i++) {
    childrens_to_improve.push_back(childrens[indexes[i]]);
  }

  // We apply the solution improvement method to each one of the childrens
  for (int i = 0; i < n_improve; i++) {
    childrens_to_improve[i] = start_local_search_solution(
        childrens_to_improve[i].positions, instance_qap);
  }

  // We replace the childrens that we improved
  for (int i = 0; i < n_improve; i++) {
    childrens[indexes[i]] = childrens_to_improve[i];
  }

  return childrens;
}

/**
 * @brief This function implements the memetic algorithm to solve the QAP
 *
 * @param[in]  instance_qap  The instance of a quadratic assignment problem
 *
 * @return     { cost, positions }
 */
QAP_solution memetic_algorithm(QAP instance_qap, int population_size,
                               int max_iterations) {
  cout << "Memetic Algorithm" << endl;
  QAP_solution ans = {};
  vector<QAP_solution> population =
      buildInitialPopulation(instance_qap, population_size);

  int iteration = 0;
  while (!end_genetic_algorithm(iteration, population, max_iterations)) {
    cout << "Iteration: " << iteration << endl;
    vector<QAP_solution> parents = select_parents(population);
    cout << "Parents selected" << endl;
    vector<QAP_solution> children = recombination(instance_qap, parents);
    cout << "Childrens created" << endl;
    children = solution_improvement(instance_qap, children, 0.5);
    cout << "Childrens improved" << endl;
    population = select_survivors(population, children, population_size);
    iteration++;
  }
  return population[0];
}