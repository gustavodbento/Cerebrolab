#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include "character.h"

typedef struct {
	character* personagens;
	character* objetos;
	int personagensLength;
	int objetosLength;
}stageCfg;