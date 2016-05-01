#include <math.h>
#include <stdlib.h>

#ifndef __my_funcs_h__
#define __my_funcs_h__
#define DATA_SIZE 3735658
extern double U_in[DATA_SIZE];
extern double Z_out[DATA_SIZE];

void   state_eq(double x[4],double beta[26],double u,double f[4]);
double OCV(double x[7],double y[8],double x0);
double output_eq(double x[4],double beta[26],double u);
void sys_output(double x[4],double beta[26],double Y[DATA_SIZE]);
double my_cost_func(double pp[30]);

#endif
