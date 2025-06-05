#include "SDL.h"
#include "linalg.h"

// a triangle in 3D space
struct Tri {
    Vector3 a1;
    Vector3 a2;
    Vector3 a3;
};

// orthonormal basis for the camera space
struct Camera {
    Vector3 right; // x vector
    Vector3 up; // y vector
    Vector3 target; // -z vector
};


