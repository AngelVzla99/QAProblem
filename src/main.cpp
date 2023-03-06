#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "QAP/QAP.hpp"
#include "exact_solver/solver/solver.hpp"
#include "iterative_local_search_solver/solver/iterative_local_search.hpp"
#include "local_search_solver/solver/local_search.hpp"
#include "tabu_search_solver/solver/tabu_search_solver.hpp"
#include "thread_killer/thread_killer.hpp"

unsigned int microsecond = 1000000;
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

void run_benchmark(const string problem_name = "", const string type_alg = "",
                   const int limit = -1, int maxTime = 10 * 60) {
  vector<string> problems;
  if (problem_name != "") {
    problems = {QAP_INSTANCE_PATH + problem_name + QAP_INSTANCE_EXTENSION};
  } else {
    problems = get_problems();
  }

  cout << "Number of problems: " << problems.size() << '\n';

  // write to each file solution_exact.csv, solution_local_search.csv and
  // solution_iterative_local_search.csv
  ofstream solution_file("benchmark/solution_file_" + type_alg + ".csv");
  solution_file << "Problem, N, Solution, Time\n";

  // pointer to the solver to be used
  QAP_solution (*f_solver)(QAP) = nullptr;
  if (type_alg == "exact") {
    f_solver = solver;
  } else if (type_alg == "local_search") {
    f_solver = local_search_solution;
  } else if (type_alg == "iterative_local_search") {
    f_solver = iterative_local_search_solution;
  } else if (type_alg == "tabu_search") {
    f_solver = tabu_search_solution;
  } else {
    cout << "Invalid algorithm\n";
    exit(1);
  }

  int test = 0;
  for (auto problem : problems) {
    cout << "Problem: " << problem << '\n';
    QAP qap = read_QAP(problem);
    if (limit != -1 && qap.N > limit) {
      continue;
    }
    cout << "N: " << qap.N << '\n';
    // cout << "Instance: " << QAP_to_string(qap) << '\n';

    // test for time break
    QAP_solution ans;
    int code = 0;
    double miliseconds =
        measureTime([&code, maxTime, test, qap, &ans, f_solver]() {
          code = kill_after_timeout(
              maxTime, test, [qap, &ans, f_solver]() { ans = f_solver(qap); });
        });
    test += 1;

    // check case of time break
    if (code == -1) {
      cout << "Time limit exceeded\n";
      solution_file << problem << ", " << qap.N << ", " << -1 << ", "
                    << maxTime * 1000 << '\n';
    } else {
      cout << "Solution: " << ans.cost << '\n';
      cout << "Time: " << miliseconds << " milliseconds\n";
      solution_file << problem << ", " << qap.N << ", " << ans.cost << ", "
                    << miliseconds << '\n';
    }
  }

  solution_file.close();
}

void print_help() {
  cout << "Options:\n";
  cout << "-h, --help: show help\n";
  cout << "-b, --benchmark: run benchmark, execute all problems, a second "
          "optional\n";
  cout << "                 argument can be passed to specify witch algorithm "
          "to use\n";
  cout << "                (e.g. exact, local_search)\n";
  cout << "-p, --problem: run benchmark with a specific problem the next "
          "argument\n";
  cout << "               must be the problem name without the extension (e.g. "
          "chr12a)\n";
  cout << "               a third optional argument can be passed to specify "
          "witch \n";
  cout << "-l, --limit: run benchmark with a specific limit of the size of the "
          "problem\n";
  cout << "            the next argument must be the limit (e.g. 36)\n";
  cout << "-t, --time: run benchmark with a specific time limit (in seconds), "
          "for a specific \n";
  cout << "            algorithm in the second argument\n";
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

  if (options == "-t" || options == "--time") {
    if (argc == 4) {
      string type_alg = argv[2];
      string max_time = argv[3];
      run_benchmark("", type_alg, -1, stoi(max_time));
      return 0;
    }
    run_benchmark();
    return 0;
  }

  return 0;
}
