#include "VertexArrayObject.h"
#include "SDL.h"
#include "math.h"

#define SQUARE(x) (x) * (x)
#define EU_DIST(x1,x2,y1,y2)  sqrt(SQUARE(x1 - x2) + SQUARE(y1 - y2))

// gets the 2-dimensional cross product (i.e. determinant) of two vectors
#define CROSS2D(a, b) (a.x * b.y) - (a.y * b.x)

// gets the subtraction of two vectors
#define VEC_SUB(a,b) ((Vector3){a.x - b.x, a.y - b.y, a.z - b.z});

void destructZbuf(int x, int y);

void render3D(Tri *meshdata, int triCount, SDL_Surface *screen, Mat4 model, Mat4 cam);

void convertToWindowCoordinates(Tri *meshdata, int triCount, Tri *result, Mat4 modelTransform, Mat4 cameraTransform);

void renderTris(Tri *screenSpaceData, int triCount, SDL_Surface *surface);

SDL_Color interpolateColor(Tri t, Vector3 b);

Vector3 barycentric(Tri t, Vector3 p);
