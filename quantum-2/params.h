// File: params.h
#ifndef PARAMS_H
#define PARAMS_H

#define hbarc (197.3) // MeV.fm = eV.nm

typedef struct params {
    double mass;        // Mass of the particle
    double Ea;          // Energy scale
    double ka;          // Momentum scale
    double r0;          // Length scale
    double x0;          // x0 = k*r0
    int ell;
    
    char *mass_unit;    // The unit of mass. Either MeV or eV
    char *length_unit;  // The unit of length. Either fm (goes with MeV) or nm (goes with eV)
    
    double nucA;        // Atomic mass
    double nucZ;        // Atomic number
} Params;

typedef struct dynamic_vars {
    double Eb;          // Absolute value of the bound energy
    double kb;          // New
    double rc;          // The turning point radius
    double Et;          // Eb/Ea
    double xc;          // ka*rc
    double rf;
    double xf;
    
    double Et_min;
    double Et_max;
} DynamicVars;

extern Params PARAM_DATA; // Run specific data

#endif