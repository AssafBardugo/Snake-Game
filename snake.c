#include "snake.h"

void initText(){

    score_text = (char*)malloc((strlen(score_message) + 1) * sizeof(char));
    strcpy(score_text, score_message);

    lives_text = (char*)malloc((strlen(lives_message) + 1) * sizeof(char));
    strcpy(lives_text, lives_message);
    resetLives();
}


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


int getIntScore(){

    return atoi(score_text + strlen(score_text) - 3);
}


int snakeLength(){
    
    return getIntScore() + FIRST_INSTANCE_SNAKE_LENGTH;
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


void increaseScore(){

    ++live_score;

    int i = strlen(score_text) - 1;

    for(int j = i; j > i - 3; --j){

        if(score_text[j] < '9'){
            ++score_text[j];
            return;
        }
        score_text[j] = '0';
    }

    // Here Score > 999. Probably bug to fix.
    printf("The Score exceeded the allowed limit.\n");
    exit(0);
}


void resetScore(){

    strcpy(score_text + strlen(score_text) - 3, "000");
}


void decreaseLives(){

    live_score = 0;

    int i = strlen(lives_text) - 1;
    --lives_text[i];

    if(lives_text[i] == '0'){
        resetGame();
    }

    Snake* to_free = snake_head;
    
    initSnake();
    genApple();

    freeSnake(to_free);
    SDL_Delay(CRASH_DELAY_TIME);
}


void resetGame(){
    
    // int curr_score = getIntScore();
    // if(curr_score > record){

    //     record = curr_score;
    //     // TODO: Print (render) some kind of record breaking message
    // }
    resetScore();
    resetLives();

    Snake* to_free = snake_head;
    
    initSnake();
    genApple();

    freeSnake(to_free);
    SDL_Delay(2 * CRASH_DELAY_TIME);
}


void resetLives(){

    lives_text[strlen(lives_text) - 1] += SNAKE_LIVES;
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
    static bool inc_next = false;
    if(inc_next){
        increaseSnake();
        inc_next = false;
    }

    // Look if the snake is eating the apple:
    if(snake_head->x == apple.x && snake_head->y == apple.y){

        inc_next = true;
        genApple();
        increaseScore();
    }
}


void detectCrash(){
    
    // Does the snake touch the borders?
    if(snake_head->x < 0 || snake_head->x >= GRID_X_SIZE || snake_head->y < 0 || snake_head->y >= GRID_Y_SIZE){
        // Crash!
        decreaseLives();
        return;
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
            decreaseLives();
            return;
        }
        track = track->next;
    }
}


void renderApple(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    SDL_Rect apple_seg;
    apple_seg.w = CELL_SIZE;
    apple_seg.h = CELL_SIZE;
    apple_seg.x = GRID_X + apple.x * CELL_SIZE;
    apple_seg.y = GRID_Y + apple.y * CELL_SIZE;

    SDL_RenderFillRect(renderer, &apple_seg);
}


void renderSnake(SDL_Renderer* renderer){
    static bool head_blinks = false;
    if(pause){
        head_blinks = 1 - head_blinks;
    }else{
        head_blinks = false;
    }

    Snake* track = snake_head;
    SDL_Rect segment;
    segment.x = GRID_X + (track->x * CELL_SIZE);
    segment.y = GRID_Y + (track->y * CELL_SIZE);
    segment.w = CELL_SIZE;
    segment.h = CELL_SIZE;

    if(!head_blinks){
        SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &segment);
    }

    track = track->next;
    bool green_cell = false;

    while(track != NULL){

        segment.x = GRID_X + (track->x * CELL_SIZE);
        segment.y = GRID_Y + (track->y * CELL_SIZE);

        if(green_cell){
            SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE);
        }else{
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderFillRect(renderer, &segment);

        track = track->next;
        green_cell = 1 - green_cell;
    }
}


void renderGrid(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, SDL_ALPHA_OPAQUE);

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

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE);

    SDL_Rect outline;
    outline.x = GRID_X;
    outline.y = GRID_Y;
    outline.w = GRID_WIDTH;
    outline.h = GRID_HEIGHT;

    SDL_RenderDrawRect(renderer, &outline);
}


void renderMonitor(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, SDL_ALPHA_OPAQUE);

    SDL_Rect monitor;
    monitor.x = MONITOR_X;
    monitor.y = MONITOR_Y;
    monitor.w = MONITOR_WIDTH;
    monitor.h = MONITOR_HEIGHT;

    SDL_RenderDrawRect(renderer, &monitor);
}


void renderText(SDL_Renderer* renderer, TTF_Font* font, 
                        const char* text, SDL_Rect rect, SDL_Color color){

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    TTF_SizeText(font, text, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
