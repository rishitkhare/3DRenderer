#include "render.h"
#include "SDL_loop.h"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
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

    // dummy data for the triangles - a green triangle
    Tri twoTris[] = {
            {
        {325,100,0},
        {375,150,0},
        {275,150,0}
        },
    {
        {325,200,0},
        {375,150,0},
        {200,150,0}
        },
    {
        {400.0f, 100.0f, 0.0f},
        {300.0f, 400.0f, 0.0f},
        {100.0f, 300.0f, 0.0f}
        }

    };

    Camera c;

    render3D(twoTris,3,screenSurface, c);
    // Update the surface
    SDL_UpdateWindowSurface(window);


    // do the main loop...
    // continue to poll for events until a quit event is reached
    while (!quit) {
        // process all events in the event queue - SDL_PollEvent will return 0 upon emptying the queue
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
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
