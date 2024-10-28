// vp.h
#ifndef VP_H
#define VP_H

// vector point
typedef struct {
    double x;
    double y;
    double z;
} VP_T;

// define functions signatures
double len(VP_T a);
void normalize(VP_T *a);
double dot(VP_T a, VP_T b);
VP_T diff(VP_T a, VP_T b);

#endif // vp.h