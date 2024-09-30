// vp.h
#ifndef VP_H
#define VP_H

// vector point
typedef struct {
    double x;
    double y;
    double z;
} VP_T;
// ray
typedef struct {
    VP_T origin;
    VP_T dir;
} RAY_T;
// sphere
typedef struct {
    VP_T center;
    double radius;
} SPHERE_T; 
// sphere color
typedef struct {
    double r;
    double g;
    double b;
} RGB_T;

#endif // VP_H