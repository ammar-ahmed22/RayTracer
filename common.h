#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility

inline double degrees_to_radians(double degrees){
    return degrees * (pi / 180.0);
}

inline double random_double(){
    // Random number in [0, 1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    // Random number in [min, max)
    return min + (max - min)*random_double();
}

// Clamps a value between min and max
inline double clamp(double x, double min, double max){
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common headers
#include "ray.h"
#include "vec3.h"



#endif