#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include "character.h"
#include <string.h>
#include "structs.h"
//carrega as configurações iniciais da fase
cenario createCenario(float x, float y, float width, float height,
	float spriteX, float spriteY,
	float spriteWidth, float spriteHeight,
	char* sprite, float partition, float stageWidth, int type, bool random) {

	cenario novo_cenario;
	int randomX = rand() % (int)stageWidth+1;
	novo_cenario.x = random ? randomX : x;
	novo_cenario.y = y;
	novo_cenario.width = width;
	novo_cenario.height = height;

	novo_cenario.spriteX = spriteX;
	novo_cenario.spriteY = spriteY;
	novo_cenario.spriteWidth = spriteWidth;
	novo_cenario.spriteHeigth = spriteHeight;
	novo_cenario.partition = partition;
	novo_cenario.sprite = al_load_bitmap(sprite);
	novo_cenario.type = type;
	return novo_cenario;
}
bool reallocCenarios(cenario **cenarios, int newSize) {
	cenario* temp = realloc(*cenarios, newSize * sizeof(cenario));
	if(temp != NULL)
	*cenarios = temp;
	return temp != NULL;
}
char* enemySpr1 = "assets/images/enemy_1.png";
char* enemySpr2 = "assets/images/enemy_2.png";
char* personagens[2] = {
	"assets/images/principal.png","assets/images/principal_1.png"
};
void changeStage(ALLEGRO_BITMAP **background, stageCfg* configs, int stage) {
	al_destroy_bitmap(*background);
	char backgroundPath[50] = "assets/images/";
	if (configs->initialized && !configs->inMenu) {
		for (int i = 0; i < configs->personagensLength; i++) {
			destroyCharacter(&configs->personagens[i]);
		}
		for (int j = 0; j < configs->objetosLength; j++) {
			destroyCharacter(&configs->objetos[j]);
		}
		for (int k = 0; k < configs->cenariosLength; k++)
			al_destroy_bitmap(configs->cenarios[k].sprite);
		free(configs->personagens);
		free(configs->objetos);
		free(configs->dialogs);
		free(configs->cenarios);
		configs->cenariosLength = 0;
		configs->objetosLength = 0;
		configs->personagensLength = 0;
		configs->dialogsLength = 0;
	}
	switch (stage) {
		case 1:
			strcpy_s(configs->background, sizeof(configs->background), "floresta/bgcolor.png");
			configs->personagensLength = 10;
			configs->personagens = malloc(configs->personagensLength * sizeof(character));
			configs->cenariosLength = 50;
			configs->cenarios = malloc(configs->cenariosLength * sizeof(cenario));
			configs->backgroundWidth = 3560;
			configs->cenarios[0] = createCenario(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 240, 160,
				"assets/images/floresta/Tlayer1.png",0, configs->backgroundWidth,1,false);
			configs->cenarios[1] = createCenario(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 240, 160,
				"assets/images/floresta/Tlayer2.png",0, configs->backgroundWidth,1, false);
			configs->cenarios[2] = createCenario(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 240, 160,
				"assets/images/floresta/Tlayer3.png", 0, configs->backgroundWidth,1, false);
			//arvores fundo
			for (int i = 3; i < 15; i++)
				configs->cenarios[i] = createCenario(0, DISPLAY_HEIGHT - 250, 64, 160, 53, 4, 22, 60,
					"assets/images/floresta/trees.png", 0, configs->backgroundWidth, 2, true);
			//arbustos
			configs->cenarios[15] = createCenario(0, DISPLAY_HEIGHT-128, DISPLAY_WIDTH, 64, 16, 75, 240, 20,
				"assets/images/floresta/trees.png",0, configs->backgroundWidth,1, false);
			//mato
			for (int i = 16; i < 36; i++)
				configs->cenarios[i] = createCenario(0, DISPLAY_HEIGHT - 112, 64, 64, 112, 107, 32, 21,
					"assets/images/floresta/trees.png", 0, configs->backgroundWidth, 2, true);
			//arvores frente
			for(int i = 36;i<configs->cenariosLength-1;i++)
				configs->cenarios[i] = createCenario(0, DISPLAY_HEIGHT - 260, 96, 192, 53, 4, 22, 60
					,"assets/images/floresta/trees.png", 0, configs->backgroundWidth,2,true);
			configs->cenarios[49] = createCenario(0, DISPLAY_HEIGHT - 64, DISPLAY_WIDTH, 128, 62, 160, 52, 52
				,"assets/images/floresta/trees.png", 128, configs->backgroundWidth, 1, false);
			configs->itensLength = 1 + rand() % 8;
			configs->itens = malloc(configs->itensLength * sizeof(cenario));
			for (int i = 0; i < configs->itensLength; i++)
				configs->itens[i] = createCenario(0, DISPLAY_HEIGHT - 112, 48, 48, 0, 32, 32, 32
					,"assets/images/ammo_sprite.png", 0, configs->backgroundWidth, 3, true);
			int numLife = rand() % 5;
			if (reallocCenarios(&configs->itens,configs->itensLength +numLife)) {
				int fistIndex = configs->itensLength;
				configs->itensLength += numLife;
				for (int i = fistIndex; i < configs->itensLength; i++) {
					configs->itens[i] = createCenario(0, DISPLAY_HEIGHT - 112, 48, 48, 0, 0, 32, 32, "assets/images/ammo_sprite.png", 0, configs->backgroundWidth, 4, true);
				}
			}

			configs->personagens[0] = createCharacter(1, 10, 128, 128,200,200,50
				,0,personagens[configs->personSelected], *configs, false, 5);
			configs->personagens[configs->personagensLength - 1] = createCharacter(5, 
				2500
				, 256, 256,80,40,40, configs->personagensLength - 1, 
				"assets/images/right_move_red-Sheet_2.png", *configs, false,2);
			configs->objetosLength = 0;
			configs->objetos = malloc(configs->objetosLength * sizeof(character));
			configs->cameraX = 0;
			configs->dialogsLength = 5;
			configs->dialogs = malloc(configs->dialogsLength * sizeof(dialog));
			
			configs->dialogs[0] = (dialog){ 2, DISPLAY_HEIGHT / 2 - 50
				,"Ardennes: Inverno de Fogo."
				, al_load_ttf_font("assets/fonts/Sono-Bold.ttf", 22, 0), true };
			configs->dialogs[1] = (dialog){ 2, DISPLAY_HEIGHT / 2
				,"Use WSAD para movimentar, P para atirar \n e Espaco para coletar itens."
				, al_load_ttf_font("assets/fonts/Sono-Bold.ttf", 16, 0), true };
			configs->dialogs[2] = (dialog){ 130, DISPLAY_HEIGHT / 2 - 50
				,"Dezembro de 1944. \n O inverno mais frio da guerra cobre a Europa \n com um manto de neve e silencio. \n Mas nas florestas das Ardenas, esse silencio logo se quebra."
				,al_load_ttf_font("assets/fonts/Sono-Bold.ttf", 16, 0), true };
			configs->dialogs[3] = (dialog){250, DISPLAY_HEIGHT / 2 - 50
				,"As forcas alemas lancam um ataque surpresa,\n empurrando as linhas aliadas para tras.\n Cercado pelo frio e pela fumaca,\n voce e um dos poucos que restaram\n para conter o avanco inimigo.\nSem reforcos, sem suprimentos\n apenas coragem e o som distante \ndos tanques ecoando entre as arvores."
				,al_load_ttf_font("assets/fonts/Sono-Bold.ttf", 16, 0), true };
			configs->dialogs[4] = (dialog){ 380, DISPLAY_HEIGHT / 2 - 50
				,"O objetivo e abater um tanque inimigo que esta proximo. \n Cuidado voce ira enfrentar soldados pelo caminho."
				,al_load_ttf_font("assets/fonts/Sono-Bold.ttf", 16, 0), true };
			configs->inMenu = false;
			configs->partitionBackground = 240;
			configs->lastPartition = 0;
			configs->initialized = true;
			configs->lastStage = 1;
			configs->stage = 1;
			configs->complete = false;
		break;
		case 2:
			strcpy_s(configs->background, sizeof(configs->background), "fundo1.png");
		break;
		default:
			strcpy_s(configs->background, sizeof(configs->background), "menu.png");
			configs->cameraX = 0;
			configs->partitionBackground = 1536;
			configs->backgroundWidth = 1536;
			configs->personagensLength = 0;
			configs->objetosLength = 0;
			configs->initialized = true;
			configs->inMenu = true;
			break;
	}
	for (int i = 1; i < configs->personagensLength - 1; i++) {
		int chance = 1 + rand() % 10;
		configs->personagens[i] = createCharacter(2, 10, 128, 128, 200, 200, 50
			, i, chance > 5? enemySpr1: enemySpr2, *configs, true, 1);

	}
		
	strcat_s(backgroundPath, sizeof(backgroundPath), configs->background);
	*background = al_load_bitmap(backgroundPath);
	
}

