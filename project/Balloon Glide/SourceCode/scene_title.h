#pragma once

#include "../GameLib/vector.h"
#include"../GameLib/sprite.h"


using namespace GameLib;



#define TITLE_TEX_W   390
#define TITLE_TEX_H   100
#define TITLE_PIVOT_X (TITLE_TEX_W/2)
#define TITLE_PIVOT_Y (TITLE_TEX_H/2)

class StartButton {
public:

	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;
	float fadeBlack;
	bool isFadeOut; // フェードアウトしているか
	bool isClicked; // ボタンがクリックされたか
	float fadeTimer;
	float clickTimer;
	int clickCount;
	int title_state;
	int title_timer;
	VECTOR2 titlePos;
	int title_move_timer;
	float title_angle;
	float fadeIn;
	bool isFadeIn; // フェードインしているか
	int fadeAfterTimer;

};

class Balloon {
	
public:
	VECTOR2 position;
	float speed;
	float scale;
	bool active;
};


void click_act();
bool click();


void title_init();
void title_deinit();
void title_update();
void title_render();
void title_act();

void balloon_act();
void title_fadeIn_act();


