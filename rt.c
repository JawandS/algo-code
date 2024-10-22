#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "rt.h"
#include "plane.h"

int intersects_sphere(RAY_T ray, OBJ_T *obj, double *t, VP_T *intersection_point, VP_T *normal) {
    SPHERE_T sphere = obj->sphere;
    // check if a ray intersects the sphere
    double a = 1.0;
    double b = 2 * (ray.dir.x * -sphere.center.x +
                    ray.dir.y * -sphere.center.y +
                    ray.dir.z * -sphere.center.z);
    double c = sphere.center.x * sphere.center.x + 
               sphere.center.y * sphere.center.y +
               sphere.center.z * sphere.center.z -
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
    intersection_point->x = ray.dir.x * *t;
    intersection_point->y = ray.dir.y * *t;
    intersection_point->z = ray.dir.z * *t;

    // normal point
    normal->x = intersection_point->x - sphere.center.x;
    normal->y = intersection_point->y - sphere.center.y;
    normal->z = intersection_point->z - sphere.center.z;
    normalize(normal);

    // return true
    return 1;
}

/*update to closest interesection point/normal */
RGB_T illuminate(OBJ_T *obj, VP_T intersection_point, VP_T normal, VP_T light_loc, RAY_T ray) {
    // decide which object color to use 
    RGB_T obj_color = obj->color;
    if (obj->checker && ((int) floor(intersection_point.x) + 
                         (int) floor(intersection_point.y) + 
                         (int) floor(intersection_point.z)) & 1) {
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
        .x = light_loc.x - intersection_point.x,
        .y = light_loc.y - intersection_point.y,
        .z = light_loc.z - intersection_point.z
    };
    normalize(&light_vector);

    double dp = dot(light_vector, normal);
    if (dp > 0) {
        color.r += dp * obj_color.r;

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

/*
updating trace:
- set closest t to 1000 and find the lowest t, save t/interesection point/normal
- if t < closest t, update closest t/intersection point/normal
- have pointer to the object with closest intersection

*/

RGB_T trace(RAY_T ray, OBJ_T *obj, RGB_T sphere_color, VP_T light_loc) {
    // calculate the color of each ray if it's an intersection point
    double t;
    VP_T intersection_point;
    VP_T normal;
    // baseline color - black
    RGB_T obj_color = (RGB_T) {0.0, 0.0, 0.0};
    if (obj->intersects(ray, obj, &t, &intersection_point, &normal))
        obj_color = illuminate(obj, intersection_point, normal, light_loc, ray);
    return obj_color; 
}

// main method
int main() {
    // set sphere color 
    RGB_T sphere_color = {
        .r = 1.0,
        .g = 0.0,
        .b = 0.0
    };
    // set light location
    VP_T light_loc = {
        .x = 5.0,
        .y = 10.0, 
        .z = 0.0 
    };
    // set eye position 
    VP_T eye_pos = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };
    // set sphere
    OBJ_T sphere_one = {
        .sphere = {
            .center = {
                .x = 0,
                .y = 0,
                .z = 10
            },
            .radius = 2.0
        },
        .type = 's',
        .color = {
            .r = 1.0,
            .g = 0.0,
            .b = 0.0
        },
        .checker = 0,
        .color2 = {
            .r = 0.0,
            .g = 0.0,
            .b = 0.0
        },
        .intersects = &intersects_sphere
    }; 
    // set image size 
    int Y_LEN = 1000;
    int X_LEN = 1000;
    /*
    Create a rray of the objects in the scene, and iterate through them in trace 
    */

    // initialize image file header
    printf("P6\n");
    printf("1000 1000\n");
    printf("255\n");

    // go through image 
    for (int y = 0; y < Y_LEN; y++) {
        for (int x = 0; x < X_LEN; x++) {
            // set ray origin and direction
            RAY_T curr_ray = {
                .origin = eye_pos,
                .dir = {
                    .x = (-0.5 + (x / 1000.0)),
                    .y = -(-0.5 + (y / 1000.0)),
                    .z = 1
                }
            };
            normalize(&curr_ray.dir);
            // write pixel 
            RGB_T point_color = trace(curr_ray, &sphere_one, sphere_color, light_loc);
            printf("%c%c%c", (unsigned char) (255 * point_color.r), (unsigned char) (255 * point_color.g), (unsigned char) (255 * point_color.b));
        }
    }
    printf("\n");

    return 0;
}
