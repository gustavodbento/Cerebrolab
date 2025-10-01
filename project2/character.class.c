
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
const float MOVE_SPEED = 5.0f;
const float JUMP_STRENGTH = -15.0f;
const float GRAVITY  = 5.0f;
typedef struct {
    float x, y, height, width;
    ALLEGRO_COLOR color;
    float damage, life;
    int jumpCount;
    float vx, vy;
    bool onGround;
} character;
void updatePhisics(character* person) {
    if (!person->onGround) {
        person->y += GRAVITY;
    }
    person->x += person->vx;
    person->y += person->vy;
        
    if (person->y >= DISPLAY_HEIGHT - (person->height + 40)) {
        person->y = DISPLAY_HEIGHT - (person->height + 40);
        person->vy = 0;
        person->onGround = true;
        person->jumpCount = 0;
    }
    else {
        person->onGround = false;
    }
}
void print(character* person) {
    al_draw_filled_rectangle(person->x, person->y, person->x + person->width, person->y + person->height, person->color);
}

void moveCharacter(ALLEGRO_EVENT event, character* person) {
    person->vx = 0;
    person->vy = 0;
    if (handleKeyBoard(event, ALLEGRO_KEY_A))
        person->vx = -MOVE_SPEED;
    if (handleKeyBoard(event, ALLEGRO_KEY_D))
        person->vx = MOVE_SPEED;
    if (handleKeyBoard(event, ALLEGRO_KEY_W)){
        person->vy = JUMP_STRENGTH;
        person->onGround = false;
    }
}