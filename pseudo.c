#include "common.h"
#include "misc.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))


double strtod_fortran(char *s) {
    for (int i=0; i<64 && s[i] != '\0'; i++) {
        if (s[i] == 'd' || s[i] == 'D')
            s[i] = 'e';
    }
    return strtod(s, NULL);
}


void read_psp8(char* file_psp8) {
    FILE *fp;
    
    fp = fopen(file_psp8, "r");

    char tmp[64];

    #define NEXT_LINE() fscanf(fp, "%*[^\n]%*c")
    #define READ_INT(I) fscanf(fp, "%64s", tmp), (I) = strtol(tmp, NULL, 10)
    #define READ_REAL(R) fscanf(fp, "%64s", tmp), (R) = strtod_fortran(tmp)

    double zatom, zion, pspd, rchrg, fchrg, qchrg;
    int pspcod, pspxc, lmax, lloc, mmax, r2well, nproj[5], extension_switch[2];

    int idummy;

    // line 1: skip
    NEXT_LINE();

    // line 2: zatom, zion, pspd
    READ_REAL(zatom);
    READ_REAL(zion);
    READ_REAL(pspd);
    NEXT_LINE();

    // line 3: pspcod, pspxc, lmax, lloc, mmax, r2well
    READ_INT(pspcod);
    READ_INT(pspxc);
    READ_INT(lmax);
    READ_INT(lloc);
    READ_INT(mmax);
    READ_INT(r2well);
    NEXT_LINE();

    // line 4: rchrg, fchrg, qchrg
    READ_REAL(rchrg);
    READ_REAL(fchrg);
    READ_REAL(qchrg);
    NEXT_LINE();

    // line 5: nproj
    READ_INT(nproj[0]);
    READ_INT(nproj[1]);
    READ_INT(nproj[2]);
    READ_INT(nproj[3]);
    READ_INT(nproj[4]);
    NEXT_LINE();

    // line 6: extension_switch
    READ_INT(extension_switch[0]);
    READ_INT(extension_switch[1]);
    NEXT_LINE();

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
        READ_INT(idummy);
        for (int iproj = 0; iproj < nproj[l]; iproj++)
            READ_REAL(EKB(l, iproj));
        NEXT_LINE();

        for (int irad = 0; irad < mmax; irad++) {
            READ_INT(idummy);
            READ_REAL(rad[irad]);
            for (int iproj = 0; iproj < nproj[l]; iproj++) {
                READ_REAL(FKB(l, irad, iproj));
            }
            NEXT_LINE();
        }
    }

    return;
}



int main() {
    read_psp8("H.psp8");
    return 0;
}


