
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
void colisionType(character* a, character* b, character c, int type) {
    float pen_x = c.width;
    float pen_y = c.height;
    switch (type) {
    case 1:
        if (pen_x < pen_y) {
            if (a->vx > 0) {
                a->x -= pen_x;
            }
            else if (a->vx < 0) {
                a->x += pen_x;
            }
            a->vx = 0;
        }
        else {
            if (a->vy > 0) {
                a->y -= pen_y;
                a->onGround = true;
            }
            else if (a->vy < 0) {
                a->y += pen_y;
            }
            a->vy = 0;
        }
        break;

    }
}
void colision(character* a, character* b, int type){
    if ((a->x + a->width) > b->x && a->x < (b->x + b->width) &&
        (a->y + a->height)>b->y && a->y < b->y + b->height) {
        //escala dos sprites tamanhoreal/tamanhodesenhado
        float escala = 50.0f / 128.0f;
        //c = intersecção entre a e b
        //ia = inicio dos pixels do sprite a em relação ao início da intersecção c
        //ib = inicio dos pixels do sprite b em relação ao início da intersecção c
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
        al_lock_bitmap(a->sprite, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
        al_lock_bitmap(b->sprite, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
        bool colision = false;
       for (int i = 0; i < c.width; i++) {
           if (colision)
               break;
            for (int j = 0; j < c.height; j++) {
                if (colision)
                    break;
                int pixelAx = (i + ia.x) * escala;
                int pixelAy = (j + ia.y) * escala;
                int pixelBx = (i + ib.x) * escala;
                int pixelBy = (j + ib.y) * escala;
                ALLEGRO_COLOR colorA = al_get_pixel(a->sprite, pixelAx, pixelAy);
                ALLEGRO_COLOR colorB = al_get_pixel(b->sprite, pixelBx, pixelBy);

                unsigned char rA, gA, bA, aColor;
                unsigned char rB, gB, bB, bColor;

                al_unmap_rgba(colorA, &rA, &gA, &bA, &aColor);
                al_unmap_rgba(colorB, &rB, &gB, &bB, &bColor);
                if (aColor > 0 && bColor > 0) {
                    colision = true;
                    colisionType(a, b, c, type);
                    al_unlock_bitmap(a->sprite);
                    al_unlock_bitmap(b->sprite);
                }
            }
        }
       
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
    //ALLEGRO_COLOR color = { 255,255,255,0.5f };
    //al_draw_filled_rectangle(person->x, person->y, person->x + (person->width), person->y + (person->height), color);
    al_draw_scaled_bitmap(person->sprite, person->spriteX, person->spriteY, 50.0f, 50.0f, person->x, person->y, person->width, person->height, person->flags);
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
    if (al_get_timer_count(timer) % 10 == 0) {
        person->spriteX += 50.0f;
        if (person->vx > 0)
            person->spriteY = 50.0f;
        if (person->spriteX == 200.0f)
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