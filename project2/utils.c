#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "structs.h"
bool keys[ALLEGRO_KEY_MAX] = { false };
enum configsGlobais {
    DISPLAY_WIDTH = 640,
    DISPLAY_HEIGHT = 480
};
char backgroundImages[2][20] = {
    "menu.png",
    "fundo1.png",
};
bool menuScreens[3] = {
    false,
    false,
    false
};
int menuLen = 3;
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
bool mouseClickButton(ALLEGRO_EVENT event, float mouseX, float mouseY, button btn) {
    float width = al_get_text_width(btn.font, btn.text) / 2;
    float y2 = btn.y + al_get_font_line_height(btn.font);
    if(mouseX >= btn.x - width && mouseX <= btn.x + width && mouseY >= btn.y && mouseY <= y2)
        return event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN;

    return false;
}
bool mouseHoverCard(card cardHover, float mouseX, float mouseY) {
    return mouseX >= cardHover.x && mouseX <= cardHover.x + cardHover.width && mouseY >= cardHover.y && mouseY <= cardHover.y + cardHover.height;
}
void mouseHoverButton(float mouseX, float mouseY, button* btn, ALLEGRO_COLOR colorHover, ALLEGRO_COLOR colorNotHover) {
    float width = al_get_text_width(btn->font, btn->text) / 2;
    float y2 = btn->y + al_get_font_line_height(btn->font);
    if (mouseX >= btn->x - width && mouseX <= btn->x + width && mouseY >= btn->y && mouseY <= y2) {
        btn->color = colorHover;
    }
    else {
        btn->color = colorNotHover;
    }
}
void drawCard(card cardSelect, ALLEGRO_COLOR color, ALLEGRO_FONT* font) {
    al_draw_filled_rounded_rectangle(cardSelect.x, cardSelect.y, cardSelect.x + cardSelect.width
        , cardSelect.y + cardSelect.height, 10, 10, color);
    al_draw_text(font, al_map_rgb(0, 0, 0), cardSelect.x + cardSelect.width / 2,
        cardSelect.y + cardSelect.height - 20, ALLEGRO_ALIGN_CENTRE, cardSelect.text);
    al_draw_scaled_bitmap(cardSelect.sprite, cardSelect.spriteX * cardSelect.partition,
        cardSelect.spriteX * cardSelect.partition, cardSelect.partition, cardSelect.partition,
        cardSelect.x, cardSelect.y, cardSelect.width, cardSelect.height, 0);
}
bool keyDown(ALLEGRO_EVENT event, int keycode) {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == keycode;
    
}