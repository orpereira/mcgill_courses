// File main_solve.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

// Initialize the functions to test and their names
double f1(double x); 
double f2(double x); 
double f3(double x); 
double f4(double x); 
double f5(double x); 
double f6(double x); 
double (*functions[])(double) = {f1, f2, f3, f4, f5, f6};
const char *function_names[];

int main(void)
{
    // Declare variables
    double tol = 1.0e-10, nu = 0.0;
    double x_min = 0.0, x_max = 5.0, x;   
    int count;  // initialize count to hold the number of iterations

    // Get the number of functions to test
    int num_functions = sizeof(functions) / sizeof(functions[0]);

    // Loop over the functions and try to solve them
      for (int i = 0; i < num_functions; i++) {
        fprintf(stdout, "----------------------------------------------------------\n");
        fprintf(stdout, "Solving %s = 0 on the interval [%.1f, %.1f]\n", 
                function_names[i], x_min, x_max);

        fprintf(stdout, "Solve_Bisect\n");
        count = 0;
        x = Solve_Bisect(nu, functions[i], x_min, x_max, tol, &count);
        fprintf(stdout, "count = %d\n", count);
        fprintf(stdout, "x = %e\n", x);


        fprintf(stdout, "Solve_Newton\n");
        count = 0;
        x = Solve_Newton(nu, functions[i], x_max, tol, &count);
        fprintf(stdout, "count = %d\n", count);
        fprintf(stdout, "x = %e\n", x);
      }

    return 1;
}


// define the functions to test and their names
double f1(double x) { return sin(x); }
double f2(double x) { return x * exp(x) - 3.0; }
double f3(double x) { return cos(x) - x; }
double f4(double x) { return x*x - 4.0; }
double f5(double x) { return log(x) - 1.0; }
double f6(double x) { return 1/x - 2; }

const char *function_names[] = 
        {
            "sin(x)", 
            "x*exp(x) - 3.0",
            "cos(x) - x",  
            "x^2 - 4.0",
            "log(x) - 1.0",
            "1/x - 2"
        };


