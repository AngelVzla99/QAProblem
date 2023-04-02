#include "solver/ant_colony_search.hpp"

const long long INF = 1e18;

int main() {
    printf("Ant colony main\n");
    int n = 100; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        D[i][j] = rand()%100;
        W[i][j] = rand()%100;
    }

    QAP problem = make_QAP( n, D, W );
    QAP_solution sol = {INF, vector<int>(n,0)};
    sol = ant_colony_search_solution(problem, sol, 0.5, 50, 4);

    cout << "Solution" << '\n';
    cout << QAP_solution_to_string(sol) << '\n';
    cout << "cost validation "<< sol.cost << " " << get_cost(problem, sol.positions) << '\n';

    return 0;
}