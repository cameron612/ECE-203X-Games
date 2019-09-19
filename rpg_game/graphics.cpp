#include "graphics.h"

#include "globals.h"


#define YELLOW 0xFFFF00
void draw_player(int u, int v, int key)
{
    //pc.printf("drawing player \r\n");
    uLCD.filled_rectangle(u, v, u+11, v+3, YELLOW);
    uLCD.filled_rectangle(u, v+3, u+11, v+7, BLACK);
    //uLCD.filled_rectangle(u, v+3, u, v+7, YELLOW);
    uLCD.filled_rectangle(u, v+8, u+11, v+11, YELLOW);
}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
void draw_img(int u, int v, const char* img)
{
    
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'W') colors[i] = WHITE;
        else if (img[i] == 'B') colors[i] = BLACK;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    //pc.printf("drawing nothing \r\n");
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    //pc.printf("drawing wall segment \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}
void draw_bus(int u, int v)
{
    //pc.printf("drawing wall segment \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, YELLOW);
    uLCD.filled_rectangle(u+2, v, u+4, v+2, BLACK);
    uLCD.filled_rectangle(u+2, v+8, u+4, v+10, BLACK);
}

void draw_plant(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}
void draw_plant_cb(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, DGREY);
}

void draw_rose(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, RED);
}
void draw_rose_cb(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
}

void draw_violet(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, BLUE);
}
void draw_violet_cb(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
}
void draw_bud(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
    uLCD.filled_rectangle(u+2, v+2, u+8, v+8, YELLOW);
}
void draw_sideline(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
}
void draw_door(int u, int v)
{
    //pc.printf("drawing plant \r\n");
    uLCD.filled_rectangle(u, v, u+10, v+5, BROWN);
    uLCD.filled_rectangle(u+8, v+5, u+10, v+6, BLACK);
}
void draw_baller(int u, int v)
{
    //pc.printf("drawing player \r\n");
    uLCD.filled_rectangle(u, v, u+11, v+3, BLACK);
    uLCD.filled_rectangle(u, v+3, u+11, v+7, RED);
    //uLCD.filled_rectangle(u, v+3, u, v+7, YELLOW);
    uLCD.filled_rectangle(u, v+8, u+11, v+11, BLACK);
    uLCD.filled_rectangle(u, v, u, v+11, BLACK);
}
void draw_T(int u, int v){
    //fill in arguments
    char* str;
    str = "BYYYYYYYYYBBYWWWWWWWYBBYWYYWYYWYBBYWYBWBYWYBBYYYBWBYYYBBBBBBWBBBBBBBBBBWBBBBBBBBBBWBBBBBBBBBYWYBBBBBBBBYWYBBBBBBBBYYYBBBB";
    draw_img(u,v,str);
    
}

void draw_border()
{
    //pc.printf("drawing border \r\n");
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}


