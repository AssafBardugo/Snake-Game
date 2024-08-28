#include "snake.h"

void initSnake(){

    Snake* new_snake = malloc(sizeof(Snake));

    new_snake->x = (rand() % (GRID_X_SIZE / 2)) + (GRID_X_SIZE / 4);
    new_snake->y = (rand() % (GRID_Y_SIZE / 2)) + (GRID_Y_SIZE / 4);
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

    if(snakeLength() > (int)(0.25 * GRID_X_SIZE * GRID_Y_SIZE)){
        // TODO: Has to jump to a new level
    }

    Snake* new_cell = malloc(sizeof(Snake));

    switch(direction){
        case SNAKE_UP:
            new_cell->x = snake_head->x;
            new_cell->y = snake_head->y - 1;
            break;
        case SNAKE_DOWN:
            new_cell->x = snake_head->x;
            new_cell->y = snake_head->y + 1;
            break;
        case SNAKE_LEFT:
            new_cell->x = snake_head->x - 1;
            new_cell->y = snake_head->y;
            break;
        case SNAKE_RIGHT:
            new_cell->x = snake_head->x + 1;
            new_cell->y = snake_head->y;
            break;
        default:
            break;
    }

    new_cell->next = snake_head;
    snake_head = new_cell;
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

    apple.x = rand() % GRID_X_SIZE;
    apple.y = rand() % GRID_Y_SIZE;

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
    if(snake_head->x < 0 || snake_head->x >= GRID_X_SIZE || snake_head->y < 0 || snake_head->y >= GRID_Y_SIZE){
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


void renderApple(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);

    SDL_Rect apple_seg;
    apple_seg.w = CELL_SIZE;
    apple_seg.h = CELL_SIZE;
    apple_seg.x = GRID_X + apple.x * CELL_SIZE;
    apple_seg.y = GRID_Y + apple.y * CELL_SIZE;

    SDL_RenderFillRect(renderer, &apple_seg);
}


void renderSnake(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);

    SDL_Rect segment;
    segment.w = CELL_SIZE;
    segment.h = CELL_SIZE;

    Snake* track = snake_head;
    while(track != NULL){

        segment.x = GRID_X + (track->x * CELL_SIZE);
        segment.y = GRID_Y + (track->y * CELL_SIZE);

        SDL_RenderFillRect(renderer, &segment);

        track = track->next;
    }
}


void renderGrid(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);

    SDL_Rect cell;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    for(int i = 0; i < GRID_X_SIZE; ++i){
        for(int j = 0; j < GRID_Y_SIZE; ++j){

            cell.x = GRID_X + (i * CELL_SIZE);
            cell.y = GRID_Y + (j * CELL_SIZE);

            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}


void renderOutline(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0xff, 255);

    SDL_Rect outline;
    outline.x = GRID_X;
    outline.y = GRID_Y;
    outline.w = GRID_WIDTH;
    outline.h = GRID_HEIGHT;

    SDL_RenderDrawRect(renderer, &outline);
}


void renderMonitor(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);

    SDL_Rect monitor;
    monitor.x = MONITOR_X;
    monitor.y = MONITOR_Y;
    monitor.w = MONITOR_WIDTH;
    monitor.h = MONITOR_HEIGHT;

    SDL_RenderDrawRect(renderer, &monitor);
}
