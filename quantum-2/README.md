# PHYS 457 Coding Project - Part 2
Olivia Pereira 260985600

## How to compile
A Makefile is included to compile using ``gcc`` as shown in the assignment instructions. 
To compile and run, do the following on your terminal:

$ make

Or, to compile only one part:
$ make solve_test
$ make deriv_test
$ make extremum_test
$ make numerov_test
$ make schroedinger_test

Or, to compile manually:
$ gcc -o solve_test main_solve.o solve.o -lm

To run each test script, do:
$ ./solve_test
$ ./deriv_test
$ ./extremum_test
$ ./numerov_test input_n_params.txt
$ ./schroedinger_test input_coulomb.txt input_n_params.txt


Where here, `input_n_params.txt` is a txt file containing the following values (just input the numbers, an example file is provided): 
    x_i  
    x_f  
    y_0  
    y_1  
    nmax  

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

## Part 3
The third part adds the functionality to solve equations using the numerov method. To test different functions, edit line 51/52 for the function you want to test, and line 116 for the desired `.dat` file name for your y-values to be outputted. You can also edit `input_n_params.txt` as shown above. Be sure to recompile between function changes. For generality, it is set to save outputs at `y.dat` for now.

To make the plots, an example python file is included to read and plot the data. Two functions, $F(x) = -1$ and $F(x) = -x$ have been run and plotted to show the code works - the plots are saved in `numerov_plots.pdf`. You can plot the generic `y.dat` (or any other file made) by running the following:

$ python plot.py

## Part 4
This fourth part sets up the preparation steps to solve the full schroedinger equation. At this stage, it tests that with inputs $m=0.511\times 10^6$ eV, $l=0$, and nuclear numbers A=Z=0, we can recover the Bohr radius and ground state energy of the hydrogen atom. For this stage, `input_n_params.txt` sets the forward and backward evolutions to `nmax=500`, and `input_coulomb.txt` sets aforementioned input parameters. The outputs of the equation solver are saved in a file `num_params.dat`.
