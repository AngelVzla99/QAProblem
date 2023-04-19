#include <iostream>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include "QAP/QAP.hpp"
#include "exact_solver/solver/solver.hpp"
#include "local_search_solver/solver/local_search.hpp"
#include "iterative_local_search_solver/solver/iterative_local_search.hpp"
#include "thread_killer/thread_killer.hpp"
#include "genetic_algorithm_solver/solver/genetic_algorithm.hpp"
#include "ant_colony_search_solver/solver/ant_colony_search.hpp"
#include "multi_modal_solver/solver/multi_modal.hpp"
#include "teacher_student_solver/solver/teacher_student.hpp"

unsigned int microsecond = 1000000;
const string QAP_INSTANCE_PATH = "benchmark/qapdata/";
const string QAP_INSTANCE_EXTENSION = ".dat";
const string QAP_INSTANCE_SOLUTION_PATH = "benchmark/qapsoln/";
const string QAP_INSTANCE_SOLUTION_EXTENSION = ".sln";

namespace fs = std::filesystem;

QAP_solution genetic_algorithm_default(QAP instance_qap){
  return genetic_algorithm(instance_qap, 100, 60);
}

QAP_solution ant_colony_search_solution_default(QAP instance_qap){
  QAP_solution current_solution = {1e18, vector<int>(instance_qap.N)};
  return ant_colony_search_solution(instance_qap, current_solution, 0.5, 150, 4);
}

vector<string> get_problems() {
  vector<string> problems;
  for (const auto &entry : fs::directory_iterator(QAP_INSTANCE_PATH)) {
    problems.push_back(entry.path());
  }
  return problems;
}

void run_benchmark(const string problem_name = "", const string type_alg = "", const int limit = -1, int maxTime = 20*60) {

  vector<string> problems;
  if (problem_name != "") {
    problems = {QAP_INSTANCE_PATH + problem_name + QAP_INSTANCE_EXTENSION};
  } else {
    problems = get_problems();
  }

  cout << "Number of problems: " << problems.size() << '\n';

  // write to each file solution_exact.csv, solution_local_search.csv and solution_iterative_local_search.csv
  ofstream solution_file("benchmark/solution_file_"+type_alg+".csv");
  solution_file << "Problem,N,Solution,Time\n";

  // pointer to the solver to be used
  QAP_solution (*f_solver)(QAP) = nullptr;
  if( type_alg == "exact" ) {
    f_solver = solver;
  }else if( type_alg == "local_search" ) {
    f_solver = local_search_solution;
  }else if( type_alg == "iterative_local_search" ) {
    f_solver = iterative_local_search_solution;
  }else if( type_alg == "genetic_algorithm" ) {
    f_solver = genetic_algorithm_default;    
  }else if( type_alg == "ant_colony_search" ) {
    f_solver = ant_colony_search_solution_default;
  }else{
    cout << "Invalid algorithm type\n";
    return;
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
		double miliseconds = measureTime([&code,maxTime,test,qap,&ans,f_solver](){
			code = kill_after_timeout(maxTime,test,[qap,&ans,f_solver](){
        ans = f_solver(qap);        
			}); 
		});
    test += 1;

    // check case of time break
		if (code == -1){      
      cout << "Time limit exceeded\n";
      solution_file << problem << ", " << qap.N << ", " << -1 << ", " << maxTime*1000 << '\n';  
    }else{
      cout << "Solution: " << ans.cost << '\n';
      cout << "Time: " << miliseconds << " milliseconds\n";
      solution_file << problem << ", " << qap.N << ", " << ans.cost << ", " << miliseconds << '\n';
    }
  }

  solution_file.close();

}

