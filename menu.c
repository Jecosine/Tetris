#include "tetris.h"

typedef struct Item {
    int number;
    int color;
    int length;
    char* content;
    COORD pos;
} Item;

typedef struct Menu {
    struct Item *group;
    struct Item *current;
    int selected;
} Menu;

struct Menu* initMenu(){
    struct Menu *menu = (struct Menu*)malloc(sizeof(struct Menu));
    struct Item *items = (struct Item*)malloc(sizeof(struct Item)*4);
    struct Item *item = items;
    COORD pos;
    setPos(&pos, 40, 16);
    initItem(item++, 0, 1, 5, "Start", pos);
    setPos(&pos, 40, 18);
    initItem(item++, 1, 1, 4, "Help", pos);
    setPos(&pos, 40, 20);
    initItem(item++, 2, 1, 7, "Join Us", pos);
    setPos(&pos, 40, 22);
    initItem(item, 3, 1, 4, "Quit", pos);
    menu -> group = items;
    menu -> current = items;
    menu -> selected = -1;
    return menu;
}
void initItem(struct Item *item, int num, int color, int length, char * content, COORD pos){
    item -> number = num;
    item -> color = color;
    item -> length = length;
    item -> content = content;
    item -> pos = pos;
}
void setPos(COORD *pos, int x, int y){
    pos -> X = x;
    pos -> Y = y;
}
void dealKey(struct Menu *menu){
    struct Item *item = menu -> current;
    struct Item *items = menu -> group;
    if (GetAsyncKeyState(VK_W)){
        if (item -> number == 0)
        {
            normalItem(item);
            menu -> current = items + 3;
            highlightItem(menu -> current);
        }
        else{
            normalItem(item);
            menu -> current = items + (item -> number - 1);
            highlightItem(menu -> current);
        }
        //might over the border after rotation
    }
    if (GetAsyncKeyState(VK_S)){
        if (item -> number == 3)
        {
            normalItem(item);
            menu -> current = items;
            highlightItem(menu -> current);
        }
        else{
            normalItem(item);
            menu -> current = items + (item -> number + 1);
            highlightItem(menu -> current);
        }
    }
    if (GetAsyncKeyState(VK_RETURN)){
        menu -> selected = item -> number;
    }
    if (GetAsyncKeyState(VK_ESCAPE)){
        menu -> selected = 999;
    }
}
void highlightItem(struct Item *item){
    locate(item -> pos.X, item -> pos.Y);
    setColor(112);
    printf(item -> content);
}
void normalItem(struct Item *item){
    locate(item -> pos.X, item -> pos.Y);
    setColor(7);
    printf(item -> content);
}
int renderMenu(struct Menu *menu){
    struct Item *items = menu -> group;
    int scene = -1;
    system("chcp 65001");
    setConsoleSize(DEFAULT_W,DEFAULT_H);
    clearScreen();
    char c;
    for (int i = 0;i < 4; i++){
        normalItem(items + i);
    }
    while(scene == -1){
        if(kbhit()){
            c = getch();
            dealKey(menu);
        }
        scene = menu -> selected;
        Sleep(400);
    }
    //printf("%d",scene);
    return scene;
}

