#include "plane.h"
#include "vec.h"
#include "rt.h"

int intersects_plane(RAY_T ray, OBJ_T *obj, double *t, VP_T *intersection_point, VP_T *normal) {
    // assumes that the object is a plane
    PLANE_T plane = obj->plane;
    *normal = plane.normal;
    double dp = dot(*normal, ray.dir);
    if (dp == 0) { // no intersection
        return 0;
    } else {
        *t = -(dot(*normal, ray.origin) + plane.D) / dp;
        if (*t < 0) { // no intersection
            return 0;
        } else { // intersection point
            intersection_point->x = ray.origin.x + *t * ray.dir.x;
            intersection_point->y = ray.origin.y + *t * ray.dir.y;
            intersection_point->z = ray.origin.z + *t * ray.dir.z;
            return 1;
        }
    }
}
