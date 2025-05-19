#include "SDL.h"

// A point in 3D space
struct Vector3 {
    float x;
    float y;
    float z;
};

// a triangle in 3D space
struct Tri {
    Vector3 a1;
    Vector3 a2;
    Vector3 a3;
};


