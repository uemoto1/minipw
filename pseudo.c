#include "common.h"
#include "misc.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))




void read_psp8(char* file_psp8) {
    FILE *fp;
    
    fp = fopen(file_psp8, "r");

    double zatom, zion, pspd, rchrg, fchrg, qchrg;
    int pspcod, pspxc, lmax, lloc, mmax, r2well, nproj[5], extension_switch[2];

    int idummy;

    // line 1: skip
    next_line(fp);

    // line 2: zatom, zion, pspd
    zatom = read_real(fp);
    zion = read_real(fp);
    pspd = read_real(fp);
    next_line(fp);

    // line 3: pspcod, pspxc, lmax, lloc, mmax, r2well
    pspcod = read_int(fp);
    pspxc = read_int(fp);
    lmax = read_int(fp);
    lloc = read_int(fp);
    mmax = read_int(fp);
    r2well = read_int(fp);
    next_line(fp);

    // line 4: rchrg, fchrg, qchrg
    rchrg = read_real(fp);
    fchrg = read_real(fp);
    qchrg = read_real(fp);
    next_line(fp);

    // line 5: nproj
    nproj[0] = read_int(fp);
    nproj[1] = read_int(fp);
    nproj[2] = read_int(fp);
    nproj[3] = read_int(fp);
    nproj[4] = read_int(fp);
    next_line(fp);

    // line 6: extension_switch
    extension_switch[0] = read_int(fp);
    extension_switch[1] = read_int(fp);
    next_line(fp);

    printf("zatom=%f\n", zatom);
    printf("zion=%f\n", zion);
    printf("pspd=%f\n", pspd);
    printf("rchrg=%f\n", rchrg);
    printf("fchrg=%f\n", fchrg);
    printf("qchrg=%f\n", qchrg);

    printf("pspcod=%d\n", pspcod);
    printf("pspxc=%d\n", pspxc);
    printf("lmax=%d\n", lmax);
    printf("lloc=%d\n", lloc);
    printf("mmax=%d\n", mmax);
    printf("r2well=%d\n", r2well);
    printf("nproj=%d\n", nproj[0]);
    printf("extension_switch=%d\n", extension_switch[0]);

    int nl = lmax + 1;
    int mproj;
    mproj = MAX(nproj[0], nproj[1]);
    mproj = MAX(mproj, nproj[2]);
    mproj = MAX(mproj, nproj[3]);
    mproj = MAX(mproj, nproj[4]);

    double *rad = (double *) malloc(sizeof(double) * mmax);
    double *ekb = (double *) malloc(sizeof(double) * nl * mproj);
    double *fkb = (double *) malloc(sizeof(double) * nl * mproj * mmax);

    #define EKB(L,IPROJ) ekb[(IPROJ)+mproj*(L)]
    #define FKB(L,IRAD,IPROJ) fkb[(IPROJ)+mproj*(IRAD)+mproj*mmax*(L)]

    for (int l = 0; l < nl; l++) {
        idummy = read_int(fp);
        for (int iproj = 0; iproj < nproj[l]; iproj++)
            EKB(l, iproj = read_real(fp));
        next_line(fp);

        for (int irad = 0; irad < mmax; irad++) {
            idummy = read_int(fp);
            rad[irad] = read_real(fp);
            for (int iproj = 0; iproj < nproj[l]; iproj++) {
                FKB(l, irad, iproj = read_real(fp));
            }
            next_line(fp);
        }
    }

    return;
}


