#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vp.h"
#include "rt.h"
#include "plane.h"
#include "sphere.h"
#include "light.h"

RGB_T trace(RAY_T ray, SCENE_T *scene) {
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
    // iterate through objects in scene
    OBJ_T *curr;
    for(curr = scene->objs; curr != NULL; curr = curr->next) {
        if (curr->intersects(ray, curr, &t, &intersection_point, &normal)) {
            if (t < closest_t) {
                closest_t = t;
                closest_intersection_point = intersection_point;
                closest_normal = normal;
                closest_obj = curr;
            }
        }
    }
    // get object color
    if (closest_t < 1000 && closest_obj != NULL)
        obj_color = illuminate(closest_obj, closest_intersection_point, closest_normal, scene, ray);    
    return obj_color; 
}

// initialization function
void init(SCENE_T *scene) {
    // default values for scene 
    scene->objs = NULL;
    scene->start_x = -0.5;
    scene->start_y = 0.5;
    scene->pixel_size = 1/1000;
    // read file for object information
    FILE *file = fopen("scene.txt", "r");
    OBJ_T *node = NULL;
    char info_type; // what the information is 
    while (fscanf(file, "%c", &info_type) != EOF) { // iterate while file is not empty 
        if (info_type == 's') { // sphere type
            // allocate space for object
            node = (OBJ_T*) malloc(sizeof(OBJ_T));
            node->type = 's'; // set type to sphere
            // read sphere center
            fscanf(file, "%lf %lf %lf", &node->sphere.center.x, &node->sphere.center.y, &node->sphere.center.z);
            // read sphere radius
            fscanf(file, "%lf", &node->sphere.radius);
            // read sphere color
            fscanf(file, "%lf %lf %lf", &node->color.r, &node->color.g, &node->color.b);
            // set checker to false
            node->checker = 0;
            // set interects function
            node->intersects = &intersects_sphere;
            // add it to the objects linked list
            node->next = scene->objs;
            scene->objs = node;
        } else if (info_type == 'p') { // plane type
            // allocate space for object
            node = (OBJ_T*) malloc(sizeof(OBJ_T));
            node->type = 'p'; // set type to plane
            // read plane normal
            fscanf(file, "%lf %lf %lf", &node->plane.normal.x, &node->plane.normal.y, &node->plane.normal.z);
            // read plane distance 
            fscanf(file, "%lf", &node->plane.D);
            // read plane color 
            fscanf(file, "%lf %lf %lf", &node->color.r, &node->color.g, &node->color.b);
            // set checker to true
            node->checker = 1;
            // read plane color2
            fscanf(file, "%lf %lf %lf", &node->color2.r, &node->color2.g, &node->color2.b);
            // set interects function
            node->intersects = &intersects_plane;
            // add it to the objects linked list
            node->next = scene->objs;
            scene->objs = node;
        } else if (info_type == 'l') { // light type
            // read light location
            fscanf(file, "%lf %lf %lf", &scene->light.loc.x, &scene->light.loc.y, &scene->light.loc.z);
        }
    }
    // close the file
    fclose(file);
}

// main method
int main() {
    // set eye position 
    VP_T eye_pos = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };
    // initialize scene
    SCENE_T scene;
    init(&scene);
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
            RGB_T point_color = trace(curr_ray, &scene); // replace with scene
            fprintf(fimg, "%c%c%c", (unsigned char) (255 * point_color.r), (unsigned char) (255 * point_color.g), (unsigned char) (255 * point_color.b));
        }
    }
    printf("\n");

    // close the file
    fclose(fimg);
    // exit
    return 0;
}

/*
Tasks:
1. Create a linked list of objects
2. Include scene structure (in main). Pass to init 
3. Updating to file writing 
4. update trace to use linked list
5. include vp subtract 
6. finish light.c 
*/
