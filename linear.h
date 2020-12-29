#pragma once

#include<complex.h>

#ifdef USE_LAPACK
void zgemm(char*, char*, int*, int*, int*, double complex*, double complex*, int*, double complex*, int*, double complex*, double complex*, int*);	
void zhegv(int*, char*, char*, int*, double complex*, int*, double complex*, int*, double*, double complex*, int*, double*, int*);
#endif


typedef struct {
    double x;
    double y;
    double z;
} vec3d;

double dot3d(vec3d*, vec3d*);
void cross3d(vec3d*, vec3d*, vec3d*);
void scal3d(vec3d*, double);

typedef struct {
    int n;
} eigensolver;

void matmul_h(int, int, int, double complex*, double complex*, double complex*);







