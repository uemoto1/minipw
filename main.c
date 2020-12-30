#include"common.h"
#include"linear.h"

void calc_unitcell(Vec3d* a1, Vec3d* a2, Vec3d* a3, UnitCell* c) {
    c->a1 = *a1;
    c->a2 = *a2;
    c->a3 = *a3;
    cross3d(&c->a1, &c->a2, &c->b3);
    cross3d(&c->a2, &c->a3, &c->b1);
    cross3d(&c->a3, &c->a1, &c->b2);
    c->vol = dot3d(&c->a1, &c->b1);
    scal3d(&c->b1, 2.0 * PI / c->vol);
    scal3d(&c->b2, 2.0 * PI / c->vol);
    scal3d(&c->b3, 2.0 * PI / c->vol);
}

Vec3d* generate_gtbl(UnitCell* c, double ecut, int* ng, int* mb1, int* mb2, int* mb3) {
    int ngtmp, mb1tmp, mb2tmp, mb3tmp;
    Vec3d *gtbl;
    Vec3d g;
    // Estimate number of G points.
    mb1tmp = sqrt(2.0 * ecut * dot3d(&c->a1, &c->a1)) / (2.0 * PI);
    mb2tmp = sqrt(2.0 * ecut * dot3d(&c->a2, &c->a2)) / (2.0 * PI);
    mb3tmp = sqrt(2.0 * ecut * dot3d(&c->a3, &c->a3)) / (2.0 * PI);
    ngtmp = (2 * mb1tmp + 1) *  (2 * mb2tmp + 1) * (2 * mb3tmp + 1);
    gtbl = (Vec3d*) malloc(sizeof(Vec3d) * ngtmp);

    int ig = 0;
    *mb1 = 0; *mb2 = 0; *mb3 = 0;
    for(int ib1 = -mb1tmp; ib1 <= mb1tmp; ib1++) {
        for(int ib2 = -mb2tmp; ib2 <= mb2tmp; ib2++) {
            for(int ib3 = -mb3tmp; ib3 <= mb3tmp; ib3++) {
                g.x = ib1 * c->b1.x + ib2 * c->b2.x + ib3 * c->b3.x;
                g.y = ib1 * c->b1.y + ib2 * c->b2.y + ib3 * c->b3.y;
                g.z = ib1 * c->b1.z + ib2 * c->b2.z + ib3 * c->b3.z;
                if (0.5 * dot3d(&g, &g) <= ecut) {
                    gtbl[ig] = g;
                    if (*mb1 < ib1) *mb1 = ib1;
                    if (*mb2 < ib2) *mb2 = ib2;
                    if (*mb3 < ib3) *mb3 = ib3;
                    ig++;
                }
            }
        }
    }
    *ng = ig;
    //gtbl = realloc(gtbl, sizeof(Vec3d) * ng);
    return gtbl;
}

Vec3d* generate_ktbl(UnitCell* c, int nk1, int nk2, int nk3, int* nk) {
    double t1, t2, t3;
    Vec3d* ktbl;

    ktbl = (Vec3d*) malloc(sizeof(Vec3d) * nk1 * nk2 * nk3);

    int ik = 0;
    for (int ik1 = 0; ik1 < nk1; ik1++) {
        for (int ik2 = 0; ik2 < nk2; ik2++) {
            for (int ik3 = 0; ik3 < nk3; ik3++) {
                t1 = ((double) ik1 + 0.5) / nk1 - 0.5;
                t2 = ((double) ik2 + 0.5) / nk2 - 0.5;
                t3 = ((double) ik3 + 0.5) / nk3 - 0.5;
                ktbl[ik].x = t1 * c->b1.x + t2 * c->b2.x + t3 * c->b3.x;
                ktbl[ik].y = t1 * c->b1.y + t2 * c->b2.y + t3 * c->b3.y;
                ktbl[ik].z = t1 * c->b1.z + t2 * c->b2.z + t3 * c->b3.z;
                ik++;
            }
        }
    }
    *nk = ik;
    return ktbl;
}

int main(int argc, char** argv) {
    Vec3d a1, a2, a3;
    double ecut;
    int nk1, nk2, nk3;
    
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
    scanf("%d", &nk1);
    scanf("%d", &nk2);
    scanf("%d", &nk3);
 
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
    printf("# nk1 = %d\n", nk1);
    printf("# nk2 = %d\n", nk2);
    printf("# nk3 = %d\n", nk3);


    printf("#### Unit cell information\n");
    UnitCell cell;
    calc_unitcell(&a1, &a2, &a3, &cell);

    printf("# cell.vol = %e\n", cell.vol);
    printf("# cell.b1.x = %e\n", cell.b1.x);
    printf("# cell.b1.y = %e\n", cell.b1.y);
    printf("# cell.b1.z = %e\n", cell.b1.z);
    printf("# cell.b2.x = %e\n", cell.b2.x);
    printf("# cell.b2.y = %e\n", cell.b2.y);
    printf("# cell.b2.z = %e\n", cell.b2.z);
    printf("# cell.b3.x = %e\n", cell.b3.x);
    printf("# cell.b3.y = %e\n", cell.b3.y);
    printf("# cell.b3.z = %e\n", cell.b3.z);


    printf("#### G table generation\n");
    int mb1, mb2, mb3, ng;
    Vec3d *gtbl;
    gtbl = generate_gtbl(&cell, ecut, &ng, &mb1, &mb2, &mb3);

    printf("# ng = %d\n", ng);
    printf("# mb1 = %d\n", mb1);
    printf("# mb2 = %d\n", mb2);
    printf("# mb3 = %d\n", mb3);


    printf("#### k table generation\n");
    int nk;
    Vec3d *ktbl;
    ktbl = generate_ktbl(&cell, nk1, nk2, nk3, &nk);
    printf("# nk = %d\n", nk);



    printf("#### Kinetic energy table generation\n");
    double* tktbl;
    Vec3d q;
    tktbl = (double*) malloc(sizeof(double) * nk * ng);
    #define TKTBL(IK,IG) tktbl[IG+ng*IK]
    for (int ik = 0; ik < nk; ik++) {
        for (int ig = 0; ig < ng; ig++) {
            q.x = ktbl[ik].x + gtbl[ig].x;
            q.y = ktbl[ik].y + gtbl[ig].y;
            q.z = ktbl[ik].z + gtbl[ig].z;
            TKTBL(ik, ig) = 0.5 * dot3d(&q, &q);
        }
    }
    #undef TKTBL




    return 0;
}
