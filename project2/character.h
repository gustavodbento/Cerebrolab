#ifndef CHARACTER_H
#define CHARACTER_H
#include "structs.h"
const float RANGE_COMBAT;
const int FIRE_TIME;
void updatePhisics(character* person,struct stageCfg *configs);
void print(character* person, float cameraX);
void moveCharacter(ALLEGRO_EVENT event, character* person, ALLEGRO_TIMER* timer);
void destroyCharacter(character* person);
void moveEnemys(ALLEGRO_EVENT event, character principalCharacter, character* enemy, stageCfg configs);
void updateSprites(ALLEGRO_TIMER* timer, character* person);
bool colision(character* a, character* b, int type, float* cameraX);
void fire(character** bullets, int* bulletsLength, character* person, ALLEGRO_EVENT event, float shotPower, ALLEGRO_TIMER* timer);
void resizeCharacters(character** arr, int addSize, int* size);
character createCharacter(int type, float x, float width, float height, float spriteWidth, float spriteHeigth,
    float partition, int id, char* sprite, stageCfg configs, bool random, int lifes);
void collectItem(cenario* item, character* person, ALLEGRO_EVENT event);
#endif
