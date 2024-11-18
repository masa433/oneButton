/*����Ȃ�����
�E���U���g��ʂɍs������i�����player�̂ق��ɍ��j
�E�w�i�i�Q�[����ʂ𔖈Â����Ďg���̂�����j
�Escore�̃v���X�}�C�i�X�iring��player?�j
�E�����N�̕]���摜�i����E�������Ă�j
(S:���@A:�@B:�Ԏ��@C:�΁@D:�I�����W�@E:�D�j
�EBGM
�ERESTART�{�^��
*/

#include "scene_result.h"
#include "common.h"
#include "system.h"
//#include "player.cpp"


using namespace input;
using namespace std;

extern int result_state;
int result_timer;

//extern int score;

//int score=0;

//Sprite* sprScore;
Sprite* sprRestart;

Restart restart;

void result_init()
{
	result_state = 0;
	result_timer = 0;
	
}

void result_deinit()
{
	//safe_delete(sprScore);
	safe_delete(sprRestart);
}

void result_update()
{
	switch (result_state)
	{
	case 0:
		//sprScore = sprite_load();
		sprRestart = sprite_load(L"./Data/Images/�^�C�g���֖߂�.png");
		result_state++;
	case 1:
		restart.position = { SCREEN_W * 0.5f, SCREEN_H * 0.9f };  // ���S�ʒu
		restart.scale = { 1.0f, 1.0f };
		restart.texPos = { 0, 0 };
		restart.texSize = { RESTART_TEX_W, RESTART_TEX_H };
		restart.pivot = { RESTART_PIVOT_X, RESTART_PIVOT_Y };
		restart.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		result_state++;
	case 2:
	/*	scored = score;*/
		result_click_act();
		break;

	case 3:
		result_fadeOut_act();
		break;
	}
	result_timer++;
}
void result_render()
{
	GameLib::clear(0.0, 0.0, 0.0);
	sprite_render(sprRestart, restart.position.x, restart.position.y, restart.scale.x, restart.scale.y,
		restart.texPos.x, restart.texPos.y, restart.texSize.x, restart.texSize.y,
		restart.pivot.x, restart.pivot.y);
	//sprite_render = (sprScore, 0, 0, 1, 1, 0, 0, 1920, 1080, 1, 1, 1, 1);
	text_out(6, "result", 100, 100, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	text_out(6, "SCORE", 600, 500, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	text_out(6, "RANK", 600, 600, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	
	text_out(6, "RESTART", 1650, 1000, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	
	/*if (score < 2000)
	{
	text_out(6, "E", 900, 600, 2, 2, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
	}*/

	/*else if (score >= 2000 && score <= 3500)
	{
		D
	}

	else if (score >= 3500 && score <= 5000)
	{
	
	}	C

	else if (score >= 5000 && score <= 6500)
	{
		B
	}

	else if (score >= 6500 && score <= 9500)
	{
		A
	}

	else if ( score >= 9500)
	{
		S
	}
	*/
	//if (player.position.y > 1080) {

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

	primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, restart.result_fadeout);
}

// �t�F�[�h�A�E�g
void result_fadeOut_act()
{
	if (restart.isResult_Fadeout)
	{
		restart.click_delay_timer += 0.03f;
		if (restart.click_delay_timer >= 2.0f)
		{
			restart.result_fadeout += 0.05f;
			if (restart.result_fadeout >= 1.0f)
			{
				restart.result_fadeout = 1.0f;
				restart.result_fadeTimer += 0.1f;
				if (restart.result_fadeTimer >= 7.0f)
				{
					title_start();
					restart.result_fadeTimer = 0.0f;
					result_state = 0;
				}
			}
		}
	}
}

void result_click_act() 
{
	if (restart.clickCount == 0 && (TRG(0) & L_CLICK) && result_click())
	{
		// �N���b�N����Ă��Ȃ� && �t�F�[�h�A�E�g���Ă��Ȃ� && ���N���b�N���͈͓��Ŕ��������ꍇ
		restart.isClicked = true;
		restart.clickCount = 1; // �N���b�N�J�E���g�𑝂₷
		restart.scale = { 1.0f, 1.0f }; // �X�P�[��������������
		
	}

	if (restart.isClicked)
	{
		restart.clickTimer += 0.2f;

		if (restart.clickTimer >= 2.0f)  // �N���b�N�^�C�}�[��2�b�ȏ�ɂȂ�����
		{
			// �X�P�[����傫������
			restart.scale = { 1.3f, 1.3f };
		}

		if (restart.clickTimer >= 5.0f)  // 5�b�o�߂Ńt�F�[�h�A�E�g������
		{
			restart.isResult_Fadeout = true;
			restart.clickTimer = 0.0f;
			result_state++;
		}
	}


	else if (restart.clickCount == 0 && !result_click()) // �N���b�N����Ă��Ȃ� && �͈͊O�̏ꍇ
	{
		restart.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�ɖ߂�
	}

	if (restart.clickCount == 0 && result_click()) // �N���b�N����Ă��Ȃ� && �͈͓��̏ꍇ
	{
		restart.scale = { 1.3f, 1.3f }; // �X�P�[����傫������
	}
}

bool result_click() 
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window::getHwnd(), &point);

	float button_right = restart.position.x + (restart.texSize.x * restart.scale.x) / 2;
	float button_left = restart.position.x - (restart.texSize.x * restart.scale.x) / 2;
	float button_top = restart.position.y - (restart.texSize.y * restart.scale.y) / 1.5;
	float button_bottom = restart.position.y + (restart.texSize.y * restart.scale.y) / 3.1;

	bool isWithinX = (point.x >= button_left && point.x <= button_right);
	bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

	return isWithinX && isWithinY;
}