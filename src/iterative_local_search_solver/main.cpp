#include <iostream>
#include <cstdlib>
#include "solver/iterative_local_search.hpp"

using namespace std;

int main() {
    
    int n = 20; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        D[i][j] = rand()%100;
        W[i][j] = rand()%100;
    }

    QAP problem = make_QAP( n, D, W );
    QAP_solution sol = iterative_local_search_solution(problem);
    cout << QAP_solution_to_string(sol) << '\n';

    return 0;
}
