# Description of the benchmarking suite

## Instances of the Quadratic Assignment Problem

In the folder qapdata you can find the instances of the Quadratic Assignment Problem. The instances are in the format of the [QAPLIB](https://www.opt.math.tugraz.at/qaplib/),

### Description of the instances

The instances are listed in alphabetical order by the names of their authors. All the instances in the current version are pure quadratic. If not stated otherwise the examples are symmetric.

The format of the problem data is

    n
    A
    B

where n is the size of the instance, and A and B are either flow or distance matrix. This corresponds to a QAP of the form

## Solutions of the Quadratic Assignment Problem

In the folder qapsoln you can find the solutions of the Quadratic Assignment Problem. The solutions are in the format of the [QAPLIB](https://www.opt.math.tugraz.at/qaplib/),

### Description of the solutions

The format of these files is

       n    sol
       p

where n gives the size of the instance, sol is the objective function value and p a corresponding permutation
