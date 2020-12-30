#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>

#define PI  3.141592653589793

typedef struct {
    double x;
    double y;
    double z;
} Vec3d;

typedef struct {
    Vec3d a1;
    Vec3d a2;
    Vec3d a3;
    Vec3d b1;
    Vec3d b2;
    Vec3d b3;
    double vol;
} UnitCell;




/*
typedef struct {
    // General variables
    int n;
    int m;
    double* w;
    double complex* v;
    // Local variables
    
    // LAPACK variables
    double complex* work;
    int lwork;
    double* rwork;
} EigenSolver;
*/

