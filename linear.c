#include "linear.h"


double dot3d(vec3d* a, vec3d* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void cross3d(vec3d* a, vec3d* b, vec3d* c) {
    c->x = a->y * b->z - a->z * b->y;
    c->y = a->z * b->x - a->x * b->z;
    c->z = a->x * b->y - a->y * b->x;
}

void scal3d(vec3d* a, double f) {
    a->x *= f;
    a->y *= f;
    a->z *= f;
}

void matmul_h(int na, int mb, int kab, double complex *a, double complex *b, double complex *c) {
    #ifdef USE_LAPACK
        double complex alpha = 1.0;
        double complex beta = 0.0;
        zgemm("c", "n", &na, &mb, &kab, &alpha, a, &kab, b, &kab, &beta, c, &mb);
    #else
        #define A(I,J) a[I+J*kab]
        #define B(I,J) b[I+J*kab]
        #define C(I,J) c[I+J*na]
        for (int i=0; i<na; i++) {
            for (int j=0; j<mb; j++) {
                C(i, j) = 0.0;
                for (int k=0; k<kab; k++)
                    C(i, j) += conj(A(k, i)) * B(k, j);
            }
        }
        #undef A
        #undef B
        #undef C
    #endif
}


