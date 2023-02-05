CC = g++
CFLAGS = -I -g -w -Wall 
BINARY_NAME =  QAPSolver

main:  main.cpp solver/solver.cpp
	$(CC) $(CFLAGS) -o $(BINARY_NAME) main.cpp solver/solver.cpp

clean:
	rm  -f $(BINARY_NAME)
