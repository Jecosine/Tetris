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
struct Tetromino* randShape(COORD pos);
struct Tetromino* genShapes(int t, COORD pos);
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
    int score;
    COORD mapPos;
    struct Tetromino *current;
    struct Tetromino *next;

} Canvas;
//Initialize screen when game start
void initScreen(int size_x, int size_y, struct Canvas *m){

    initCanvas(MAP_W, MAP_H, m);
    //printMap(m);
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
    SMALL_RECT rc = {0, 0, size.X - 1, size.Y - 1};
    //hide cursor
    CONSOLE_CURSOR_INFO cur = {1, FALSE};
    SetConsoleWindowInfo(h, TRUE, &rc);
    SetConsoleScreenBufferSize(h, size);
    SetConsoleCursorInfo(h, &cur);
}
void initCanvas(int width,int height, struct Canvas *m){
    //init members of map
    m -> width = width;
    m -> height = height;
    COORD pos;
    COORD mapPos;
    pos.X = m -> width/2;
    pos.Y = 0;
    //generate current and next tetromino
    m -> current = randShape(pos);
    m -> next = randShape(pos);
    int *p = (int *)malloc(sizeof(int)*(height*width));
    for (int i = 0;i < (height*width);i++)
        *(p + i) = 0;
    m -> canvas = p;
    mapPos.X = (DEFAULT_W - MAP_W) / 2;
    mapPos.Y = (DEFAULT_H - MAP_H) / 2;
    m -> mapPos = mapPos;
    //printMap(m);
}
void printScene(struct Canvas *m){
    for(int i = m -> mapPos.Y; i <= m -> mapPos.Y + m -> height; i++){
        for(int j = m -> mapPos.X; j <= m -> mapPos.X + m -> width;j++){
            if((i == m -> mapPos.Y) || (i == (m -> mapPos.Y + m -> height))){
                if((j == m -> mapPos.X)||(j == (m -> mapPos.X + m -> width))){
                    locate(j, i);
                    setColor(7);
                    printf("|");
                }
                else{
                    locate(j, i);
                    setColor(7);
                    printf("=");
                }
            }

        }
    }
}
struct Tetromino* randShape(COORD pos){
    int x = rand() % 7;
    struct Tetromino *t = genShapes(x, pos);
    return t;
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
        int right = -99;
        int left = 99;
        for(int i = 0; i < 4; i++){
            struct pixel *next = (struct pixel*)malloc(sizeof(pixel));
            COORD localpos;
            localpos.X = *(ps + 2*i);
            localpos.Y = *(ps + 2*i + 1);
            if(localpos.X < left)
                left = localpos.X;
            if(localpos.X > right)
                right = localpos.X;
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
        s -> left = left;
        s -> right = right;
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
COORD mapPosAdd(COORD a, COORD m){
    COORD c;
    c.X = a.X + m.X;
    c.Y = a.Y + m.Y;
    return c;
}
void cShape(struct Canvas *m){
    struct Tetromino *t = m -> current;
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){
        COORD pos = mapPosAdd(posAdd(p -> pos, t -> world_pos), m -> mapPos);
        locate(pos.X, pos.Y);
        printf(" ");
    }
}
void rShape(struct Canvas *m){
    struct Tetromino *t = m -> current;
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){
        COORD pos = mapPosAdd(posAdd(p -> pos, t -> world_pos), m -> mapPos);
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
    //printf("\nInset: %d,%d\n",x,y);
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
    if (GetAsyncKeyState(VK_ESCAPE)){
        return 5;
    }
    return 0;
}
void fuckKey(int key, struct Canvas *m){
    struct Tetromino *s = m -> current;
    switch(key){
        case 1: cShape(m);s->world_pos.X -= 2;rShape(m);break;
        case 2: cShape(m);s->world_pos.X += 2;rShape(m);break;
        //case 3: trans(s);break;
        case 4: drop(m);break;
        default: break;
    }
}
void falling(struct Canvas *m){
    struct Tetromino *s = m -> current;
    struct pixel *p = s -> head;
    COORD opos = {m -> width / 2,0};
    int h[4],w[4];
    short flag = 1;
    for (int i = 0; i < 4; i++){
        COORD pos = posAdd(p -> pos, s -> world_pos);
        h[i] = pos.Y + 1;
        w[i] = pos.X;
        p = p->next;
    }
    for (int i = 0;i < 4; i++){
        //printf("in drop %d -> %d\n",i, h[i]);
        if ((getMap(w[i], h[i], m) != 0) || (h[i] > 29)){
            flag = 0;
            break;
        }
    }
    if (flag == 0){
        for (p = s -> head;p != NULL;p = p -> next){
            COORD pos = posAdd(p -> pos, s -> world_pos);
            //printf("%d,%d\n",pos.X,pos.Y);
            setMap(pos.X, pos.Y, m, s -> color);
        }
        if(checkTop(m) == 0){
            gameOver();
        }
        else{
            if (checkBottom(m) != 0)
                refreshScore(m -> score);
            free(m -> current);
            m -> current = m -> next;
            m -> next = randShape(opos);
        }

    }
    else{
        cShape(m);
        s -> world_pos.Y ++;
        rShape(m);
    }
}

void drop(struct Canvas *m){
    struct Tetromino *t = m -> current;
    cShape(m);
    COORD opos = {m -> width / 2, 0};
    struct pixel *p = t -> head;
    int h[4], w[4];
    int delta = 0;
    int score = 0;
    short flag = 1;
    for (int i = 0; i < 4; i++){
        COORD pos = posAdd(p -> pos, t -> world_pos);
        h[i] = pos.Y;
        w[i] = pos.X;
        p = p->next;
    }
    //printf("\n%d, %d\n",t->world_pos.X, t->world_pos.Y);
    while (flag == 1){
        if (delta >=40) {
            break;
        }
        for (int i = 0; i < 4; i++){
            h[i] ++;
            //printf("in drop %d -> %d\n",i, h[i]);
            if ((getMap(w[i], h[i], m) != 0) || (h[i] > 29)){
                flag = 0;
                break;
            }
        }
        if(flag != 0)
            delta++;
    }
    //printf("%d",delta);
    //cShape(t);
    t -> world_pos.Y += delta;
    //rShape(t);
    COORD pos;
    for (p = t -> head;p != NULL;p = p -> next){
        pos = posAdd(p -> pos, t -> world_pos);
        //printf("%d,%d\n",pos.X,pos.Y);
        setMap(pos.X, pos.Y, m, t -> color);
        pos = mapPosAdd(posAdd(p -> pos, t -> world_pos), m -> mapPos);
        locate(pos.X,pos.Y);
        setColor(t -> color);
        printf("■");
    }
    if(checkTop(m) == 0){
        gameOver();
    }
    else{
        if (checkBottom(m) != 0)
            refreshScore(m -> score);
        free(m -> current);
        m -> current = m -> next;
        m -> next = randShape(opos);
    }
}
void gameOver(struct Canvas *m){
    printf("====================================================================================================");
    clearScreen();
    initScreen(DEFAULT_W,DEFAULT_H,m);
}
void printMapData(struct Canvas *m){
    int *canvas = m -> canvas;
    int w = m -> width;
    int h = m -> height;
    for (int i = 0; i < h; i++){
        for(int j = 0;j < w; j ++){
            printf("%d",getMap(j, i, m));
        }
        printf("\n");
    }
    //printf("%d   %d\n",w,h);
}
void printMap(struct Canvas *m){
    int *canvas = m -> canvas;
    int w = m -> width;
    int h = m -> height;
    int c;
    for (int i = 0; i < h; i++){
        for(int j = 0;j < w; j += 2){
            c = getMap(j, i, m);
            if(c != 0){
                setColor(c);
                locate(j, i);
                printf("■");
            }
        }
        printf("\n");
    }
}
int checkBottom(struct Canvas* m){
    int w = m -> width;
    int h = m -> height;
    int flag = 1;
    int score = 0;
    for (int i = h - 6; i >= 0; i--){
        for(int j = 0;j < w; j += 2){
            if (getMap(j, i, m) == 0){
                flag = 0;
                break;
            }

            //printf("%d ",i);
        }
        if (flag == 0) break;
        else score++;
    }
    if(score == 0)
        return 0;
    m -> score += score;
    //sinkCanva(score, m);
    return 1;
}
void refreshScore(int score){
    locate(0, 39);
    printf("%d",score);
}
int checkTop(struct Canvas *m){
    int w = m -> width;
    for (int i = 0;i < w;i++){
        if(getMap(i, 0, m) != 0){
            return 0;
        }
    }
    return 1;
}

void sinkCanva(int n, struct Canvas *m){
    int w = m -> width;
    int h = m -> height;
    clearScreen();
    //printf("%d",n);
    for(int i = h-n-5;i >= 0;i--){
        //printf("%d to %d\n",i,i+n);
        for(int j = 0;j < w;j++){
            setMap(j, i+n, m, getMap(j, i, m));
        }
    }
    for(int i = 0;i < n;i++){
        for(int j = 0;j < w;j++){
            setMap(j, i, m, 0);
        }
    }
    //dprintMap(m);
}
void startGame(struct Canvas *map){
    char c;
    int delay = 600;
    int bottomLine = 35;
    int key = 0;

    setColor(7);
    clearScreen();
    printScene(map);

    while(key != 5){
        falling(map);
        if(kbhit()){
            c = getch();
            key = getKey(map -> current);
            if(key != 0)
                fuckKey(key,  map);
        }
        //clearScreen();
        //printMapData(map);
        Sleep(delay);
    }
}
void switchScene(int scene, struct Canvas *map){
    switch(scene){
    case 0:startGame(map);break;
    case 1:break;
    default:break;
    }
}
int main(){
    time_t t = time(0);
    srand(t);
    COORD pos = {50,0};
//    for(int i =0 ;i < map->height;i++)
//    {
//        locate(0,i);
//        printf("%d",i);
//    }

//    FILE *fp = stdin;
//    fclose(fp);
    //render menu
    int scene = 0;
    struct Menu *menu = initMenu();
    struct Canvas *map = (struct Canvas*)malloc(sizeof(struct Canvas));
    system("chcp 65001");
    clearScreen();
    initScreen(100, 40, map);
    while(scene != 3){
        scene = renderMenu(menu);
        switchScene(scene, map);
    }
//    for(int i = 0;i<40;i++){
//        setColor(i+80);
//        printf("%d -> c\n",i);
//    }

//    struct Tetromino *s = genShapes(0, pos);
//    map -> current = s;
//    drop(map);
    return 0;
}
