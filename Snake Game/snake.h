#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>

typedef struct snake Snake;

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

void renderApple(SDL_Renderer* renderer, int x, int y);
void renderSnake(SDL_Renderer* renderer, int x, int y);
void renderGrid(SDL_Renderer* renderer, int x, int y);
void renderOutline(SDL_Renderer* renderer, int x, int y);

#endif /* SNAKE_H */