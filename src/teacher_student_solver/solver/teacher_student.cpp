#include "teacher_student.hpp"

const long long INF = 1e18;

/**
 * @brief This function build the initial population of the genetic algorithm
 * In this version we are building random permutations for the QAP problem
 * 
 * @param[in]   instance_qap  The instance of a quadratic assignment problem
 * @param[in]   amount The size of the initial population
 * 
 * @return     List of QAP_solutions with random permutations
 */
vector<QAP_solution> buildInitialPopulation_VTS(QAP instance_qap, int amount){
  vector<QAP_solution> ans = vector<QAP_solution>(amount);

  vector<int> facilities(instance_qap.N);
  iota(facilities.begin(), facilities.end(), 0);

  FOR(i,0,amount){
    QAP_solution sol;
    random_shuffle(facilities.begin(), facilities.end());
    sol.positions = facilities;
    sol.cost = get_cost(instance_qap,facilities);
    ans[i] = sol;
  }

  return ans;
}

void learn_from_teacher( QAP instance_qap, QAP_solution teacher, QAP_solution &student ){
  int N = instance_qap.N;
  // map positions in the student
  vector<int> student_positions(N);
  FOR(i,0,N){
    if( student.positions[i] >= N || student.positions[i] < 0 ){
      cout << "ERROR" << '\n';
      exit(1);
    }
    student_positions[student.positions[i]] = i;
  }

  // pick a random position and replace it with the teacher's position
  int pos = rand()%N;
  int teacher_value = teacher.positions[pos];
  if( teacher_value >= N || teacher_value < 0 ){
    cout << "ERROR1" << '\n';
    exit(1);
  }
  int pos_teacher_value = student_positions[teacher_value];
  if( pos_teacher_value >= N || pos_teacher_value < 0 ){
    cout << "ERROR2" << '\n';
    exit(1);
  }
  swap(student.positions[pos], student.positions[pos_teacher_value]);
}

QAP_solution teacher_student_solution(
  QAP instance_qap,
  QAP_solution &ans,
  int n_students = 4,
  int n_teachers = 100,
  int max_iterations = 20,
  bool verbose = true
){
  cout << "Teacher student solver" << '\n';
  
  // generate teachers 
  vector<QAP_solution> teachers = buildInitialPopulation_VTS(instance_qap, n_teachers);

  FOR(iteration,0,max_iterations){
    // generate students
    vector<QAP_solution> students = buildInitialPopulation_VTS(instance_qap, n_students);

    // iterate over teachers, for each teacher the students will learn from him
    FOR(i,0,n_teachers){
      // this represent the learning process in a classroom
      FOR(j,0,n_students){
        learn_from_teacher(instance_qap, teachers[i], students[j]);
      }
    }

    // chose the best students to be the new teachers (and update the best solution)
    FOR(i,0,n_students){
      if( students[i].cost < ans.cost ){
        ans = students[i];
      }
    }
    sort(students.begin(), students.end(), [](QAP_solution a, QAP_solution b){ return a.cost < b.cost; });
    teachers = vector<QAP_solution>(students.begin(), students.begin()+n_teachers);

    if(verbose) cout << "Iteration " << iteration << " best cost: " << ans.cost << '\n';
  }

  return ans;
}