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
    {{0,0},{0,1},{1,0},{1,1}},
    {{-1,0},{0,0},{0,-1},{1,-1}},
    {{-1,0},{0,0},{0,1},{1,1}},
    {{-1,0},{0,0},{0,1},{1,0}}
};
//Color settings
//I:03  J:02    L:06    O:0E    S:0A    Z:0C    T:05
static int colorList[7] = {3, 2, 6, 14, 10, 12, 5};
typedef struct pixel{
    COORD pos;
    struct pixel *next;
} pixel;
typedef struct Tetromino{
    COORD world_pos;
    int color;
    int left;
    int right;
    struct pixel *head;
    struct pixel *center;
    struct pixel *tail;
} Tetromino;
void initScreen(int size_x, int size_y){
    setConsoleSize(size_x, size_y);
}
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
        s -> color = colorList[t];
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
            //printf("%d, %d\n", p -> pos.X, p->pos.Y);
            if (s -> head == NULL){
                s -> head = p;
            }
            if ((localpos.X == 0) && (localpos.Y == 0))
                s -> center = p;
            if (i == 3){
                p -> next = NULL;
                break;
            }
            p -> next = next;
            p = p -> next;

        }
        return s;
    }
    else{
        return NULL;
    }

}
void locate(int x, int y){
    COORD pos = {x , y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//local + world
COORD posAdd(COORD a, COORD b){
    COORD c;
    c.X = 2 * a.X + b.X;
    c.Y = a.Y + b.Y;
    return c;
}
void rShape(struct Tetromino* t){
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){

        COORD pos = posAdd(p -> pos, t -> world_pos);
        //printf("%d ,%d\n",pos.X,pos.Y);
        locate(pos.X, pos.Y);
        setColor(t -> color);
        printf("■");
    }

}
int main()
{

    system("chcp 65001");
    clearScreen();
    initScreen(100, 40);

    for (int i = 6; i >= 0;i--){
        COORD pos = {50, 5 + 5 * i};
        struct Tetromino *s = genShapes(i,pos);

        rShape(s);
        free(s);
    }

    //locate(50,20);
    //printf("aaa");

    return 0;
}
