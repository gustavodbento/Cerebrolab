#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "stage.h"
#include "structs.h"
const float MOVE_SPEED = 3.0f;
const float JUMP_STRENGTH = -15.0f;
const float MAX_JUMP_LENGTH = 120.0f;
const float GRAVITY  = 5.0f;
const float RANGE_COMBAT = 500;
const float BULLET_SPEED = 7;
const int FIRE_TIME = 15;
const int ENEMY_FIRE_TIME = 40;
//tipos 1 - personagem principal 2 - inimigo comuns 3 - chefão 
character createCharacter(int type, float x, float width, float height,float spriteWidth, float spriteHeigth,
    float partition,int id, char* sprite, stageCfg configs, bool random, int lifes) {
    character c = { 0 };
    c.x = !random ? x : DISPLAY_WIDTH + 200 + rand() % (int)(1500 - DISPLAY_WIDTH / 2);
    c.y = DISPLAY_HEIGHT - 104.0f;
    c.width = width;
    c.height = height;
    c.sprite = al_load_bitmap(sprite);
    c.damage = 0;
    c.life = lifes;
    c.jumpCount = 0;
    c.vx = 0;
    c.vy = 0;
    c.onGround = true;
    c.spriteX = 0;
    c.spriteY = 0;
    c.flags = 0;
    c.jumpPositionY = 0;
    c.jumpMaxPositionY = 0;
    c.spriteWidth = spriteWidth;
    c.spriteHeight = spriteHeigth;
    c.spritePartition = partition;
    c.animationFrameCount = 0;
    c.animationDuration = 0;
    c.active = true;
    c.bullets = 50;
    c.power = 15;
    c.typeCharacter = type;
    c.fireFrameCount = 0;
    c.direction = 0;
    c.id = id;
    return c;
}

