#ifndef CAMERA_H
#define CAMERA_H


#include "common.h"
#include <iostream>

class camera{
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov, 
            double aspect_ratio,
            double aperture,
            double focus_dist
        ){  
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            
            // auto aspect_ratio = 16.0 / 9.0;
            // Creating a virtual viewport of height 2 with 16:9 aspect ratio
            // Rays will use this to draw image
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Distance from viewport to image
            auto focal_length = 1.0;

            origin = lookfrom;
            // Horizontal max of viewport
            horizontal = focus_dist * viewport_width * u;
            // Vertical max of viewport
            vertical = focus_dist * viewport_height * v;
            // Bottom left corner of image
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t){
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }
    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};


#endif 