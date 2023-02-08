#include "solver/local_search.hpp"

int main() {
    printf("Local search solver main\n");
    int n = 7; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        D[i][j] = rand()%100;
        W[i][j] = rand()%100;
    }


    QAP problem = make_QAP( n, D, W );
    QAP_solution sol = local_search_solution(problem);

    cout << "Solution" << '\n';
    cout << QAP_solution_to_string(sol) << '\n';

    return 0;
}