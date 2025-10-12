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
    bool flipSprite;
} character;
void updatePhisics(character* person);
void print(character* person);
void moveCharacter(ALLEGRO_EVENT event, character* person1, ALLEGRO_TIMER* timer);
void destroyCharacter(character* person);
#endif
