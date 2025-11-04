#ifndef UTILS_H
#define UTILS_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
enum configsGlobais {
    DISPLAY_WIDTH = 640,
    DISPLAY_HEIGHT = 480
};
char backgroundImages[2][20];
typedef struct {
    float x, y;
    char text[50];
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR color;
} button;
bool menuScreens[2];
bool handleKeyBoard(ALLEGRO_EVENT event, int keycode);
bool keyDown(ALLEGRO_EVENT event, int keycode);
bool mouseButtonHover(float mouseX, float mouseY, button btn);
bool mouseClick(ALLEGRO_EVENT event);
#endif