
#ifndef RT_H
#define RT_H
#include "vp.h"

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

// declare intersects function
int intersects_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *intersection_point, VP_T *normal);
RGB_T illuminate(RGB_T obj_color, VP_T intersection_point, VP_T normal, VP_T light_loc, RAY_T ray);

#endif