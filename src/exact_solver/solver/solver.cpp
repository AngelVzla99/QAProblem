#include "solver.hpp"

// current problem 
QAP cuProblem;

// ======================
//     DP solver       //
// ======================

// next element to take in a state
map< vector<int>, pair<int,int> > nxt;
// dp[pos] = minimum cost to add the
// remaining elements
map< vector<int>, int > dp; 
// remember prev solutions
map< vector<int>, bool > used;

ll dpSolver( vector<int> &pos ){
  int N = cuProblem.N;

  // base case
  int cntAssignations = 0;
  for(int p: pos) if(p!=-1) cntAssignations++;
  if( cntAssignations==N ){
    nxt[pos] = {-1,-1};
    return 0;
  }

  // memorization of the DP
  if( used[pos] ) return dp[pos];

  // remember what is used
  bool usedFacilities[N]={};
  bool usedPos[N]={};
  FOR(i,0,N) if( pos[i]!=-1 ){
    usedPos[pos[i]] = true;
    usedFacilities[i] = true;
  }

  // try all the possible combinations (nxn)
  ll bestVal = INFLL;
  pair<int,int> bestNext = {-1,-1};
  FOR(fac,0,N) FOR(p,0,N){
    if( !usedFacilities[fac] && !usedPos[p] ){
      pos[fac] = p;

      // cost
      ll cost = 0;
      FOR(fac2,0,N) if(pos[fac2]!=-1){
        int p2 = pos[fac2];
        ll weight = cuProblem.W[fac][fac2];
        ll dist = cuProblem.D[p][p2];
        cost += weight*dist;

        ll distInv = cuProblem.D[p2][p];
        ll weightInv = cuProblem.W[fac2][fac];
        if(p!=p2) cost += weightInv*distInv;
      }

      ll tempValue = dpSolver( pos ) + cost;
      if( tempValue < bestVal ){
        bestVal = tempValue;
        bestNext = {fac,p};
      }

      pos[fac] = -1;
    }
  }

  // save and return 
  used[pos] = true;
  dp[pos] = bestVal;
  nxt[pos] = bestNext;
  return bestVal;
}

// ======================
//  factorial solver   //
// ======================

pair<ll,vector<int>> facSolver( QAP problem ){
  int N = problem.N;
  vector<int> a(N,-1);
  FOR(i,0,N) a[i] = i;

  vector<int> ans;
  ll bestVal = INFLL;
  do{    
    ll tempVal = get_cost( problem, a );
    if( tempVal < bestVal ){
      bestVal = tempVal;
      ans = a;
    }

  } while (next_permutation(a.begin(), a.end()));

  return {bestVal,ans};
}


// ======================
//        solver       //
// ======================

QAP_solution solver( QAP problem ){  
  cout << "Exact solver\n";
  // ==>  solver using DP

  // // save the problem as a global variables 
  // cuProblem = problem;

  // // find the best solution
  // vector<int> cuPos = vector<int>(problem.N,-1);
  // nxt.clear();
  // dp.clear();
  // used.clear();
  // ll bestVal = dpSolver( cuPos );
  // cout << "Best cost = " << bestVal << '\n';

  // // recreate the solution 
  // while( nxt[cuPos].first!=-1 ){
  //   pair<int,int> temp = nxt[cuPos];
  //   cuPos[temp.first] = temp.second;
  // }

  // ===> solver using factorial
  pair<ll,vector<int>> ans = facSolver(problem);

  return {ans.first,ans.second};
}