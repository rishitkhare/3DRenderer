#include "SDL.h"

int init_window(SDL_Window **windowOut, SDL_Surface **surfaceOut);
void doLoop(SDL_Window *window, SDL_Surface *screenSurface);
int gameExit(SDL_Window *window);
