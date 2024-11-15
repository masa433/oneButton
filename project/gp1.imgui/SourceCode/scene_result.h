#ifndef SCENE_RISULT_H
#define SCENE_RISULT_H

#include "../GameLib/vector.h"
#include "../GameLib/sprite.h"
#include "../GameLib/util.h"

#define RESTART_TEX_W   390
#define RESTART_TEX_H   100
#define RESTART_PIVOT_X (RESTART_TEX_W/2)
#define RESTART_PIVOT_Y (RESTART_TEX_H/2)

class Restart
{
public:
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;

	bool isClicked; // ボタンがクリックされたか
	float clickTimer;
	int clickCount;

	
	float result_fadeout;
	float result_fadeTimer;
	float click_delay_timer = 0.0f;
	
	bool isResult_Fadeout;
};

void result_init();
void result_deinit();
void result_update();
void result_render();

void result_click_act();
bool result_click();
void result_fadeOut_act();

#endif

