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
#include "audio.h"

using namespace input;
using namespace std;

extern int result_state;
int result_timer;

// �����O�f�[�^�\��
struct RingInfo {
    Sprite* sprite;     // �����O�̃X�v���C�g
    int count;          // �����O�̃J�E���g
    float posX;         // �`��ʒu X
    float posY;         // �`��ʒu Y
};

RingInfo rings[] = {
    { nullptr, 0, 300, 230 },  // �S�[���h�����O
    { nullptr, 0, 300, 430 },  // ���b�h�����O
    { nullptr, 0, 300, 630 }   // ���C���{�[�����O
};

extern int score;
extern int gold_ring_count;
extern int red_ring_count;
extern int rainbow_ring_count;

int current_display_step = 0; // ���݂̕\���i�K
int display_timer = 0;        // �\���p�^�C�}�[
static float scale = 2.0f;  // �����X�P�[��

Sprite* sprRestart;
Sprite* sprResultback;
Sprite* ringGold;
Sprite* ringRed;
Sprite* ringRainbow;
Sprite* sprRank[6]; // �����N�̃X�v���C�g

Restart restart;

void result_init() {
    result_state = 0;
    result_timer = 0;
    restart.result_fadein = 1.0f;
    restart.isResult_Fadein = true;
    restart.isClicked = false;
    restart.clickCount = 0;
    restart.clickTimer = 0.0f;
    restart.result_fadeout = 0.0f;
    restart.result_fadeTimer = 0.0f;
    scale = 2.0f;
}


void result_deinit() {
    safe_delete(sprRestart);
    safe_delete(sprResultback);
    safe_delete(ringGold);
    safe_delete(ringRed);
    safe_delete(ringRainbow);
    for (int i = 0; i < 6; i++) {
        safe_delete(sprRank[i]);
    }
    music::stop(BGM_RESULT);
    music::stop(BGM_BUTTON);

    result_state = 0;  
}

void result_update() {
    switch (result_state) {
    case 0:
        rings[0].sprite = sprite_load(L"./Data/Images/ring_gold.png");
        rings[1].sprite = sprite_load(L"./Data/Images/ring_red.png");
        rings[2].sprite = sprite_load(L"./Data/Images/ring_rainbow.png");

        rings[0].count = gold_ring_count;
        rings[1].count = red_ring_count;
        rings[2].count = rainbow_ring_count;

        sprRank[0] = sprite_load(L"./Data/Images/S.png");
        sprRank[1] = sprite_load(L"./Data/Images/A.png");
        sprRank[2] = sprite_load(L"./Data/Images/B.png");
        sprRank[3] = sprite_load(L"./Data/Images/C.png");
        sprRank[4] = sprite_load(L"./Data/Images/D.png");
        sprRank[5] = sprite_load(L"./Data/Images/E.png");

        sprRestart = sprite_load(L"./Data/Images/�^�C�g���֖߂�.png");
        sprResultback = sprite_load(L"./Data/Images/title_haikei.png");
        current_display_step = 0;
        display_timer = 0;
        result_state++;
        break;

    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        music::play(BGM_RESULT, true);
        restart.position = { SCREEN_W * 0.5f, SCREEN_H * 0.9f };
        restart.scale = { 1.0f, 1.0f };
        restart.texPos = { 0, 0 };
        restart.texSize = { RESTART_TEX_W, RESTART_TEX_H };
        restart.pivot = { RESTART_PIVOT_X, RESTART_PIVOT_Y };
        restart.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        result_state++;
        break;

    case 2:
        if (restart.isResult_Fadein) {
            restart.result_fadein -= 0.03f;
            if (restart.result_fadein <= 0.0f) {
                restart.result_fadein = 0.0f;
                restart.isResult_Fadein = false;
            }
        }
        display_timer++;

        if (display_timer >= 60) {
            current_display_step++;
            display_timer = 0;
        }

        if (current_display_step > 6) {
            result_click_act();
        }

        break;

    case 3:
        result_fadeOut_act();
        break;
    }

    result_timer++;
}

void result_render() {
    sprite_render(sprResultback, SCREEN_W * 0.5f, SCREEN_H * 0.5f, 0.8f, 0.8f, 0, 0, 2732, 2048, 2732 / 2.0f, 2048 / 2.0f, ToRadian(0), 0.2,0.2,0.2);

    for (int i = 0; i < current_display_step && i < 3; ++i) {
        // Display each ring with the appropriate count
        sprite_render(rings[i].sprite, rings[i].posX, rings[i].posY, 0.5f, 0.5f, 0, 0, 350, 350, 350 / 2.0f, 350 / 2.0f);
        text_out(6, "x  " + to_string(rings[i].count), rings[i].posX + 200, rings[i].posY - 30, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, TEXT_ALIGN::UPPER_LEFT);
    }

    // Display the total score after all rings are shown
    if (current_display_step > 3) {
        text_out(6, "TOTAL SCORE:" + to_string(score), 100, 800, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, TEXT_ALIGN::UPPER_LEFT);
    }


    if (current_display_step > 4) {
        

        // �X�R�A�ɉ����ă����N������
        Sprite* rank_sprite = sprRank[5];  // �f�t�H���g��E�����N
        if (score >= 1000) rank_sprite = sprRank[0];  // S�����N
        else if (score >= 800) rank_sprite = sprRank[1];  // A�����N
        else if (score >= 600) rank_sprite = sprRank[2];  // B�����N
        else if (score >= 400) rank_sprite = sprRank[3];  // C�����N
        else if (score >= 200) rank_sprite = sprRank[4];  // D�����N

        // �����N�X�v���C�g�̕`��i�X�P�[�������������Ă����j
        
        scale -= 0.02f;  // ���X�ɏk��
        if (scale < 0.5f) scale = 0.5f;

        sprite_render(rank_sprite, SCREEN_W * 0.7f, SCREEN_H * 0.5f, scale, scale, 0, 0, 1366, 1024, 1366 / 2.0f, 1024 / 2.0f);
       
    }
    if (current_display_step > 6) 
    {
        sprite_render(sprRestart, restart.position.x, restart.position.y, restart.scale.x, restart.scale.y,
            restart.texPos.x, restart.texPos.y, restart.texSize.x, restart.texSize.y, restart.pivot.x, restart.pivot.y);
    }
 
    
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, restart.result_fadein);
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
		music::play(BGM_BUTTON, false);
		
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