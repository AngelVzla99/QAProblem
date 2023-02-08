#include <iostream>
#include <filesystem>
#include <fstream>
#include "QAP/QAP.hpp"
#include "exact_solver/solver/solver.hpp"
#include "local_search_solver/solver/local_search.hpp"
#include "iterative_local_search_solver/solver/iterative_local_search.hpp"

const string QAP_INSTANCE_PATH = "benchmark/qapdata/";
const string QAP_INSTANCE_EXTENSION = ".dat";
const string QAP_INSTANCE_SOLUTION_PATH = "benchmark/qapsoln/";
const string QAP_INSTANCE_SOLUTION_EXTENSION = ".sln";

namespace fs = std::filesystem;

vector<string> get_problems() {
  vector<string> problems;
  for (const auto &entry : fs::directory_iterator(QAP_INSTANCE_PATH)) {
    problems.push_back(entry.path());
  }
  return problems;
}

void run_benchmark(const string problem_name = "", const string type_alg = "", const int limit = -1) {
  
  vector<string> problems;
  if (problem_name != "") {
    problems = {QAP_INSTANCE_PATH + problem_name + QAP_INSTANCE_EXTENSION};
  } else {
    problems = get_problems();
  }

  cout << "Number of problems: " << problems.size() << '\n';

  // write to each file solution_exact.csv, solution_local_search.csv and solution_iterative_local_search.csv
  ofstream solution_exact("benchmark/solution_exact.csv");
  solution_exact << "Problem, N, Solution, Time\n";
  ofstream solution_local_search("benchmark/solution_local_search.csv");
  solution_local_search << "Problem, N, Solution, Time\n";
  ofstream solution_iterative_local_search("benchmark/solution_iterative_local_search.csv");
  solution_iterative_local_search << "Problem, N, Solution cost, Time\n";

  for (auto problem : problems) {
    cout << "Problem: " << problem << '\n';
    QAP qap = read_QAP(problem);
    if (limit != -1 && qap.N > limit) {
      continue;
    }
    cout << "N: " << qap.N << '\n';
    cout << "Instance: " << QAP_to_string(qap) << '\n';

    ll exact_solution_cost = -1;
    chrono::microseconds exact_duration = chrono::microseconds(0);

    if (type_alg == "exact") {
      // exact solver
      cout << "Exact solver:\n";
      auto start = chrono::high_resolution_clock::now();
      auto exact_solution = solver(qap);
      auto end = chrono::high_resolution_clock::now();
      exact_duration = chrono::duration_cast<chrono::microseconds>(end - start);
      exact_solution_cost = get_cost(qap, exact_solution);
      cout << "Solution: " << exact_solution_cost << '\n';
      cout << "Time: " << exact_duration.count() << " microseconds\n";
      // write to file solution_exact.csv
      solution_exact << problem << ", " << qap.N << ", " << exact_solution_cost << ", " << exact_duration.count() << '\n';
    }

    // local search solver
    if (type_alg == "local_search") {
      cout << "Local search solver:\n";
      auto start = chrono::high_resolution_clock::now();
      auto ls_solution = local_search_solution(qap);
      auto end = chrono::high_resolution_clock::now();
      auto ls_duration = chrono::duration_cast<chrono::microseconds>(end - start);
      cout << "Solution: " << ls_solution.cost << '\n';
      cout << "Time: " << ls_duration.count() << " microseconds\n";
      // write to file solution_local_search.csv
      solution_local_search << problem << ", " << qap.N << ", " << ls_solution.cost << ", " << ls_duration.count() << '\n';
    }

    // iterative local search solver
    if (type_alg == "iterative_local_search") {
      cout << "Iterative local search solver:\n";
      auto start = chrono::high_resolution_clock::now();
      auto ls_solution = iterative_local_search_solution(qap);
      auto end = chrono::high_resolution_clock::now();
      auto ls_duration = chrono::duration_cast<chrono::microseconds>(end - start);
      cout << "Solution: " << ls_solution.cost << '\n';
      cout << "Time: " << ls_duration.count() << " microseconds\n";
      // write to file solution_iterative_local_search.csv
      solution_iterative_local_search << problem << ", " << qap.N << ", " << ls_solution.cost << ", " << ls_duration.count() << '\n';
    }
  }
}

void print_help() {
  cout << "Options:\n";
  cout << "-h, --help: show help\n";
  cout << "-b, --benchmark: run benchmark, execute all problems, a second optional\n";
  cout << "                 argument can be passed to specify witch algorithm to use\n";
  cout << "                (e.g. exact, local_search)\n";
  cout << "-p, --problem: run benchmark with a specific problem the next argument\n";
  cout << "               must be the problem name without the extension (e.g. chr12a)\n";
  cout << "               a third optional argument can be passed to specify witch \n";
  cout << "-l, --limit: run benchmark with a specific limit of the size of the problem\n";
  cout << "            the next argument must be the limit (e.g. 36)\n";
}

/**
 * @brief Main function to run benchmark the options are:
 * -h, --help: show help
 * -b, --benchmark: run benchmark, execute all problems, a second optional
 *                 argument can be passed to specify witch algorithm to use
 *                (e.g. exact, local_search, iterative_local_search)
 * -p, --problem: run benchmark with a specific problem the next argument
 *               must be the problem name without the extension (e.g. chr12a)
 *               a third optional argument can be passed to specify witch 
 *               algorithm to use (e.g. exact, local_search)
 * -l, --limit: run benchmark with a specific limit of the size of the problem
 *            the next argument must be the limit (e.g. 36) 
 */
int main(const int argc, const char *argv[]) {

  if (argc == 1) {
    cout << "No arguments, use -h or --help to see the options\n";
    return 0;
  }

  string options = argv[1];

  if (options == "-h" || options == "--help") {
    print_help();
    return 0;
  }

  if (options == "-b" || options == "--benchmark") {
    if (argc == 3) {
      string type_alg = argv[2];
      run_benchmark("", type_alg);
      return 0;
    }
    run_benchmark();
    return 0;
  }

  if (options == "-p" || options == "--problem") {
    if (argc < 3) {
      cout << "No problem name, use -h or --help to see the options\n";
      return 0;
    }
    string problem_name = argv[2];
    if (argc == 4) {
      string type_alg = argv[3];
      run_benchmark(problem_name, type_alg);
      return 0;
    }
    run_benchmark(problem_name);
    return 0;
  }

  if (options == "-l" || options == "--limit") {
    if (argc != 3) {
      cout << "No limit, use -h or --help to see the options\n";
      return 0;
    }
    int limit = stoi(argv[2]);
    run_benchmark("", "", limit);
    return 0;
  }
  return 0;
}
