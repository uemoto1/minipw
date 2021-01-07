#include "common.h"
#include "linear.h"
#include "inputparam.h"
#include "misc.h"

void calc_unitcell(Param *p, UnitCell *c) {
  c->a1.x = p->a1_x;
  c->a1.y = p->a1_y;
  c->a1.z = p->a1_z;
  c->a2.x = p->a2_x;
  c->a2.y = p->a2_y;
  c->a2.z = p->a2_z;
  c->a3.x = p->a3_x;
  c->a3.y = p->a3_y;
  c->a3.z = p->a3_z;
  cross3d(&c->a1, &c->a2, &c->b3);
  cross3d(&c->a2, &c->a3, &c->b1);
  cross3d(&c->a3, &c->a1, &c->b2);
  c->vol = dot3d(&c->a1, &c->b1);
  scal3d(&c->b1, 2.0 * PI / c->vol);
  scal3d(&c->b2, 2.0 * PI / c->vol);
  scal3d(&c->b3, 2.0 * PI / c->vol);
}

Vec3d *generate_gtbl(UnitCell *c, double ecut, int *ng, int *mb1, int *mb2,
                     int *mb3) {
  int ngtmp, mb1tmp, mb2tmp, mb3tmp;
  Vec3d *gtbl;
  Vec3d g;
  // Estimate number of G points.
  mb1tmp = sqrt(2.0 * ecut * dot3d(&c->a1, &c->a1)) / (2.0 * PI);
  mb2tmp = sqrt(2.0 * ecut * dot3d(&c->a2, &c->a2)) / (2.0 * PI);
  mb3tmp = sqrt(2.0 * ecut * dot3d(&c->a3, &c->a3)) / (2.0 * PI);
  ngtmp = (2 * mb1tmp + 1) * (2 * mb2tmp + 1) * (2 * mb3tmp + 1);
  gtbl = (Vec3d *)malloc(sizeof(Vec3d) * ngtmp);

  int ig = 0;
  *mb1 = 0;
  *mb2 = 0;
  *mb3 = 0;
  for (int ib1 = -mb1tmp; ib1 <= mb1tmp; ib1++) {
    for (int ib2 = -mb2tmp; ib2 <= mb2tmp; ib2++) {
      for (int ib3 = -mb3tmp; ib3 <= mb3tmp; ib3++) {
        g.x = ib1 * c->b1.x + ib2 * c->b2.x + ib3 * c->b3.x;
        g.y = ib1 * c->b1.y + ib2 * c->b2.y + ib3 * c->b3.y;
        g.z = ib1 * c->b1.z + ib2 * c->b2.z + ib3 * c->b3.z;
        if (0.5 * dot3d(&g, &g) <= ecut) {
          gtbl[ig] = g;
          if (*mb1 < ib1)
            *mb1 = ib1;
          if (*mb2 < ib2)
            *mb2 = ib2;
          if (*mb3 < ib3)
            *mb3 = ib3;
          ig++;
        }
      }
    }
  }
  *ng = ig;
  gtbl = realloc(gtbl, sizeof(Vec3d) * (*ng));
  return gtbl;
}

