#include "common.h"

#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "material.h"
#include "camera.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std::chrono;

color ray_color(const ray& r, const hittable& world, int depth){
    hit_record rec;

    // Exceeded ray bounce limit, no more light is gathered
    // Bounced too many times, color is black
    if (depth <= 0){
        return color(0, 0, 0);
    }

    // if ray intersects with any object in the world
    if (world.hit(r, 0.001, infinity, rec)){
        ray scattered;
        color attenuation;

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0, 0, 0);
    }

    // Drawing background gradient
    vec3 unit_direction = unit_vector(r.direction());
    
    auto t = 0.5*(unit_direction.y() + 1);

    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.71, 1);
}

hittable_list random_scene(){
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.3373, 0.4902, 0.2745));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // Loop through 11 x 11 grid
    for (int a = -11; a < 11; a++){
        for (int b = -11; b < 11; b++){
            auto choose_mat = random_double();

            // Choose random center in grid area
            point3 sphere_center(a + 0.9*random_double(), 0.2, b+0.9*random_double());

            // If center far enough from center stage area (where the 3 big boys will go)
            if ((sphere_center - point3(4, 0.2, 0)).length() > 0.9){
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.75){
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(sphere_center, 0.2, sphere_material));
                }else{
                    auto albedo = color::random(0.5, 0.1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(sphere_center, 0.2, sphere_material));
                }
            }

        }
    }

    auto gold = make_shared<metal>(color(0.6, 0.52, 0), 0.1);
    auto silver = make_shared<metal>(color(0.75, 0.75, 0.75), 0.1);
    auto bronze = make_shared<metal>(color(0.6902, 0.5529, 0.3412), 0.1);

    world.add(make_shared<sphere>(point3(4, 1, 0), 1, gold));
    world.add(make_shared<sphere>(point3(0, 1, 0), 1, silver));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1, bronze));

    return world;
}

std::string to_fixed_string(double value, int precision){
    std::ostringstream streamObj;

    streamObj << std::fixed;
    streamObj << std::setprecision(precision);
    streamObj << value;

    return streamObj.str();

}

std::string format_time_rem(int ms){
    auto s = ms / 1000;
    auto m = s / 60;
    auto h = m / 60;

    s = s % 60;
    m = m % 60;

    if (h > 0){
        return std::to_string(h) + "h" + std::to_string(m) + "m" + std::to_string(s) + "s";
    }else if (m > 0){
        return std::to_string(m) + "m" + std::to_string(s) + "s";
    }else if (s > 0){
        return std::to_string(s) + "s";
    }else{
        return std::to_string(ms) + "ms";
    }
}

int main(){
    auto start = high_resolution_clock::now();
    // ============= Image ==============
    auto aspect_ratio = 3.0 / 2.0;
    // Image is 400 pixels wide with 16:9 aspect ratio
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_pp = 100;
    const int max_depth = 50;

    // ============= World ==============
    hittable_list world = random_scene();

    

    // ============= Camera ==============
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // =========== Render ================

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    auto time_rem = 0;
    int elapsed;
    for(int j = image_height - 1; j >=0; --j){
        auto percent = 1 - (double(j) / image_height);

        auto iter_start = high_resolution_clock::now();
        
        std::cerr.precision(3);
        std::cerr << "\nTime Remaining: " << format_time_rem(time_rem) << "\nPercent Complete: " << percent*100 << '%' << "\n====================================" << std::flush;
        for (int i = 0; i < image_width; ++i){
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_pp; ++s){
                auto u = double(i + random_double()) / (image_width - 1);
                auto v = double(j + random_double()) / (image_height - 1);

                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
        
            write_color(std::cout, pixel_color, samples_pp);
            
        }

        auto iter_end = high_resolution_clock::now();
        elapsed = duration_cast<milliseconds>(iter_end - iter_start).count();

        time_rem = elapsed * j;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    

    std::cerr.precision(3);
    std::cerr << "\nDone in " << format_time_rem(duration.count()) << " ✨\n";
}