#pragma once

#include <complex.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double a1_x;
  double a1_y;
  double a1_z;
  double a2_x;
  double a2_y;
  double a2_z;
  double a3_x;
  double a3_y;
  double a3_z;
  double ecut;
  int nk1;
  int nk2;
  int nk3;
  int nb;
} Param;

#define PI 3.141592653589793

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

typedef struct {
  char name[256];

  double zatom;
  double zion;
  double pspd;
  int pspcod;
  int pspxc;
  int lmax;
  int lloc;
  int mmax;
  int r2well;
  double rchrg;
  double fchrg;
  double qchrg;
  int nproj[5];
  int extension_switch[2];

  int nproj_total;

  int *l;
  double *ekb;
  double *r;
  double *f;
} Psp8_file;

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
