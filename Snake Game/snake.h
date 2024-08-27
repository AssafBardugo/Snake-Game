#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>

#define FULLSCREEN 1

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

#define USE_GRID_BOARD true
#define GRID_SIZE 25
#define GRID_DIMENSION 400
#define CELL_SIZE (GRID_DIMENSION / GRID_SIZE)

#define GRID_X  ((WINDOW_WIDTH / 2) - (GRID_DIMENSION / 2))
#define GRID_Y  ((WINDOW_HEIGHT / 2) - (GRID_DIMENSION / 2))

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


extern Apple apple;
extern Snake* snake_head;
extern Snake_Dir direction;
extern int score;
extern int record;
extern int delay_time;


void initSnake();
void freeSnake(Snake* to_free);

int snakeLength();
void increaseSnake();
void moveSnakeRecursive(Snake* track);
void moveSnake();

void genApple();
void detectApple();

void resetSnake();
void detectCrash();

void renderApple(SDL_Renderer* renderer);
void renderSnake(SDL_Renderer* renderer);
void renderGrid(SDL_Renderer* renderer);
void renderOutline(SDL_Renderer* renderer);

#endif /* SNAKE_H */