#ifndef PLAHE_H
#define PLAHE_H
#include "vec.h"
#include "rt.h"

int intersects_plane(RAY_T ray, PLANE_T plane, double *t, VP_T *intersection_point, VP_T *normal);

#endif // plane.h
