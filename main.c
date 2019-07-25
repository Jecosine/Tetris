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
//local position of each pixel for every tetromino
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
//rotation Matrix ,multi on right side
static int rMatrix[2][2] = {{0,-1},
                            {1,0}};
struct Tetromino* randShape(COORD pos);
struct Tetromino* genShapes(int t, COORD pos);
//Color settings(shape:color)
//I:03  J:02    L:06    O:0E    S:0A    Z:0C    T:05
static int colorList[7] = {3, 2, 6, 14, 10, 12, 5};
int canvas[DEFAULT_W][DEFAULT_H];
//define struct of pixel Tetromino Map
typedef struct pixel{
    COORD pos;
    struct pixel *next;
} pixel;
//define tetromino
typedef struct Tetromino{
    COORD world_pos;
    int id;
    int color;
    int left;
    int right;
    int degree;
    struct pixel *head;
    struct pixel *center;
    struct pixel *tail;
} Tetromino;
//define canvas
typedef struct Canvas{
    int* canvas;
    int width;
    int height;
    int score;
    short gg;
    COORD mapPos;
    struct Tetromino *current;
    struct Tetromino *next;
    struct Tetromino *preview;
    struct Menu* menu;
    struct Menu* ggp;
    struct Menu* help;
} Canvas;
//Initialize screen when running
void initScreen(int size_x, int size_y, struct Canvas *m){
    //initialize canvas with MAP_W and MAP_W
    initCanvas(MAP_W, MAP_H, m);
    //set cursor hidden;set console size in size_x, size_y
    setConsoleSize(size_x, size_y);
}
//set color of next text
//white : 07; black : 01;
void setColor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
//clear screen
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
    //hide cursor
    SetConsoleCursorInfo(h, &cur);
}
//Initialize Canvas
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
    m -> preview = genShapes(m -> next -> id, pos);
    int *p = (int *)malloc(sizeof(int)*(height*width));
    for (int i = 0;i < (height*width);i++)
        *(p + i) = 0;
    m -> canvas = p;
    mapPos.X = (DEFAULT_W - MAP_W) / 2;
    mapPos.Y = (DEFAULT_H - MAP_H) / 2;
    m -> mapPos = mapPos;
    m -> gg = 0;
    m -> score = 0;
    //printMap(m);
}
//do multiply for matrix
void matrixMulti(COORD* a, int degree){
    int a1 = a -> X;
    int a2 = a -> Y;
    a -> X = -a2;
    a -> Y = a1;
}
//rotate tetromino
void tShape(struct Canvas *m){
    cShape(m);
    struct Tetromino *t = m -> current;
    struct pixel *p = t -> head;
    int left = 999;
    int right = -999;
    for(;p != NULL; p = p -> next){
        matrixMulti(&(p -> pos),t -> degree + 90);
        //printf("%d, %d", p -> pos.X, p -> pos.Y);
        if(p -> pos.X > right)
            right = p -> pos.X;
        if(p -> pos.X < left)
            left = p -> pos.X;
    }
    t -> left = left;
    t -> right = right;
    rShape(m);
}
//print preview
void printNext(struct Canvas *m){
    cShape_origin(m -> preview);
    free(m -> preview);
    COORD pos = {m -> mapPos.X + m -> width + 12, m -> mapPos.Y + 7};
    m -> preview = genShapes(m -> next -> id, pos);
    rShape_origin(m -> preview);

}
//print border and score
void printScene(struct Canvas *m){
    for(int i = m -> mapPos.Y-2; i <= m -> mapPos.Y + m -> height; i++){
        for(int j = m -> mapPos.X-1; j <= m -> mapPos.X + m -> width;j++){
            if((i == m -> mapPos.Y - 2) || (i == (m -> mapPos.Y + m -> height))){
                locate(j, i);
                setColor(7);
                printf("=");
            }
            if((j == m -> mapPos.X-1)||(j == (m -> mapPos.X + m -> width))){
                locate(j, i);
                setColor(7);
                printf("|");
            }

        }
    }
    locate(m -> mapPos.X + m -> width + 6, m -> mapPos.Y);
    printf("Score: %d", m -> score);
    //print preview
    //rect
    locate(m -> mapPos.X + m -> width + 6, m -> mapPos.Y + 2);
    printf("Next Tetromino:");
    //clean and print border
    for (int i = 0; i <= 12; i++){
        for(int j = 0; j <= 6; j++){
            locate(i + m -> mapPos.X + m -> width + 6, j + m -> mapPos.Y + 4);
            if((i == 0)||(i == 12)){
                printf("|");
                continue;
            }
            if((j == 0)||(j == 6)){
                printf("-");
                continue;
            }
            printf(" ");
        }
    }

}
//get random shape
struct Tetromino* randShape(COORD pos){
    int x = rand() % 7;
    struct Tetromino *t = genShapes(x, pos);
    return t;
}
//get an exact type of shape
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
        s -> id = t;
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
        s -> degree = 0;
        s -> left = left;
        s -> right = right;
        return s;
    }
    else{
        return NULL;
    }
}
//set cursor to x, y
void locate(int x, int y){
    COORD pos = {x , y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//local pos + world pos
COORD posAdd(COORD a, COORD b){
    COORD c;
    c.X = 2 * a.X + b.X;
    c.Y = a.Y + b.Y;
    return c;
}
//add in map
COORD mapPosAdd(COORD a, COORD m){
    COORD c;
    c.X = a.X + m.X;
    c.Y = a.Y + m.Y;
    return c;
}
//clear current tetromino's pixels
void cShape(struct Canvas *m){
    struct Tetromino *t = m -> current;
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){
        COORD pos = mapPosAdd(posAdd(p -> pos, t -> world_pos), m -> mapPos);
        locate(pos.X, pos.Y);
        printf(" ");
    }
}
//clear the tetromino's pixels
void cShape_origin(struct Tetromino *t){
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){
        COORD pos = posAdd(p -> pos, t -> world_pos);
        locate(pos.X, pos.Y);
        printf(" ");
    }
}
//render the shape of current tetromino
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
//render the shape of given tetromino
void rShape_origin(struct Tetromino *t){
    struct pixel *p = t -> head;
    for(;p != NULL; p = p -> next){
        COORD pos = posAdd(p -> pos, t -> world_pos);
        //printf("%d ,%d\n",pos.X,pos.Y);
        locate(pos.X, pos.Y);
        setColor(t -> color);
        printf("■");
    }
}
//get map data from map pointer
int getMap(int x,int y, struct Canvas *m){
    int w = m -> width;
    int *p = m -> canvas;
    return *(p + w*y + x);
}
//set map data through map pointer
void setMap(int x, int y, struct Canvas *m, int value){
    int w = m -> width;
    int *p = m -> canvas;
    *(p + w*y + x) = value;
    //printf("\nInset: %d,%d\n",x,y);
}
//get keyboard input and avoid overflow input
int getKey(struct Tetromino *s){
    if ((GetAsyncKeyState(VK_A) || GetAsyncKeyState(VK_LEFT)) && (s -> world_pos.X + s ->left)){
        return 1;
    }
    if ((GetAsyncKeyState(VK_D) || GetAsyncKeyState(VK_RIGHT)) && (s -> world_pos.X + s ->right)){
        return 2;
    }
    if (GetAsyncKeyState(VK_W) || GetAsyncKeyState(VK_UP)){
        //might over the border after rotation
        return 3;
    }
    if (GetAsyncKeyState(VK_S) || GetAsyncKeyState(VK_DOWN)){
        return 4;
    }
    if (GetAsyncKeyState(VK_ESCAPE)){
        return 5;
    }
    return 0;
}
//handle pressed key
void handleKey(int key, struct Canvas *m){
    struct Tetromino *s = m -> current;
    switch(key){
        case 1: if(s->world_pos.X + s -> left*2 - 2 >=  0){cShape(m);s->world_pos.X -= 2;rShape(m);}break;
        case 2: if(s->world_pos.X + s -> right*2+ 2 < m -> width){cShape(m);s->world_pos.X += 2;rShape(m);}break;
        case 3: tShape(m);break;
        case 4: drop(m);break;
        default: break;
    }
}
//fall a unit every frame, judge whether collided into other pixel
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
        if ((getMap(w[i], h[i], m) != 0) || (h[i] > m -> height - 1)){
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
            m -> gg = 1;
        }
        else{
            if (checkBottom(m) != 0)
                printScene(m);
            free(m -> current);
            m -> current = m -> next;
            m -> next = randShape(opos);
            printNext(m);
        }

    }
    else{
        cShape(m);
        s -> world_pos.Y ++;
        rShape(m);
    }
}
//drop to the deepest immediately
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
            if ((getMap(w[i], h[i], m) != 0) || (h[i] > m -> height - 1)){
                flag = 0;
                break;
            }
        }
        if(flag != 0)
            delta++;
    }
    t -> world_pos.Y += delta;
    //rShape(t);
    COORD pos;
    //printf("left: %d;right: %d", m -> current -> left, m -> current -> right);
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
        m -> gg = 1;
    }
    else{
        checkBottom(m);

        free(m -> current);
        m -> current = m -> next;
        m -> next = randShape(opos);


        printNext(m);

    }
}
//void gameOver(struct Canvas *m){
//    clearScreen();
//    initScreen(DEFAULT_W,DEFAULT_H,m);
//}
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
}
//Draw map according to mapdata
void printMap(struct Canvas *m){
    int *canvas = m -> canvas;
    int w = m -> width;
    int h = m -> height;
    int c;
    for (int i = 0; i < h; i++){
        for(int j = 0;j < w; j += 2){
            c = getMap(j, i, m);
            setColor(c);
            locate(j + m -> mapPos.X, i + m -> mapPos.Y);
            if(c != 0) printf("■");
            else printf(" ");
        }
        printf("\n");
    }
}
//check if getting point
int checkBottom(struct Canvas* m){
    int w = m -> width;
    int h = m -> height;
    int flag = 1;
    int score = 0;
    for (int i = h - 1; i >= 0; i--){
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
    sinkCanva(score, m);
    return 1;
}
void refreshScore(int score){
    locate(50, 7);
    printf("Score: %d",score);
}
//check the top to decide game over or not
int checkTop(struct Canvas *m){
    int w = m -> width;
    for (int i = 0;i < w;i++){
        if(getMap(i, 0, m) != 0){
            return 0;
        }
    }
    return 1;
}
//sink the canva if got point
void sinkCanva(int n, struct Canvas *m){
    int w = m -> width;
    int h = m -> height;
    printf("fuck!!!!");
    //printf("%d",n);
    for(int i = h-n;i >= 0;i--){
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
    printScene(m);
    printMap(m);

}
//Start game loop
int startGame(struct Canvas *map){
    char c;
    int delay = 400;
    int bottomLine = 35;
    int key = 0;
    setColor(7);
    system("chcp 65001");
    clearScreen();
    initScreen(100, 40, map);
    printScene(map);
    printNext(map);


    while(key != 5 && map -> gg == 0){
        falling(map);
        if(kbhit()){
            c = getch();
            key = getKey(map -> current);
            if(key != 0)
                handleKey(key,  map);
        }
        //clearScreen();
        //printMapData(map);
        Sleep(delay);
    }

    return renderGG(map -> ggp);
}
//Deal with scene change
int switchScene(int scene, struct Canvas *map){
    int s = 3;
    switch(scene){
    case 0:s = startGame(map);break;
    case 1:s = renderHelp(map -> help);break;
    case 2:s = renderMenu(map -> menu);break;
    case 3:break;
    case 4:s = renderGG(map -> ggp);break;
    default:break;
    }
    return s;
}
int main(){
    time_t t = time(0);
    srand(t);
    COORD pos = {50,0};
//    FILE *fp = stdin;
//    fclose(fp);
    //render menu
    int scene = 2;
    struct Menu *menu = initMenu();
    struct Menu *gg = initGG();
    struct Menu *help = initHelp();

    struct Canvas *map = (struct Canvas*)malloc(sizeof(struct Canvas));
    map -> menu = menu;
    map -> ggp = gg;
    map -> help = help;

    setColor(7);
    while(scene != 3){
        //scene = renderMenu(menu);
        scene = switchScene(scene, map);
    }
    free(map);
    free(menu);

    return 0;
}
