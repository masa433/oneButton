#include "common.h"
#include "scene_tuto.h"
#include "system.h"
using namespace std;
using namespace input;

int tuto_state;
int tuto_timer;
float tuto_fadein;
float tuto_fadeout;
float tuto_fadeTimer;

bool isTuto_Fadein;
bool isTuto_Fadeout;

Sprite* sprBack;
Sprite* sprTuto;



float scroll_x; // ���X�N���[���̈ʒu
float click_delay_timer = 0.0f; // �N���b�N��̃t�F�[�h�A�E�g�x���^�C�}�[

void tuto_init()
{
    tuto_timer = 0;
    tuto_state = 0;
    scroll_x = 0.0f; // �X�N���[���ʒu�̏�����
    tuto_fadein = 1.0f;
    tuto_fadeout = 0.0f;

    isTuto_Fadein = false;
    isTuto_Fadeout = false; // �t�F�[�h�A�E�g���J�n���Ă��Ȃ����
    click_delay_timer = 0.0f; // �^�C�}�[�̏�����
    tuto_fadeTimer = 0.0f;
}

void tuto_update()
{
    switch (tuto_state)
    {
    case 0:
        sprBack = sprite_load(L"./Data/Images/back(��).png");
        sprTuto = sprite_load(L"./Data/Images/tuto.png");
        tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        tuto_state++;
        /*fallthrough*/
    case 2:
        fadeIn_act();
        break;

    case 3:
        // ���N���b�N�������ꂽ��t�F�[�h�A�E�g�x���^�C�}�[���J�n
        if (TRG(0) & L_CLICK)
        {
            isTuto_Fadeout = true;
            click_delay_timer = 0.0f; // �^�C�}�[�����Z�b�g
            tuto_state++;
        }
        break;

    case 4:
        fadeOut_act();
        break;
    }

    // �X�N���[������
    scroll_x += 0.5f; // �X�N���[�����x
    if (scroll_x >= 5464) { // �摜�̕��Ń��[�v
        scroll_x = 0.0f; // ���[�v�̏�����
    }

    tuto_timer++;
}

void tuto_render()
{
    GameLib::clear(1.0, 0.0, 0.0);

    // �w�i�X�N���[��
    sprite_render(sprBack, -scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // First part
    sprite_render(sprBack, -scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // Repeated part

    // �t�F�[�h�C���E�t�F�[�h�A�E�g�ɉ����ăX�P�[���ύX
    float scale_up = 1.0f + (1.0f - tuto_fadein); // �t�F�[�h�C���ŃX�P�[����傫������
    float scale_down = 1.0f - tuto_fadeout * 0.5f; // �t�F�[�h�A�E�g�ŃX�P�[��������������
    VECTOR2 scale = { 1.5f * scale_up * scale_down ,1.5f * scale_up * scale_down };//�ŏI�I�ȃX�P�[��
    

    // �����摜�̕`��
    sprite_render(sprTuto, SCREEN_W * 0.5f, SCREEN_H * 0.4f, scale.x, scale.y, 0, 0, 640, 500, 640 / 2, 500 / 2);

    // �t�F�[�h�C���̎l�p�`�`��
    if (tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, tuto_fadein);
    }

    // �t�F�[�h�A�E�g�̎l�p�`�`��
    if (tuto_fadeout > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, tuto_fadeout);
    }
}


void tuto_deinit()
{
    safe_delete(sprBack);
    safe_delete(sprTuto);
}

void fadeIn_act() 
{
    if (!isTuto_Fadein)
    {
        isTuto_Fadein = true; // �t�F�[�h�C���J�n
    }

    if (isTuto_Fadein)
    {
        tuto_fadein -= 0.03f; // �t�F�[�h�C���̑��x
        if (tuto_fadein <= 0.0f)
        {
            tuto_fadein = 0.0f;
            isTuto_Fadein = false;
            tuto_state++;
        }
    }
}

void fadeOut_act() 
{
    // �x���^�C�}�[����莞�ԂɒB������t�F�[�h�A�E�g�J�n
        if (isTuto_Fadeout)
        {
            click_delay_timer += 0.03f; // �^�C�}�[�̑���
            if (click_delay_timer >= 2.0f) // 2�b�̒x��
            {
                tuto_fadeout += 0.05f; // �t�F�[�h�A�E�g�̑��x
                if (tuto_fadeout >= 1.0f)
                {
                    tuto_fadeout = 1.0f;
                    tuto_fadeTimer += 0.1f;
                    if (tuto_fadeTimer >= 10.0f) {//�t�F�[�h�A�E�g���Ă���10�o�߂�����
                        
                        game_start(); // �t�F�[�h�A�E�g������A�Q�[����ʂɈڍs
                        tuto_fadeTimer = 0.0f;
                    }
                    
                }
            }
        }
}
