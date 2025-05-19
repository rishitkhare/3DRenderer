#include <SDL.h>
#include "SDL_loop.h"

int main(int argc, char* args[]) {

    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    init_window(&window, &surface);

    doLoop(window, surface);

    gameExit(window);
}