void run_benchmark_V2(
  // f_solver, lambda function of the form <lambda(QAP, QAP_solution)>
  function<QAP_solution(QAP, QAP_solution&)> f_solver,
  string folder, 
  string file_name, 
  int maxTime = 20*60
) {

  vector<string> problems = get_problems();
  cout << "Number of problems: " << problems.size() << '\n';

  // write to each file solution_exact.csv, solution_local_search.csv and solution_iterative_local_search.csv
  ofstream solution_file("benchmark/"+folder+"/solution_file_"+file_name+".csv");
  solution_file << "Problem,N,Solution,Time\n";

  int test = 0;
  QAP_solution ans_list[problems.size()];
  for (auto problem : problems) {
    cout << "Problem: " << problem << '\n';
    QAP qap = read_QAP(problem);
    cout << "N: " << qap.N << '\n';

    // test for time break
    int code = 0;
    QAP_solution &ans = ans_list[test] = {1e18,{}};
		double miliseconds = measureTime([&code,maxTime,test,qap,&ans,f_solver](){
			code = kill_after_timeout(maxTime,test,[qap,&ans,f_solver](){
        ans = f_solver(qap,ans);
			}); 
		});
    
    // check case of time break
		if ( ans.cost==1e18 ){  
      // time limit exceeded in red
      cout << "\033[1;31mTime limit exceeded\033[0m\n";
      solution_file << problem << ", " << qap.N << ", " << -1 << ", " << maxTime*1000 << '\n';  
    }else{
      if( code == -1 ){
        // time limit exceeded in red
        cout << "\033[1;31mTime limit exceeded\033[0m\n";
      }

      cout << "Solution: " << ans.cost << '\n';
      cout << "Time: " << miliseconds << " milliseconds\n";
      solution_file << problem << ", " << qap.N << ", " << ans.cost << ", " << miliseconds << '\n';
    }

    test += 1;
  }

  solution_file.close();
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
  cout << "-t, --time: run benchmark with a specific time limit (in seconds), for a specific \n";
  cout << "            algorithm in the second argument\n";
  cout << "-b2 --benchmark2: run benchmark with a specific algorithm in the second argument\n";
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
      run_benchmark("", type_alg,-1,stoi(max_time));
      return 0;
    }
    run_benchmark();
    return 0;
  }

  if(options == "-b2" || options == "--benchmark2"){
    if (argc == 3) {
      string type_alg = argv[2];

      if(type_alg == "multi_modal"){
        // multi_modal tuning parameters
        // * changing the population size
        // return multi_modal_solution(qap, ans, 10, 100, 10, 10, false);

        vector<int> pop_sizes = { 100 };
        vector<int> max_iters = { 15 };
        vector<int> steps = { 15 };

        for( auto pop_size : pop_sizes ){
          for( auto max_iter : max_iters ){
            for( auto step : steps ){
              auto f = [&](QAP qap, QAP_solution &ans){
                return multi_modal_solution(qap, ans, 5, pop_size, max_iter, step, false);
              };

              run_benchmark_V2(f, type_alg, "multi_modal_LS:75_popSize:"+to_string(pop_size)+"_maxIter:"+to_string(max_iter)+"_step:"+to_string(step));
            }
          }
        }
      }else if(type_alg=="teacher_student"){
        // teacher_student tuning parameters
        vector<int> n_students = { 20, 50, 100 };
        vector<int> n_teachers = { 20, 50, 100 };
        vector<int> max_iterations = { 10, 50, 100 }; 

        for( auto n_stu : n_students ){
          for( auto n_tea : n_teachers ){
            for( auto n_iter : max_iterations ){
              if(n_stu < n_tea) continue;
              auto f = [&](QAP qap, QAP_solution &ans){
                return teacher_student_solution(qap, ans, n_stu, n_tea, n_iter, false);
              };

              run_benchmark_V2(f, type_alg, "teacher_student_nStudents:"+to_string(n_stu)+"_nTeachers:"+to_string(n_tea)+"_maxIter:"+to_string(n_iter));
            }
          }
        }
      }
      return 0;
    }else{
      cout << "No algorithm specified, use -h or --help to see the options\n";
      exit(1);
    }
  }



  return 0;
}
