#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>

/*** Can modify by the user ***/
#define FULLSCREEN false
#define USE_GRID_BOARD true

#define LEFT_KEY    SDLK_j  // define in Snake-Game\src\include\SDL_keycode.h
#define RIGHT_KEY   SDLK_l
#define UP_KEY      SDLK_i
#define DOWN_KEY    SDLK_k

#define FIRST_INSTANCE_SNAKE_LENGTH 2   // at least 1
#define CRASH_DELAY_TIME 500    // in ms
/*******************************/


#if FULLSCREEN

    #define WINDOW_X SDL_WINDOWPOS_CENTERED
    #define WINDOW_Y SDL_WINDOWPOS_CENTERED
    #define WINDOW_WIDTH 1920
    #define WINDOW_HEIGHT 1080

#else

    #define WINDOW_X 1000
    #define WINDOW_Y 60
    #define WINDOW_WIDTH 850
    #define WINDOW_HEIGHT 900

#endif

// GRID DIMENTIONS:
#define GRID_WIDTH  (2 * ((WINDOW_WIDTH - 100) / 3))
#define GRID_HEIGHT (WINDOW_HEIGHT - 100)
#define GRID_X      50
#define GRID_Y      50

#define CELL_SIZE 25
#define GRID_X_SIZE (GRID_WIDTH / CELL_SIZE)
#define GRID_Y_SIZE (GRID_HEIGHT / CELL_SIZE)

// MONITOR DIMENTIONS:
#define MONITOR_WIDTH   (((WINDOW_WIDTH - 100) / 3) - 25)
#define MONITOR_HEIGHT  GRID_HEIGHT
#define MONITOR_X       (GRID_X + GRID_WIDTH + 25)
#define MONITOR_Y       GRID_Y    


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
void renderMonitor(SDL_Renderer* renderer);

#endif /* SNAKE_H */