#pragma once

#include<complex.h>

typedef struct {
    double x;
    double y;
    double z;
} vec3d;

#ifdef USE_LAPACK
void zheev(char*, char*, int*, double complex*, int*, double*, double complex*, int*, double*, int*);
void zgemm(char*, char*, int*, int*, int*, double complex*, double complex*, int*, double complex*, int*, double complex*, double complex*, int*);	
#endif

void matmul_h(int, int, int, double complex*, double complex*, double complex*);


double dot3d(vec3d*, vec3d*);
void cross3d(vec3d*, vec3d*, vec3d*);
void scal3d(vec3d*, double);





