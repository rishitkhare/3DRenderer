#include "SDL.h"
#include "linalg.h"

// vertex object
struct Vert {
    Vector3 position;
    SDL_Color color;
};

// a triangle in 3D space
struct Tri {
    Vert v1;
    Vert v2;
    Vert v3;
};

// orthonormal basis for the camera space
struct Camera {
    Vector3 right; // x vector
    Vector3 up; // y vector
    Vector3 target; // -z vector
};


