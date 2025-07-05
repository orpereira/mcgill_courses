#ifndef RADIAL_EQ_FUNCTIONS_H
#define RADIAL_EQ_FUNCTIONS_H
#include "params.h" 

double RadialEqFunctions_V(double r);   // Potential energy in r
double RadialEqFunctions_Veff(double r);    // V_eff = V + ell(ell+1)/(2m r^2)
double RadialEqFunctions_F_Forward(double x, DynamicVars *Dyn_Vars);
double RadialEqFunctions_F_Backward(double x, DynamicVars *Dyn_Vars);

#endif