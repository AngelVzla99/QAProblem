CC = g++
CFLAGS = -I -g -w -Wall 
BINARY_NAME =  QAPSolver

main:  main.cpp solver/solver.cpp
	$(CC) $(CFLAGS) -o $(BINARY_NAME) main.cpp solver/solver.cpp

exact_solver: exact_solver/main.cpp exact_solver/solver/solver.cpp QAP/QAP.cpp
	$(CC) $(CFLAGS) -o $(BINARY_NAME) exact_solver/main.cpp exact_solver/solver/solver.cpp QAP/QAP.cpp

clean:
	rm  -f $(BINARY_NAME)
