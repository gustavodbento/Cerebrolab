
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
    ALLEGRO_FONT* font = al_load_ttf_font("assets/fonts/font2.ttf", 24, 0);
    ALLEGRO_AUDIO_STREAM* musica = al_load_audio_stream("assets/sounds/musica_1.mp3", 4, 2048);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_set_window_title(display, "Metal Slug");
    al_start_timer(timer);
    stageCfg configs;
    configs.personagensLength = 2;
    configs.personagens = malloc(configs.personagensLength * sizeof(character));
    configs.personagens[0] = (character) { 10.0f,DISPLAY_HEIGHT - 104.0f,128.0f, 128.0f, al_load_bitmap("assets/images/principal.png"),0,3,0,0,0,false,0,0,0,0,0,200,200,50,true,0,0,true,50 };
        
    configs.personagens[1] = (character){ DISPLAY_WIDTH - 128.0f,DISPLAY_HEIGHT - 104.0f,128.0f, 128.0f, al_load_bitmap("assets/images/principal.png"),0,3,0,0,0,false,0,0,0,0,0,200,200,50,true,0,0,true,50 };
    configs.objetosLength = 0;
    configs.objetos = malloc(configs.objetosLength * sizeof(character));
    char imagesPath[50] = "assets/images/";
    char backgroundPath[50];
    strcpy_s(backgroundPath, sizeof(backgroundPath), imagesPath);
    strcat_s(backgroundPath, sizeof(backgroundPath), backgroundImages[0]);
    ALLEGRO_BITMAP* background = al_load_bitmap(backgroundPath);
    ALLEGRO_COLOR fontColorWhite = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR fontColorYellow = al_map_rgb(255, 255, 0);
    button menuOptions[3] = {
        {DISPLAY_WIDTH / 2,140, "Novo Jogo", optionsFont, fontColorWhite},
        {DISPLAY_WIDTH / 2,170, "Opcoes", optionsFont, fontColorWhite},
        {DISPLAY_WIDTH / 2,200, "Sair", optionsFont, fontColorWhite}
    };
    bool isRunning = true;
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
        
        al_draw_scaled_bitmap(background, 0, 0, 1536, 1024, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
        
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mouseX = event.mouse.x;
            mouseY = event.mouse.y;
        }
        //menu
        if (!menuScreens[0]) {
            al_draw_text(titleFont, fontColorWhite, DISPLAY_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTRE, "Metal Slug");
            if (!menuScreens[1]) {
                for (int i = 0; i < 3; i++) {
                    if (mouseButtonHover(mouseX, mouseY, menuOptions[i])) {
                        menuOptions[i].color = fontColorYellow;
                    }else {
                        menuOptions[i].color = fontColorWhite;
                    }
                    al_draw_text(menuOptions[i].font, menuOptions[i].color, menuOptions[i].x, menuOptions[i].y, ALLEGRO_ALIGN_CENTRE, menuOptions[i].text);
                    if (mouseButtonHover(mouseX, mouseY, menuOptions[i]) && mouseClick(event)) {
                        if (i == 0) {
                            changeStage(&background,&configs,1);
                            menuScreens[0] = true;
                            break;
                        }
                        if (i == 2) {
                            isRunning = false;
                            break;
                        }
                    }
                    
                   
                }
            }

        }
        if (menuScreens[0]) {
            char bulletsText[10];
            snprintf(bulletsText, sizeof(bulletsText), "%d", configs.personagens[0].bullets);
            al_draw_text(font, fontColorYellow, 20, 20, ALLEGRO_ALIGN_CENTRE, bulletsText);
            moveCharacter(event, &configs.personagens[0], timer);
            fire(&configs.objetos, &configs.objetosLength, &configs.personagens[0], event);
            for (int i = 1; i < configs.personagensLength; i++) {
                if (configs.personagens[i].active) {
                    moveEnemys(event, configs.personagens[0], &configs.personagens[i]);
                    colision(&configs.personagens[0], &configs.personagens[i], 1);
                }
            }
            int characterInactives = 0;
            for (int i = 0; i < configs.personagensLength; i++) {
                if (configs.personagens[i].active) {
                    updatePhisics(&configs.personagens[i]);
                    print(&configs.personagens[i]);
                    updateSprites(timer, &configs.personagens[i]);
                }
                else {
                    characterInactives--;
                }
            }
            resizeCharacters(&configs.personagens, characterInactives, &configs.personagensLength);
            if (configs.objetosLength > 0) {
                int bulletsInactive = 0;
                for (int j = 0; j < configs.objetosLength; j++) {
                    updatePhisics(&configs.objetos[j]);
                    print(&configs.objetos[j]);
                    updateSprites(timer, &configs.objetos[j]);
                    if (configs.objetos[j].x > DISPLAY_WIDTH)
                        destroyCharacter(&configs.objetos[j]);

                    for (int i = 1; i < configs.personagensLength; i++) {
                        if (configs.objetos[j].active) {
                            if (configs.personagens[i].active && colision(&configs.personagens[i], &configs.objetos[j], 2)) {
                                destroyCharacter(&configs.objetos[0]);
                            }
                        }
                    }
                    if (!configs.objetos[j].active)
                        bulletsInactive--;
                }
                resizeCharacters(&configs.objetos, bulletsInactive, &configs.objetosLength);
            }
        }
        al_flip_display();
    }
    for (int i = 0; i < configs.personagensLength; i++) {
        destroyCharacter(&configs.personagens[i]);
    }
    for(int j = 0; j< configs.objetosLength; j++)
        destroyCharacter(&configs.objetos[j]);

    al_destroy_audio_stream(musica);
    al_uninstall_audio();
    free(configs.objetos);
    free(configs.personagens);
    al_destroy_bitmap(background);
    al_destroy_font(titleFont);
    al_destroy_font(optionsFont);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
