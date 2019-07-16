#include "tetris.h"
//Shape test
//S Z L J I O T
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
int canvas[DEFAULT_W][DEFAULT_H];
//define struct of pixel Tetromino Map
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
typedef struct Canvas{
    int* canvas;
    int width;
    int height;

} Canvas;
//Initialize screen when game start
void initScreen(int size_x, int size_y, struct Canvas *m){

    initCanvas(size_x, size_y, m);
    //printMap(m);
    //setConsoleSize(size_x, size_y);
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
void initCanvas(int width,int height, struct Canvas *m){
    m -> width = width;
    m -> height = height;
    int *p = (int *)malloc(sizeof(int)*(height*width));
    for (int i = 0;i < (height*width);i++)
        *(p + i) = 0;
//    int canvas[m -> width][m -> height];
//    for(int i = 0;i < width;i++){
//        for(int j = 0; j<height; j++){
//            canvas[i][j] = 0;
//        }
//    }
    printf("sadasd");
    m -> canvas = p;
    //printMap(m);
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
void cShape(struct Tetromino* t){
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){
        COORD pos = posAdd(p -> pos, t -> world_pos);
        locate(pos.X, pos.Y);
        printf(" ");
    }
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
int getMap(int x,int y, struct Canvas *m){
    int w = m -> width;
    int *p = m -> canvas;
    return *(p + w*y + x);
}
void setMap(int x, int y, struct Canvas *m, int value){
    int w = m -> width;
    int *p = m -> canvas;
    *(p + w*y + x) = value;
    printf("\nInset: %d,%d\n",x,y);
}
int getKey(struct Tetromino *s){
    if (GetAsyncKeyState(VK_A) && (s -> world_pos.X + s ->left)){
        return 1;
    }
    if (GetAsyncKeyState(VK_D) && (s -> world_pos.X + s ->left)){
        return 2;
    }
    if (GetAsyncKeyState(VK_W)){
        //might over the border after rotation
        return 3;
    }
    if (GetAsyncKeyState(VK_S)){
        return 4;
    }
    return 0;
}
void fuckKey(int key, struct Tetromino* s, int *canvas){
    switch(key){
        case 1: s->world_pos.X -= 2;break;
        case 2: s->world_pos.X += 2;break;
        //case 3: trans(s);break;
        //case 4: drop(s, canvas);break;
        default: break;
    }
}
void drop(struct Tetromino *t, struct Canvas *m){
    struct pixel *p = t -> head;
    int h[4], w[4];
    int delta = 0;
    short flag = 1;
    for (int i = 0; i < 4; i++){
        COORD pos = posAdd(p -> pos, t -> world_pos);
        h[i] = pos.Y;
        w[i] = pos.X;
    }
    //printf("\n%d, %d\n",t->world_pos.X, t->world_pos.Y);
    while (flag == 1){
        if (delta >=40) {
            break;
        }
        for (int i = 0; i < 4; i++){
            h[i] ++;
            printf("%d", h[i]);
            if ((getMap(w[i], h[i], m) != 0) || (h[i] > 35)){
                flag = 0;
                break;
            }
        }
        if(flag != 0)
            delta++;
    }
    //cShape(t);
    t -> world_pos.Y += delta;

    //rShape(t);
    for (;p != NULL;p = p -> next){
        COORD pos = posAdd(p -> pos, t -> world_pos);
        //printf("%d,%d\n",pos.X,pos.Y);
        setMap(pos.X, pos.Y, m, t -> color);
    }
}
void printMap(struct Canvas *m){
    int *canvas = m -> canvas;
    int w = m -> width;
    int h = m -> height;
    for (int i = 0; i < h; i++){
        for(int j = 0;j < w; j ++){
            printf("%d",getMap(j, i, m));
        }
        printf("\n");
    }
}
int checkBottom(struct Canvas* m){

    int w = m -> width;
    int h = m -> height;
    int flag = 1;
    int score = 0;
    for (int i = h - 1; i >= 0; i++){
        for(int j = 0;j < w; j += 2){
            if (getMap(j, i, m) == 0){
                flag = 0;
                break;
            }
        }
        if (flag == 0) break;
    }
}
int main(){
    struct Canvas *map = (struct Canvas*)malloc(sizeof(struct Canvas));
    //system("chcp 65001");
    //clearScreen();
    initScreen(80, 40, map);
    COORD pos = {50,0};
    struct Tetromino *t = genShapes(1, pos);
    //printMap(map);
    printf("Sadsadas");
    drop(t, map);
    printf("\n");
    //for (int j = 0; j < 40;j++){
    //for(int i = 0;i<map->width;i++)
      //  printf("%d",*(map->canvas + 80*j + i));printf("\n");}
    printMap(map);
    printf("\n%d %d\n",map->width,map->height);
    free(map);
    free(t);
//    COORD spawnPoint = {50,0};
//    int delay = 300;
//    int bottomLine = 35;
    system("pause");
    return 0;
}
