#include <stdio.h>

#define rows 20
#define cols 60
#define margin_style '#'

char board[rows][cols];

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

int main(int argc, const char * argv[]) {
    
    fillBoard();
    printBoard();
    
    return 0;
}
