#include "vp.h"
# include <math.h>

double length(VP_T a) {
    return sqrt(a.x * a.x + 
                a.y * a.y + 
                a.z * a.z);
}

double normalize(VP_T *a) {
    double len = length(*a);
    a->x /= len;
    a->y /= len;
    a->z /= len;
    return len;
}

double dot(VP_T a, VP_T b) {
    return a.x * b.x + 
           a.y * b.y + 
           a.z * b.z;
}