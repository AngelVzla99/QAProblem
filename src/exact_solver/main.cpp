#include <iostream>
#include "solver/solver.hpp"
#include <cstdlib>

using namespace std;

int main() {
    
    int n = 6; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        D[i][j] = rand()%100;
        W[i][j] = rand()%100;
    }

    QAP problem = make_QAP( n, D, W );
    vector<int> sol = solver(problem);
    for(int p: sol) cout << p << " ";
    cout << endl;

    return 0;
}
