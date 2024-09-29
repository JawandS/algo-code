#include <stdio.h>
#include <math.h>

// typedef
// vector point
    typedef struct {
        double x;
        double y;
        double z;
    } VP_T;
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
    // sphere color
    typedef struct {
        double r;
        double g;
        double b;
    } RGB_T;

// declare intersects function
int intersects_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *intersection_point, VP_T *normal);

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
    double pos_t = (-b + sqrt(b*b - 4*a*c)) / (2*a);
    double neg_t = (-b - sqrt(b*b - 4*a*c)) / (2*a);
    if (pos_t <= 0 || neg_t <= 0) { // invalid sphere pos
        return 0; 
    }
    // select smaller 
    // printf("final disc %lf %lf %lf %lf\n", discriminant, ray.dir.x, ray.dir.y, ray.dir.z);
    *t = neg_t;
    if (pos_t < neg_t)
        *t = pos_t;
    return 1;
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
            if (intersects_sphere(curr_ray, sphere, &t, &intersection_point, &normal))
                printf("%d %d %d ", 255, 0, 0);
            else 
                printf("%d %d %d ", 0, 0, 0);
        }
    }
    printf("\n");

    return 0;
}
