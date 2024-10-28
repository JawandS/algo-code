// sphere.h
#ifndef SPHERE_H
#define SPHERE_H
#include "vp.h"
#include "rt.h"

int intersects_sphere(RAY_T ray, OBJ_T *obj, double *t, VP_T *intersection_point, VP_T *normal);

#endif //sphere.h