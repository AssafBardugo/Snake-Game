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


#define KEY_LEFT    SDLK_j
#define KEY_RIGHT   SDLK_l
#define KEY_UP      SDLK_i
#define KEY_DOWN    SDLK_k

#define GRID_SIZE 25
#define GRID_DIMENSION 400
#define CELL_SIZE (GRID_DIMENSION / GRID_SIZE)

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

Apple apple;
Snake* head;
// Snake* tail;
Snake_Dir direction;

int score = 0;


void initSnake(){

    Snake* new_snake = malloc(sizeof(Snake));

    new_snake->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new_snake->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new_snake->next = NULL;

    head = new_snake;
    direction = rand() % DIRECTIONS_COUNT;
}


void freeSnake(Snake* to_free){

    if(to_free->next != NULL){
        freeSnake(to_free->next);
    }
    free(to_free);
}


void increaseSnake(){

    Snake* new_snake = malloc(sizeof(Snake));

    new_snake->x = head->x;
    new_snake->y = head->y;
    new_snake->next = head;

    head = new_snake;
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

    moveSnakeRecursive(head);

    switch(direction){
        case SNAKE_UP:
            --head->y;
            break;
        case SNAKE_DOWN:
            ++head->y;
            break;
        case SNAKE_LEFT:
            --head->x;
            break;
        case SNAKE_RIGHT:
            ++head->x;
            break;
        default:
            break;
    }
}


void genApple(){

    apple.x = rand() % GRID_SIZE;   // TODO: Make sure the cell is not a part of the snake.
    apple.y = rand() % GRID_SIZE;
}


void detectApple(){

    // Look if the snake is eating the apple:
    if(head->x == apple.x && head->y == apple.y){

        increaseSnake();
        genApple();
        ++score;
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

    Snake* track = head;
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


int main(int argc, char* argv[]){

    srand(time(NULL));

    initSnake();
    increaseSnake();

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
        SDL_WINDOW_BORDERLESS
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event event;

    while(!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case KEY_LEFT:
                            direction = SNAKE_LEFT;
                            break;
                        case KEY_RIGHT:
                            direction = SNAKE_RIGHT;
                            break;
                        case KEY_UP:
                            direction = SNAKE_UP;
                            break;
                        case KEY_DOWN:
                            direction = SNAKE_DOWN;
                            break;
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);
        // render loop start

        moveSnake();
        detectApple();

        int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIMENSION / 2);
        int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIMENSION / 2);
        renderGrid(renderer, grid_x, grid_y);
        renderSnake(renderer, grid_x, grid_y);
        renderApple(renderer, grid_x, grid_y);

        // render loop end
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(200);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    freeSnake(head);

    return 0;
}