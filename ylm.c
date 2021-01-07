#include"common.h"

double calc_ylm(int l, int m, double x, double y, double z) {
    double ylm = 0.0;
    double r = sqrt(x*x + y*y + z*z);
    double pi = 3.14159265358979; 
    if (l == 0) {
        if (m == 0) {
            ylm = 1.0/(2.0*sqrt(pi));
        }
    } else if (l == 1) {
        if (m == -1) {
            ylm = sqrt(3.0)*y/(2.0*sqrt(pi)*r);
        } else if (m == 0) {
            ylm = sqrt(3.0)*z/(2.0*sqrt(pi)*r);
        } else if (m == 1) {
            ylm = sqrt(3.0)*x/(2.0*sqrt(pi)*r);
        }
    } else if (l == 2) {
        if (m == -2) {
            ylm = sqrt(15.0)*x*y/(2.0*sqrt(pi)*r*r);
        } else if (m == -1) {
            ylm = sqrt(15.0)*y*z/(2.0*sqrt(pi)*r*r);
        } else if (m == 0) {
            ylm = sqrt(5.0)*(-r*r+3.0*z*z)/(4.0*sqrt(pi)*r*r);
        } else if (m == 1) {
            ylm = sqrt(15.0)*x*z/(2.0*sqrt(pi)*r*r);
        } else if (m == 2) {
            ylm = sqrt(15.0)*(x*x-y*y)/(4.0*sqrt(pi)*r*r);
        }
    }
    return ylm;
}

