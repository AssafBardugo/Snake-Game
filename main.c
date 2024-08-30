#include "snake.h"

/***** Global Variables *****/
Apple apple;
Snake* snake_head;
Snake_Dir direction;

const char* font_file_path = "src/font_file/Retro_Gaming.ttf";
char* score_text;
const char* score_message = "Score: 000";
char* lives_text;
const char* lives_message = "Lives: 0";
// char* record;
// const char* record_message = "Record: ";

int live_score = 0;
bool pause = false;

SDL_Color sdl_white = {0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE};
SDL_Color sdl_green = {0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE};
/***************************/

int main(int argc, char *argv[]){

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    TTF_Font* font;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 0;
    }

    TTF_Init();
    font = TTF_OpenFont(font_file_path, TEXT_FONT_SIZE);
    if(font == NULL){
        printf("Failed to load font: %s\n", TTF_GetError());
        return 0;
    }

    window = SDL_CreateWindow(
        "Snake Game",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(time(NULL));
    initText();
    initSnake();
    genApple();

    SDL_Delay(INIT_DELAY_TIME);

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
                            if(!pause && direction != SNAKE_RIGHT)
                                direction = SNAKE_LEFT;
                            break;
                        case RIGHT_KEY:
                            if(!pause && direction != SNAKE_LEFT)
                                direction = SNAKE_RIGHT;
                            break;
                        case UP_KEY:
                            if(!pause && direction != SNAKE_DOWN)
                                direction = SNAKE_UP;
                            break;
                        case DOWN_KEY:
                            if(!pause && direction != SNAKE_UP)
                                direction = SNAKE_DOWN;
                            break;
                        case SDLK_SPACE:
                            pause = 1 - pause;
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
        if(pause){
            SDL_Rect pause_rect = {PAUSE_X, PAUSE_Y, 0, 0};
            renderText(renderer, font, "Pause ON", pause_rect, sdl_green);
        }else{
            moveSnake();
            detectApple();
            detectCrash();
        }

        renderSnake(renderer);
        renderApple(renderer);
#if USE_GRID_BOARD
        renderGrid(renderer);
#else
        renderOutline(renderer);
#endif
        renderMonitor(renderer);

        SDL_Rect score_rect = {SCORE_X, SCORE_Y, 0, 0};
        renderText(renderer, font, score_text, score_rect, sdl_white);

        SDL_Rect lives_rect = {LIVES_X, LIVES_Y, 0, 0};
        renderText(renderer, font, lives_text, lives_rect, sdl_white);

        // SDL_Rect record_rect = {RECORD_X, RECORD_Y, 0, 0};
        // renderText(renderer, font, record_message, record_rect, sdl_white);
        /* Render loop end */

        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(renderer);

        SDL_Delay(MAX(DELAY_TIME - live_score * 3, 30));
    }
GameQuit:

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    freeSnake(snake_head);
    free(score_text);
    free(lives_text);
    return 0;
}