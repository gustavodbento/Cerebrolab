
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_image.h>
#include "character.h"
#include "utils.h"
#include "stage.h"
#include <string.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
int main()
{
    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);
    ALLEGRO_DISPLAY* display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ALLEGRO_FONT* titleFont = al_load_ttf_font("assets/fonts/metal_slug.ttf", 64, 0);
    ALLEGRO_FONT* optionsFont = al_load_ttf_font("assets/fonts/metal_slug.ttf", 32, 0);
    ALLEGRO_FONT* font = al_load_ttf_font("assets/fonts/Sono-Medium.ttf", 16, 0);
    ALLEGRO_AUDIO_STREAM* musica = al_load_audio_stream("assets/sounds/musica_1.mp3", 4, 2048);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_set_window_title(display, "Ardennes");
    al_start_timer(timer);
    stageCfg configs;
    configs.cameraX = 0;
    configs.partitionBackground = 1536;
    configs.backgroundWidth = 1536;
    configs.initialized = false;
    configs.lastStage = 0;
    configs.stage = 0;
    configs.inMenu = true;
    configs.complete = false;
    char imagesPath[50] = "assets/images/";
    char backgroundPath[50];
    strcpy_s(backgroundPath, sizeof(backgroundPath), imagesPath);
    strcat_s(backgroundPath, sizeof(backgroundPath), backgroundImages[0]);
    ALLEGRO_BITMAP* background = al_load_bitmap(backgroundPath);
    ALLEGRO_BITMAP* life = al_load_bitmap("assets/images/heart.png");
    ALLEGRO_BITMAP* ammo = al_load_bitmap("assets/images/ammo_sprite.png");
    ALLEGRO_COLOR fontColorWhite = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR fontColorYellow = al_map_rgb(255, 255, 0);
    button menuOptions[3] = {
        {DISPLAY_WIDTH / 2,140, "Novo Jogo", optionsFont, fontColorWhite},
        {DISPLAY_WIDTH / 2,170, "Opcoes", optionsFont, fontColorWhite},
        {DISPLAY_WIDTH / 2,200, "Sair", optionsFont, fontColorWhite}
    };
    card selectCharacter[2] = {
        {DISPLAY_WIDTH / 2 - 148,DISPLAY_HEIGHT/2,128,128,50,0,0, al_load_bitmap("assets/images/principal.png"),"Fred",optionsFont, al_map_rgb(255,255,255), true},
        {DISPLAY_WIDTH / 2 + 20,DISPLAY_HEIGHT / 2,128,128,50,0,0, al_load_bitmap("assets/images/principal_1.png"),"Lucia",optionsFont,al_map_rgb(255,255,255), true}
    };
    bool isRunning = true;
    bool gameIsActive = false;
    float mouseX = 0;
    float mouseY = 0;
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_gain(musica, 0.8);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    while (isRunning) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue,&event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        al_clear_to_color(al_map_rgb(184, 209, 203));
        al_draw_scaled_bitmap(background, configs.cameraX, 0, configs.partitionBackground, 
            !menuScreens[0]?1024:160, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
        handleKeyBoard(event, ALLEGRO_KEY_MAX);
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mouseX = event.mouse.x;
            mouseY = event.mouse.y;
        }
        //menu
        if (!menuScreens[0]) {
            if (!configs.inMenu && (!configs.initialized || configs.lastStage != 0))
                changeStage(&background, &configs, configs.stage);
            char* title;
            if (configs.lastStage == 0) {
                title = "Ardennes";
            }
            else if (configs.complete) {
                title = "Vitoria";
            }
            else {
                title = "Game Over";
            }
            al_draw_text(titleFont, fontColorWhite, DISPLAY_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTRE,
                title
            );
            
            if (!menuScreens[1]) {
                for (int i = 0; i < 3; i++) {
                    mouseHoverButton(mouseX, mouseY, &menuOptions[i], fontColorYellow, fontColorWhite);
                    al_draw_text(menuOptions[i].font, menuOptions[i].color, menuOptions[i].x, menuOptions[i].y, ALLEGRO_ALIGN_CENTRE, menuOptions[i].text);
                   
                    if (mouseClickButton(event,mouseX,mouseY, menuOptions[i])) {
                        if (i == 0) {
                            menuScreens[1] = true;
                            break;
                        }
                        if (i == 2) {
                            isRunning = false;
                            break;
                        }
                    }
                    
                   
                }
            }else if (!menuScreens[2]) {
                for (int i = 0; i < 2; i++) {
                    card cardSelect = selectCharacter[i];
                    ALLEGRO_COLOR color = cardSelect.color;
                    if (mouseHoverCard(cardSelect, mouseX, mouseY))
                        color = al_map_rgb(255, 255, 0);
                    drawCard(cardSelect, color, font);
                    if (mouseHoverCard(cardSelect, mouseX, mouseY) && event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        configs.personSelected = i;
                        menuScreens[0] = true;
                        menuScreens[2] = true;
                        changeStage(&background,&configs,1);
                        gameIsActive = true;
                        break;
                        
                    }
                }
            }

        }
        //inicio do jogo
        if (menuScreens[0]) {
            if (gameIsActive) {
                character *person = &configs.personagens[0];
                bool bordas = (person->vx > 0 && configs.cameraX + configs.partitionBackground < configs.backgroundWidth ||
                    (person->vx < 0 && configs.cameraX > 0));
                for (int i = 0; i < configs.cenariosLength; i++) {
                    cenario *cenarioSpr = &configs.cenarios[i];
                    if (cenarioSpr->type > 1 && bordas) {
                        cenarioSpr->x += -person->vx;
                    }
                    if (cenarioSpr->partition == 0)
                    {
                        al_draw_scaled_bitmap(cenarioSpr->sprite, cenarioSpr->spriteX, cenarioSpr->spriteY, cenarioSpr->spriteWidth,
                            cenarioSpr->spriteHeigth, cenarioSpr->x, cenarioSpr->y, cenarioSpr->width, cenarioSpr->height, 0);
                    }
                    else {
                        int areaDesenhada = cenarioSpr->width / cenarioSpr->partition;
                        for (int i = 0; i <= areaDesenhada; i++) {
                            al_draw_scaled_bitmap(cenarioSpr->sprite, cenarioSpr->spriteX, cenarioSpr->spriteY, cenarioSpr->spriteWidth,
                                cenarioSpr->spriteHeigth, i * cenarioSpr->partition, cenarioSpr->y, cenarioSpr->partition, cenarioSpr->height, 0);
                        }
                    }
                }
                for (int i = 0; i < configs.itensLength; i++) {
                    cenario *itemSpr = &configs.itens[i];
                    if (itemSpr->active){
                        if (bordas) {
                            itemSpr->x += -person->vx;
                        }
                        al_draw_scaled_bitmap(itemSpr->sprite, itemSpr->spriteX, itemSpr->spriteY, itemSpr->spriteWidth,
                            itemSpr->spriteHeigth, itemSpr->x, itemSpr->y, itemSpr->width, itemSpr->height, 0);
                        collectItem(itemSpr, person, event);
                        
                    }
                }
                char bulletsText[10];
                //desenha a contagem de balas
                snprintf(bulletsText, sizeof(bulletsText), "%d", configs.personagens[0].bullets);
                al_draw_text(font, (ALLEGRO_COLOR) { 255, 0, 0, 1 }, 20, 60, ALLEGRO_ALIGN_CENTRE, bulletsText);
                al_draw_bitmap_region(ammo, 0, 32, 32, 32, 30, 52, 0);
                for (int i = 0; i < configs.personagens[0].life; i++) {
                    al_draw_bitmap(life, i * 36 + 5, 20, 0);
                }
                //dialogos
                if (!printDialog(configs, event, mouseX, mouseY)) {
                    //move o personagem
                    moveCharacter(event, &configs.personagens[0], timer);
                    //atira
                    if (keyDown(event, ALLEGRO_KEY_P)) {
                        fire(&configs.objetos, &configs.objetosLength, person, event, 25, timer);
                    }
                    for (int i = 1; i < configs.personagensLength; i++) {
                        if (configs.personagens[i].active) {
                            moveEnemys(event, configs.personagens[0], &configs.personagens[i], configs);
                            for (int j = 1; j < configs.personagensLength; j++) {
                                if (j != i) {
                                    colision(&configs.personagens[i], &configs.personagens[j], 3, &configs.cameraX);
                                }
                            }
                            float distanceX = fabs(configs.personagens[0].x - configs.personagens[i].x);
                            if (distanceX <= RANGE_COMBAT) {
                                fire(&configs.objetos, &configs.objetosLength, &configs.personagens[i], event,
                                    configs.personagens[i].typeCharacter == 2 ? 25 : 50, timer);
                            }
                            if (colision(&configs.personagens[0], &configs.personagens[i], 1, &configs.cameraX))
                                break;


                        }
                    }
                } else {
                    for (int i = 0; i < configs.personagensLength; i++) {
                        configs.personagens[i].vx = 0;
                        configs.personagens[i].vy = 0;
                    }

                }
                
                int characterInactives = 0;
                for (int i = 0; i < configs.personagensLength; i++) {
                    if (configs.personagens[i].active) {
                        updatePhisics(&configs.personagens[i], &configs);
                        print(&configs.personagens[i], configs.cameraX);
                        updateSprites(timer, &configs.personagens[i]);
                    }
                    else {
                        if (configs.personagens[i].typeCharacter == 1)
                            gameIsActive = false;
                        if (configs.personagens[i].typeCharacter == 5) {
                            gameIsActive = false;
                            configs.complete = true;
                        }
                            
                        characterInactives--;
                    }
                }
                resizeCharacters(&configs.personagens, characterInactives, &configs.personagensLength);
                if (configs.objetosLength > 0) {
                    int bulletsInactive = 0;
                    //array de objetos tipos 3 e 4 são balas
                    for (int j = 0; j < configs.objetosLength; j++) {
                        if(configs.objetos[j].typeCharacter == 3 || configs.objetos[j].typeCharacter == 4)
                            updatePhisics(&configs.objetos[j], &configs);

                        print(&configs.objetos[j], configs.cameraX);
                        updateSprites(timer, &configs.objetos[j]);
                        if (configs.objetos[j].x > DISPLAY_WIDTH || configs.objetos[j].x < 0)
                            destroyCharacter(&configs.objetos[j]);
                        if (configs.objetos[j].typeCharacter == 3) {
                            for (int i = 1; i < configs.personagensLength; i++) {
                                if (configs.objetos[j].active) {
                                    if (configs.personagens[i].active && colision(&configs.personagens[i], &configs.objetos[j], 2, &configs.cameraX)) {
                                        destroyCharacter(&configs.objetos[j]);
                                    }
                                }
                            }
                        }
                        else if (configs.objetos[j].typeCharacter == 4 &&
                            configs.personagens[0].active &&
                            colision(&configs.personagens[0], &configs.objetos[j], 2, &configs.cameraX)) {
                            destroyCharacter(&configs.objetos[j]);
                        }

                        if (!configs.objetos[j].active)
                            bulletsInactive--;
                    }
                    resizeCharacters(&configs.objetos, bulletsInactive, &configs.objetosLength);
                }
            }
            else {
                for (int i = 0; i < menuLen; i++)
                    menuScreens[i] = false;
                configs.stage = 0;
            }
        }
        al_flip_display();
    }
    if (menuScreens[0]) {
        for (int i = 0; i < configs.personagensLength; i++) {
            destroyCharacter(&configs.personagens[i]);
        }
        for (int j = 0; j < configs.objetosLength; j++)
            destroyCharacter(&configs.objetos[j]);
        for (int k = 0; k < configs.cenariosLength; k++)
            al_destroy_bitmap(configs.cenarios[k].sprite);
        free(configs.personagens);
        free(configs.objetos);
        free(configs.dialogs);
        free(configs.cenarios);
        free(configs.itens);
    }
    // bitmaps
    al_destroy_bitmap(background);
    al_destroy_bitmap(life);
    al_destroy_bitmap(ammo);
    // fontes
    al_destroy_font(titleFont);
    al_destroy_font(optionsFont);
    al_destroy_font(font);

    // música e Allegro
    al_destroy_audio_stream(musica);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}
