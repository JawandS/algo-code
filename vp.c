#include <math.h>
#include "vp.h"

double length(VP_T a) {
    // get the length of a vector 
    return sqrt(a.x * a.x + 
                a.y * a.y + 
                a.z * a.z);
}

void normalize(VP_T *a) {
    // normalize a vector
    double len = length(*a);
    a->x /= len;
    a->y /= len;
    a->z /= len;
}

double dot(VP_T a, VP_T b) {
    // get the dot product of two vectors
    return a.x * b.x + 
           a.y * b.y + 
           a.z * b.z;
}

VP_T diff(VP_T a, VP_T b) {
    // return the difference between two vectors
    VP_T new_vec = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
    return new_vec;
}