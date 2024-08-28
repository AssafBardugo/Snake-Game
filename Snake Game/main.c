#include "snake.h"

/***** Global Variables *****/
Apple apple;
Snake* snake_head;
Snake_Dir direction;
int score = 0;
int record = 0;     // TODO: save in a file.
int delay_time = 200;


int main(int argc, char* argv[]){

    srand(time(NULL));

    initSnake();
    genApple();

    SDL_Window* window;
    SDL_Renderer* renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Snake Game",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        // SDL_WINDOW_BORDERLESS || SDL_WINDOW_INPUT_GRABBED
        // SDL_WINDOW_INPUT_GRABBED
        SDL_WINDOW_BORDERLESS
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    while(true){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    goto GameQuit;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case LEFT_KEY:
                            if(direction != SNAKE_RIGHT)
                                direction = SNAKE_LEFT;
                            break;
                        case RIGHT_KEY:
                            if(direction != SNAKE_LEFT)
                                direction = SNAKE_RIGHT;
                            break;
                        case UP_KEY:
                            if(direction != SNAKE_DOWN)
                                direction = SNAKE_UP;
                            break;
                        case DOWN_KEY:
                            if(direction != SNAKE_UP)
                                direction = SNAKE_DOWN;
                            break;
                        case SDLK_ESCAPE:
                            goto GameQuit;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        SDL_RenderClear(renderer);

        /* Render loop start */
        moveSnake();
        detectApple();
        detectCrash();

#if USE_GRID_BOARD
        renderGrid(renderer);
#else
        renderOutline(renderer);
#endif
        renderSnake(renderer);
        renderApple(renderer);
        renderMonitor(renderer);
        /* Render loop end */

        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(delay_time);
    }
GameQuit:

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    freeSnake(snake_head);
    return 0;
}