"""
In this file we compress the information that we get for each solver, to make a 
result table, we calculate the percentage of error, and the time for each solver.
"""

import pandas as pd

solutions = pd.read_csv('benchmark/solutions.csv')

model_names = ['solution_file_local_search']
model_names.append('solution_file_iterative_local_search')
model_names.append('solution_file_tabu_search')
model_names.append('solution_file_genetic_algorithm')
# ant colony search
model_names.append('ant_colony_results/solution_file_ant_colony_search_50_4')
model_names.append('ant_colony_results/solution_file_ant_colony_search_50_25')
model_names.append('ant_colony_results/solution_file_ant_colony_search_50_50')
model_names.append('ant_colony_results/solution_file_ant_colony_search_150_4')
# memetic algorithm
model_names.append('solution_file_memetic_algorithm')
# Multi modal
model_names.append('multi_modal/solution_file_multi_modal_popSize:100_maxIter:10_step:10')
model_names.append('multi_modal/solution_file_multi_modal_popSize:100_maxIter:10_step:15')
model_names.append('multi_modal/solution_file_multi_modal_popSize:100_maxIter:15_step:15')
model_names.append('multi_modal/solution_file_multi_modal_LS:25_popSize:100_maxIter:15_step:15')
model_names.append('multi_modal/solution_file_multi_modal_LS:75_popSize:100_maxIter:15_step:15')
# Teacher student
model_names.append('teacher_student/solution_file_teacher_student_nStudents:5_nTeachers:5_maxIter:10')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:5_nTeachers:5_maxIter:50')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:5_nTeachers:5_maxIter:100')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:20_nTeachers:20_maxIter:10')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:20_nTeachers:20_maxIter:50')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:20_nTeachers:20_maxIter:100')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:50_maxIter:10')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:20_maxIter:10')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:20_maxIter:50')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:20_maxIter:100')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:50_maxIter:10')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:50_maxIter:50')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:50_nTeachers:50_maxIter:100')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:100_nTeachers:20_maxIter:10')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:100_nTeachers:20_maxIter:50')
model_names.append('teacher_student/solution_file_teacher_student_nStudents:100_nTeachers:20_maxIter:100')
ans = []
for model in model_names:
  # Read csv from file
  df = pd.read_csv('../../benchmark/'+model+'.csv')

  # joint between solutions and df
  df = df.merge(solutions, on='Problem', how='left')

  # Answer ----- 100%
  # Solution --- x
  # Solution*100/Answer = x

  # df manipulation
  df['Ans_diff'] = (df['Solution']*100)/df['Answer'] - 100
  df = df[df['Time'] > -1] 
  df = df[df['Answer'] > -1] 
  df = df[df['Solution'] > -1] 
  # count zeros in Ans_diff
  df['zeros'] = df['Ans_diff'].apply(lambda x: 1 if x == 0 else 0)

  # print(df.sort_values('Ans_diff').head(100))

  # => results
  # Time
  avg_time = df['Time'].mean(axis=0)
  std_time = df['Time'].std(axis=0)
  min_time = df['Time'].min(axis=0)
  max_time = df['Time'].max(axis=0)
  # Answer
  perc_error_avg = df['Ans_diff'].mean(axis=0)
  perc_error_std = df['Ans_diff'].std(axis=0)
  perc_error_min = df['Ans_diff'].min(axis=0)
  perc_error_max = df['Ans_diff'].max(axis=0)
  # zeros
  zeros = df['zeros'].sum(axis=0)
  # Accumulate
  ans.append( {'algorithm':model, 'time_avg':avg_time, 'std_time':std_time, 'min_time':min_time, 'max_time':max_time, 'perc_error_avg':perc_error_avg, 'perc_error_std':perc_error_std, 'perc_error_min':perc_error_min, 'perc_error_max':perc_error_max, 'exact':zeros} )

df_models = pd.DataFrame(ans)
df_models.to_csv('benchmark/results.csv', index=False)
print(df_models.head(20))
