// File radial_eq_functions.c
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "radial_eq_functions.h"
#include "numerov_params.h"
#include "params.h"

#define alpha_EM (1.0/137.0)

double RadialEqFunctions_V(double r)
{
    double f;
    
    if(PARAM_DATA.nucA == 0.0) {
        // Coulomb potential for hydrogen atom
        f = -alpha_EM/r;
    } else {
        // Nuclear Woods-Saxon potential
        double V0 = 50.0/hbarc; // MeV to 1/fm
        double a = 0.7;
        double R_A = PARAM_DATA.r0;
        f = -V0/(1.0 + exp((r-R_A)/a));
    }
    
    return f;
}

double RadialEqFunctions_Veff(double r)
{
    double f, ell, mass;
    
    ell = (double) PARAM_DATA.ell;
    mass = PARAM_DATA.mass;
    f = RadialEqFunctions_V(r) + ell*(ell+1)/(2.0*mass*r*r);
    
    return f;
}

double RadialEqFunctions_F_Forward(double x, DynamicVars *Dyn_Vars)
{
    double x0, ka, r, f, Ea, Et, ell, eps;
    
    ell = (double) PARAM_DATA.ell;
    x0 = PARAM_DATA.x0;
    ka = PARAM_DATA.ka;
    Ea = PARAM_DATA.Ea;
    Et = Dyn_Vars->Et;
    
    eps = 1.0e-15;
    x += eps;
    r = x/ka;
    
    f = ell*(ell+1)/(x*x) + RadialEqFunctions_V(r)/Ea + Et;
    return f;
}

double RadialEqFunctions_F_Backward(double y, DynamicVars *Dyn_Vars)
{
    double ka, r, f, ell, Ea, Et, x;
    
    ell = (double) PARAM_DATA.ell;
    ka = PARAM_DATA.ka;
    Ea = PARAM_DATA.Ea;
    Et = Dyn_Vars->Et;
    
    x = Dyn_Vars->xf - y;
    r = x/ka;
    
    f = ell*(ell+1)/(x*x) + RadialEqFunctions_V(r)/Ea + Et;
    return f;
}