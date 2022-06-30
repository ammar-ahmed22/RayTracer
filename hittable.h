#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;

struct hit_record{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Negative dot product indicates there is a component of both vectors in the same direction.
        // In our case, this means the ray intersection occurs inside the sphere (or other object)
        front_face = dot(r.direction(), outward_normal) < 0;

        // If ray intersection is outside, keep normal, otherwise, invert normal to point inwards (for internal reflection etc.)
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable{
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif