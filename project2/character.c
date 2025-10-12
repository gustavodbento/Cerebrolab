
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>

const float MOVE_SPEED = 3.0f;
const float JUMP_STRENGTH = -15.0f;
const float GRAVITY  = 5.0f;
typedef struct {
    float x, y, height, width;
    ALLEGRO_BITMAP *sprite;
    float damage, life;
    int jumpCount;
    float vx, vy;
    bool onGround;
    float spriteX, spriteY;
    int flags;
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
    al_draw_scaled_bitmap(person->sprite, person->spriteX, person->spriteY,45.0f,45.0f, person->x, person->y, person->width, person->height, person->flags);
}
void destroyCharacter(character* person) {
    al_destroy_bitmap(person->sprite);
}
void moveCharacter(ALLEGRO_EVENT event, character* person, ALLEGRO_TIMER* timer) {
    person->vx = 0;
    person->vy = 0;
    person->flags = 0;
    if (handleKeyBoard(event, ALLEGRO_KEY_A))
    {
        person->vx = -MOVE_SPEED;
        person->flags = ALLEGRO_FLIP_HORIZONTAL;
    }
    if (handleKeyBoard(event, ALLEGRO_KEY_D))
    {
        person->vx = MOVE_SPEED;
    }
    if (al_get_timer_count(timer) % 8 == 0 && person->vx != 0) {
        person->spriteX += 45.0f;
        if (person->spriteX == 360.0f)
            person->spriteX = 0;
    }
    if (handleKeyBoard(event, ALLEGRO_KEY_W)){
        person->vy = JUMP_STRENGTH;
        person->onGround = false;
    }
}