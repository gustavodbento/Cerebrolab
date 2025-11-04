#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include "character.h"
#include <string.h>
typedef struct {
	character *personagens;
	character *objetos;
	int personagensLength;
	int objetosLength;
	char background[50];
}stageCfg;

void changeStage(ALLEGRO_BITMAP **background, stageCfg* configs, int stage) {
	al_destroy_bitmap(*background);
	char backgroundPath[50] = "assets/images/";
	switch (stage) {
		case 1:
			strcpy_s(configs->background, sizeof(configs->background), "fundo1.png");
		break;
		case 2:
			strcpy_s(configs->background, sizeof(configs->background), "fundo1.png");
		break;
	}
	strcat_s(backgroundPath, sizeof(backgroundPath), configs->background);
	*background = al_load_bitmap(backgroundPath);
	
}