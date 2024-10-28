#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
        .x = light_loc.x - intersection_point.x,
        .y = light_loc.y - intersection_point.y,
        .z = light_loc.z - intersection_point.z
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

RGB_T trace(RAY_T ray, OBJ_T *obj, RGB_T sphere_color, VP_T light_loc) {
    // calculate the color of each ray if it's an intersection point
    double closest_t = 1000;
    double t;
    VP_T closest_intersection_point;
    VP_T intersection_point;
    VP_T closest_normal;
    VP_T normal;
    OBJ_T *closest_obj = NULL;
    // baseline color
    RGB_T obj_color = (RGB_T) {0.3, 0.3, 0.5};
    // print objects array
    for(int idx = 0; idx < 3; idx++) {
        if (obj[idx].intersects(ray, &obj[idx], &t, &intersection_point, &normal)) {
            if (t < closest_t) {
                closest_t = t;
                closest_intersection_point = intersection_point;
                closest_normal = normal;
                closest_obj = &obj[idx];
            }
        }
    }
    if (closest_t < 1000 && closest_obj != NULL)
        obj_color = illuminate(closest_obj, closest_intersection_point, closest_normal, light_loc, ray);    
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
        .z = -2.0 
    };
    // set eye position 
    VP_T eye_pos = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };
    // set sphere
    OBJ_T red_sphere = {
        .sphere = {
            .center = {
                .x = 0.5,
                .y = 0.8,
                .z = 4.0
            },
            .radius = 0.5
        },
        .type = 's',
        .color = {
            .r = 0.8,
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
    OBJ_T green_sphere = {
        .sphere = {
            .center = {
                .x = -0.5,
                .y = 0.15,
                .z = 4.2
            },
            .radius = 0.6
        },
        .type = 's',
        .color = {
            .r = 0.0,
            .g = 0.8,
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
    // main plane
    OBJ_T plane = {
        .plane = {
            .normal = {
                .x = 0,
                .y = 1,
                .z = 0
            },
            .D = 0.9
        },
        .type = 'p',
        .color = {
            .r = 0.0,
            .g = 0.0,
            .b = 0.0
        },
        .checker = 1,
        .color2 = {
            .r = 1.0,
            .g = 1.0,
            .b = 1.0
        },
        .intersects = &intersects_plane
    };
    // array of objects
    OBJ_T objects[] = {red_sphere, green_sphere, plane};
    // set image size 
    int Y_LEN = 1000;
    int X_LEN = 1000;

    // open the file 
    FILE *fimg = fopen("img.ppm", "w");
    // initialize image file header
    fprintf(fimg, "P6\n");
    fprintf(fimg, "1000 1000\n");
    fprintf(fimg, "255\n");

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
            RGB_T point_color = trace(curr_ray, objects, sphere_color, light_loc);
            fprintf(fimg, "%c%c%c", (unsigned char) (255 * point_color.r), (unsigned char) (255 * point_color.g), (unsigned char) (255 * point_color.b));
        }
    }
    printf("\n");

    return 0;
}

/* Create linked list of objects - init function 
// linked list of objects
scene->objs = NULL;
OBJ_T *node = NULL;
while (new lines in file) { // read from file
    node = (OBJ_T*) malloc(sizeof(OBJ_T));
    // initialize the node, ex: node->sphere.ctr.x = 0.0; 
    // have node->next point to list and set list to node 
    node->next = scene->objs;
    scene->objs = node; 
}
*/


/* file handling
FILE *file = fopen("file.txt", "r");
---
reading:
fscanf(fp, "%c, &obj->type);
--- read line by line
run loop until end of file
    read character (s or p)
    if s:
        set type to s 
        3 more lines to read: center, radius, color
        set checker to false
        set intersects function pointer
    if p:
        set type to p
        4 more lines: normal, d, color1, color2 
        assume checker is true 
        set intersects function pointer 
    if l:
        1 line: fscanf(fp, "%lf %lf %lf", &scene->light.loc.x, &scene->light.loc.y, &scene->light.loc.z);
*/

/*
Tasks:
1. Create a linked list of objects
2. Include scene structure (in main). Pass to init 
3. Updating to file writing 
*/
