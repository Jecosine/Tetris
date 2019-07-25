#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED
#endif // TETRIS_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <math.h>
// define game element
#define PI 3.1415
#define upward 1
#define rightward 2
#define downward 3
#define leftward 4
#define DEFAULT_W 100
#define DEFAULT_H 40
#define MAP_W 40
#define MAP_H 25
//Shapes
#define S 0
#define Z 1
#define L 2
#define J 3
#define I 4
#define O 5
#define T 6
//define keys
#define VK_A 0x41
#define VK_D 0x44
#define VK_W 0x57
#define VK_S 0x53
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1b
//function declare
void locate(int x, int y);
void setColor(int c);
void cleanScreen();
//void rShape(struct Tetromino* t);
void rotateShape(struct Tetromino* t);
void setConsoleSize(int x, int y);
//void drop(struct Tetromino *t);
char shuffleShape();
//struct Tetromino* genShapes(int t, COORD pos);
//int checkBottom();
//int checkTop();




