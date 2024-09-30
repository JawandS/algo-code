// vp.h
#ifndef VP_H
#define VP_H

// vector point
typedef struct {
    double x;
    double y;
    double z;
} VP_T;

// define functions
double len(VP_T a);
double normalize(VP_T *a);
double dot(VP_T a, VP_T b);

#endif // VP_H