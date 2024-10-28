
/* shadow test, called from illuminate (after ambient before diffuse)
- if the light ray intersects an object, the point is in shadow
- arguments: int_pt, scene 
RAY_T shadow_ray = 
shadow_ray.vector = vp_subtract(scene->light.loc, int_pt) // then normalize
for (objs) {
    if intersect: is in shadow, return 1;
}

*/