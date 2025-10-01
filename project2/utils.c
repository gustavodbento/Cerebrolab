#include <allegro5/allegro.h>
bool keys[ALLEGRO_KEY_MAX] = { false };
enum configsGlobais {
    DISPLAY_WIDTH = 640,
    DISPLAY_HEIGHT = 480
};
bool handleKeyBoard(ALLEGRO_EVENT event, int keycode) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keys[event.keyboard.keycode] = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        keys[event.keyboard.keycode] = false;
    }
    return keys[keycode];

}