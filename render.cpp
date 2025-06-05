#include "render.h"

/*
 * Take an array of triangles (in world space)
 * and converts them all into screen space (z=0).
 */
void convertToWindowCoordinates(Tri *meshdata, int triCount, Tri *screenSpaceData, Camera c) {
    for(int i = 0; i < triCount; i++) {
        // TODO perform space transformations
        //   (currently no transformations are applied and we just directly copy the data)
        Vector3 world;
        Vector3 camera;

        // right.x   right.y   right.z      x
        // up.x      up.y.     up.x         y
        // target.x  target.y  target.z     z

        //

        world.x = camera.x * c.right.x + camera.y * c.up.x + camera.z * c.target.x;
        world.y = camera.x * c.right.y + camera.y * c.up.y + camera.z * c.target.y;
        world.z = camera.x * c.right.z + camera.y * c.up.z + camera.z * c.target.z;

        screenSpaceData[i] = meshdata[i];
    }
}

/*
 * Take a set of screen-space triangles and rasterize them to the surface
 */
void renderTris(Tri *screenSpaceData, int triCount, SDL_Surface *surface) {
    // avoid corruption by locking the surface (thread-safety..? idk)
    SDL_LockSurface(surface);

    // for each triangle, we will iterate over all the pixels in the surface
    // and determine if the pixel intersects with the triangle (and color it)
    Uint32 *pixels = (Uint32*)surface->pixels;

    // for now, we will make all triangles white
    // this is theoretically where a vertex or fragment shader would be applied
    Uint32 trianglePixelValue = SDL_MapRGB(surface->format,0xFF,0xFF,0xFF);

    for(int i = 0; i < triCount; i++) {
        Tri currentTri = screenSpaceData[i];

        for(int y = 0; y < surface->h; y++) {
            for(int x = 0; x < surface->w; x++) {
                // address arithmetic - "pitch" tells us the length of each row, in bytes,
                // and each individual pixel is an uint32.
                Uint32 *currentPixel = ((Uint32*) (((char*) pixels) + (surface->pitch * y))) + x;

                Vector3 tricoord = barycentric(currentTri, {(float) x,(float) y, 0.0});
                if(tricoord.x >= 0 && tricoord.y >= 0 && tricoord.z >= 0) {
                    // the point is within the triangle
                    *currentPixel = trianglePixelValue;
                }
            }
        }
    }

    SDL_UnlockSurface(surface);
}

/*
 * Get the barycentric coordinates of a point with respect to a triangle
 */
Vector3 barycentric(Tri t, Vector3 p) {
    // calculate vectors from point p to each triangle point
    Vector3 pa = VEC_SUB(p, t.a1);
    Vector3 pb = VEC_SUB(p, t.a2);
    Vector3 pc = VEC_SUB(p, t.a3);

    // calculate the areas of the subtriangles
    float area1 = CROSS2D(pb, pc);
    float area2 = CROSS2D(pc, pa);
    float area3 = CROSS2D(pa, pb);

    // calculate the total area
    Vector3 ab = VEC_SUB(t.a2,t.a1);
    Vector3 ac = VEC_SUB(t.a3,t.a1);
    float totalarea = CROSS2D(ab, ac);

    // calculate barycentric coordinates
    float t1 = area1 / totalarea;
    float t2 = area2 / totalarea;
    float t3 = area3 / totalarea;


    return {t1,t2,t3};
}


/*
 * Put it all together.
 */
void render3D(Tri *meshdata, int triCount, SDL_Surface *screen, Camera c) {
    // space transforms
    Tri *screenspace = (Tri*) malloc(sizeof(Tri) * triCount);
    convertToWindowCoordinates(meshdata, triCount, screenspace, c);

    // put triangles through the rasterizer!
    renderTris(screenspace, triCount, screen);
    free(screenspace);
}
