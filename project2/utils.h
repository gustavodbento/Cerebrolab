#ifndef UTILS_H
#define UTILS_H
#include <allegro5/allegro.h>
enum configsGlobais {
    DISPLAY_WIDTH = 640,
    DISPLAY_HEIGHT = 480
};
bool handleKeyBoard(ALLEGRO_EVENT event, int keycode);
#endif