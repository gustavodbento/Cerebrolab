#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
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
    int animationFrameCount, animationDuration;
    bool active;
    int bullets;
    float power;
    int typeCharacter, fireFrameCount, direction, id;
} character;
typedef struct {
    float x,y;
    char* text;
    ALLEGRO_FONT* font;
} dialog;
typedef struct {
    float x, y, height, width, spriteX, 
        spriteY, spriteWidth,spriteHeigth, partition;
    int type;
    bool active;
    ALLEGRO_BITMAP* sprite;
}cenario;
typedef struct {
    character* personagens;
    character* objetos;
    cenario* cenarios;
    cenario* itens;
    int itensLength;
    int cenariosLength;
    int personagensLength;
    int objetosLength;
    char background[50];
    float cameraX;
    dialog* dialogs;
    int dialogsLength;
    int enemys;
    float partitionBackground;
    float backgroundWidth;
    int lastPartition;
    bool initialized;
    int lastStage, stage;
    bool inMenu;
    bool complete;
}stageCfg;