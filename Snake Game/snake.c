#include "snake.h"

#define FULLSCREEN 0

#if FULLSCREEN

    #define WINDOW_X SDL_WINDOWPOS_CENTERED
    #define WINDOW_Y SDL_WINDOWPOS_CENTERED
    #define WINDOW_WIDTH 1920
    #define WINDOW_HEIGHT 1080

#else

    #define WINDOW_X 800
    #define WINDOW_Y SDL_WINDOWPOS_CENTERED
    #define WINDOW_WIDTH 700
    #define WINDOW_HEIGHT 600

#endif


#define LEFT_KEY    SDLK_j
#define RIGHT_KEY   SDLK_l
#define UP_KEY      SDLK_i
#define DOWN_KEY    SDLK_k

#define USE_GRID_BOARD false
#define GRID_SIZE 25
#define GRID_DIMENSION 400
#define CELL_SIZE (GRID_DIMENSION / GRID_SIZE)

#define FIRST_INSTANCE_SNAKE_LENGTH 2
#define CRASH_DELAY_TIME 500

typedef enum SNAKE_DIRECTION{
    SNAKE_UP, 
    SNAKE_DOWN, 
    SNAKE_LEFT, 
    SNAKE_RIGHT, 
    DIRECTIONS_COUNT
} Snake_Dir;

typedef struct {
    int x;
    int y;
} Apple;

typedef struct snake{
    int x;
    int y;
    struct snake* next;
} Snake;

/***** Global Variables *****/

Apple apple;
Snake* snake_head;
Snake_Dir direction;
int score = 0;
int record = 0;     // TODO: save in a file.
int delay_time = 200;

/****************************/


void initSnake(){

    Snake* new_snake = malloc(sizeof(Snake));

    new_snake->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new_snake->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new_snake->next = NULL;

    snake_head = new_snake;
    direction = rand() % DIRECTIONS_COUNT;

    for(int i = 1; i < FIRST_INSTANCE_SNAKE_LENGTH; ++i){
        increaseSnake();
    }
}


void freeSnake(Snake* to_free){

    if(to_free->next != NULL){
        freeSnake(to_free->next);
    }
    free(to_free);
}


int snakeLength(){
    return score + 2;
}


void increaseSnake(){

    if(snakeLength() > (int)(0.25 * GRID_SIZE * GRID_SIZE)){
        // TODO: Has to jump to a new level
    }

    Snake* new_snake = malloc(sizeof(Snake));

    new_snake->x = snake_head->x;
    new_snake->y = snake_head->y;
    new_snake->next = snake_head;

    snake_head = new_snake;
}


void moveSnakeRecursive(Snake* track){

    if(track->next == NULL){
        return;
    }else{
        moveSnakeRecursive(track->next);
    }

    track->next->x = track->x;
    track->next->y = track->y;
}


void moveSnake(){

    moveSnakeRecursive(snake_head);

    switch(direction){
        case SNAKE_UP:
            --snake_head->y;
            break;
        case SNAKE_DOWN:
            ++snake_head->y;
            break;
        case SNAKE_LEFT:
            --snake_head->x;
            break;
        case SNAKE_RIGHT:
            ++snake_head->x;
            break;
        default:
            break;
    }
}


void genApple(){

    apple.x = rand() % GRID_SIZE;
    apple.y = rand() % GRID_SIZE;

    // Make sure the apple's cell is not belong to the snake.
    Snake* track = snake_head;
    while(track != NULL){

        if(track->x == apple.x && track->y == apple.y){
            genApple();
            break;
        }
        track = track->next;
    }
}


void detectApple(){

    // Look if the snake is eating the apple:
    if(snake_head->x == apple.x && snake_head->y == apple.y){

        increaseSnake();
        genApple();
        ++score;
    }
}


void resetSnake(){

    Snake* to_free = snake_head;
    
    initSnake();
    genApple();
    // TODO: saveScore()
    score = 0;

    freeSnake(to_free);
    SDL_Delay(CRASH_DELAY_TIME);
}


void detectCrash(){

    // Does the snake touch the borders?
    if(snake_head->x < 0 || snake_head->x >= GRID_SIZE || snake_head->y < 0 || snake_head->y >= GRID_SIZE){
        // Crash!
        resetSnake();
    }

    // Does the snake touch itself?
    if(snakeLength() < 5){
        // Can't be possible
        return;
    }
    Snake* track = snake_head->next->next->next;
    while(track != NULL){

        if(snake_head->x == track->x && snake_head->y == track->y){
            // Crash!
            resetSnake();
        }
        track = track->next;
    }
}


void renderApple(SDL_Renderer* renderer, int x, int y){

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);

    SDL_Rect apple_seg;
    apple_seg.w = CELL_SIZE;
    apple_seg.h = CELL_SIZE;
    apple_seg.x = x + apple.x * CELL_SIZE;
    apple_seg.y = y + apple.y * CELL_SIZE;

    SDL_RenderFillRect(renderer, &apple_seg);
}


void renderSnake(SDL_Renderer* renderer, int x, int y){

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);

    SDL_Rect segment;
    segment.w = CELL_SIZE;
    segment.h = CELL_SIZE;

    Snake* track = snake_head;
    while(track != NULL){

        segment.x = x + (track->x * CELL_SIZE);
        segment.y = y + (track->y * CELL_SIZE);

        SDL_RenderFillRect(renderer, &segment);

        track = track->next;
    }
}


void renderGrid(SDL_Renderer* renderer, int x, int y){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);

    SDL_Rect cell;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    for(int i = 0; i < GRID_SIZE; ++i){
        for(int j = 0; j < GRID_SIZE; ++j){

            cell.x = x + (i * CELL_SIZE);
            cell.y = y + (j * CELL_SIZE);

            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}


void renderOutline(SDL_Renderer* renderer, int x, int y){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0xff, 255);

    SDL_Rect outline;
    outline.x = x;
    outline.y = y;
    outline.w = GRID_DIMENSION;
    outline.h = GRID_DIMENSION;

    SDL_RenderDrawRect(renderer, &outline);
}


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
        // render loop start

        moveSnake();
        detectApple();
        detectCrash();

        int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIMENSION / 2);
        int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIMENSION / 2);
#if USE_GRID_BOARD
        renderGrid(renderer, grid_x, grid_y);
#else
        renderOutline(renderer, grid_x, grid_y);
#endif
        renderSnake(renderer, grid_x, grid_y);
        renderApple(renderer, grid_x, grid_y);

        // render loop end
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