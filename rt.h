// rt.h
#ifndef RT_H
#define RT_H
#include "vec.h"

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
// color
typedef struct {
    double r;
    double g;
    double b;
} RGB_T;
// plane 
typedef struct {
    VP_T normal;
    double D;
} PLANE_T;
// light
typedef struct light {
    VP_T loc; // light location
} LIGHT_T;
// object type
typedef struct OBJ {
    union {
        SPHERE_T sphere;
        PLANE_T plane;
    };
    char type; // s or p, indicates sphere or plane
    RGB_T color;
    int checker; // boolean, is a checkered plane
    RGB_T color2; // used if checkered 
    int (*intersects) (RAY_T ray, struct OBJ *obj, double *t, VP_T *intersection_point, VP_T *normal);
    struct OBJ *next; // pointer to the next object in the linked list 
} OBJ_T;
// scene type
typedef struct {
    OBJ_T *objs; // linked list of objects
    LIGHT_T light; // location of the light
    int start_x; // default -0.5, top left of the image 
    int start_y; // default 0.5
    double pixel_size; // default 1/1000
} SCENE_T;

// declare function signatures
typedef int (*intersects)(RAY_T, OBJ_T*, double*, VP_T*, VP_T*);
int intersects_sphere(RAY_T ray, OBJ_T *obj, double *t, VP_T *intersection_point, VP_T *normal);
RGB_T illuminate(OBJ_T *obj, VP_T intersection_point, VP_T normal, VP_T light_loc, RAY_T ray);
RGB_T trace(RAY_T ray, OBJ_T *obj, RGB_T sphere_color, VP_T light_loc);

#endif //rt.h