bool printDialog(stageCfg configs, ALLEGRO_EVENT event, float mouseX, float mouseY) {
	ALLEGRO_FONT* optionsFont = al_load_ttf_font("assets/fonts/metal_slug.ttf", 32, 0);
	ALLEGRO_COLOR fontColorYellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR fontColorWhite = al_map_rgb(255, 255, 255);
	bool isDialogPrint = false;
	button closeDialog = { DISPLAY_WIDTH / 2,200, "Ok", optionsFont, fontColorWhite };
	float lineHeight = 0;
	float y = 0;
	for (int i = 0; i < configs.dialogsLength; i++) {
		dialog* printDialog = &configs.dialogs[i];
		if (printDialog->active && printDialog->x >= configs.cameraX && printDialog->x <= configs.cameraX + 128) {
			isDialogPrint = true;
			char* text = printDialog->text;
			char temp[512];
			strcpy_s(temp, sizeof(temp), text);
			lineHeight = al_get_font_line_height(printDialog->font);
			y = printDialog->y;
			char* context = NULL;
			char* line = strtok_s(temp, "\n", &context);

			while (line != NULL) {

				al_draw_text( 
					printDialog->font,
					al_map_rgb(255, 0, 0),
					DISPLAY_WIDTH / 2,
					y,
					ALLEGRO_ALIGN_CENTRE,
					line
				);
				y += lineHeight;
				line = strtok_s(NULL, "\n", &context);
			}			
		}
	}
	if (isDialogPrint) {
		closeDialog.y = y + lineHeight;
		mouseHoverButton(mouseX, mouseY, &closeDialog, fontColorYellow, fontColorWhite);
		al_draw_text(closeDialog.font, closeDialog.color, closeDialog.x, closeDialog.y, ALLEGRO_ALIGN_CENTRE, closeDialog.text);
		if (mouseClickButton(event, mouseX, mouseY, closeDialog)) {
			for (int i = 0; i < configs.dialogsLength; i++) {
				dialog* printDialog = &configs.dialogs[i];
				if (printDialog->active && printDialog->x >= configs.cameraX && printDialog->x <= configs.cameraX + 128)
					printDialog->active = false;
			}

		}
	}
	al_destroy_font(optionsFont);
	return isDialogPrint;
}