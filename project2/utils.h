#ifndef UTILS_H
#define UTILS_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "structs.h"

enum configsGlobais {
    DISPLAY_WIDTH = 640,
    DISPLAY_HEIGHT = 480
};
char backgroundImages[2][20];
int menuLen;

typedef struct {
    float x, y;
    char text[50];
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR color;
} button;
bool menuScreens[3];
bool handleKeyBoard(ALLEGRO_EVENT event, int keycode);
bool keyDown(ALLEGRO_EVENT event, int keycode);
void mouseHoverButton(float mouseX, float mouseY, button* btn, ALLEGRO_COLOR colorHover, ALLEGRO_COLOR colorNotHover);
bool mouseClickButton(ALLEGRO_EVENT event, float mouseX, float mouseY, button btn);
bool mouseHoverCard(card cardHover, float mouseX, float mouseY);
void drawCard(card cardSelect, ALLEGRO_COLOR color, ALLEGRO_FONT* font);
#endif