#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED
#endif // TETRIS_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

// define game element
#define up 1
#define right 2
#define down 3
#define left 4
#define DEFAULT_W 100
#define DEFAULT_H 40
//Shapes
#define S 0
#define Z 1
#define L 2
#define J 3
#define I 4
#define O 5
#define T 6
//function declare
void locate(COORD pos);
void setColor(int c);
void cleanScreen();
void placeShape();
void renderShape();
char shuffleShape();
int loadShapes();
int checkBottom();
int checkTop();
int* initCanvas();


//define Tetromino
//Shape type S Z L J I O T
struct pixel{
    COORD pos;
    struct pixel *next;
};
struct Tetromino{
    struct pixel *head;
    struct pixel *tail;
};



