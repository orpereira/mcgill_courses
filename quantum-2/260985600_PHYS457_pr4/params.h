#ifndef PARAMS_H
#define PARAMS_H

#define hbarc (197.3) // MeV.fm = eV.nm


// Set parameters that will be constant during calculations

typedef struct params
{
    double mass;    // Mass of the particle
    double Ea;      // Energy scale
    double ka;      // Momentum scale
    double r0;      // Length scale
    double x0;      // x0 = k*r0
    int ell;

    char *mass_unit;    // The unit of mass. Either MeV or eV
    char *length_unit;  // The unit of length. Either fm (goes with MeV)
                        // or nm (goes with eV)

    double nucA; // Atomic mass
    double nucZ; // Atomic number

    // To bracket the energy eigenvalue search
    double Et_min;
    double Et_max;
} Params;

// Variable parameters that will change during calculations
typedef struct dynamic_vars
{
    double Eb; // Absolute value of the bound energy
    double kb; // sqrt(2*mass*Eb)
    double rc; // The turning point radius
    double Et; // Eb/Ea
    double xc; // ka*rc
    double rf; // The last point
    double xf; // xf = ka*rf.
} DynamicVars;

extern Params PARAM_DATA; // Run specific data

#endif