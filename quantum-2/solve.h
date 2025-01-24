// File solve.h

#ifndef SOLVE_H // if SOLVE_H is not defined
#define SOLVE_H // define SOLVE_H
        // All header files should start like this.
        // This is to avoid including the same header file twice
        // If you do include this header file multiple times
        // then SOLVE_H will be defined when it was first seen by the compiler.
        // When the compiler encounters the header file the next time,
        // SOLVE_H is already defined. Hence, the content will
        // be bypassed by the ifndef/define/endif (at the end of the file)
        // statements.

// Functions in the same file should all share the same prefix.
// There are many reasons for this. The most prominent one is that
// you instantly know where the function is when the function is
// used in another file.

// Solve_Bisect solves f(x) = nu using the bisect search method
double Solve_Bisect(double nu, double (*func)(double), double x_min, double x_max, double tol, 
                    int *count);
                
// double (*func)(double) is C’s way of passing a function to another
// function. See how it is used in main_solve.c below

// Solve_Newton solves f(x) = nu using Newton’s method
double Solve_Newton(double nu, double (*func)(double), double x_0, double tol, int *count);

#endif // end if