// light.h
#ifndef LIGHT_H
#define LIGHT_H
#include "vp.h"
#include "rt.h"

RGB_T illuminate(OBJ_T *obj, VP_T intersection_point, VP_T normal, SCENE_T *scene, RAY_T ray);

#endif // light.h