#gcc -o solve_test main_solve.c solve.c -lm

# for part 2:
# gcc -o deriv_test main_deriv.c derivatives.c -lm
# gcc -o extremum_test main_extremum.c extremum.c derivatives.c solve.c -lm

# default to run all tests:
all: solve_test deriv_test extremum_test

# compiling the test results
solve_test:
	gcc -o solve_test main_solve.c solve.c -lm

deriv_test:
	gcc -o deriv_test main_deriv.c derivatives.c -lm

extremum_test:
	gcc -o extremum_test main_extremum.c extremum.c derivatives.c solve.c -lm

# remove all object files and the test results
clean:
	rm -f *.o *.dat solve_test deriv_test extremum_test 