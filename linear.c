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
/*
void vdot(int n, int m, int l, double complex* a, double complex* b, double complex* c) {

}
*/