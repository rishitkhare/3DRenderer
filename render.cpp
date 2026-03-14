#include "render.h"


float **zbuf = NULL;

void resetZbuf(int x, int y) {
    // set all values in buffer to infinity
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            zbuf[i][j] = INFINITY;
        }
    }
}

void initZBuf(int x, int y) {
    if(zbuf == NULL) {
        zbuf = (float **) malloc(sizeof(float*) * x);
        for(int i = 0; i < x; i++) {
            zbuf[i] = (float*) malloc(sizeof(float) * y);
        }
    }

    resetZbuf(x, y);
}



void destructZbuf(int x, int y) {
    for(int i = 0; i < x; i++) {
        free(zbuf[i]);
    }
    free(zbuf);
}

// takes all three points in the triangle and applies the mat4 as an affine transformation.
void multiplyTriByMatrix(Tri *triPtr, Mat4 *matPtr) {
    triPtr->v1.position = vec3(multiply(*matPtr, vec4(triPtr->v1.position)));
    triPtr->v2.position = vec3(multiply(*matPtr, vec4(triPtr->v2.position)));
    triPtr->v3.position = vec3(multiply(*matPtr, vec4(triPtr->v3.position)));
}

/*
 * Take an array of triangles (in camera space)
 * and converts them all into screen space.
 */
void convertToWindowCoordinates(Tri *meshdata, int triCount, Tri *result, Mat4 modelTransform, Mat4 cameraTransform) {

    // matrix to convert a screen space
    Mat4 planeToScreen {{
            {180, 0, 0, 180},
            {0, -180, 0, 180},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
    }};


    // TODO parallelize these pixel calculations using worker threads?
    for(int i = 0; i < triCount; i++) {
        result[i] = meshdata[i];

        multiplyTriByMatrix(&result[i], &modelTransform);
        multiplyTriByMatrix(&result[i], &cameraTransform);

        result[i].v1.position.x /= result[i].v1.position.z;
        result[i].v1.position.y /= result[i].v1.position.z;

        result[i].v2.position.x /= result[i].v2.position.z;
        result[i].v2.position.y /= result[i].v2.position.z;

        result[i].v3.position.x /= result[i].v3.position.z;
        result[i].v3.position.y /= result[i].v3.position.z;

        multiplyTriByMatrix(&result[i], &planeToScreen);
    }
}

/*
 * Takes a triangle and set of barycentric coordinates and returns an interpolated z value
 */
float interpolateZ(Tri t, Vector3 b) {
    return t.v1.position.z * b.x + t.v2.position.z * b.y + t.v3.position.z * b.z;
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

    // clipping planes
    float near = 1.f; // plane defined as z = near
    float far = 80000.f; // plane defined as z = far

    for(int i = 0; i < triCount; i++) {
        Tri currentTri = screenSpaceData[i];

        // calculate a bounding box for the triangle before iterating over tris
        int minX = (int) fminf(fminf(currentTri.v1.position.x, currentTri.v2.position.x), currentTri.v3.position.x) - 1;
        int minY = (int) fminf(fminf(currentTri.v1.position.y, currentTri.v2.position.y), currentTri.v3.position.y) - 1;
        int maxX = (int) fmaxf(fmaxf(currentTri.v1.position.x, currentTri.v2.position.x), currentTri.v3.position.x) + 1;
        int maxY = (int) fmaxf(fmaxf(currentTri.v1.position.y, currentTri.v2.position.y), currentTri.v3.position.y) + 1;

        // iterate over the pixels in the bounding box (do not go past edges of screen)
        for(int y = minY > 0 ? minY : 0; y < (maxY < surface->h ? maxY : surface->h); y++) {
            for(int x = minX > 0 ? minX : 0; x < (maxX < surface->w ? maxX : surface->w); x++) {
                // address arithmetic - "pitch" tells us the length of each row, in bytes,
                // and each individual pixel is an uint32.
                Uint32 *currentPixel = ((Uint32*) (((char*) pixels) + (surface->pitch * y))) + x;

                Vector3 tricoord = barycentric(currentTri, {(float) x,(float) y, 0.0});

                float depth = interpolateZ(currentTri, tricoord);

                // check if the current pixel is actually within the triangle.
                if(tricoord.x >= 0 && tricoord.y >= 0 && tricoord.z >= 0) {

                    // if the current tri is behind something in the z buffer, skip this pixel.
                    if(zbuf[x][y] < depth) {
                        continue;
                    }

                    // if the current pixel is not within the near or far plane, skip writing the pixel.
                    if(depth < near || depth > far) {
                        continue;
                    }

                    // write the pixel to the z buffer
                    zbuf[x][y] = depth;

                    // calculate the fragment color (apply fragment shader type code here)
                    SDL_Color fragColor = interpolateColor(currentTri, tricoord);

                    // write the pixel to the screen
                    *currentPixel = SDL_MapRGBA(surface->format, fragColor.r, fragColor.g, fragColor.b, fragColor.a);
                }
            }
        }
    }

    SDL_UnlockSurface(surface);
}

/*
 * Takes a triangle and a set of barycentric coordinates and returns
 * an interpolated color across the color data of all three vertices.
 */
SDL_Color interpolateColor(Tri t, Vector3 b) {
    Uint8 lerpR = t.v1.color.r * b.x + t.v2.color.r * b.y + t.v3.color.r * b.z;
    Uint8 lerpG = t.v1.color.g * b.x + t.v2.color.g * b.y + t.v3.color.g * b.z;
    Uint8 lerpB = t.v1.color.b * b.x + t.v2.color.b * b.y + t.v3.color.b * b.z;
    Uint8 lerpA = t.v1.color.a * b.x + t.v2.color.a * b.y + t.v3.color.a * b.z;

    return {lerpR, lerpG, lerpB, lerpA};
}

/*
 * Get the barycentric coordinates of a point with respect to a triangle
 */
Vector3 barycentric(Tri t, Vector3 p) {
    // calculate vectors from point p to each triangle point
    Vector3 pa = VEC_SUB(p, t.v1.position);
    Vector3 pb = VEC_SUB(p, t.v2.position);
    Vector3 pc = VEC_SUB(p, t.v3.position);

    // calculate the areas of the subtriangles
    float area1 = CROSS2D(pb, pc);
    float area2 = CROSS2D(pc, pa);
    float area3 = CROSS2D(pa, pb);

    // calculate the total area
    Vector3 ab = VEC_SUB(t.v2.position, t.v1.position);
    Vector3 ac = VEC_SUB(t.v3.position ,t.v1.position);
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
void render3D(Tri *meshdata, int triCount, SDL_Surface *screen, Mat4 model, Mat4 cam) {
    // initialization of z-buffer if necessary
    if(zbuf == NULL) {
        initZBuf(screen->w, screen->h);
    }
    else {
        resetZbuf(screen->w, screen->h);
    }

    // space transforms
    Tri *screenspace = (Tri*) malloc(sizeof(Tri) * triCount);
    convertToWindowCoordinates(meshdata, triCount, screenspace, model, cam);

    // put triangles through the rasterizer!
    renderTris(screenspace, triCount, screen);
    free(screenspace);
}
