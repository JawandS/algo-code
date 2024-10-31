#include <stdio.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"

static int shadow_test(VP_T intersection_point, OBJ_T *curr_obj, SCENE_T *scene) {
    // Create the shadow ray 
    RAY_T shadow_ray;
    shadow_ray.origin = intersection_point;
    shadow_ray.dir = diff(scene->light.loc, intersection_point);
    normalize(&shadow_ray.dir);

    // Check for intersection with any object
    OBJ_T *curr;
    double t;
    VP_T normal;
    VP_T tmp_int_pt;
    for (curr = scene->objs; curr != NULL; curr = curr->next) {
        if (curr == curr_obj) 
            continue; // don't check the same object
        if (curr->intersects(shadow_ray, curr, &t, &tmp_int_pt, &normal)) {
            return 1; // in shadow
        }
    }
    return 0; // Not in shadow
}

// get the color of an intersection point
RGB_T illuminate(OBJ_T *obj, VP_T intersection_point, VP_T normal, SCENE_T *scene, RAY_T ray) {
    // decide which object color to use 
    RGB_T obj_color = obj->color;
    if (obj->checker && !(((int) floor(intersection_point.x) + 
                         (int) floor(intersection_point.y) + 
                         (int) floor(intersection_point.z)) & 1)) {
        obj_color = obj->color2;
    }

    // calculate the color of each intersection point 
    RGB_T color;

    // ambient light
    color.r = 0.1 * obj_color.r;
    color.g = 0.1 * obj_color.g;
    color.b = 0.1 * obj_color.b;

    // check for shadow
    if (!shadow_test(intersection_point, obj, scene)) {
        // get the attentuation - TODO FIX THIS 
        double dl = length(diff(scene->light.loc, intersection_point));
        double atten_b = (0.002 * dl) * (0.002 * dl) + 0.02 * dl + 0.2;
        double atten = 1 / atten_b;

        // diffuse light
        VP_T light_vector = diff(scene->light.loc, intersection_point);
        normalize(&light_vector);

        double dp = dot(light_vector, normal);
        if (dp > 0) {
            color.r += dp * obj_color.r * atten;
            color.g += dp * obj_color.g * atten;
            color.b += dp * obj_color.b * atten;

            // specular light (only if the first dot product is positive)
            VP_T r_vector;
            r_vector.x = light_vector.x - normal.x * 2 * dp;
            r_vector.y = light_vector.y - normal.y * 2 * dp;
            r_vector.z = light_vector.z - normal.z * 2 * dp;
            normalize(&r_vector);
            double dp2 = dot(r_vector, ray.dir);
            if (dp2 > 0) {
                color.r += pow(dp2, 200) * atten;
                color.g += pow(dp2, 200) * atten;
                color.b += pow(dp2, 200) * atten;
            }
        }
    }

    // set colors to max value
    if (color.r > 1.0)
        color.r = 1.0;
    if (color.g > 1.0)
        color.g = 1.0;
    if (color.b > 1.0)
        color.b = 1.0;

    return color;
}