#include"scene_title.h"
#include"common.h"
#include"system.h"
#include"button.h"

int title_state;
int title_timer;
float fadeBlack;
bool isFadeOut; // �t�F�[�h�A�E�g���Ă��邩
bool isClicked; // �{�^�����N���b�N���ꂽ��
float fadeTimer;
float clickTimer;

StartButton Start;
Sprite* sprStart;

using namespace input;

void title_init() 
{
	title_state = 0;
	title_timer = 0;
	fadeBlack = 0.0f;
	isFadeOut = false; // �t�F�[�h�A�E�g���Ă��Ȃ�
	isClicked = false; // �N���b�N����Ă��Ȃ�
	fadeTimer = 0.0f;
	clickTimer = 0.0f;
}

void title_deinit()
{
	safe_delete(sprStart);
	title_state = 0;
	title_timer = 0;
}

void title_update() 
{
	
	
		

		switch (title_state)
		{
		case 0:
			//////// �����ݒ� ////////

			sprStart = sprite_load(L"./Data/Images/start(��).png");
			
			title_state++;
			/*fallthrough*/
		case 1:
			//////// �p�����[�^�̐ݒ� ////////
			GameLib::setBlendMode(Blender::BS_ALPHA);

			Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };  // ���S�ʒu
			Start.scale = { 1.0f, 1.0f };
			Start.texPos = { 0, 0 };
			Start.texSize = { TITLE_TEX_W, TITLE_TEX_H };
			Start.pivot = { TITLE_PIVOT_X, TITLE_PIVOT_Y };
			Start.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			
			title_state++;
			/*fallthrough*/
		case 2:
			//////// �ʏ펞 ////////
            title_button_act();
			
			break;
		}

		title_timer++;
	
}

void title_render() 
{
	GameLib::clear(0.0, 0.0, 0.0);

	text_out(6, "title", 100, 100, 1, 1, 1.0f, 1.0f, 1.0f);

	sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
		Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
		Start.pivot.x, Start.pivot.y);
	debug::setString("clickTimer%f", clickTimer);
	debug::setString("faceBlack%f", fadeBlack);
	debug::setString("fadeTimer%f", fadeTimer);
	// ��ʑS�̂Ƀt�F�[�h��K�p
	primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, fadeBlack);

	
}

void title_button_act() // �N���b�N�����Ƃ��̓���
{
    if (!isFadeOut && (TRG(0) & L_CLICK) && click())
    {
        isClicked = true;
    }


    if (isClicked && !isFadeOut)
    {
        clickTimer += 0.1f;

        if (clickTimer >= 10.0f) // �N���b�N����Ă���10.0f�o�߂�����t�F�[�h�A�E�g
        {
            isFadeOut = true;
            clickTimer = 0.0f; // �^�C�}�[���Z�b�g
        }
    }

    if (isFadeOut)
    {
        // �t�F�[�h�A�E�g
        fadeBlack += 0.05f;
        if (fadeBlack >= 1.0f) {
            fadeBlack = 1.0f;
            fadeTimer += 0.1f;

            if (fadeTimer >= 10.0f) {
                game_start(); // ���S�Ƀt�F�[�h�A�E�g������V�[���J��
                fadeTimer = 0.0f;
            }
        }
    }
    else
    {
        // �}�E�X���{�^���ɐG�ꂽ��T�C�Y��ύX
        if (click())
        {
            Start.scale = { 1.3f, 1.3f };  // �G�ꂽ���̃T�C�Y
        }
        else
        {
            Start.scale = { 1.0f, 1.0f };  // �ʏ�T�C�Y
        }
    }
}

bool click() // �}�E�X�N���b�N�̓����蔻��
{
    // �}�E�X�J�[�\���̍��W�擾
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // �{�^���̎l�ӂ��擾 (�T�C�Y�ɃX�P�[�����l��)
    float button_right = Start.position.x + (Start.texSize.x * Start.scale.x) / 2;
    float button_left = Start.position.x - (Start.texSize.x * Start.scale.x) / 2;
    float button_top = Start.position.y - (Start.texSize.y * Start.scale.y) / 2;
    float button_bottom = Start.position.y + (Start.texSize.y * Start.scale.y) / 2;

    // �}�E�X�̍��W���{�^���͈͓̔����`�F�b�N
    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    // �}�E�X���{�^���͈͓��Ȃ�N���b�N���ꂽ�Ɣ���
    return isWithinX && isWithinY;
}

