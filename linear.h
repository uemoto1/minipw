#pragma once

#include <complex.h>

void zgemm(char *, char *, int *, int *, int *, double complex *,
           double complex *, int *, double complex *, int *, double complex *,
           double complex *, int *);
void zhegv(int *, char *, char *, int *, double complex *, int *,
           double complex *, int *, double *, double complex *, int *, double *,
           int *);

double dot3d(Vec3d *, Vec3d *);
void cross3d(Vec3d *, Vec3d *, Vec3d *);
void scal3d(Vec3d *, double);

typedef struct {
  int n;
} eigensolver;

void matmul_h(int, int, int, double complex *, double complex *,
              double complex *);

void lobpcg(int, int, double *, double complex *, double *);
