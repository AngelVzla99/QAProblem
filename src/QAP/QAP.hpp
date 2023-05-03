#ifndef QAP_HPP
#define QAP_HPP
#define FOR(i, n, m) for (int i = n; i < (int)m; i++)
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef long long ll;
const ll INFLL = 1e18;

struct QAP {
  int N;
  vector<vector<int>> D, W;
};

struct QAP_solution {
  ll cost;
  vector<int> positions;
};

// position is an array that can be understand as
// a function position[i] is a number between 0 and
// problem.N-1, it's the position of a facility
int get_cost(QAP problem, vector<int> positions);
// Function  to make a QAP problem given the minimum
QAP make_QAP(int N, vector<vector<int>> D, vector<vector<int>> W);
// Function to transform the structure to a string
string QAP_to_string(QAP problem);
// Function to transform the solution to a string
string QAP_solution_to_string(QAP_solution solution);
// Function to read a QAP problem from a file
QAP read_QAP(string filename);

#endif  // QAP_HPP