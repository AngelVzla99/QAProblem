#include "./tabu_search_solver.hpp"

#include <math.h>

// local search modified to find n-th best solution
element nth_best_neighbour(QAP instance_qap, element positions, int n) {
  circular_list_t<element> new_circular_list(n);
  element best_neighbour = positions;
  int best_cost = get_cost(instance_qap, positions);
  new_circular_list.add_element(positions);
  for (int i = 0; i < instance_qap.N; i++) {
    for (int j = i + 1; j < instance_qap.N; j++) {
      element neighbour = positions;
      int aux = neighbour[i];
      neighbour[i] = neighbour[j];
      neighbour[j] = aux;
      int cost = get_cost(instance_qap, neighbour);
      if (cost < best_cost) {
        best_cost = cost;
        best_neighbour = neighbour;
        if (!new_circular_list.is_in_list(neighbour)) {
          new_circular_list.add_element(neighbour);
        }
      }
    }
  }
  return new_circular_list.get_element(n - 1);
}

QAP_solution local_search_solution_modified(QAP instance_qap,
                                            QAP_solution solution, int n) {
  element old_positions, new_positions = solution.positions;
  int m = n;
  do {
    old_positions = new_positions;
    new_positions = nth_best_neighbour(instance_qap, old_positions, m);
    if (m > 1) {
      m--;
    }
  } while (new_positions != old_positions);

  QAP_solution new_solution;
  new_solution.cost = get_cost(instance_qap, new_positions);
  new_solution.positions = new_positions;
  return new_solution;
}

// Acc: E -> B
bool accept_solution(QAP_solution solution, QAP_solution new_solution,
                     movement move, circular_list tabu_list) {
  // if the new solution is better than the current solution
  if (new_solution.cost < solution.cost) {
    return true;
  }
  // if the new solution is not in the tabu list
  if (!is_in_list(&tabu_list, move)) {
    return true;
  }
  return false;
}

pair<vector<int>, movement> generate_neighbor(QAP instance_qap,
                                              QAP_solution solution) {
  // generate a random movement
  movement move = {rand() % instance_qap.N, rand() % instance_qap.N};
  element neighbor = solution.positions;
  // apply the movement
  int aux = neighbor[move.first];
  neighbor[move.first] = neighbor[move.second];
  neighbor[move.second] = aux;
  return {neighbor, move};
}

// best element in acceptant neighborhood
element best_element_in_acceptant_neighborhood(QAP instance_qap,
                                               QAP_solution old_solution,
                                               circular_list* tabu_list) {
  pair<vector<int>, movement> neighbor;
  QAP_solution solution = old_solution;
  QAP_solution new_solution;

  // walk through the neighborhood
  for (int i = 0; i < 3 * tabu_list->limit; i++) {
    while (true) {
      // generate a neighbor
      neighbor = generate_neighbor(instance_qap, solution);
      // calculate the cost of the neighbor
      int cost = get_cost(instance_qap, neighbor.first);
      new_solution = {cost, neighbor.first};
      if (accept_solution(solution, new_solution, neighbor.second,
                          *tabu_list)) {
        break;
      }
    }
    add_filo_element(tabu_list, neighbor.second);
    solution = new_solution;
  }
  return solution.positions;
}

vector<pair<int, int>> find_common_features(
    vector<circular_list> best_features) {
  vector<pair<int, int>> common_features;
  for (int i = 0; i < best_features[0].limit; i++) {
    for (int j = 1; i < best_features.size(); i++) {
      if (is_in_list(&best_features[j], best_features[0].list[i])) {
        common_features.push_back(best_features[0].list[i]);
      }
    }
  }
  return common_features;
}