void destroyCharacter(character* person) {
    if(person->active)
    {
        al_destroy_bitmap(person->sprite);
        person->active = false;
    }
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
void fire(character **bullets,int *bulletsLength, character *person,ALLEGRO_EVENT event, float shotPower, ALLEGRO_TIMER* timer) {
    if (person->bullets > 0 && (al_get_timer_count(timer) - person->animationFrameCount) > person->animationDuration) {
        resizeCharacters(bullets, 1, bulletsLength);
        if (*bulletsLength > 0) {
            person->bullets--;
            person->animationFrameCount = al_get_timer_count(timer);
            person->animationDuration = person->typeCharacter==1? FIRE_TIME: ENEMY_FIRE_TIME;
            if(person->vx != 0)
                person->spriteY = 2;
            else
               person->spriteY = 3;

            character* bala = &((*bullets)[*bulletsLength - 1]);
            float lado = person->flags == 0 ? person->width * 0.8 : 0;
            bala->x = person->x + lado;
            bala->y = person->y + person->height / (person->typeCharacter != 5 ? 3 : 2);
            bala->width = 24.0f;
            bala->height = 24.0f;
            bala->vx = person->flags == 0 ? BULLET_SPEED : -BULLET_SPEED;
            bala->vy = 0;
            bala->sprite = al_load_bitmap("assets/images/bala_teste.png");
            bala->spriteHeight = 100.0f;
            bala->spriteWidth = 100.0f;
            bala->spritePartition = 100.0f;
            bala->spriteX = 0;
            bala->spriteY = 0;
            bala->flags = 0;
            bala->typeCharacter = person->typeCharacter == 1 ? 3 : 4; //diferenciar o tiro de inimigos do personagem principal
            bala->power = shotPower;
            bala->active = true;
        }
    }
}
void colisionType(character* a, character* b, int type, float *cameraX) {
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
                a->jumpPositionY = 0;
                a->jumpMaxPositionY = 0;
            }
            else { 
                a->y += MOVE_SPEED;
                a->vy = 0;
            }
        }
        else {
            if (colisionRight) {
                a->x -= MOVE_SPEED;
                //*cameraX -= MOVE_SPEED;
            }
            else {
                a->x += MOVE_SPEED;
                //*cameraX += MOVE_SPEED;
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
    case 3:
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

    }
}
bool colision(character* a, character* b, int type, float *cameraX){
    if (a->active && b->active &&
        (a->x + a->width) > b->x && a->x < (b->x + b->width) &&
        (a->y + a->height)>b->y && a->y < b->y + b->height) {
        if (type >1) {
            if(type <=3)
            colisionType(a, b, type, cameraX);
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
                    colisionType(a, b, type, cameraX);
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
void collectItem(cenario* item, character* person, ALLEGRO_EVENT event) {
    bool overlappingX = person->x + person->width > item->x && person->x < item->x + item->width;
    bool overlappingY = person->y + person->height > item->y && person->y < item->y + item->height;
    if (overlappingX && overlappingY) {
        if (keyDown(event, ALLEGRO_KEY_SPACE)) {
            switch (item->type) {
            case 3:
                person->bullets += 10;
                break;
            case 4:
                person->life += 1;
                break;

            }
            item->active = false;
        }
    }
}
void updatePhisics(character* person, stageCfg *configs) {
    bool bulletType = (person->typeCharacter == 3 || person->typeCharacter == 4);
    if (!person->onGround && !bulletType) {
        person->y += GRAVITY;
    }
    float centerX = DISPLAY_WIDTH / 2;
    bool bordas = (person->vx > 0 && configs->cameraX + configs->partitionBackground < configs->backgroundWidth ||
        (person->vx < 0 && configs->cameraX > 0));
    if (person->typeCharacter == 1 && bordas
       ) {
        configs->cameraX += person->vx;
    }
    if ((person->typeCharacter == 2 || person->typeCharacter == 5) && bordas) {
        person->x += -configs->personagens[0].vx;
    }
    if (bulletType ||
        ((person->x > 0 && (person->x + person->width <= centerX ||
            (person->vx < 0 && person->x + person->width >= centerX))) ||
            (person->x <= 0 && person->vx > 0))
        || (person->vx > 0 && configs->cameraX + configs->partitionBackground >= configs->backgroundWidth &&
            person->x + person->width <= DISPLAY_WIDTH)
        )
    {
        person->x += person->vx;
    }
    if (configs->cameraX <= 0)
        configs->cameraX = 0;
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
void print(character* person, float cameraX) {
    int maxSprite = person->spriteHeight / person->spritePartition;
    if (person->spriteY > maxSprite - 1)
        person->spriteY = 0;
    al_draw_scaled_bitmap(
        person->sprite,
        person->spriteX * person->spritePartition,
        person->spriteY * person->spritePartition,
        person->spritePartition,
        person->spritePartition,
        person->x,
        person->y,
        person->width,
        person->height,
        person->flags
    );
}
void moveEnemys(ALLEGRO_EVENT event, character principalCharacter, character* enemy, stageCfg configs) {
    float distanceX;
    float playerCenter = principalCharacter.x + principalCharacter.width / 2;
    float enemyCenter = enemy->x + enemy->width / 2;
    distanceX = fabs(playerCenter - enemyCenter);


    if (distanceX > RANGE_COMBAT) {
        enemy->spriteY = 1;

        if (enemy->direction == 0) { 

            enemy->vx = MOVE_SPEED / 2;
            enemy->x += enemy->vx;
            if (rand() % 200 == 0) enemy->direction = 1; 
        } else {
            enemy->vx = -MOVE_SPEED / 2;
            enemy->x += enemy->vx;
            if (rand() % 200 == 0) enemy->direction = 0;
        }
    }
    else if (distanceX > 300 && distanceX <= RANGE_COMBAT) {
        enemy->spriteY = 1;

        if (enemy->x > principalCharacter.x)
            enemy->vx = -MOVE_SPEED;
        else
            enemy->vx = MOVE_SPEED;

        enemy->x += enemy->vx;
    }else {
        enemy->spriteY = 2;
        enemy->vx = 0;
    }
    if (enemy->x + enemy->width >= configs.backgroundWidth) {
        enemy->x -= MOVE_SPEED;
        enemy->vx = -MOVE_SPEED;
        enemy->direction = 1;
    }
    else if (enemy->x <= -configs.cameraX) {
        enemy->x += MOVE_SPEED;
        enemy->vx = MOVE_SPEED;
        enemy->direction = 0;
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
        if (handleKeyBoard(event, ALLEGRO_KEY_A) || handleKeyBoard(event, ALLEGRO_KEY_D))
            person->spriteY = 1;
    }
    
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