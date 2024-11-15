#pragma once
#ifndef SCENE_TUTO_H
#define SCENE_TUTO_H

#define BUTTON_TEX_W   103
#define BUTTON_TEX_H   192
#define BUTTON_PIVOT_X (BUTTON_TEX_W/2)
#define BUTTON_PIVOT_Y (BUTTON_TEX_H/2)

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

	float scroll_x; // ���X�N���[���̈ʒu
	float click_delay_timer = 0.0f; // �N���b�N��̃t�F�[�h�A�E�g�x���^�C�}�[
	int currentPage;
};

class TutoButton
{
public:
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;

	bool isClicked; // �{�^�����N���b�N���ꂽ��

};

void tuto_init();
void tuto_update();
void tuto_render();
void tuto_deinit();

void fadeIn_act();
void fadeOut_act();

void tuto_click_act();
bool tuto_click();



#endif//SCENE_TUTO_H