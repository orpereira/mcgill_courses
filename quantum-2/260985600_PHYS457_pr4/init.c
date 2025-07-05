#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "params.h"
#include "init.h"
#include "radial_eq_functions.h"
#include "solve.h"
#include "extremum.h"
#include "derivatives.h"

double Init_Rmin_Function(double r);

// The function to minimize to get Ea and ka, equals V(r) + 1/(2 m r^2)
void Init_CalcScales(void)
{
    double r_min, r_init, E_min, Eb, mass, r_c, curvature;
    double R_A;
    FILE *output;

    mass = PARAM_DATA.mass;

    if(PARAM_DATA.nucA == 0.0) // Not a nuclear potential
    {
        r_init = 0.01; // to start the iteration

        // Get the minimum radius r_min 
        r_min = Extremum_GetExtremum(Init_Rmin_Function, r_init, &curvature);
        PARAM_DATA.r0 = r_min;

        // Calculate E_min as the value of Init_Rmin_Function at r_min
        E_min = Init_Rmin_Function(r_min);
        PARAM_DATA.Ea = fabs(E_min);
        // TD: Put |E_min| in Ea in PARAM_DATA
    }

    else // Nuclear potential. We know scales in this case.
    {
        // Next section
        PARAM_DATA.r0 = 1.3*pow(PARAM_DATA.nucA, 1.0/3.0); // Nuclear radius
        PARAM_DATA.Ea = 50.0/hbarc; // about 50 MeV
    }
    
    PARAM_DATA.ka = sqrt(2.0*mass*PARAM_DATA.Ea);
    PARAM_DATA.x0 = PARAM_DATA.ka*PARAM_DATA.r0;

    return;
}// Init_CalcScales

double Init_Rmin_Function(double r) // Function to minimize = V(r) + 1/(2 m r^2)
{
    double f, mass;
    
    mass = PARAM_DATA.mass;
    f = RadialEqFunctions_V(r) + 1.0/(2.0*mass*r*r);

    return f;
}// Init_Rmin_function