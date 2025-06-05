#include <SDL.h>
#include "SDL_loop.h"
#include "linalg.h"

int main(int argc, char* args[]) {
//    Mat4 test = {
//            1231,4.4,4,0,
//            1,3,-23,14,
//            2,2,0,1,
//            -2.3,3,13,0
//    };


    // Mat4 testinverse = invert(test);

    // printf("correct:\n[ 1 0 0 0 ]\n[ 0 1 0 0 ]\n[ 0 0 1 0 ]\n[ 0 0 0 1 ]\n\n");

    // printMatrix(multiply(testinverse, test));
    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    init_window(&window, &surface);

    doLoop(window, surface);

    gameExit(window);
}
