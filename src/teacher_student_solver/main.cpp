#include "solver/teacher_student.hpp"

int main() {
    printf("Teacher student solver main\n");
    int n = 40; 
    vector<vector<int>> D = vector<vector<int>>( n, vector<int>(n,1) );
    vector<vector<int>> W = vector<vector<int>>( n, vector<int>(n,2) );

    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        D[i][j] = rand()%100;
        W[i][j] = rand()%100;
    }

    QAP problem = make_QAP( n, D, W );

    QAP_solution ans = {1e18,{}};
    QAP_solution sol = teacher_student_solution(problem,ans,100,10,100,true);

    cout << "Solution" << '\n';
    cout << QAP_solution_to_string(sol) << '\n';

    return 0;
}