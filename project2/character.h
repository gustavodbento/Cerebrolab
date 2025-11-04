#ifndef CHARACTER_H
#define CHARACTER_H



typedef struct {
    float x, y, height, width;
    ALLEGRO_BITMAP* sprite;
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
} character;
void updatePhisics(character* person);
void print(character* person);
void moveCharacter(ALLEGRO_EVENT event, character* person, ALLEGRO_TIMER* timer);
void destroyCharacter(character* person);
void moveEnemys(ALLEGRO_EVENT event, character principalCharacter, character* enemy);
void updateSprites(ALLEGRO_TIMER* timer, character* person);
bool colision(character* a, character* b, int type);
void fire(character** bullets, int* bulletsLength, character** characters, ALLEGRO_EVENT event);
void resizeCharacters(character** arr, int addSize, int* size);
#endif
