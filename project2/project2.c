
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <allegro5/allegro_image.h>
#include "character.h"
#include "utils.h"
int main()
{
    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    ALLEGRO_DISPLAY* display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    character characters[2] = {
        { 10.0f,DISPLAY_HEIGHT - 104.0f,128.0f, 128.0f, al_load_bitmap("assets/images/principal.png"),0,3,0,0,0,false,0,0,0,0,0 },
        { DISPLAY_WIDTH - 128.0f,DISPLAY_HEIGHT - 104.0f,128.0f, 128.0f, al_load_bitmap("assets/images/principal.png"),0,3,0,0,0,false,0,0,0,0,0 }
    };
    ALLEGRO_BITMAP* background = al_load_bitmap("assets/images/fundo1.png");
    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue,&event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        //al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(background, 0, 0, 1536, 1024, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,0);
        moveCharacter(event, &characters[0]);
        for (int i = 1; i < 2; i++) {
            moveEnemys(event, characters[0], &characters[i]);
            if (characters[0].vx != 0) {
                colision(&characters[0], &characters[i], 1);
            }
        }
        for (int i = 0; i < 2; i++) {
            updatePhisics(&characters[i]);
            print(&characters[i]);
            updateSprites(timer, &characters[i]);
        }
        al_flip_display();
    }
    for (int i = 0; i < 2; i++) {
        destroyCharacter(&characters[i]);
    }
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
