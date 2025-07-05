#ifndef NUMEROV_PARAMS_H
#define NUMEROV_PARAMS_H
#include "params.h"

typedef double (*Func_1D)(double, DynamicVars *);

typedef struct numerov_params
{
    double x_f; // maximum x
    double x_i; // minimum x
    double y_0; // y(x_i)
    double y_1; // y(x_i + h)
    int nmax;   // number of sampling points
    double h;   // step size h = (x_i - x_f)/nmax

    Func_1D NumerovFunc_F; // The function F in y’’ = Fy
} NumerovParams; 

#endif