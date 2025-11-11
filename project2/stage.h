#ifndef STAGE_H
#define STAGE_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include "structs.h"

void changeStage(ALLEGRO_BITMAP** background, stageCfg *configs, int stage);
void printDialog(stageCfg configs);
#endif