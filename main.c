#include "tetris.h"

//Shape test
//S Z L J I O T
//��  ��
//■

//��������

//��
//������

//  ��
//������

//����
//����

// ����
//����

//����
//  ����

// ��
//������
//Color settings
//I:03  J:02    L:06    O:0E    S:0A    Z:0C    T:05
//I = (0,-1),(0,0),(0,1),(0,2)
//J = (-1,-1),(-1,0),(0,0),(1,0)
//L = (-1,0),(0,0),(1,0),(1,1)
//O = (0,0),(0,1),(1,0),(0,1)
//S = (-1,1),(0,1),(0,0),(0,1)
//Z = (-1,0),(0,0),(0,1),(1,1)
//T = (-1,0),(0,0),(0,1),(1,0)
//define Tetromino
//Shape type S Z L J I O T
static int shapeList[7][4][2] =
{
    {{0,-1},{0,0},{0,1},{0,2}},
    {{-1,-1},{-1,0},{0,0},{1,0}},
    {{-1,0},{0,0},{1,0},{1,1}},
    {{0,0},{0,1},{1,0},{0,1}},
    {{-1,1},{0,1},{0,0},{0,1}},
    {{-1,0},{0,0},{0,1},{1,1}},
    {{-1,0},{0,0},{0,1},{1,0}}
};
typedef struct pixel{
    COORD pos;
    struct pixel *next;
} pixel;
typedef struct Tetromino{
    COORD world_pos;
    int left;
    int right;
    struct pixel *head;
    struct pixel *center;
    struct pixel *tail;
} Tetromino;



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
struct Tetromino* genShapes(int t, COORD pos){
    int *ps;
    if((t >= 0) && (t < 7)){
        ps = shapeList[t];
        struct Tetromino* s = (struct Tetromino*)malloc(sizeof(Tetromino));
        //init tetromino
        s -> head = NULL;
        s -> center = NULL;
        s -> tail = NULL;
        s -> world_pos = pos;
        struct pixel *p = (struct pixel*)malloc(sizeof(pixel));
        int right = 0;
        int left = 0;
        for(int i = 0; i < 4; i++){
            struct pixel *next = (struct pixel*)malloc(sizeof(pixel));
            COORD localpos;
            localpos.X = *(ps + 2*i);
            localpos.Y = *(ps + 2*i + 1);
            p -> pos = localpos;
            printf("%d, %d\n", p -> pos.X, p->pos.Y);
            if (s -> head == NULL){
                s -> head = p;
            }
            if ((localpos.X == 0) && (localpos.Y == 0))
                s -> center = p;

            p -> next = next;
            p = p -> next;


        }
        return s;
    }
    else
        return NULL;

}
int main()
{
    COORD pos = {1,1};
    int *p = shapeList[1];
    struct Tetromino *s = genShapes(1,pos);
    for (int i = 0;i < 8; i++){

            printf("%d -> %d\n",i,  *(p + i));
    }
    return 0;
}
