# generate a scene file for the creative project
import math

def create_sphere(f, center, color, radius=0.5):
    return f.write(f"s\n{center}\n{radius}\n{color}\n\n")

def create_plane(f, normal, distance, color_one, color_two):
    return f.write(f"p\n{normal}\n{distance}\n{color_one}\n{color_two}\n\n")

def create_light(f, center):
    return f.write(f"l\n{center}\n")

# main method
with open("c_scene.txt", "w") as f:
    # create bakcground plane
    create_plane(f, "0 0 -1", 5, "0 0 0", "1 1 1")
    
    # colors of the rainbow
    colors = ["1 0 0", "1 0.5 0", "1 1 0", "0 1 0", "0 0 1", "0.29 0 0.51", "0.56 0 1"]
    red = "1 0 0"
    green = "0 1 0"
    
    # radius of the circle and spheres
    circle_radius = 1
    
    # create a ring of seven spheres in a circle the color of the rainbow
    for i in range(7):
        angle = 2 * math.pi * i / 7
        x = circle_radius * math.cos(angle)
        z = circle_radius * math.sin(angle)
        center = f"{x} {z} 4.0"
        create_sphere(f, center, colors[i])

    # add light
    create_light(f, "0 0 2")