#include "tetris.h"
//Item which can be selected
typedef struct Item {
    int number;
    int color;
    int length;
    char* content;
    COORD pos;
} Item;
//Menu which contain items
typedef struct Menu {
    struct Item *group;
    struct Item *current;
    int count;
    int selected;
} Menu;
//init main menu and its items
struct Menu* initMenu(){
    struct Menu *menu = (struct Menu*)malloc(sizeof(struct Menu));
    struct Item *items = (struct Item*)malloc(sizeof(struct Item)*4);
    struct Item *item = items;
    COORD pos;
    setPos(&pos, 45, 20);
    initItem(item++, 0, 1, 9, "  Start  ", pos);
    setPos(&pos, 45, 22);
    initItem(item++, 1, 1, 8, "  Help  ", pos);
    setPos(&pos, 45, 24);
    initItem(item++, 2, 1, 11, "  Join Us  ", pos);
    setPos(&pos, 45, 26);
    initItem(item, 3, 1, 8, "  Quit  ", pos);
    menu -> group = items;
    menu -> current = items;
    menu -> count = 4;
    menu -> selected = -1;
    return menu;
}
//init an item with its number color length content and position
void initItem(struct Item *item, int num, int color, int length, char * content, COORD pos){
    item -> number = num;
    item -> color = color;
    item -> length = length;
    item -> content = content;
    item -> pos = pos;
}
//set position data for a COORD
void setPos(COORD *pos, int x, int y){
    pos -> X = x;
    pos -> Y = y;
}
//deal with the input when selecting item
void dealKey(struct Menu *menu){
    struct Item *item = menu -> current;
    struct Item *items = menu -> group;
    if (GetAsyncKeyState(VK_W) || GetAsyncKeyState(VK_UP)){
        if (item -> number == 0)
        {
            //went bottom if you want to get upper on the top
            normalItem(item);
            menu -> current = items + menu -> count - 1;
            highlightItem(menu -> current);
        }
        else{
            // went top if you want to get lower at bottom
            normalItem(item);
            menu -> current = items + (item -> number - 1);
            highlightItem(menu -> current);
        }
    }
    if (GetAsyncKeyState(VK_S) || GetAsyncKeyState(VK_DOWN)){
        if (item -> number == menu->count - 1)
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
//make item highlight
void highlightItem(struct Item *item){
    locate(item -> pos.X, item -> pos.Y);
    setColor(112);
    printf(item -> content);
}
//reset item
void normalItem(struct Item *item){
    locate(item -> pos.X, item -> pos.Y);
    setColor(7);
    printf(item -> content);
}
//render Menu (including canvas items paintings...)
int renderMenu(struct Menu *menu){
    setColor(7);
    clearScreen();

    struct Item *items = menu -> group;
    int scene = -1;
    //system("chcp 65001");
    setConsoleSize(DEFAULT_W,DEFAULT_H);
    //clearScreen();
    menu -> selected = -1;
    FILE *fp;
    int co;
    int *canva = (int *)malloc(sizeof(int)*DEFAULT_H*DEFAULT_W);
    int *p = canva;
    if ((fp = fopen("res/menu_page.txt","r")) == NULL){
        printf("ERROR:Cannot find canva file");
    }
    while(!feof(fp)){
        fscanf(fp,"%d ",&co);
        *p = co;
        //printf("%d ",co);
        p++;
    }
    fclose(fp);
    for(int i = 0; i < DEFAULT_W; i++){
        for(int j = 0; j < DEFAULT_H; j++){
            if(*(canva + j*DEFAULT_W + i) != 0){
                locate(i, j);
                setColor(105 + *(canva + j*DEFAULT_W + i));
                printf(" ");
            }
        }
    }
    //tip
    setColor(7);
    locate (20, 37);
    printf("Use UpArrow and DownArrow to select, press ENTER to confirm");
    free(canva);
    char c;
    for (int i = 0;i < 4; i++){
        normalItem(items + i);
    }
    highlightItem(menu -> current);
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
//init help panel
struct Menu* initHelp(){
    struct Menu *menu = (struct Menu*)malloc(sizeof(struct Menu));
    struct Item *items = (struct Item*)malloc(sizeof(struct Item)*2);
    struct Item *item = items;
    COORD pos;
    setPos(&pos, 30, 30);
    initItem(item++, 0, 1, 9, "  Start  ", pos);
    setPos(&pos, 50, 30);
    initItem(item++, 1, 1, 8, "  Back  ", pos);
    menu -> group = items;
    menu -> current = items;
    menu -> selected = -1;
    return menu;
}
//init game over menu
struct Menu* initGG(){
    struct Menu *menu = (struct Menu*)malloc(sizeof(struct Menu));
    struct Item *items = (struct Item*)malloc(sizeof(struct Item)*2);
    struct Item *item = items;
    COORD pos;
    setPos(&pos, 40, 30);
    initItem(item++, 0, 1, 11, "  Restart  ", pos);
    setPos(&pos, 60, 30);
    initItem(item++, 1, 1, 8, "  Quit  ", pos);
    menu -> group = items;
    menu -> current = items;
    menu -> selected = -1;
    return menu;
}
//render gameover panel
int renderGG(struct Menu *menu){
    clearScreen();
    struct Item *items = menu -> group;
    int scene = -1;
    menu -> selected = -1;
    FILE *fp;
    int co;
    int *canva = (int *)malloc(sizeof(int)*DEFAULT_H*DEFAULT_W);
    int *p = canva;
    if ((fp = fopen("res/gg_page.txt","r")) == NULL){
        printf("ERROR:Cannot find canva file");
    }
    while(!feof(fp)){
        fscanf(fp,"%d ",&co);
        *p = co;
        //printf("%d ",co);
        p++;
    }
    fclose(fp);
    for(int i = 0; i < DEFAULT_W; i++){
        for(int j = 0; j < DEFAULT_H; j++){
            if(*(canva + j*DEFAULT_W + i) != 0){
                locate(i, j);
                setColor(105 + *(canva + j*DEFAULT_W + i));
                printf(" ");
            }
        }
    }

    free(canva);
    //render
    char c;
    for (int i = 0;i < 2; i++){
        normalItem(items + i);
    }
    highlightItem(menu -> current);
    while(scene == -1){
        if(kbhit()){
            c = getch();
            dealKey(menu);
        }
        scene = menu -> selected;
        Sleep(400);
    }
    //printf("%d",scene);
    if (scene == 1) scene = 3;
    return scene;
}
//render help panel
int renderHelp(struct Menu *menu){
    setColor(7);
    clearScreen();
    menu -> selected = -1;
    struct Item *items = menu -> group;
    int scene = -1;
    FILE *fp;
    int co;
    int *canva = (int *)malloc(sizeof(int)*DEFAULT_H*DEFAULT_W);
    int *p = canva;
    if ((fp = fopen("res/help_page.txt","r")) == NULL){
        printf("ERROR:Cannot find canva file");
    }
    while(!feof(fp)){
        fscanf(fp,"%d ",&co);
        *p = co;
        p++;
    }
    locate(22, 15);
    printf("UpArrow: Rotate Tetromino in 90 degree.\n");
    locate(22, 17);
    printf("LeftArrow/RightArrow: Move Tetromino to left/right in a unit.");
    locate(22, 19);
    printf("Space: Pause/Resume the game.");
    locate(22, 21);
    printf("ESC: Exit game.");
    fclose(fp);
    for(int i = 0; i < DEFAULT_W; i++){
        for(int j = 0; j < DEFAULT_H; j++){
            if(*(canva + j*DEFAULT_W + i) != 0){
                locate(i, j);
                setColor(105 + *(canva + j*DEFAULT_W + i));
                printf(" ");
            }
        }
    }
    //render
    char c;
    for (int i = 0;i < 2; i++){
        normalItem(items + i);
    }
    highlightItem(menu -> current);
    while(scene == -1){
        if(kbhit()){
            c = getch();
            dealKey(menu);
        }
        scene = menu -> selected;
        Sleep(400);
    }
    //printf("%d",scene);
    if (scene == 1) return 2;
    return scene;
}
