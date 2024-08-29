#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>

/*** Can be modify by the user ***/
#define FULLSCREEN false
#define USE_GRID_BOARD true

#define LEFT_KEY    SDLK_j  // Define in src\include\SDL_keycode.h
#define RIGHT_KEY   SDLK_l
#define UP_KEY      SDLK_i
#define DOWN_KEY    SDLK_k

#define SNAKE_LIVES 3   // One digit only!
#define FIRST_INSTANCE_SNAKE_LENGTH 3   // at least 1
#define CRASH_DELAY_TIME 500
#define INIT_DELAY_TIME 500
/**********************************/


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

// GRID SIZE:
#define CELL_SIZE 25
#define GRID_X_SIZE (GRID_WIDTH / CELL_SIZE)
#define GRID_Y_SIZE (GRID_HEIGHT / CELL_SIZE)

// MONITOR DIMENTIONS:
#define MONITOR_WIDTH   (((WINDOW_WIDTH - 100) / 3) - 25)
#define MONITOR_HEIGHT  GRID_HEIGHT
#define MONITOR_X       (GRID_X + GRID_WIDTH + 25)
#define MONITOR_Y       GRID_Y

#define TEXT_FONT_SIZE 28
#define PAUSE_FONT_SIZE 30

// PAUSE-MESSAGE POSITION:
#define PAUSE_X     (MONITOR_X + 15)
#define PAUSE_Y     (MONITOR_Y + 30)

// LIVES-MESSAGE POSITION:
#define LIVES_X     (MONITOR_X + 13)
#define LIVES_Y     (MONITOR_Y + 3 * (MONITOR_HEIGHT / 16))

// SCORE-MESSAGE POSITION:
#define SCORE_X     (MONITOR_X + 13)
#define SCORE_Y     (MONITOR_Y + 5 * (MONITOR_HEIGHT / 16))

// RECORD-MESSAGE POSITION:
#define RECORD_X    (MONITOR_X + 13)
#define RECORD_Y    (MONITOR_Y + 6 * (MONITOR_HEIGHT / 16))


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

extern const char* font_file_path;
extern char* score_text;
extern char* lives_text;
extern char* record;
extern const char* score_message;
extern const char* lives_message;

extern int delay_time;
extern SDL_Color sdl_white;


void initText();
void initSnake();
void freeSnake(Snake* to_free);

int getIntScore();
int snakeLength();

void increaseSnake();
void increaseScore();
void resetScore();
void decreaseLives();
void resetLives();

void moveSnakeRecursive(Snake* track);
void moveSnake();

void genApple();
void detectApple();

void detectCrash();
void resetGame();

void renderApple(SDL_Renderer* renderer);
void renderSnake(SDL_Renderer* renderer);
void renderGrid(SDL_Renderer* renderer);
void renderOutline(SDL_Renderer* renderer);
void renderMonitor(SDL_Renderer* renderer);
void renderText(SDL_Renderer* renderer, TTF_Font* font, 
                        const char* text, SDL_Rect rect, SDL_Color color);

#endif /* SNAKE_H */