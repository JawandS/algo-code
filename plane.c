#include "plane.h"
#include "vec.h"
#include "rt.h"

/*
intersects plane function: normal and distance from origin 
dp = normal * ray.dir 
if dp == 0:
    no intersection
else:
    t = -(dot(normal, ray.origin) + d) / dp
    if t < 0:
        no intersection
    else:
        intersection point = ray.origin + t * ray.dir
        return intersection point
*/

int intersects_plane(RAY_T ray, OBJ_T *obj, double *t, VP_T *intersection_point, VP_T *normal) {
    PLANE_T plane = obj->plane;
    double dp = dot(*normal, ray.dir);
    if (dp == 0) { // no intersection
        return 0;
    } else {
        *t = -(dot(*normal, ray.origin) + plane.D) / dp;
        if (*t < 0) { // no intersection
            return 0;
        } else {
            intersection_point->x = ray.origin.x + *t * ray.dir.x;
            intersection_point->y = ray.origin.y + *t * ray.dir.y;
            intersection_point->z = ray.origin.z + *t * ray.dir.z;
            return 1;
        }
    }
}
