#include"system.h"
#include"common.h"

using namespace input;

//ここにシーン遷移を書いていく

void game_start() {
	nextScene = SCENE_GAME;
}
void tuto_start() {
	nextScene = SCENE_TUTO;
}