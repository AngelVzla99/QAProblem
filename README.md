# QAProblem
This repository have different solvers for the [Quadratic assignment problem](https://en.wikipedia.org/wiki/Quadratic_assignment_problem) in c++, this is a well know NP-Complete problem, for this we decide to try different meta heuristics to the better solutions as posible in our benchmark. 

# Algorithms used:
* Local search
* Iterative local search
* Tabu search
* Genetic algorithm
* Genetic algorithm with multiple populations
* Ant cololny optimization
* Memetic algorithm

# How is structured this repository ?

In the src folder you will find multiple folder, each one of then have an algorithm to solve an instance of a QAP problem also they have a main.cpp for testin purpose

# How to run a solver ?

If you only want to test one solver, you can edit his main.cpp, then run

```sh
$ make main_{SOLVER_NAME}
$ ./bin/main_{SOLVER_NAME}
```

(you can find the name of the solvers in our Make file)

If you want to test a solver in the benchmark you can do this:

```sh
$ make main_main_benchmark
$ ./bin/main_main_benchmark
```

We add a simple CLI to select the solver and the test.

# Results 

The results can be found in documents/paper.pdf file
