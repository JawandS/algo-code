#include <stdio.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "sphere.h"

int intersects_sphere(RAY_T ray, OBJ_T *obj, double *t, VP_T *intersection_point, VP_T *normal) {
    SPHERE_T sphere = obj->sphere;
    // get the vector from the ray origin to the sphere center
    VP_T oc = diff(ray.origin, sphere.center);
    // check if a ray intersects the sphere
    double a = 1.0;
    double b = 2 * (ray.dir.x * oc.x +
                    ray.dir.y * oc.y +
                    ray.dir.z * oc.z);
    double c = oc.x * oc.x + 
               oc.y * oc.y +
               oc.z * oc.z -
               sphere.radius * sphere.radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant <= 0) { // sphere invalid position
        return 0; // return false 
    }
    // quadratic formula 
    double sqrt_discriminant = sqrt(discriminant);
    double pos_t = (-b + sqrt_discriminant) / (2*a);
    double neg_t = (-b - sqrt_discriminant) / (2*a);
    if (pos_t <= 0 || neg_t <= 0) { // invalid sphere pos
        return 0; 
    }
    // select smaller t
    *t = neg_t;
    if (pos_t < neg_t)
        *t = pos_t;
    
    // intersection point
    intersection_point->x = ray.origin.x + ray.dir.x * *t;
    intersection_point->y = ray.origin.y + ray.dir.y * *t;
    intersection_point->z = ray.origin.z + ray.dir.z * *t;

    // normal point
    normal->x = intersection_point->x - sphere.center.x;
    normal->y = intersection_point->y - sphere.center.y;
    normal->z = intersection_point->z - sphere.center.z;
    normalize(normal);

    // return true
    return 1;
}