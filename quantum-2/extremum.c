#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h"
#include "extremum.h"
#include "solve.h"

typedef double (*FuncPT)(double);   // FuncPT is a pointer to a function
                                    // that takes in a double and
                                    // returns a double.
                                    // We use this to pass an arbitrary function
                                    // to another function.
FuncPT ORIG_FUNC;   // A common variable. Only valid within this file
double Extremum_DF(double x); // Used only within this file

// Extremum_GetExtremum finds the miminum or maximum near x_init
// This function returns the value of x where the extremum is
// The variable curvature has the value of the second derivative
// at the extremum
//

double Extremum_GetExtremum(FuncPT func, double x_init, double *curvature)
{
    double x, tol, ddf;
    int count;
    
    ORIG_FUNC = func; // To communicate with Extremum_DF
    
    // TD: Use Solve_Newton to solve 0 = Extremum_DF(x) starting with x_init
    tol = 1.0E-5;   // Set tol to 1.0E-5
    count = 100;    // Set count to 100
    x = Solve_Newton(0.0, Extremum_DF, x_init, tol, &count);

    // TD: Use Derivative_SecondD to calculate the second derivative
    // at the extremum
    ddf = Derivative_SecondD(x, func);
    
    // TD: Set curvature to the second derivative.
    // Note that curvature is a pointer.
    // Hence, you need to do *curvature = (calculated value);
    // If you do curvature = (calculated value), you’ll get an error
    // since the types don’t match
    *curvature = ddf;

    // TD: Return the value of x at the extremum
    return x;
}// Extremum_GetExtremum

// We are using FuncPT func -> ORIG_FUNC here because
// Solve_Newton can only take in double func(double) type of function
double Extremum_DF(double x)
{
    double f;
    // TD: Calculate the first derivative of ORIG_FUNC using Derivative_FirstD
    f = Derivative_FirstD(x, ORIG_FUNC);

    // TD: Return the calculated val
    return f;
}//Extremum_DF