QAP_solution intensification(vector<circular_list> bf_all,
                             QAP_solution solution, QAP instance_qap) {
  vector<pair<int, int>> common_features = find_common_features(bf_all);
  element neighbor = solution.positions;
  if (common_features.size() > 0) {
    for (int i = 0; i < common_features.size(); i++) {
      movement move = common_features[i];
      // apply the movement
      int aux = neighbor[move.first];
      neighbor[move.first] = neighbor[move.second];
      neighbor[move.second] = aux;
    }
    int cost = get_cost(instance_qap, neighbor);
    QAP_solution new_solution = {cost, neighbor};
    new_solution =
        local_search_solution_modified(instance_qap, new_solution, 1);
    if (new_solution.cost < solution.cost) {
      return new_solution;
    }
  }
  return {INFLL, {}};
}

QAP_solution diversification(vector<circular_list> best_features,
                             QAP_solution solution, QAP instance_qap) {
  vector<pair<int, int>> common_features = find_common_features(best_features);
  element new_solution;
  element neighbor = solution.positions;
  if (common_features.size() > 0) {
    for (int i = 0; i < 3 * common_features.size();) {
      // generate a random movement
      movement move = {rand() % instance_qap.N, rand() % instance_qap.N};
      // check if the movement isn't in the common features
      if (find(common_features.begin(), common_features.end(), move) ==
              common_features.end() &&
          move != common_features[common_features.size() - 1]) {
        // apply the movement
        int aux = neighbor[move.first];
        neighbor[move.first] = neighbor[move.second];
        neighbor[move.second] = aux;
        i++;
      }
    }
    int cost = get_cost(instance_qap, neighbor);
    QAP_solution new_solution = {cost, neighbor};
    new_solution =
        local_search_solution_modified(instance_qap, new_solution, 1);
    if (new_solution.cost < solution.cost) {
      return new_solution;
    }
  }
  return {INFLL, {}};
}

// function tabu_search(E: space, V: function, Acc: function) -> element of E
QAP_solution tabu_search_solution(QAP instance_qap) {
  // generate a random solution to QAP
  // S* <- generateInitialSolution(E)
  vector<int> rand_sol = generate_random_positions(instance_qap.N);
  QAP_solution solution = {get_cost(instance_qap, rand_sol), rand_sol};
  // S' <- S*
  QAP_solution prev_solution = solution;
  QAP_solution best_solution = solution;
  // memory of the solutions
  // vector<QAP_solution> memory;
  // tabu <- empty list
  // memory of the tabu list (short memory)
  circular_list tabu_list;
  // memory of best features (intermediate memory) intensification
  vector<circular_list> best_features;
  // memory of best features of all time (long memory) diversification
  vector<circular_list> best_features_all_time;

  // while (termination condition is not met) do
  while (true) {
    for (int i = 0; i < 3; i++) {
      // clean tabu list
      tabu_list = make_circular_list(rand_sol.size());
      // S' <- bestElementInAcceptantNeighborhood(S', tabu)
      solution.positions = best_element_in_acceptant_neighborhood(
          instance_qap, solution, &tabu_list);
      // S' <- localSearch(S')
      solution = local_search_solution_modified(instance_qap, solution,
                                                sqrt(instance_qap.N));
      // store the tabu list
      best_features.push_back(tabu_list);
      best_features_all_time.push_back(tabu_list);
      if (solution.cost < best_solution.cost) {
        prev_solution = best_solution;
        best_solution = solution;
        // store the best solution
        // memory.push_back(best_solution);
        break;
      }
    }
    if (best_solution.positions == solution.positions) {
      best_features.clear();
      continue;
    }
    // intensification
    best_solution = intensification(best_features, best_solution, instance_qap);
    if (best_solution.cost < solution.cost) {
      prev_solution = best_solution;
      // memory.push_back(best_solution);
      best_features.clear();
      best_features_all_time.push_back(tabu_list);
      continue;
    }
    // diversification
    best_solution =
        diversification(best_features_all_time, best_solution, instance_qap);
    if (best_solution.cost < solution.cost) {
      prev_solution = best_solution;
      // memory.push_back(best_solution);
      best_features.clear();
      best_features_all_time.push_back(tabu_list);
      continue;
    }
    break;
  }
  return solution;
}
