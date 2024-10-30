// light.h
#ifndef LIGHT_H
#define LIGHT_H
#include "vp.h"
#include "rt.h"

static int shadow_test(VP_T intersection_point, OBJ_T *curr_obj, SCENE_T *scene);
RGB_T illuminate(OBJ_T *obj, VP_T intersection_point, VP_T normal, SCENE_T *scene, RAY_T ray);

#endif // light.h