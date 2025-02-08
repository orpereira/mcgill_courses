#ifndef DERIVATIVES_H
#define DERIVATIVES_H

double Derivative_FirstD(double x, double (*func)(double));
double Derivative_SecondD(double x, double (*func)(double));

#endif