#include<stdio.h>
#include<stdlib.h>
#include<complex.h>
#include<math.h>

#define PI  3.141592653589793

typedef struct {
    double x;
    double y;
    double z;
} vec3d;

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


int main(int argc, char** argv) {
    vec3d a1, a2, a3;
    double ecut;
    
    scanf("%lf", &a1.x);
    scanf("%lf", &a1.y);
    scanf("%lf", &a1.z);
    scanf("%lf", &a2.x);
    scanf("%lf", &a2.y);
    scanf("%lf", &a2.z);
    scanf("%lf", &a3.x);
    scanf("%lf", &a3.y);
    scanf("%lf", &a3.z);
    scanf("%lf", &ecut);
    
    printf("# a1.x = %e\n", a1.x);
    printf("# a1.y = %e\n", a1.y);
    printf("# a1.z = %e\n", a1.z);
    printf("# a2.x = %e\n", a2.x);
    printf("# a2.y = %e\n", a2.y);
    printf("# a2.z = %e\n", a2.z);
    printf("# a3.x = %e\n", a3.x);
    printf("# a3.y = %e\n", a3.y);
    printf("# a3.z = %e\n", a3.z);
    printf("# ecut = %e\n", ecut);

    vec3d b1, b2, b3;
    cross3d(&a1, &a2, &b3);
    cross3d(&a2, &a3, &b1);
    cross3d(&a3, &a1, &b2);
    double vcell = dot3d(&a1, &b1);
    scal3d(&b1, 2.0 * PI / vcell);
    scal3d(&b2, 2.0 * PI / vcell);
    scal3d(&b3, 2.0 * PI / vcell);

    printf("################\n");
    printf("# vcell = %e\n", vcell);
    printf("# b1.x = %e\n", b1.x);
    printf("# b1.y = %e\n", b1.y);
    printf("# b1.z = %e\n", b1.z);
    printf("# b2.x = %e\n", b2.x);
    printf("# b2.y = %e\n", b2.y);
    printf("# b2.z = %e\n", b2.z);
    printf("# b3.x = %e\n", b3.x);
    printf("# b3.y = %e\n", b3.y);
    printf("# b3.z = %e\n", b3.z);

    int mb1, mb2, mb3, ngtmp;

    mb1 = sqrt(2.0 * ecut * dot3d(&a1, &a1)) / (2.0 * PI);
    mb2 = sqrt(2.0 * ecut * dot3d(&a2, &a2)) / (2.0 * PI);
    mb3 = sqrt(2.0 * ecut * dot3d(&a3, &a3)) / (2.0 * PI);
    printf("# mb1 = %d\n", mb1);
    printf("# mb2 = %d\n", mb2);
    printf("# mb3 = %d\n", mb3);
    ngtmp = (2 * mb1 + 1) * (2 * mb2 + 1) * (2 * mb3 + 1);
    printf("# ngtmp = %d\n", ngtmp);

    int ib1, ib2, ib3, ig, ng;
    vec3d *gtbl;
    vec3d g;

    gtbl = (vec3d*) malloc(sizeof(vec3d) * ngtmp);

    ig = 0;
    for (int ib1 = -mb1; ib1 <= mb1; ib1++) {
        for (int ib2 = -mb2; ib2 <= mb2; ib2++) {
            for (int ib3 = -mb3; ib3 <= mb3; ib3++) {
                g.x = ib1 * b1.x + ib2 * b2.x + ib3 * b3.x;
                g.y = ib1 * b1.y + ib2 * b2.y + ib3 * b3.y;
                g.z = ib1 * b1.z + ib2 * b2.z + ib3 * b3.z;
                if (0.5 * dot3d(&g, &g) <= ecut) {
                    gtbl[ig] = g;
                    ig++;
                }
            }
        }
    }
    ng = ig;
    realloc(gtbl, sizeof(vec3d) * ng);

    printf("# ng = %d\n", ng);


    // Test
    double complex *h;

    h = (double complex*) malloc(sizeof(double complex) * ng * ng);

    #define H(I,J) h[i+ng*j]
    for (int j=0; j<ng; j++) {
        for (int i=0; i<ng; i++) {
            if (i == j) {
                H(i, i) = 0.5 * dot3d(gtbl[i], gtbl[i]);
            } else {
                H(i, j) = 0.0;
            }
        }
    }


    #undef H


    return 0;
}
