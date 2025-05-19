#include "VertexArrayObject.h"
#include "SDL.h"
#include "math.h"

#define SQUARE(x) (x) * (x)
#define EU_DIST(x1,x2,y1,y2)  sqrt(SQUARE(x1 - x2) + SQUARE(y1 - y2))

// gets the 2-dimensional cross product of two vectors
#define CROSS2D(a, b) (a.x * b.y) - (a.y * b.x)

// gets the subtraction of two vectors
#define VEC_SUB(a,b) ((Vector3){a.x - b.x, a.y - b.y, a.z - b.z});

void render3D(Tri *meshdata, int triCount, SDL_Surface *screen);

void convertToWindowCoordinates(Tri meshdata[], int triCount, Tri *screenSpaceData[]);

void renderTris(Tri *screenSpaceData[], int triCount, SDL_Surface *surface);

Vector3 barycentric(Tri t, Vector3 p);
