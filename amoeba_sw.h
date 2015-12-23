#ifndef AMOEBA_SW_H
#define AMOEBA_SW_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void get_normMeanStd(float trMean, float trStd, float normMean, float normStd);
void amoeba(float **p, float y[], int ndim, float ftol, float (*funk)(float []), int *nfunk) ;
float amotry(float **p, float *y, float *psum, int ndim, float (*funk)(float *), int ihi, float fac) ;
float mse( float *x )  ;
float logit( float x, float xMin, float xMax) ;
float inv_logit( float x, float xMin, float xMax ) ;
void gen_nrand(float mean, float std, float *rvalues);
void get_meanstd(float values[], float mean, float std);

#endif
