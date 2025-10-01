#ifndef CHARACTER_CLASS_H
#define CHARACTER_CLASS_H



typedef struct {
    float x, y, height, width;
    ALLEGRO_COLOR color;
    float damage, life;
    int jumpCount;
    float vx, vy;
    bool onGround;
} character;

void updatePhisics(character* person);
void print(character* person);
void moveCharacter(ALLEGRO_EVENT event, character* person1);

#endif
