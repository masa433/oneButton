#include"system.h"
#include"common.h"
#include "scene_game.h"
#include "scene_result.h"
#include "scene_tuto.h"

using namespace input;

GAME Game;
TUTO Tuto;

int result_state;


void game_start()
{
	nextScene = SCENE_GAME;
}
void tuto_start()
{
	nextScene = SCENE_TUTO;
}
void result_start() 
{
	nextScene = SCENE_RESULT;
	result_init();
}
void title_start() 
{
	nextScene = SCENE_TITLE;
	

}