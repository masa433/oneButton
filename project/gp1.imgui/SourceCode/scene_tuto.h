#pragma once
#ifndef SCENE_TUTO_H
#define SCENE_TUTO_H

class TUTO 
{
public:
	int tuto_state;
	int tuto_timer;
	float tuto_fadein;
	float tuto_fadeout;
	float tuto_fadeTimer;

	bool isTuto_Fadein;
	bool isTuto_Fadeout;

	float scroll_x; // 横スクロールの位置
	float click_delay_timer = 0.0f; // クリック後のフェードアウト遅延タイマー
};

void tuto_init();
void tuto_update();
void tuto_render();
void tuto_deinit();

void fadeIn_act();
void fadeOut_act();



#endif//SCENE_TUTO_H