#include <iostream>
#include "solver/solver.hpp"
#include "../QAP/QAP.hpp"

using namespace std;

int main() {
    
	cout << "Prueba "<<endl;
    cout << solver() << endl;

    int n = 10; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    int position[n]={};
    for(int i=0; i<n; i++) position[i] = i;

    QAP problem = make_QAP( n, D, W );
    cout << QAP_to_string( problem ) << '\n';
    cout << get_cost( problem, position ) << '\n';

    return 0;
}
