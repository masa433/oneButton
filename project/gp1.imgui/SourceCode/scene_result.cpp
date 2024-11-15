#include "scene_result.h"
#include "common.h"
#include "system.h"
//#include "player.cpp"


using namespace input;
using namespace std;

int result_state;
int result_timer;

//extern int score;

int scored;

Sprite* sprScore;

void result_init()
{
	result_state = 0;
	result_timer = 0;
}

void result_deinit()
{
	//safe_delete(sprScore);
}

void result_update()
{
	switch (result_state)
	{
	case 0:
		//sprScore = sprite_load();
		result_state++;
	case 1:
		result_state++;
	case 2:
		//scored = score;
		if (TRG(0) & L_CLICK)
		{
			title_start();
		}
		break;
	}
	result_timer++;
}
void result_render()
{
	GameLib::clear(0.0, 0.0, 0.0);
	text_out(6, "result", 100, 100, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	text_out(6, "score", 600, 500, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	text_out(6, "rank", 600, 600, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	//if (player.position.y > 1080) {
	//	//sprite_render = (sprScore, 0, 0, 1, 1, 0, 0, 1920, 1080, 1, 1, 1, 1);

	//	std::to_string(score);
	//	font::textOut(
	//		4,
	//		std::to_string(score),
	//		VECTOR2(450, 400),
	//		VECTOR2(4.0f, 4.0f),
	//		VECTOR4(1, 1, 1, 1)
	//	);

	//	if (score < 5000) {
	//		font::textOut(
	//			4, "D", VECTOR2(SCREEN_W * 0.48f, 550), VECTOR2(4.0f, 4.0f), VECTOR4(1, 1, 1, 1)
	//		);
	//	}
	//	else if (score >= 5000 && score < 10000) {
	//		font::textOut(
	//			4, "C", VECTOR2(SCREEN_W * 0.48f, 550), VECTOR2(4.0f, 4.0f), VECTOR4(1, 1, 1, 1)
	//		);
	//	}
	//	else if (score >= 10000 && score < 15000) {
	//		font::textOut(
	//			4, "B", VECTOR2(SCREEN_W * 0.48f, 550), VECTOR2(4.0f, 4.0f), VECTOR4(1, 0, 0, 1)
	//		);
	//	}
	//	else if (score >= 15000 && score < 20000) {
	//		font::textOut(
	//			4, "A", VECTOR2(SCREEN_W * 0.48f, 550), VECTOR2(4.0f, 4.0f), VECTOR4(1, 0, 0, 1)
	//		);
	//	}
	//	else if (score >= 23000) {
	//		font::textOut(
	//			4, "S", VECTOR2(SCREEN_W * 0.48f, 550), VECTOR2(4.0f, 4.0f), VECTOR4(1, 1, 0, 1)
	//		);
	//	}
	//}
}