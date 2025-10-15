
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>

const float MOVE_SPEED = 3.0f;
const float JUMP_STRENGTH = -15.0f;
const float MAX_JUMP_LENGTH = 120.0f;
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
    float jumpPositionY, jumpMaxPositionY;
} character;
void colision(character* a, character* b){
    if ((a->x + a->width) > b->x && a->x < (b->x + b->width) &&
        (a->y + a->height)>b->y && a->y < b->y + b->height) {
        printf("colidiu");
        character c, ia, ib;
        if (a->x > b->x) {
            c.x = a->x;
            c.width = b->width - (a->x - b->x);
            ia.x = 0;
            ib.x = b->width - c.width;
        }
        else {
            c.x = b->x;
            c.width = a->width - (b->x - a->x);
            ib.x = 0;
            ia.x = a->width - c.width;
        }
        if (a->y > b->y) {
            c.y = a->y;
            c.height = b->height - (a->y - b->y);
            ia.y = 0;
            ib.y = b->height - c.height;
        }
        else {
            c.y = b->y;
            c.height = a->height - (b->y - a->y);
            ia.y = 0;
            ib.y = a->height - c.height ;
        }
        /*
        todo: terminar a validação da colisão
        for (int i = 0; i < c.width; i++) {
            for (int j = 0; j < c.height; j++) {
                if (al_get_pixel(a->sprite,)) {

                }
            }
        }*/
    }

}
void updatePhisics(character* person) {
    if (!person->onGround) {
        person->y += GRAVITY;
    }
    person->x += person->vx;
    float maxJump = person->jumpPositionY - MAX_JUMP_LENGTH;
    if (person->y > maxJump && person->jumpCount <=3 && person->jumpMaxPositionY > maxJump) {
        person->y += person->vy;
        if(person->y <= maxJump)
            person->jumpMaxPositionY = person->y;
    }

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
    ALLEGRO_COLOR color = { 255,0,0,1 };
    al_draw_filled_rectangle(person->x, person->y, person->x + person->width, person->y + person->height, color);
    al_draw_scaled_bitmap(person->sprite, person->spriteX, person->spriteY,45.0f,45.0f, person->x, person->y, person->width, person->height, person->flags);
}
void destroyCharacter(character* person) {
    al_destroy_bitmap(person->sprite);
}
void moveEnemys(ALLEGRO_EVENT event, character principalCharacter, character* enemy) {
    enemy->vx = 0;
    /*if (enemy->x > principalCharacter.x + principalCharacter.width) {
        enemy->vx = -MOVE_SPEED;
    }
    else if (enemy->x < principalCharacter.x - principalCharacter.width) {
        enemy->vx = MOVE_SPEED;
    }*/

}
void updateSprites(ALLEGRO_TIMER* timer, character* person) {
    if (person->vx < 0)
        person->flags = ALLEGRO_FLIP_HORIZONTAL;
    else if (person->vx > 0)
        person->flags = 0;
    if (al_get_timer_count(timer) % 8 == 0 && person->vx != 0) {
        person->spriteX += 45.0f;
        if (person->spriteX == 360.0f)
            person->spriteX = 0;
    }
}
void moveCharacter(ALLEGRO_EVENT event, character* person) {
    person->vx = 0;
    person->vy = 0;
    if (handleKeyBoard(event, ALLEGRO_KEY_A))
    {
        person->vx = -MOVE_SPEED;
    }
    if (handleKeyBoard(event, ALLEGRO_KEY_D))
    {
        person->vx = MOVE_SPEED;
    }
    if (keyDown(event, ALLEGRO_KEY_W)) {
        person->jumpCount++;
        person->jumpPositionY = person->y;
        person->jumpMaxPositionY = person->y;
    }
    if (handleKeyBoard(event, ALLEGRO_KEY_W)){
        person->vy = JUMP_STRENGTH;
        person->onGround = false;
    }
}