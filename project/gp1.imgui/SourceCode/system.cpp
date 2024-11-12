#include"system.h"
#include"common.h"

using namespace input;



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
}
void title_start() 
{
	nextScene = SCENE_TITLE;
}