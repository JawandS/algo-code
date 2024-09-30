#include <stdio.h>
#include <math.h>
#include "vp.h"
#include "rt.h"

// implement intersection function
int intersects_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *intersection_point, VP_T *normal) {
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
    // select smaller 
    *t = neg_t;
    if (pos_t < neg_t)
        *t = pos_t;
    return 1;
}

//lighting - ray, int pt, obj color, normal, light loc 
RGB_T illuminate(RGB_T obj_color, VP_T intersection_point, VP_T normal, VP_T light_loc, RAY_T ray) {
    RGB_T color;
    // ambient light
    color.r = 0.1 * obj_color.r;

    /** diffuse - dot product gives us cos(theta)
     get light vector: light minus intersection 
     light loc - intersection point (three equations then normalize)
     dp = dot(light vector, normal vector): filter for negative 
     color.R += dp * obj_color.R
    */ 
    // diffuse light
    VP_T light_vector = {
        .x = light_loc.x - intersection_point.x,
        .y = light_loc.y - intersection_point.y,
        .z = light_loc.z - intersection_point.z
    };
    double light_vector_len = sqrt(light_vector.x * light_vector.x + light_vector.y * light_vector.y + light_vector.z * light_vector.z);
    light_vector.x /= light_vector_len;
    light_vector.y /= light_vector_len;
    light_vector.z /= light_vector_len;
    double dp = dot(light_vector, normal);
    if (dp > 0) {
        color.r += dp * obj_color.r;
    } else {
        return color;
    }

    /*
    Only if the dot product is positive, the light is in front of the object.
    R = L - N * 2 * dp
    noramlize R
    dp2 = dot(R, ray.dir) - ray is the original ray from the eye 
    if (dp2 > 0)
        color.r += pow(dp2, 200)
    */
    // specular light

    if (color.r > 1.0)
        color.r = 1.0;
    return color;
}


// main method
int main() {
    // set sphere
    SPHERE_T sphere = {
        .center = {
            .x = 0,
            .y = 0,
            .z = 20
        },
        .radius = 2.0
    }; 
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
    // set image size 
    int Y_LEN = 1000;
    int X_LEN = 1000;

    // initialize image file header
    printf("P3\n");
    printf("1000 1000\n");
    printf("255\n");

    // go through image 
    for (int y = 0; y < Y_LEN; y++) {
        for (int x = 0; x < X_LEN; x++) {
            // set ray origin and direction
            double length = sqrt((-0.5 + (x / 1000.0)) * (-0.5 + (x / 1000.0)) + (-(-0.5 + (y / 1000.0))) * (-(-0.5 + (y / 1000.0))) + 1);
            RAY_T curr_ray = {
                .origin = eye_pos,
                .dir = {
                    .x = (-0.5 + (x / 1000.0)) / length,
                    .y = -(-0.5 + (y / 1000.0)) / length,
                    .z = 1 / length
                }
            };
            // write pixel 
            double t;
            VP_T intersection_point;
            VP_T normal;
            if (intersects_sphere(curr_ray, sphere, &t, &intersection_point, &normal)) {
                RGB_T point_color = illuminate(sphere_color, intersection_point, normal, light_loc, curr_ray);

                printf("%d %d %d ", (int) (255 * point_color.r), 0, 0);
            } else {
                printf("%d %d %d ", 0, 0, 0);
            }
        }
    }
    printf("\n");

    return 0;
}

/*
* intersects sphere
* illuminate
* trace: returns a color type, if intersects return illuminate else return black if no intersects 
* main
    init structs
    int ppm
    for
        for 
            init ray
            call trace 
            print ppm pixel 
*/