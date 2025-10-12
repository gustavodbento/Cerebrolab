
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
    character person1 = { 10.0f,DISPLAY_HEIGHT - 104.0f,128.0f, 128.0f, al_load_bitmap("assets/images/principal.png"),0,3,0,0,0,false,0,0,false};
    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue,&event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        al_clear_to_color(al_map_rgb(0, 0, 0));
        moveCharacter(event, &person1, timer);
        updatePhisics(&person1);
        print(&person1);
        al_flip_display();
    }
    destroyCharacter(&person1);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
