#include "solver/multi_modal.hpp"

int main() {
    printf("Local search solver main\n");
    int n = 20; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        D[i][j] = rand()%100;
        W[i][j] = rand()%100;
    }

    QAP problem = make_QAP( n, D, W );

    QAP_solution ans = {1e18,{}};
    QAP_solution sol = multi_modal_solution(problem,ans,4,100,20,10,true);

    cout << "Solution" << '\n';
    cout << QAP_solution_to_string(sol) << '\n';

    return 0;
}