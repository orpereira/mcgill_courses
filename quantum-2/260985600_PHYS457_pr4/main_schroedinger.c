#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "params.h"
#include "numerov_params.h"
#include "init.h"
// #include "schroedinger.h"
#include "vector_mtx.h"

void ReadIn_Params(char *input_file);

void ReadIn_Num_Params
(char *input_file_name,
NumerovParams *Num_Params_f, NumerovParams *Num_Params_b);

// Record parameters
void Record_Params(NumerovParams Num_Params_f, NumerovParams Num_Params_b);

// Record results
void Record_Results(DynamicVars Dyn_Vars,
NumerovParams Num_Params_f, NumerovParams Num_Params_b, double *yf, double *yb);
Params PARAM_DATA;

int main(int argc, char **argv)
{
    DynamicVars Dyn_Vars; // These parameters are calculated
    NumerovParams Num_Params_f; // For the forward evolution of u_I
    NumerovParams Num_Params_b; // For the backward evolution of u_II
    double *yf, *yb; // yf contains u_I, yb contains u_II

    // input for PARAM_DATA
    ReadIn_Params(argv[1]); 

    // initial data for Num_Params_f and Num_Params_b
    ReadIn_Num_Params(argv[2], &Num_Params_f, &Num_Params_b);
    Init_CalcScales(); // get energy and length scales

    // Record the parameters
    Record_Params(Num_Params_f, Num_Params_b);

    /* Will be implemented in the next section */
    /*
    // Allocate memory for the forward wavefunction yf
    yf = vector_malloc(Num_Params_f.nmax+1);
    // Allocate memory for the backward wavefunction yb
    yb = vector_malloc(Num_Params_b.nmax+1);
    Schroedinger_GetBoundState(&Dyn_Vars, &Num_Params_f, &Num_Params_b, yf, yb);
    Record_Results(Dyn_Vars, Num_Params_f, Num_Params_b, yf, yb);
    */
    /* Next section */

    return 0;
}// main


void ReadIn_Params(char *input_file)
{
    FILE *input;
    double x;
    int ix;
    char *mass_unit;
    input = fopen(input_file, "r"); 

    // Read in the mass and divide it by hbarc so that it has the 1/length unit. 
    fscanf(input, "%le", &x);
    PARAM_DATA.mass = x/hbarc;

    // Read in the mass unit
    mass_unit = (char *) malloc(sizeof(char)*10);
    fscanf(input, "%s", mass_unit);
    PARAM_DATA.mass_unit = mass_unit;

    // According to the mass unit, determine the length unit
    if(strcmp(mass_unit, "eV")==0) {PARAM_DATA.length_unit = "nm";}
    else if(strcmp(mass_unit, "MeV")==0) {PARAM_DATA.length_unit = "fm";}
    else {
        fprintf(stderr, "ReadIn_Params: %s is an unknown unit.\n", mass_unit);
        fprintf(stderr, "Known units are eV and MeV.\n");
        fprintf(stderr, "Exiting.\n");
        exit(0);
    }

    // Read in the orbital angular momentum ell
    fscanf(input, "%d", &ix);
    PARAM_DATA.ell = ix;

    // Read in the atomic mass A as a double
    fscanf(input, "%le", &x);
    PARAM_DATA.nucA = x;

    // Read in the atomic mass Z as a double 
    fscanf(input, "%le", &x);
    PARAM_DATA.nucZ = x;

    fclose(input); // Always close an opened file
    return;
}// ReadIn_Params

void Record_Params(NumerovParams Num_Params_f, NumerovParams Num_Params_b)
{
    double x;
    int i;
    FILE *output;
    output = fopen("num_params.dat", "w");

    // Record mass, r0, Ea, ka, ell, x0, nucA, nucZ, and
    // nmax (both forward and backward) in the data file.
    fprintf(output, "mass = %e %s\n", PARAM_DATA.mass, PARAM_DATA.mass_unit);
    fprintf(output, "r0 = %e %s\n", PARAM_DATA.r0, PARAM_DATA.length_unit);
    fprintf(output, "Ea = %e %s\n", PARAM_DATA.Ea, PARAM_DATA.mass_unit);
    fprintf(output, "ka = %e %s\n", PARAM_DATA.ka, PARAM_DATA.length_unit);
    fprintf(output, "ell = %d\n", PARAM_DATA.ell);
    fprintf(output, "x0 = %e %s\n", PARAM_DATA.x0, PARAM_DATA.length_unit);
    fprintf(output, "nucA = %e\n", PARAM_DATA.nucA);
    fprintf(output, "nucZ = %e\n", PARAM_DATA.nucZ);
    fprintf(output, "nmax_f = %d\n", Num_Params_f.nmax);
    fprintf(output, "nmax_b = %d\n", Num_Params_b.nmax);

    fclose(output);
    return;
}// Record_Params

// Read in NumerovParams data
void ReadIn_Num_Params
(char *input_file_name, NumerovParams *Num_Params_f, NumerovParams *Num_Params_b)
{
    FILE *input_file;
    double x;
    int ix;

    // Open the input file input_file_name to read
    input_file = fopen(input_file_name, "r");

    fscanf(input_file, "%d", &Num_Params_f->nmax);  // Read in nmax for the forward evolution
    fscanf(input_file, "%d", &Num_Params_b->nmax);  // Read in nmax for the backward evolution
 
    fclose(input_file);
    return;
}// ReadIn_Num_Params

void Record_Results
(DynamicVars Dyn_Vars, NumerovParams Num_Params_f, 
    NumerovParams Num_Params_b, double *yf, double *yb)
{
    // Next section
    return;
}// Record_Results
