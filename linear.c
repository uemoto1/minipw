#include "common.h"
#include "linear.h"


double dot3d(Vec3d* a, Vec3d* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void cross3d(Vec3d* a, Vec3d* b, Vec3d* c) {
    c->x = a->y * b->z - a->z * b->y;
    c->y = a->z * b->x - a->x * b->z;
    c->z = a->x * b->y - a->y * b->x;
}

void scal3d(Vec3d* a, double f) {
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
        #define A(I,J) a[I+kab*(J)]
        #define B(I,J) b[I+kab*(J)]
        #define C(I,J) c[I+na*(J)]
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
/*
void init_eigensolver(int n, EigenSolver* es) {
    es->n = n;
    es->n = m;
    es->w = (double*) malloc(sizeof(double) * m);
    es->v = (double complex*) malloc(sizeof(double complex) * n * m);
    es->lwork = 2 * n - 1;
    es->work = (double complex*) malloc(sizeof(double complex) * lwork);
    es->rwork = (double*) malloc(sizeof(double) * (3 * n - 2));
}

void destroy_eigensolver(EigenSolver* es) {
    free(es->w);
    free(es->v);
    free(es->work);
    free(es->rwork);
}
*/

void operate_h(int n, int m, double complex* x, double complex* hx) {
    #define X(I,J) x[I+n*(J)]
    #define HX(I,J) hx[I+n*(J)]
    for (int j=0; j<m; j++)
        for (int i=0; i<n; i++)
            HX(i, j) = (i + 1.0) * X(i, j);
    #undef X
    #undef HX
}

void lobpcg(int n, int m, double* e, double complex* v) {
    double complex hv[n*m];
    double complex r[n*m], hr[n*m];
    double complex p[n*m], hp[n*m];
    double complex u[n*m*3], hu[n*m*3];
    double complex a[9*m*m], b[9*m*m];
    double w[3*m], rr[m];
    int nsub = 2 * m;

    int itype = 1;
    int lwork = 2 * (3 * m) - 1;
    double complex work[lwork];
    double rwork[3 * (3 * m) - 2];
    int info;

    #define V(I,J) v[I+n*(J)]
    #define HV(I,J) hv[I+n*(J)]
    #define R(I,J) r[I+n*(J)]
    #define HR(I,J) hr[I+n*(J)]
    #define P(I,J) p[I+n*(J)]
    #define HP(I,J) hp[I+n*(J)]
    #define U(I,J) u[I+n*(J)]
    #define HU(I,J) hu[I+n*(J)]

    // |Hv> = h|v>;
    operate_h(n, m, v, hv);
    // e = <v|hv>
    for(int j=0; j<m; j++) {
        e[j] = 0.0;
        for(int i=0; i<n; i++) {
            e[j] += creal(conj(V(i, j)) * HV(i, j));
        }
    }

    for(int j=0; j<m; j++) {
        printf("e[%d] = %e\n", j, e[j]);
    }

    for(int iter=0; iter<10; iter++) {
        // |r> = |Hv> - e|v>
        for(int j=0; j<m; j++) {
            for(int i=0; i<n; i++) {
                R(i, j) = HV(i, j) - e[j] * V(i, j);
            }
        }

        // rr = <r|r>
        for(int j=0; j<m; j++) {
            rr[j] = 0.0;
            for(int i=0; i<n; i++)
                rr[j] += creal(conj(R(i, j)) * R(i, j));
        }

        double rr_min = rr[0];
        for (int j = 1; j < m; j++)
            if (rr[j] < rr_min)
                rr_min = rr[j];
       
        printf("# LOBPCG: iter=%d, rr_min = %e\n", iter, rr_min);

        if (rr_min < 1e-6) return;

        operate_h(n, m, r, hr);
        // Precondition R->R Not implemented;
        
        // Construct |u>, |Hu>
        for(int j=0; j<m; j++) {
            for(int i=0; i<n; i++) {
                U(i, j) = V(i, j);
                HU(i, j) = HV(i, j);
                U(i, j+m) = R(i, j);
                HU(i, j+m) = HR(i, j);
            }
        }

        if (nsub == 3 * m) {
            for(int j=0; j<m; j++) {
                for(int i=0; i<n; i++) {
                    U(i, j+2*m) = P(i, j);
                    HU(i, j+2*m) = HP(i, j);
                }
            }
        }
        
        #define A(I,J) a[I+nsub*(J)]
        #define B(I,J) b[I+nsub*(J)]
        // #define Q(I,J) I+nsub*(J)
        for (int j=0; j<nsub; j++) {
            for (int i=0; i<nsub; i++) {
                A(i, j) = 0.0;
                B(i, j) = 0.0;
                for (int k=0; k<n; k++) {
                    A(i, j) += conj(U(k, i)) * HU(k, j);
                    B(i, j) += conj(U(k, i)) * U(k, j);
                }
            }
        }

        // Solve A|u> = wB|u>
        zhegv(&itype, "V", "L", &nsub, a, &nsub, b, &nsub, w, work, &lwork, rwork, &info);

        for (int j=0; j<m; j++) {
            for (int i=0; i<n; i++) {
                V(i, j) = 0.0;
                HV(i, j) = 0.0;
                for (int k=0; k<nsub; k++) {
                    V(i, j) += U(i, k) * A(k, j);
                    HV(i, j) += HU(i, k) * A(k, j);
                }
                P(i, j) = 0.0;
                HP(i, j) = 0.0;
                for (int k=m; k<nsub; k++) {
                    P(i, j) += U(i, k) * A(k, j);
                    HP(i, j) += HU(i, k) * A(k, j);
                }
            }
            e[j] = w[j];
        }
        #undef A
        #undef B

        nsub = 3 * m;
    }

    #undef V
    #undef HV
    #undef R
    #undef HR
    #undef P
    #undef HP
    #undef U
    #undef HU

}
