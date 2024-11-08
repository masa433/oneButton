#include "common.h"
#include "scene_tuto.h"
#include "system.h"
using namespace std;

int tuto_state;
int tuto_timer;
float tuto_fadein;

bool isTuto_Fadein;

Sprite* sprBack;
Sprite* sprTuto;

float scroll_x; // ���X�N���[���̈ʒu

void tuto_init()
{
    tuto_timer = 0;
    tuto_state = 0;
    scroll_x = 0.0f; // �X�N���[���ʒu�̏�����
    tuto_fadein = 1.0f;

    isTuto_Fadein = false;
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
                tuto_state++;   // ���̏�Ԃɐi��
            }
        }

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

    // �w�i���X�N���[��
    sprite_render(sprBack, -scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // 1����
    sprite_render(sprBack, -scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // 2���ځi�J��Ԃ��j

    // �`���[�g���A���摜�̕`��i�t�F�[�h�C���ƃX�P�[�����O�j
    VECTOR2 scale = { 2.0f + (1.0f * (1.0f - tuto_fadein)), 1.5f + (1.0f * (1.0f - tuto_fadein)) }; // �X�P�[�����t�F�[�h�C���ɍ��킹�đ傫������
    sprite_render(sprTuto, SCREEN_W * 0.5f, SCREEN_H * 0.4f, scale.x, scale.y, 0, 0, 640, 500, 640 / 2, 500/2);

    // �t�F�[�h�C���̋�`�`��
    if (tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, tuto_fadein);
    }
}

void tuto_deinit()
{
    safe_delete(sprBack);
}
