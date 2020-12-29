#pragma once

typedef struct {
    double x;
    double y;
    double z;
} vec3d;

#ifdef USE_LAPACK
void zheev(char*, char*, int*, double complex*, int*, double*, double complex*, int*, double*, int*);
#endif

double dot3d(vec3d*, vec3d*);
void cross3d(vec3d*, vec3d*, vec3d*);
void scal3d(vec3d*, double);


// void vdot(int, int, int, double complex*, double complex*, double complex*);



