#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "derivatives.h"

double test_func(double x);
int main(void)
{
    int i, imax;
    double x, dx, x_min, x_max, f, df, ddf;
    FILE *output;   
                    x_min = 0.0;
                    x_max = 15.0;

                    imax = 1000;

                    dx = (x_max - x_min)/((double) imax);
                    
                    output = fopen("deriv_test.dat","w");   

    for(i=0; i<=imax; i++)
    {
        x = x_min + dx*i;
        // Calculate test_func(x) and put it in f.
        f = test_func(x);

        // Calculate the first derivative of test_func and put it in df
        df = Derivative_FirstD(x, test_func);
        // Calculate the second derivative of test_func and put it in ddf
        ddf = Derivative_SecondD(x, test_func);

        // Write to the file
        fprintf(output, "%e  %e  %e  %e\n", x, f, df, ddf);

    }// i
    fclose(output); 
    
    return 0;
}// main

double test_func(double x)
{
    return sin(x);
}// test_fun