
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>
#include <math.h>

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
    int spriteX, spriteY;
    int flags;
    float jumpPositionY, jumpMaxPositionY;
    float spriteWidth, spriteHeight, spritePartition;
    bool affectedByGravity;
    int animationFrameCount, animationDuration;
    bool active;
    int bullets;
    float power;
} character;
void destroyCharacter(character* person) {
    al_destroy_bitmap(person->sprite);
    person->active = false;
}
void resizeCharacters(character** arr, int addSize, int* size) {
    int newCount = *size + addSize;
    character* tmp = malloc(newCount * sizeof(character));
    int j = 0;
    for (int i = 0; i < *size; i++) {
        if ((*arr)[i].active)
            tmp[j++] = (*arr)[i];
    }
    free(*arr);
    *arr = tmp;
    *size = newCount;
}
void fire(character **bullets,int *bulletsLength, character *person,ALLEGRO_EVENT event) {
    if (keyDown(event, ALLEGRO_KEY_P) && person->bullets > 0) {
        resizeCharacters(bullets, 1, bulletsLength);
        if (*bulletsLength >0) {
            person->bullets--;
            character* bala = &((*bullets)[*bulletsLength - 1]);
            float lado = person->flags == 0? person->width * 0.8 : 0;
            bala->x = person->x + lado;
            bala->y = person->y + person->height / 2;
            bala->width = 24.0f;
            bala->height = 24.0f;
            bala->vx = person->flags == 0?5.0f:-5.0f;
            bala->vy = 0;
            bala->sprite = al_load_bitmap("assets/images/bala_teste.png");
            bala->spriteHeight = 100.0f;
            bala->spriteWidth = 100.0f;
            bala->spritePartition = 100.0f;
            bala->spriteX = 0;
            bala->spriteY = 0;
            bala->flags = 0;
            bala->affectedByGravity = false;
            bala->power = 25;
            bala->active = true;
       }
    }
}
void colisionType(character* a, character* b, int type) {
    float overlapX = (a->x + a->width) - b->x;
    if (a->x < b->x)
        overlapX = (a->x + a->width) - b->x;
    else
        overlapX = (b->x + b->width) - a->x;

    float overlapY;
    if (a->y < b->y)
        overlapY = (a->y + a->height) - b->y;
    else
        overlapY = (b->y + b->height) - a->y;
    bool colisionTop = a->y < b->y;
    bool colisionRight = a->x < b->x;
        switch (type) {
    case 1:
        if (overlapY < overlapX) {
            if (colisionTop) {
                a->onGround = true;
                a->vy = 0;
            }
            else { 
                a->y += MOVE_SPEED;
                a->vy = 0;
            }
        }
        else {
            if (colisionRight) {
                a->x -= MOVE_SPEED;
            }
            else {
                a->x += MOVE_SPEED;
            }
            a->vx = 0;
        }
        break;
    case 2:
        if (a->damage >=100 && a->life>0) {
            a->life--;
            a->damage = 0;
        }
        a->damage += b->power;
        if (a->life <= 0)
            destroyCharacter(a);
        b->vx = 0;
        break;

    }
}
bool colision(character* a, character* b, int type){
    if ((a->x + a->width) > b->x && a->x < (b->x + b->width) &&
        (a->y + a->height)>b->y && a->y < b->y + b->height) {
        if (type == 2) {
            colisionType(a, b, type);
            return true;
        }
            
        //escala dos sprites tamanhoreal/tamanhodesenhado
        float escalaA = a->spritePartition / a->width;
        float escalaB = b->spritePartition / b->width;
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
        int i = 0;
        int j = 0;
       while ( j < c.height && !colision) {
            while (i < c.width && !colision) {
                int pixelAx = (i + ia.x) * escalaA;
                int pixelAy = (j + ia.y) * escalaA;
                int pixelBx = (i + ib.x) * escalaB;
                int pixelBy = (j + ib.y) * escalaB;
                ALLEGRO_COLOR colorA = al_get_pixel(a->sprite, pixelAx, pixelAy);
                ALLEGRO_COLOR colorB = al_get_pixel(b->sprite, pixelBx, pixelBy);
                if (colorA.a > 0 && colorB.a > 0) {
                    colision = true;
                    al_unlock_bitmap(a->sprite);
                    al_unlock_bitmap(b->sprite);
                    colisionType(a, b, type);
                    return true;
                }
                i++;
            }
            j++;
            i = 0;
        }
       al_unlock_bitmap(a->sprite);
       al_unlock_bitmap(b->sprite);
       return false;
       
    }
    return false;

}

void updatePhisics(character* person) {
    if (!person->onGround && person->affectedByGravity) {
        person->y += GRAVITY;
    }
    if ( !person->affectedByGravity ||
        ((person->x > 0 && (person->x+person->width <= DISPLAY_WIDTH / 2 || (person->vx < 0 && person->x + person->width >= DISPLAY_WIDTH / 2))) || 
        (person->x <= 0 && person->vx > 0)))
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
    //al_draw_filled_rectangle(person->x, person->y, person->x + person->width, person->y + person->height, (ALLEGRO_COLOR) { 255, 0, 0, 1 });
    al_draw_scaled_bitmap(person->sprite, person->spriteX* person->spritePartition, person->spriteY* person->spritePartition, person->spritePartition, person->spritePartition, person->x, person->y, person->width, person->height, person->flags);
}

void moveEnemys(ALLEGRO_EVENT event, character principalCharacter, character* enemy) {
    enemy->vx = 0;
    float distanceX = fabs(principalCharacter.x - enemy->x);
    if (distanceX <= 300) {
        enemy->spriteY = 1;
        if (enemy->x > principalCharacter.x + principalCharacter.width) {
            enemy->vx = -MOVE_SPEED;
        }
        else if (enemy->x < principalCharacter.x - principalCharacter.width) {
            enemy->vx = MOVE_SPEED;
        }
        else {
            enemy->spriteY = 0;
        }
    }
    else {
        enemy->vx = 0; 
    }

}
void updateSprites(ALLEGRO_TIMER* timer, character* person) {
    if (person->vx < 0)
        person->flags = ALLEGRO_FLIP_HORIZONTAL;
    else if (person->vx > 0)
        person->flags = 0;
    if (al_get_timer_count(timer) % 10 == 0) {
        person->spriteX++;
        
        if (person->spriteX >= (int)(person->spriteWidth/ person->spritePartition))
            person->spriteX = 0;
    }
}
void moveCharacter(ALLEGRO_EVENT event, character* person, ALLEGRO_TIMER* timer) {
    person->vx = 0;
    person->vy = 0;
    if ((al_get_timer_count(timer) - person->animationFrameCount) > person->animationDuration) {
        person->spriteY = 0;
    }
    
    if (handleKeyBoard(event, ALLEGRO_KEY_A))
    {
        person->vx = -MOVE_SPEED;
        person->spriteY = 1;
    }
    if (handleKeyBoard(event, ALLEGRO_KEY_D))
    {
        person->vx = MOVE_SPEED;
        person->spriteY = 1;
    }
    if (keyDown(event, ALLEGRO_KEY_P)) {
        person->spriteY = 2;
        person->animationDuration = 35;
        person->animationFrameCount = al_get_timer_count(timer);

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