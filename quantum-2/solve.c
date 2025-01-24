// File: solve.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

// Numerical derivative function, only needed within this file
double Solve_Get_Df(double (*func)(double), double x);

//
// Solve_Bisect
double Solve_Bisect(double nu, double (*func)(double), double x_min, double x_max, 
                    double tol, int *count)
{
    /*
    Solve f(x) = nu using bisect method
    inputs:
        nu: the value to solve for
        func: the function to solve
        x_min: the lower bound of the range
        x_max: the upper bound of the range
        tol: the tolerance
        count: the number of iterations (pass by reference)

    returns: the solution 
    */

    // Declare variables
    double x_mid, f_max, f_min, f_mid, err;
    int count_max;

    // Set count_max to a large number and increment count by 1
    count_max = 1000; 
    *count += 1;    

    // If the number of iterations exceeds count_max, warn and exit. Prevents infinite loop.
    if(*count > count_max)
        {
            fprintf
            (stderr, "Solve_Bisect: Done %d iterations without convergence.\n", count_max);
            fprintf(stderr, "Exiting.\n");
            return NAN;
        }

    // Calculate f_max = f(x_max) - nu 
    f_max = (*func)(x_max) - nu;

    // Calculate f_min = f(x_min) - nu 
    f_min = (*func)(x_min) - nu;

    // If f_max*f_min > 0.0, root has not been bracketed, warn and exit.
    if(f_max*f_min > 0.0) 
        {
            fprintf(stderr, "Solve_Bisect: No solution found within the range [%f, %f].\n",
                    x_min, x_max);
            fprintf(stderr, "Exiting.\n");
            return NAN;
        }

    // Calculate x_mid = (x_min + x_max)/2.0 
    x_mid = (x_min + x_max)/2.0;
    
    // Calculate f_mid = f(x_mid) - nu
    f_mid = (*func)(x_mid) - nu;

    // Calculate the error, err = |f_mid/nu| or |f_mid| if nu = 0.0.
    if (nu != 0.0) { err = fabs(f_mid/nu); }
    else { err = fabs(f_mid); }

    // If err < tol, we have a solution and the calculation ends.
    if(err < tol) { return x_mid; }

    if(f_mid*f_max < 0.0) // the solution is between x_mid and x_max
        {
            // Recursive call to Solve_Bisect with the new range (x_mid, x_max)
            return Solve_Bisect(nu, func, x_mid, x_max, tol, count);
        }
    
    else if(f_min*f_mid < 0.0) // the solution is between x_min and x_mid
        {
            // Recursive call to Solve_Bisect with the range (x_min, x_mid) (done)
            return Solve_Bisect(nu, func, x_min, x_mid, tol, count);
        }
    
    else // one of the factors/boundary points is zero
        {
            // If f_mid = 0.0, then return x_mid
            if(f_mid == 0.0) { return x_mid; }

            // Else if f_max = 0.0,  then return x_max
            else if(f_max == 0.0) { return x_max; }

            // Else return x_min (done)
            else { return x_min; }
        }     
} // Solve_Bisect

// ---------------------------------------------------------------------
//
// Solve_Get_Df

double Solve_Get_Df(double (*func)(double), double x_old)
{
    /*
    Get the derivative of a function f’(x) = (f(x+h) - f(x-h))/(2h) + O(h^2)
    inputs:
        func: the function to differentiate
        x_old: the point at which to differentiate

    returns: the derivative
    */

    // Declare variables
    double h, df;
    
    // If x_old is not zero, set h = x_old*1.0E-5, else set h = 1.0E-5 (small enough number)
    if(x_old != 0.0) { h = x_old*1.0E-5; }
    else { h = 1.0E-5; }
    
    // Calculate the derivative using the formula
    df = (*func)(x_old+h) - (*func)(x_old-h);
    df /= 2.0*h;
    
    return df;
} //Solve_Get_Df

// ---------------------------------------------------------------------
//
// Solve_Newton
double Solve_Newton
(double nu, double (*func)(double), double x_0, double tol, int *count)
{
    /*
    Solve f(x) = nu using Newton’s method x_{n+1} = x_n + (nu -f(x_n))/f’(x_n)
    inputs:
        nu: the value to solve for
        func: the function to solve
        x_0: the initial guess (starting point)
        tol: the tolerance
        count: the number of iterations (pass by reference)

    returns: the solution
    */

    // Declare variables
    double x_old, x_new, err, df, h;
    int count_max;

    // Set count_max to a large number
    count_max = 1000;
    
    // Initial value
    x_old = x_0;

    // Using a do-while loop to iteratively call until the error is less than the tolerance
    do  { 
        df = Solve_Get_Df(func, x_old); // Get the derivative
        
        // If the derivative is too small, warn and exit
        if(fabs(df) < tol)
            {
                fprintf(stderr, "Solve_Newton: Derivative is too small.\n");
                fprintf(stderr, "Exiting.\n");
                return NAN;
            }

        // Calculate x_new using Newton's formula
        x_new = x_old + (nu - (*func)(x_old))/df;

        // Calculate err = |((x_new-x_old)/x_old)|
        err = fabs((x_new - x_old)/x_old);

        // Set x_old = x_new 
        x_old = x_new;

        // Increment count by 1
        *count += 1;

        // If the number of iterations exceeds count_max, warn and exit
        if(*count == count_max) 
            {
            fprintf(stderr, "Solve_Newton: Done %d iterations without convergence.\n", count_max);
            fprintf(stderr, "Exiting.\n");
            return NAN;
            }
        } while(err > tol); // while this condition is satisfied
    return x_new;
} // Solve_Newton