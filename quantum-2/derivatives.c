#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h" // Contains Derivative_FirstD
                         // and Derivative_Second

double Derivative_FirstD(double x, double (*func)(double))
{
    double df, h;
    h = 1.0E-5; // Set h to 1.0E-5

    // If x is non-zero, set h to h*
    if (x != 0.0) { h = h*x; }

    // Implement df = ( f(x+h) - f(x-h) )/(2h)
    df = (func(x+h) - func(x-h))/(2.0*h);

    return df;
}// Derivative_First

double Derivative_SecondD(double x, double (*func)(double))
{
    double ddf, h;
    h = 1.0E-5; // Set h to 1.0E-5

    // If x is non-zero, set h to h*x
    if (x != 0.0) { h = h*x; }
    
    // Implement ddf = ( f(x+h) + f(x-h) - 2 f(x) )/(h*h)
    ddf = (func(x+h) + func(x-h) - 2.0*func(x))/(h*h);
    
    return ddf;
} // Derivative_Second