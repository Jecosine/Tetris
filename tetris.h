#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED
#endif // TETRIS_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <random>
// define game element
#define up 1
#define right 2
#define down 3
#define left 4
#define DEFAULT_W 100
#define DEFAULT_H 40
//function declare
void locate(COORD pos);
void color(int c);
void cleanScreen();
char shuffleShape();
int loadShapes();
int checkBottom();
int checkTop();
int[][] initCanvas();


//define Tetromino
//Shape type S Z L J I O T
struct Tetromino{
    pixel *head;
};
struct pixel{
    COORD pos;
    pixel *next;
};


