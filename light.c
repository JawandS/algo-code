#include <stdio.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"

/*update to closest interesection point/normal */
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

    // diffuse light
    VP_T light_vector = {
        .x = scene->light.loc.x - intersection_point.x,
        .y = scene->light.loc.y - intersection_point.y,
        .z = scene->light.loc.z - intersection_point.z
    };
    normalize(&light_vector);

    double dp = dot(light_vector, normal);
    if (dp > 0) {
        color.r += dp * obj_color.r;
        color.g += dp * obj_color.g;
        color.b += dp * obj_color.b;

        // specular light (only if the first dot product is positive)
        VP_T r_vector;
        r_vector.x = light_vector.x - normal.x * 2 * dp;
        r_vector.y = light_vector.y - normal.y * 2 * dp;
        r_vector.z = light_vector.z - normal.z * 2 * dp;
        normalize(&r_vector);
        double dp2 = dot(r_vector, ray.dir);
        if (dp2 > 0) {
            color.r += pow(dp2, 200);
            color.g += pow(dp2, 200);
            color.b += pow(dp2, 200);
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

/* shadow test, called from illuminate (after ambient before diffuse)
- if the light ray intersects an object, the point is in shadow
- arguments: int_pt, scene 
RAY_T shadow_ray = 
shadow_ray.vector = vp_subtract(scene->light.loc, int_pt) // then normalize
for (objs) {
    if intersect: is in shadow, return 1;
}

*/