Vec3d *generate_ktbl(UnitCell *c, int nk1, int nk2, int nk3, int *nk) {
  double t1, t2, t3;
  Vec3d *ktbl;

  ktbl = (Vec3d *)malloc(sizeof(Vec3d) * nk1 * nk2 * nk3);

  int ik = 0;
  for (int ik1 = 0; ik1 < nk1; ik1++) {
    for (int ik2 = 0; ik2 < nk2; ik2++) {
      for (int ik3 = 0; ik3 < nk3; ik3++) {
        t1 = ((double)ik1 + 0.5) / nk1 - 0.5;
        t2 = ((double)ik2 + 0.5) / nk2 - 0.5;
        t3 = ((double)ik3 + 0.5) / nk3 - 0.5;
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

int main(int argc, char **argv) {
  int nproj = 32;

  printf("#### Read input parameters\n");
  Param param;
  read_inputparam(stdin, &param);

  printf("# param.a1_x = %e\n", param.a1_x);
  printf("# param.a1_y = %e\n", param.a1_y);
  printf("# param.a1_z = %e\n", param.a1_z);
  printf("# param.a2_x = %e\n", param.a2_x);
  printf("# param.a2_y = %e\n", param.a2_y);
  printf("# param.a2_z = %e\n", param.a2_z);
  printf("# param.a3_x = %e\n", param.a3_x);
  printf("# param.a3_y = %e\n", param.a3_y);
  printf("# param.a3_z = %e\n", param.a3_z);
  printf("# param.ecut = %e\n", param.ecut);
  printf("# param.nk1 = %d\n", param.nk1);
  printf("# param.nk2 = %d\n", param.nk2);
  printf("# param.nk3 = %d\n", param.nk3);
  printf("# param.nb = %d\n", param.nb);

  printf("#### Unit cell information\n");
  UnitCell cell;
  calc_unitcell(&param, &cell);

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
  gtbl = generate_gtbl(&cell, param.ecut, &ng, &mb1, &mb2, &mb3);

  printf("# ng = %d\n", ng);
  printf("# mb1 = %d\n", mb1);
  printf("# mb2 = %d\n", mb2);
  printf("# mb3 = %d\n", mb3);

  printf("#### k table generation\n");
  int nk;
  Vec3d *ktbl;
  ktbl = generate_ktbl(&cell, param.nk1, param.nk2, param.nk3, &nk);
  printf("# nk = %d\n", nk);

  printf("#### Allocate wavefunction\n");
  double complex *wf;
  double complex *proj;
  double *tkin;
  double *eig;
  int nb = param.nb;

  wf = (double complex *)malloc(sizeof(double complex) * nk * nb * ng);
  eig = (double *)malloc(sizeof(double) * nk * nb);
  tkin = (double *)malloc(sizeof(double) * nk * ng);
  proj = (double complex *)malloc(sizeof(double complex) * nk * nproj * ng);
  printf("# allocated wf: %lu [bytes]\n",
         sizeof(double complex) * nk * nb * ng);
  printf("# allocated eig: %lu [bytes]\n", sizeof(double) * nk * nb);
  printf("# allocated tkin: %lu [bytes]\n", sizeof(double) * nk * ng);
  printf("# allocated proj: %lu [bytes]\n",
         sizeof(double complex) * nk * nproj * ng);

#define WF(IK, IB, IG) wf[(IG) + ng * (IB) + ng * nb * (IK)]
#define EIG(IK, IB) eig[(IB) + nb * (IK)]
#define TKIN(IK, IG) tkin[(IG) + ng * (IK)]

  printf("#### Allocate realspace variables\n");
  double complex *wf_r;
  double *rho_r;
  double *vloc_r;
  int nr1, nr2, nr3;
  nr1 = 2 * mb1;
  nr2 = 2 * mb2;
  nr3 = 2 * mb3;

  rho_r = (double *)malloc(sizeof(double) * nr1 * nr2 * nr3);
  vloc_r = (double *)malloc(sizeof(double) * nr1 * nr2 * nr3);
  wf_r = (double complex *)malloc(sizeof(double complex) * nr1 * nr2 * nr3);

  printf("# allocated wf_r: %lu [bytes]\n",
         sizeof(double complex) * nr1 * nr2 * nr3);
  printf("# allocated vloc_r: %lu [bytes]\n", sizeof(double) * nr1 * nr2 * nr3);
  printf("# allocated rho_r: %lu [bytes]\n", sizeof(double) * nr1 * nr2 * nr3);

  Vec3d q;

  printf("#### Kinetic energy table generation\n");
  for (int ik = 0; ik < nk; ik++) {
    for (int ig = 0; ig < ng; ig++) {
      q.x = ktbl[ik].x + gtbl[ig].x;
      q.y = ktbl[ik].y + gtbl[ig].y;
      q.z = ktbl[ik].z + gtbl[ig].z;
      TKIN(ik, ig) = 0.5 * dot3d(&q, &q);
    }
  }

  printf("#### Initialize wavefunction\n");
  for (int ik = 0; ik < nk; ik++) {
    for (int ib = 0; ib < nb; ib++) {
      double r = 0.0;
      for (int ig = 0; ig < ng; ig++) {
        WF(ik, ib, ig) = 1.0 * rand_xorshift() + 1.0i * rand_xorshift();
        r += creal(conj(WF(ik, ib, ig)) * WF(ik, ib, ig));
      }
      r = 1.0 / sqrt(r);
      for (int ig = 0; ig < ng; ig++) {
        WF(ik, ib, ig) = r * WF(ik, ib, ig);
      }
    }
  }

  for (int iscf = 0; iscf < 10; iscf++) {
    printf("# iscf = %d\n", iscf);
    for (int ik = 0; ik < nk; ik++) {
      lobpcg(ng, nb, &EIG(ik, 0), &WF(ik, 0, 0), &TKIN(ik, 0));
    }
  }

  for (int j = 0; j < param.nb; j++) {
    printf("e[%d] = %e\n", j, eig[j]);
  }

#undef WF
#undef EIG
#undef TKIN

  return 0;
}
