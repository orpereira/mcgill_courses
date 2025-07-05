#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h"
#include "extremum.h"
#include "solve.h"

typedef double (*FuncPT)(double);   
FuncPT ORIG_FUNC;   
double Extremum_DF(double x); 


double Extremum_GetExtremum(FuncPT func, double x_init, double *curvature)
{
    double x, tol, ddf;
    int count;
    
    ORIG_FUNC = func; // To communicate with Extremum_DF
    
    // Use Solve_Newton to solve 0 = Extremum_DF(x) starting with x_init
    tol = 1.0E-5;   // Set tol to 1.0E-5
    count = 100;    // Set count to 100
    x = Solve_Newton(0.0, Extremum_DF, x_init, tol, &count);

    // Use Derivative_SecondD to calculate the second derivative
    // at the extremum
    ddf = Derivative_SecondD(x, func);
    
    // Set curvature to the second derivative.
    *curvature = ddf;

    // Return the value of x at the extremum
    return x;
}// Extremum_GetExtremum


double Extremum_DF(double x)
{
    double f;
    // Calculate the first derivative of ORIG_FUNC using Derivative_FirstD
    f = Derivative_FirstD(x, ORIG_FUNC);

    // Return the calculated val
    return f;
}//Extremum_DF