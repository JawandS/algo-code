#include <stdio.h>

int main() {
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

    // set sphere
    SPHERE_T sphere = {
        .center = {
            .x = 0,
            .y = 0,
            .z = 10
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
            
            // write pixel 
            printf("%d %d %d ", 255, 0, 0);
        }
    }
    printf("\n");

    return 0;
}
