# PHYS 457 Coding Project - Part 2
Olivia Pereira 260985600

## How to compile
a Makefile is included to compile using ``gcc`` as shown in the assignment instructions. 
To compile and run, do the following on your terminal:

$ make

Or, to compile manually:
First, we want to make the solve_test file:
$ gcc -o solve_test main_solve.c solve.c -lm

To make the deriv_test file:
$ gcc -o deriv_test main_deriv.c derivatives.c -lm

To make the extremum_test file:
$ gcc -o extremum_test main_extremum.c extremum.c derivatives.c solve.c -lm

To run each test script, do:
$ ./solve_test
$ ./deriv_test
$ ./extremum_test

To clean all output files created (i.e., remove all .dat and test files), run 
$ make clean

## Functionality
### Part 1
This will test 5 different functions on ranges [0.0, 5.0] to find their roots:
1. sin(x) 
2. x*exp(x) - 3.0
3. cos(x) - x
4. x^2 - 4.0 
5. log(x) - 1.0 

We also test some edge cases to make sure the program fails where we expect it to:
6. log(x) - 10 (does not have a root within the rang [0, 5], so Bisection fails but Newton succeeds)
7. 1/x - 2 (Derivative will be small after a few iterations, so Newton fails but Bisection succeeds)

### Adding functions to test
To add functions to test, in ``main_solve.c`` change lines 13-15 to reflect an added function, and 
then add the function and its name in lines 51-67. Parameters can also be played with on lines 20
and 21 to change tolerance and range tested over, as well as the nu value.

## A note on error handling
Note that in solve.c, I've chosen to ``return NAN`` instead of ``exit(0)`` in the case of an error. 
This is just for illustrative purposes to make sure that main_solve.c shows the functionality of the
program and can show failure cases without the whole program ending. I will likely change it back to
``exit(0)`` once this needs to be used for implementation in the future parts of the project.

## Part 2
The second part of the project adds functionality to compute derivatives and find function extrema. Running `deriv_test` will generate a file called `deriv_test.dat` with the computed derivates. 

Running `extremum_test` will print the extremum location, value, and curvature at the extremum (serving to indicate whether it is a minimum or a maximum). 
The following functions are tested: 
- $x*x - 2.0*x + 1$
- $1.0/x/x - 2.0/x$ 
- $1.0/x/x - 1.0/(1.0 + exp(x - 5.0))$