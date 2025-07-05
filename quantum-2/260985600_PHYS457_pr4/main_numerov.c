#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector_mtx.h"
#include "numerov.h"
#include "numerov_params.h"
#include "params.h"

double TestF(double x, DynamicVars *Dyn_Vars); // Test purposes

void ReadInNum_Params(char *input_file_name, NumerovParams *Num_Params);
void PrintParams(NumerovParams Num_Params);
void PrintY(double *y, NumerovParams Num_Params);

int main(int argc, char **argv)
{
    double *y; 
    char *input; 

    NumerovParams Num_Params; // To hold static data needed for Numerov method
    DynamicVars Dyn_Vars; // To hold data that may change during the calculations

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    input = argv[1];
    ReadInNum_Params(input, &Num_Params); // Fill Num_Params, for this test only
    
    PrintParams(Num_Params); // Check if everything is read-in correctly

    // Record the read-in data in an output file
    y = vector_malloc(Num_Params.nmax+1); // Allocates an array
    if (y == NULL) {
        fprintf(stderr, "Memory allocation failed for y\n");
        return 1;
    }

    Num_Params.NumerovFunc_F = TestF; // Assign the F function in y’’ = Fy
    
    Numerov_Advance(y, &Num_Params, &Dyn_Vars); // Solve y’’ = Fy

    PrintY(y, Num_Params); // Print y to an output file
    free(y); // Always free allocated memories after use
    return 0;
}// main

double TestF(double x, DynamicVars *Dyn_Vars)
{
    return -x; // Airy
    // return -1.0; // SHO
}// TestF

// For this test only - later, we will use Schroedinger_InitializeCommonVar(void)
void ReadInNum_Params(char *input_file_name, NumerovParams *Num_Params)
{
    FILE *input_file;
    double x;
    int ix;
    input_file = fopen(input_file_name, "r"); // Open the input file
    if (input_file == NULL) {
        fprintf(stderr, "Failed to open input file: %s\n", input_file_name);
        exit(1);
    }

    fscanf(input_file, "%le", &x); // Read in the first line and put its value in x
    Num_Params->x_i = x;

    // Read in x_f, y_0 and y_1.
    fscanf(input_file, "%le", &x);
    Num_Params->x_f = x;
    fscanf(input_file, "%le", &x);
    Num_Params->y_0 = x;
    fscanf(input_file, "%le", &x);
    Num_Params->y_1 = x;

    // Read in an integer value and put its value in ix
    fscanf(input_file, "%d", &ix);
    Num_Params->nmax = ix;

    // Calculate h = (x_f - x_i)/nmax and put it in Num_Params->h
    Num_Params->h = (Num_Params->x_f - Num_Params->x_i)/Num_Params->nmax;

    fclose(input_file); 
    return;
}// ReadInNum_Params

void PrintParams(NumerovParams Num_Params)
{
    FILE *output;
    output = fopen("params.dat","w"); // Open a file for "w"riting
    if (output == NULL) {
        fprintf(stderr, "Failed to open output file: params.dat\n");
        return;
    }
    fprintf(output, "x_i = %e\n", Num_Params.x_i);
    
    // Record x_f, y_0, y_1, nmax and h
    fprintf(output, "x_f = %e\n", Num_Params.x_f);
    fprintf(output, "y_0 = %e\n", Num_Params.y_0);
    fprintf(output, "y_1 = %e\n", Num_Params.y_1);
    fprintf(output, "nmax = %d\n", Num_Params.nmax);
    fprintf(output, "h = %e\n", Num_Params.h);
    
    // To print an integer, use %d
    fclose(output); 
    return;
}// PrintParams

void PrintY(double *y, NumerovParams Num_Params)
{
    FILE *output;
    int n;
    double xn;
    // Open a file for writing and assign it to output.
    output = fopen("y.dat", "w");

    for( n=0; n<=Num_Params.nmax; n++ )
    {
        xn = Num_Params.x_i + n*Num_Params.h;
        fprintf(output, "%e %e\n", xn, y[n]);
    }
    fclose(output);
    return;
}// PrintY