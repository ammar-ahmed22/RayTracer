#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"


#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        hittable_list(){}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        // remove all objects
        void clear() { objects.clear(); }

        // add objects to world vector
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        // override hit function
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    
    public:
        // vector of shared_ptr hittable objects
        std::vector<shared_ptr<hittable>> objects;
};

// Overridden hit function
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;

    bool hit_anything = false;

    auto closest_so_far = t_max;

    // Loop over all objects
    for (const auto& object : objects){
        // Each object should have it's own hit function defined and should be extend hittable class
        if (object->hit(r, t_min, closest_so_far, temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif