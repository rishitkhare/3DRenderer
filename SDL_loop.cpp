#include "render.h"
#include "SDL_loop.h"

// Screen dimension constants
const int SCREEN_WIDTH = 360;
const int SCREEN_HEIGHT = 360;

/**
 *
 * @param windowOut pass in a reference to a pointer
 * @param screenSurfaceOut
 * @return
 */
int init_window(SDL_Window **windowOut, SDL_Surface **screenSurfaceOut) {
    // Create the window we'll render to
    SDL_Window *window = NULL;

    // The window will contain a single surface
    SDL_Surface *screenSurface = NULL;

    /*
     * An SDL_Surface is a 2D image. A 2D image can be loaded from a file or
     * be the image inside a window. (We are using the second use case)
     *
     * SDL_Surfaces are used for CPU rendering
     */
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // SDL_Init returns -1 upon error
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        // SDL_GetError() returns the most recent error. Remember it and use it everywhere!
        return -1; // Logically this return statement makes sense but isn't in the tutorial?
    }

    // initialization was successful
    window = SDL_CreateWindow("MyFirstWindow",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, //
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Get the window surface
    screenSurface = SDL_GetWindowSurface(window);

    *windowOut = window;
    *screenSurfaceOut = screenSurface;

    return 0;
}

void doLoop(SDL_Window *window, SDL_Surface *screenSurface) {
    // Event loop
    SDL_Event e;
    bool quit = false;

    // we are going to render before the actual loop (to be changed later) TODO

    // Fill surface black
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

    float camdownangle = 25.f;

    // dummy data for the triangles
    Tri triArray[] = {
    {
            {{0, 100, 0}, {255,0,0,255}},
            {{75, 0, -75},  {0,255,0,255}},
            {{-75, 0, -75},  {0,0,255,255}},

        },
    {
            {{-75, 0, 75}, {255,0,0,255}},
            {{0, 100, 0},  {0,255,0,255}},
            {{75, 0, 75},  {0,0,255,255}},

        },
    {
            {{75, 0, -75}, {255,0,0,255}},
            {{75, 0, 75},  {0,255,0,255}},
            {{0, 100, 0},  {0,0,255,255}},

        },
    {
            {{0, 100, 0}, {255,0,0,255}},
            {{-75, 0, 75},  {0,255,0,255}},
            {{-75, 0, -75},  {0,0,255,255}},

        },

    {
            {{0, -100, 0}, {255,0,0,255}},
            {{75, 0, -75},  {0,255,0,255}},
            {{-75, 0, -75},  {0,0,255,255}},

    },
    {
            {{-75, 0, 75}, {255,0,0,255}},
            {{0, -100, 0},  {0,255,0,255}},
            {{75, 0, 75},  {0,0,255,255}},

    },
    {
            {{75, 0, -75}, {255,0,0,255}},
            {{75, 0, 75},  {0,255,0,255}},
            {{0, -100, 0},  {0,0,255,255}},

    },
    {
            {{0, -100, 0}, {255,0,0,255}},
            {{-75, 0, 75},  {0,255,0,255}},
            {{-75, 0, -75},  {0,0,255,255}},

    }



    };
    int triCount = sizeof(triArray) / sizeof(Tri);

    Mat4 camera = IDENTITY;

    // tilt camera down
    camera.values[1][1] = cosf(camdownangle);
    camera.values[1][2] = -sinf(camdownangle);
    camera.values[2][1] = sinf(camdownangle);
    camera.values[2][2] = cosf(camdownangle);

    // translate camera
    camera.values[1][3] = 30.f; // y
    camera.values[2][3] = 300.f; // z


    Mat4 model = IDENTITY;

//    render3D(triArray, triCount,screenSurface, camera);
//    // Update the surface
//    SDL_UpdateWindowSurface(window);


    // do the main loop...
    // continue to poll for events until a quit event is reached
    float mvmt = 3.f;

    // rotation matrix definitions

    float deg = 0.01f;

    Mat4 leftrot = {{
                                 {cosf(deg),0,-sinf(deg),0},
                                 {0, 1, 0, 0},
                                 {sinf(deg), 0, cosf(deg),0},
                                 {0,0,0,1},
    }};

    Mat4 rightrot = {{
                            {1,0,0,0},
                            {0, cosf(-deg), -sinf(-deg),0},
                            {0, sinf(-deg), cosf(-deg),0},
                            {0,0,0,1},
                    }};

    Mat4 rot = multiply(leftrot, rightrot);



    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    while (!quit) {
        // process all events in the event queue - SDL_PollEvent will return 0 upon emptying the queue
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        // refresh the screen
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
        render3D(triArray, triCount,screenSurface, model, camera);
        // Update the surface
        SDL_UpdateWindowSurface(window);

        // pan camera left/right controls
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            camera.values[0][3] -= mvmt;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            camera.values[0][3] += mvmt;
        }

        // if space is held, up/down controls z axis camera mvmnt.
        // otherwise default to panning on y axis.
        if (keyboardState[SDL_SCANCODE_SPACE]) {
            if (keyboardState[SDL_SCANCODE_UP]) {
                camera.values[2][3] += mvmt;
            }
            if (keyboardState[SDL_SCANCODE_DOWN]) {
                camera.values[2][3] -= mvmt;
            }
        }
        else {
            if (keyboardState[SDL_SCANCODE_UP]) {
                camera.values[1][3] += mvmt;
            }
            if (keyboardState[SDL_SCANCODE_DOWN]) {
                camera.values[1][3] -= mvmt;
            }
        }

        model = multiply(rot, model);

        // printMatrix(camera);

    }
}
int gameExit(SDL_Window *window, SDL_Surface *screenSurface) {
    // destruct z buffer in renderer
    destructZbuf(screenSurface->w, screenSurface->h);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}

