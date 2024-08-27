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

Apple apple;

typedef struct snake{
    int x;
    int y;
    Snake_Dir direction;
    struct snake* next;
} Snake;

Snake* head;
Snake* tail;

void initSnake(){

    Snake* new_snake = malloc(sizeof(Snake));

    srand(time(NULL));

    new_snake->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new_snake->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new_snake->direction = rand() % DIRECTIONS_COUNT;
    new_snake->next = NULL;

    head = new_snake;
    tail = new_snake;
}


void freeSnake(Snake* to_free){

    if(to_free->next != NULL){
        freeSnake(to_free->next);
    }
    free(to_free);
}


void increaseSnake(int new_x, int new_y){

    Snake* new_snake = malloc(sizeof(Snake));

    new_snake->x = new_x;
    new_snake->y = new_y;
    new_snake->direction = head->direction;
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
    track->next->direction = track->direction;
}


void moveSnake(){

    moveSnakeRecursive(head);

    switch(head->direction){
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

    srand(time(NULL));

    apple.x = rand() % GRID_SIZE;   // TODO: Make sure the cell is not a part of the snake.
    apple.y = rand() % GRID_SIZE;
}


void renderApple(SDL_Renderer* renderer, int x, int y){

    SDL_Rect apple;


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

    initSnake();
    increaseSnake(head->x + 1, head->y);    // TODO: should depand on the head's direction
    increaseSnake(head->x + 1, head->y);    // TODO: should depand on the head's direction
    increaseSnake(head->x + 1, head->y);    // TODO: should depand on the head's direction
    increaseSnake(head->x + 1, head->y);    // TODO: should depand on the head's direction


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
                            head->direction = SNAKE_LEFT;
                            break;
                        case KEY_RIGHT:
                            head->direction = SNAKE_RIGHT;
                            break;
                        case KEY_UP:
                            head->direction = SNAKE_UP;
                            break;
                        case KEY_DOWN:
                            head->direction = SNAKE_DOWN;
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

        int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIMENSION / 2);
        int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIMENSION / 2);
        renderGrid(renderer, grid_x, grid_y);
        renderSnake(renderer, grid_x, grid_y);

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