#include "tetris.h"

//Shape test
//S Z L J I O T
//¡ö  ¡õ

//¡ö¡ö¡ö¡ö

//¡ö
//¡ö¡ö¡ö

//  ¡ö
//¡ö¡ö¡ö

//¡ö¡ö
//¡ö¡ö

// ¡ö¡ö
//¡ö¡ö

//¡ö¡ö
//  ¡ö¡ö

// ¡ö
//¡ö¡ö¡ö
//Color settings
//I:03  J:02    L:06    O:0E    S:0A    Z:0C    T:05
void setColor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
int main()
{
    for(int i = 0;i < 50;i++){
        setColor(i);
        printf("%d -> ¡ö\n",i);
    }
    return 0;
}
