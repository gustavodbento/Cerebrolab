#ifndef CHARACTER_H
#define CHARACTER_H



typedef struct {
    float x, y, height, width;
    ALLEGRO_BITMAP* sprite;
    float damage, life;
    int jumpCount;
    float vx, vy;
    bool onGround;
    float spriteX, spriteY;
    int flags;
    float jumpPositionY, jumpMaxPositionY;
} character;
void updatePhisics(character* person);
void print(character* person);
void moveCharacter(ALLEGRO_EVENT event, character* person1);
void destroyCharacter(character* person);
void moveEnemys(ALLEGRO_EVENT event, character principalCharacter, character* enemy);
void updateSprites(ALLEGRO_TIMER* timer, character* person);
void colision(character* person1, character* person2);
#endif
