#include "tetris.h"
namespace tetris{
typedef struct Item {
    int color;
    int length;
    char* content;
    COORD pos;
} Item;

typedef struct Row{
    struct Item *head;
    struct Item *next;
    struct Item *before;
}
typedef struct Col{
    struct Item *head;
    struct Item *next;
    struct Item *before;
}

struct Item* initMenu(struct Item* item){


}

}
