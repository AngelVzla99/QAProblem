#include "crossover.hpp"

// =========================
//   Crossover functions  //
// =========================

QAP_solution random( QAP instance_qap, QAP_solution parent1, QAP_solution parent2 ){
  int n = parent1.positions.size();
  QAP_solution child;
  child.positions = vector<int>( n, -1 );
  bool used[n]={};

  // Randomly select a position from parent1 or parent2
  vector<int> free_positions;
  FOR(i,0,n){
    int v1 = parent1.positions[i];
    int v2 = parent2.positions[i];
    if( used[v1] && used[v2] ) continue;
    else if( used[v1] ){ 
      child.positions[i] = v2;
      used[v2] = true;
    }else if( used[v2] ){
      child.positions[i] = v1;
      used[v1] = true;
    }else{
      if( rand()%2 ){
        child.positions[i] = v1;
        used[v1] = true;
      }else{
        child.positions[i] = v2;
        used[v2] = true;
      }
    }
  }

  // Get the remaining values
  vector<int> free_values;
  FOR(i,0,n) if( !used[i] ) free_values.push_back(i);
  random_shuffle( free_values.begin(), free_values.end() );

  // Fill the rest of the positions with remaining values
  int pos_free_values = 0;
  FOR(i,0,n){
    if( child.positions[i] == -1 ){
      child.positions[i] = free_values[pos_free_values];
      pos_free_values++;
    }
  }

  child.cost = get_cost(instance_qap,child.positions);

  return child;
}

QAP_solution random_with_order( QAP instance_qap, QAP_solution parent1, QAP_solution parent2 ){
  int n = parent1.positions.size();
  QAP_solution child;
  child.positions = vector<int>( n, -1 );
  bool used[n]={};

  // Randomly select a position from parent1
  FOR(i,0,n){
    if( rand()%2 ){
      child.positions[i] = parent1.positions[i];
      used[parent1.positions[i]] = true;
    }
  }

  // Fill the rest of the positions with parent2
  int pos_parent_2 = 0;
  FOR(i,0,n){
    if( child.positions[i] == -1 ){
      while( used[parent2.positions[pos_parent_2]] ) pos_parent_2++;
      child.positions[i] = parent2.positions[pos_parent_2];
      used[parent2.positions[pos_parent_2]] = true;
    }
  }
  child.cost = get_cost(instance_qap,child.positions);

  return child;
}

QAP_solution two_points( QAP instance_qap, QAP_solution parent1, QAP_solution parent2 ){
  int n = parent1.positions.size();
  QAP_solution child;
  child.positions = vector<int>( n, -1 );
  bool used[n]={};

  // Randomly select a position from parent1
  int a = rand()%n;
  int b = rand()%n;
  if(a>b) swap(a,b);
  FOR(i,a,b){
    child.positions[i] = parent1.positions[i];
    used[parent1.positions[i]] = true;
  }

  // Fill the rest of the positions with parent2
  int pos_parent_2 = 0;
  FOR(i,0,n){
    if( child.positions[i] == -1 ){
      while( used[parent2.positions[pos_parent_2]] ) pos_parent_2++;
      child.positions[i] = parent2.positions[pos_parent_2];
      used[parent2.positions[pos_parent_2]] = true;
    }
  }
  child.cost = get_cost(instance_qap,child.positions);

  return child;
}

QAP_solution partially_mapped_junction( QAP instance_qap, QAP_solution parent1, QAP_solution parent2 ){
  int n = parent1.positions.size();
  QAP_solution child;
  child.positions = vector<int>( n, -1 );
  bool used[n]={};

  // Mappers from each parent
  int mapper1[n], mapper2[n];
  FOR(i,0,n) mapper1[parent1.positions[i]] = i;
  FOR(i,0,n) mapper2[parent2.positions[i]] = i;

  // Randomly select two positions from the first parent
  int a = rand()%n, b = rand()%n;
  if(a>b) swap(a,b);
  FOR(i,a,b){
    child.positions[i] = parent1.positions[i];
    used[parent1.positions[i]] = true;
  }
  
  // Fill not used position from parent2
  FOR(i,a,b) if( !used[parent2.positions[i]] ){
    int p = a;
    int val1 = parent1.positions[i];      
    int val2 = parent2.positions[i];
    do{ 
      p = mapper2[val1];
      if( child.positions[p]!=-1 ) 
        val1 = parent1.positions[p];
    }while(child.positions[p]!=-1);      
    child.positions[p] = val2;
    used[val2] = true;
  }

  // Fill the rest of the positions with parent2
  int pos_parent_2 = 0;
  FOR(i,0,n){
    if( child.positions[i] == -1 ){
      while( used[parent2.positions[pos_parent_2]] ) pos_parent_2++;
      child.positions[i] = parent2.positions[pos_parent_2];
      used[parent2.positions[pos_parent_2]] = true;
    }
  }

  child.cost = get_cost(instance_qap,child.positions);

  return child;
}

// =========================
// Main mutation function //
// =========================

vector<QAP_solution> crossover( QAP instance_qap, vector<QAP_solution> parents ){
  int N = parents.size();

  // array of crossover functions
  QAP_solution (*crossover_functions[])(QAP,QAP_solution,QAP_solution) = {
    random_with_order,
    two_points,
    partially_mapped_junction
  };

  // Cross all possible parents
  vector<QAP_solution> ans;
  FOR(i,0,N){
    FOR(j,i+1,N){
      QAP_solution child = crossover_functions[rand()%3](instance_qap,parents[i],parents[j]);
      ans.push_back(child); 
    }
  }

  return ans;
}
