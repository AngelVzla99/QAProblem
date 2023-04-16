#include "multi_modal.hpp"

const long long INF = 1e18;

QAP_solution multi_modal_solution(
  QAP instance_qap,
  QAP_solution &ans,
  int number_of_modes = 4,
  int population_size = 100,
  int max_iterations = 20,
  int steps = 10,
  bool verbose = true
){
  cout << "Multi modal solver" << '\n';
  
  vector<QAP_solution> modes[number_of_modes];
  FOR(i,0,number_of_modes){
    modes[i] = buildInitialPopulation(instance_qap, population_size);
  }

  int start = 0;
  FOR(i,0,max_iterations){
    if(verbose) cout << "============ Epoch " << i << " ============" << '\n';

    // calculate the best solution for each mode
    FOR(j,0,number_of_modes){
      genetic_algorithm_multi_modal( instance_qap, modes[j], start, start+steps, ans );
      if(verbose) cout << "Best mode " << j << "\n" << QAP_solution_to_string(ans) << "\n\n\n";
    }

    // move best individual from each mode to the next mode
    FOR(j,0,number_of_modes){
      int next_mode = (j+1)%number_of_modes;
      modes[next_mode].push_back( modes[j][0] );
      modes[j].erase( modes[j].begin() );
    }

    start += steps;
  }

  return ans;
}