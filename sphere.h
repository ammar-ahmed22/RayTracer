#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"


class sphere : public hittable {
    public:
        sphere(){}
        sphere(point3 cen, double r, shared_ptr<material> m): center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray& r,
            double t_min,
            double t_max,
            hit_record& rec
        ) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    // (Ray_orig + t * Ray_dir - center)^2 = R^2
    // (Ray - center of sphere)^2 = Radius of sphere^2
    // Expands to quadratic:
    // 0 roots -> no intersection
    // 1 root -> intersection on edge
    // 2 roots -> ray goes through

    // Simplified quadratic solver
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // Update record if root found
    rec.t = root; // t at which intersection occurs on ther ray
    rec.p = r.at(rec.t); // point at which intersection occurs
    vec3 outward_normal = (rec.p - center) / radius; // outward normal
    rec.set_face_normal(r, outward_normal); // setting face normal
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif