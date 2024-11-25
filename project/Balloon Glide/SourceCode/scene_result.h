#pragma once

#include "../GameLib/vector.h"
#include "../GameLib/sprite.h"
#include "../GameLib/util.h"

#define RESTART_TEX_W   390
#define RESTART_TEX_H   100
#define RESTART_PIVOT_X (RESTART_TEX_W/2.0f)
#define RESTART_PIVOT_Y (RESTART_TEX_H/2.0f)

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
	float result_fadein;
	bool isResult_Fadein;
};

void result_init();
void result_deinit();
void result_update();
void result_render();

void result_click_act();
bool result_click();
void result_fadeOut_act();
void restart_reset();



