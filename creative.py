# generate a scene file for the creative project
import math

def create_sphere(f, center, radius, color):
    return f.write(f"s\n{center}\n{radius}\n{color}\n\n")

def create_plane(f, normal, distance, color_one, color_two):
    return f.write(f"p\n{normal}\n{distance}\n{color_one}\n{color_two}\n\n")

def create_light(f, center):
    return f.write(f"l\n{center}\n")

# main method
with open("c_scene.txt", "w") as f:
    # create bakcground plane
    # create_plane(f, "0 1 0", 1, "1 1 1", "1 1 1")
    
    # colors of the rainbow
    colors = ["1 0 0", "1 0.5 0", "1 1 0", "0 1 0", "0 0 1", "0.29 0 0.51", "0.56 0 1"]
    
    # radius of the circle and spheres
    circle_radius = 3
    sphere_radius = 0.25
    
    # create a ring of seven spheres in a circle the color of the rainbow
    # for i in range(7):
    #     angle = 2 * math.pi * i / 7
    #     x = circle_radius * math.cos(angle)
    #     z = circle_radius * math.sin(angle)
    #     center = f"{x} {z} 1"
    #     print(f"{x} {z}")
    #     create_sphere(f, center, sphere_radius, colors[i])

    create_sphere(f, "1.0 1 2", 0.1, "1 0 0")
    create_sphere(f, "2.0 1 3", 0.1, "1 1 0")
    create_sphere(f, "3.0 1 4", 0.1, "1 1 1")
    create_sphere(f, "4.0 1 5", 0.1, "0 0 0")

    # add light
    create_light(f, "0 0 0")