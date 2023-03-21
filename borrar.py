import pandas as pd

df = pd.read_csv('benchmark/solution_file_local_search.csv')

ans = []
for file_name in df['Problem'].tolist():
    # data file
    data_file = file_name

    # remove last three characters
    file_name = file_name.replace('.dat', '.sln')
    file_name = file_name.replace('qapdata', 'qapsoln')
    print(file_name)

    try:
      with open(file_name) as f:
        lines = f.read() ##Assume the sample file has 3 lines
        first = lines.split('\n', 1)[0].strip()
        print(first.split(' ')[-1])
        ans.append({'Problem': data_file, 'Answer': int(first.split(' ')[-1])})
    except:
      print('error')
      ans.append({'Problem': data_file, 'Answer': -1})


# delete kra32 
df = pd.DataFrame(ans)
df.to_csv('benchmark/solutions.csv', index=False)