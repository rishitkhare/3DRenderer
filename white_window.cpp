// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



/**
 * SDL will require a main method with this type of signature and this
 * type of signature ONLY. This is because it must be compatible with multiple
 * platforms.
 *
 * @param argc
 * @param args
 * @return
 */
int main(int argc, char* args[]) {
    // Create the window we'll render to
    SDL_Window* window = NULL;

    // The window will contain a single surface
    SDL_Surface* screenSurface = NULL;
    /*
     * An SDL_Surface is a 2D image. A 2D image can be loaded from a file or
     * be the image inside a window. (We are using the second use case)
     */

    // Initialize SDL
    //
    if(SDL_Init( SDL_INIT_VIDEO) < 0) { // SDL_Init returns -1 upon error
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        // SDL_GetError() returns the most recent error. Remember it and use it everywhere!
        return -1; // Logically this return statement makes sense but isn't in the tutorial?
    }

    // initialization was successful
    window = SDL_CreateWindow("MyFirstWindow",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, //
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Get the window surface
    screenSurface = SDL_GetWindowSurface(window);

    // Fill surface white
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(window);

    // Hack to get window to stay up (????)
    SDL_Event e;
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
                quit = true;
        }
    }

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}