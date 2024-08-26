#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)

    #include <conio.h>

//#else
//
//    #include <curses.h>

#endif


#define rows 20
#define cols 60

#define margin_style '#'
#define snake_style '$'

#define left_key    'j'
#define right_key   'l'
#define down_key    'k'
#define up_key      'i'


char board[rows][cols];

int snake_x = 5;
int snake_y = 5;

int is_game_over = 0;

void fillBoard(void){
    int x, y;
    
    for(x = 0; x < rows; ++x){
        for(y = 0; y < cols; ++y){
            if(x == 0 || y == 0 || x == rows - 1 || y == cols - 1)
                board[x][y] = margin_style;
            else
                board[x][y] = ' ';
        }
    }
    board[snake_x][snake_y] = snake_style;
}

void printBoard(void){
    int x, y;
    
    for(x = 0; x < rows; ++x){
        for(y = 0; y < cols; ++y){
            putchar(board[x][y]);
        }
        putchar('\n');
    }
    putchar('\n');
}

//void drawSnake(void){
//
//    board[snake_x][snake_y] = snake_style;
//}

void moveSnake(int delta_x, int delta_y){
    
    snake_x += delta_x;
    snake_y += delta_y;
}

char my_getch(void){
    
    char ch;
    
#if defined(_WIN32) || defined(_WIN64)
    
    ch = getch();
    
#else
    
//    initscr();
//    cbreak();
//    noecho();
//    ch = getch();
//    endwin();
    
    ch = getchar();
    
#endif
    
    return ch;
}

void readKeyboard(void){
    
    char ch = my_getch();
    
    switch (ch) {
        case left_key:
            // move snake left
            moveSnake(-1, 0);
            break;
        case right_key:
            // move snake right
            moveSnake(1, 0);
            break;
        case up_key:
            // move snake up
            moveSnake(0, 1);
            break;
        case down_key:
            // move snake down
            moveSnake(0, -1);
            break;
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    while(!is_game_over){
        
        fillBoard();
        // drawSnake();
        printBoard();
        
        readKeyboard();
    }
    
    return 0;
}
