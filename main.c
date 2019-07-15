#include "tetris.h"

//Shape test
//S Z L J I O T
//¡ö  ¡õ

//¡ö¡ö¡ö¡ö

//¡ö
//¡ö¡ö¡ö

//  ¡ö
//¡ö¡ö¡ö

//¡ö¡ö
//¡ö¡ö

// ¡ö¡ö
//¡ö¡ö

//¡ö¡ö
//  ¡ö¡ö

// ¡ö
//¡ö¡ö¡ö
//Color settings
//I:03  J:02    L:06    O:0E    S:0A    Z:0C    T:05
//I = (0,-1),(0,0),(0,1),(0,2)
//J = (-1,-1),(-1,0),(0,0),(1,0)
//L = (-1,0),(0,0),(1,0),(1,1)
//O = (0,0),(0,1),(1,0),(0,1)
//S = (-1,1),(0,1),(0,0),(0,1)
//Z = (-1,0),(0,0),(0,1),(1,1)
//T = (-1,0),(0,0),(0,1),(1,0)
void setColor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void clearScreen(){
    system("cls");
}
void setConsoleSize(int x, int y){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size = {x, y};
    SMALL_RECT rc = {0,0,size.X - 1,size.Y - 1};
    //hide cursor
    CONSOLE_CURSOR_INFO cur = {1, FALSE};
    SetConsoleWindowInfo(h, TRUE, &rc);
    SetConsoleScreenBufferSize(h, size);
    SetConsoleCursorInfo(h, &cur);

}
int* initCanvas(int width,int height){
    int canvas[height][width];
    for(int i = 0;i < height;i++){
        for(int j = 0;j<width;j++){
            canvas[i][j] = 0;
        }
    }
}
struct Tetromino* genShapes(int t){
    switch(t){
    case 0:
        int poses[4][2] = {(0,-1),(0,0),(0,1),(0,2)};
    case 1:
        int poses[4][2] = {(-1,-1),(-1,0),(0,0),(1,0)};
    case 2:
        int poses[4][2] = {(-1,0),(0,0),(1,0),(1,1)};
    case 3:
        int poses[4][2] = {(0,0),(0,1),(1,0),(0,1)};
    case 4:
        int poses[4][2] = {(-1,1),(0,1),(0,0),(0,1)};
    case 5:
        int poses[4][2] = {(-1,0),(0,0),(0,1),(1,1)};
    case 6:
        int poses[4][2] = {(-1,0),(0,0),(0,1),(1,0)};

    }
}
int main()
{
    int poses[4][2] = {{0,0},{},{},{}};
    free(poses);
    printf("%d,%d",poses[0][0],poses[0][1]);
    return 0;
}
