#include "vec.h"
# include <math.h>

double length(VP_T a) {
    // get the length of a vector 
    return sqrt(a.x * a.x + 
                a.y * a.y + 
                a.z * a.z);
}

double normalize(VP_T *a) {
    // normalize a vector
    double len = length(*a);
    a->x /= len;
    a->y /= len;
    a->z /= len;
    return len;
}

double dot(VP_T a, VP_T b) {
    // get the dot product of two vectors
    return a.x * b.x + 
           a.y * b.y + 
           a.z * b.z;
}