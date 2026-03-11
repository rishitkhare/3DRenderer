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

    // dummy data for the triangles
    Tri triArray[] = {
    {
            {{100, 0, 100}, {255,0,0,255}},
            {{0, 0, 50},  {0,255,0,255}},
            {{0, 100, 100},  {0,0,255,255}},

        },
    {
            {{0, 0, 50}, {255,0,0,255}},
            {{0, 100, 100},  {0,255,0,255}},
            {{-100, 0, 100},  {0,0,255,255}},

        },
    {
            {{-100, 0, 100}, {255,0,0,255}},
            {{0, -100, 100},  {0,255,0,255}},
            {{0, 0, 50},  {0,0,255,255}},

        },
    {
            {{0, -100, 100}, {255,0,0,255}},
            {{100, 0, 100},  {0,255,0,255}},
            {{0, 0, 50},  {0,0,255,255}},

        },

//    {
//        {{325,200,0}, {255,255,255,255}},
//        {{375,150,0},  {255,255,255,255}},
//        {{200,150,0}, {255,255,255,255}}
//        },
//    {
//        {{400.0f, 100.0f, 0.0f},  {255,0,0,255}},
//        {{300.0f, 400.0f, 0.0f},  {0,255,0,255}},
//        {{100.0f, 300.0f, 0.0f},  {0,0,255,255}}
//        }

    };
    int triCount = sizeof(triArray) / sizeof(Tri);

    Mat4 camera = IDENTITY;

//    render3D(triArray, triCount,screenSurface, camera);
//    // Update the surface
//    SDL_UpdateWindowSurface(window);


    // do the main loop...
    // continue to poll for events until a quit event is reached
    float mvmt = 3.f;
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    while (!quit) {
        float xmove = 0.f;
        float ymove = 0.f;
        float zmove = 0.f;

        // process all events in the event queue - SDL_PollEvent will return 0 upon emptying the queue
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        // refresh the screen
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
        render3D(triArray, triCount,screenSurface, camera);
        // Update the surface
        SDL_UpdateWindowSurface(window);

        if (keyboardState[SDL_SCANCODE_LEFT]) {
            xmove -= mvmt;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            xmove += mvmt;
        }
        if (keyboardState[SDL_SCANCODE_UP]) {
            ymove += mvmt;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            ymove -= mvmt;
        }
        if (keyboardState[SDL_SCANCODE_SPACE]) {
            zmove += 0.5f;
        }

        for(int i = 0; i < triCount; i++) {
            triArray[i].v1.position.x += xmove;
            triArray[i].v2.position.x += xmove;
            triArray[i].v3.position.x += xmove;

            triArray[i].v1.position.y += ymove;
            triArray[i].v2.position.y += ymove;
            triArray[i].v3.position.y += ymove;

            triArray[i].v1.position.z += zmove;
            triArray[i].v2.position.z += zmove;
            triArray[i].v3.position.z += zmove;

        }

    }
}

int gameExit(SDL_Window *window) {
    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
