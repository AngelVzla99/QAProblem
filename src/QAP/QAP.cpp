#include "QAP.hpp"

const string ERROR_BAD_POSITION = "ERROR: BAD POSITION in QAP/get_cost";

int get_cost( QAP problem, int *position ){
  int n = problem.N;

  // check if position is a permutation of [0..n-1]
  int cnt[n]={};
  FOR(i,0,n){
    // not in range
    if( position[i]<0 || position[i]>=n ){
      cout << ERROR_BAD_POSITION << '\n';
      exit(1);
    }
    // repeated elements
    if( cnt[position[i]] ){
      cout << ERROR_BAD_POSITION << '\n';
      exit(1);
    }
    cnt[position[i]]++;
  }

  // calculate the QAP value
  ll ans = 0;
  FOR(i,0,n){
    FOR(j,0,n){
      ll weight = problem.W[i][j];
      ll dist = problem.D[ position[i] ][ position[j] ];
      ans += weight*dist;
    }
  }
  return ans;
}

QAP make_QAP( int N, vector<vector<int>> D, vector<vector<int>> W ){  
  return { N, D, W };
}

string QAP_to_string( QAP problem ){
  string ans = "";

  ans += "Distance matrix \n";
  FOR(i,0,problem.N){
    FOR(j,0,problem.N){
      ans += to_string( problem.D[i][j] );
      ans += " ";
    }
    ans += "\n";
  }

  ans += "\nWeight matrix \n";
  FOR(i,0,problem.N){
    FOR(j,0,problem.N){
      ans += to_string( problem.W[i][j] );
      ans += " ";
    }
    ans += "\n";
  }

  return ans;
}