#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
bool keys[ALLEGRO_KEY_MAX] = { false };
enum configsGlobais {
    DISPLAY_WIDTH = 640,
    DISPLAY_HEIGHT = 480
};
char backgroundImages[2][20] = {
    "menu.png",
    "fundo1.png",
};
bool menuScreens[2] = {
    false,
    false
};
typedef struct {
    float x, y;
    char text[50];
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR color;
} button;
bool handleKeyBoard(ALLEGRO_EVENT event, int keycode) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keys[event.keyboard.keycode] = true;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        keys[event.keyboard.keycode] = false;
    }
    return keys[keycode];

}
bool mouseClick(ALLEGRO_EVENT event) {
    return event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN;
}
bool mouseButtonHover(float mouseX, float mouseY, button btn) {
    float width = al_get_text_width(btn.font, btn.text) / 2;
    float y2 = btn.y + al_get_font_line_height(btn.font);
    return mouseX >= btn.x - width && mouseX <= btn.x + width && mouseY >= btn.y && mouseY <= y2;
}
bool keyDown(ALLEGRO_EVENT event, int keycode) {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == keycode;
    
}