#include "./iterative_local_search.hpp"

vector<int> perturbation( QAP_solution current_sol, vector<QAP_solution> mem ){  
  // perturbation that we can't reach using our local_search()
  vector<int> good_permutation = current_sol.positions;
  int n = current_sol.positions.size(), cnt_similar = n, max_iter = 1000;
  while( cnt_similar > n*PER_SIMILARITY && max_iter-- ){
    vector<int> ans = current_sol.positions;
    // make 3 swap
    int i = rand()%n, j = rand()%n, k = rand()%n;
    swap( ans[i], ans[j] );
    swap( ans[j], ans[k] );
    good_permutation = ans;
    // search more similar in memory
    cnt_similar = 0;
    for( QAP_solution sol: mem ){
      int temp_cnt_similarity = 0;
      FOR(pos,0,n) 
        temp_cnt_similarity += bool(ans[pos]==sol.positions[pos]);
      cnt_similar = max( cnt_similar, temp_cnt_similarity );      
    }
  }
  return good_permutation;
}

QAP_solution iterative_local_search_solution(QAP instance_qap){
  cout << "Iterative local search solver\n";
  // generate a random solution to QAP
  vector<int> rand_sol = generate_random_positions( instance_qap.N );
  // first local search
  QAP_solution solution = { get_cost(instance_qap,rand_sol), rand_sol };
  // memory of the solutions
  vector<QAP_solution> memory;
  while(true){
    // perturbation
    vector<int> sol_mutated = perturbation( solution, memory );    
    // local search in the perturbation
    QAP_solution new_sink = start_local_search_solution( sol_mutated, instance_qap );
    memory.push_back( new_sink );
    // acceptance requirements
    if( solution.cost >= new_sink.cost ){
      solution.cost = new_sink.cost;
      solution.positions = new_sink.positions;
    }else{
      break;
    }
  }
  return solution;
}