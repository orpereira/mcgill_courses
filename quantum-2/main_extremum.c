#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extremum.h"

double test_func1(double x);
double test_func2(double x);
double test_func3(double x);

void run_test(double (*func)(double), double x_init)
{
    double x_extremum, curvature;
    x_extremum = Extremum_GetExtremum(func, x_init, &curvature);
    printf("x_extremum = %e, test_func(x_extremum) = %e, curvature = %e\n", 
                                        x_extremum, func(x_extremum), curvature);
}// run_test

int main(void)
{
    // Declare necessary variables
    double x_init, x_extremum, curvature;
    
    // Test 1
    printf("Test 1: x*x - 2.0*x + 1\n ");
    x_init = 0.0;
    run_test(test_func1, x_init);

    // Test 2
    printf("Test 2: 1.0/x/x - 2.0/x\n");
    x_init = 1.0;
    run_test(test_func2, x_init);

    // Test 3
    printf("Test 3: 1.0/x/x - 1.0/(1.0 + exp(x - 5.0))\n");
    x_init = 1.0;
    run_test(test_func3, x_init);
    
    return 0;
}// main

double test_func1(double x)
{
    double f;
    f = x*x - 2.0*x + 1;
    // (x-1)^2 which has the minimum at x = 1

    return f;
}// test_func1

double test_func2(double x)
{
    double f;
    f = 1.0/x/x - 2.0/x;
    return f;
}// test_func2

double test_func3(double x)
{
    double f;
    f = 1.0/x/x - 1.0/(1.0 + exp(x - 5.0));
    return f;
}// test_func3