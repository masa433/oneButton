#include "button.h"
#include "system.h"
#include "common.h"

using namespace input;

int title_button_state;

StartButton Start;
Sprite* sprStart;

void title_button_init()
{
    title_button_state = 0;
}

void title_button_deinit()
{
    safe_delete(sprStart);
}

void title_button_update()
{
    switch (title_button_state)
    {
    case 0:
        // �����ݒ�
        sprStart = sprite_load(L"./Data/Images/start(��).png");

        ++title_button_state;
        /*fallthrough*/

    case 1:
        // �p�����[�^�̐ݒ�
        Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };  // ���S�ʒu
        Start.scale = { 1.0f, 1.0f };
        Start.texPos = { 0, 0 };
        Start.texSize = { TITLE_TEX_W, TITLE_TEX_H };
        Start.pivot = { TITLE_PIVOT_X, TITLE_PIVOT_Y };
        Start.color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ++title_button_state;
        /*fallthrough*/

    case 2:
        // �ʏ펞�̏���
        title_button_act();
        break;
    }
}

void title_button_render()
{
    // �{�^���̃T�C�Y�ɉ������X�v���C�g�̕`��
    sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
        Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
        Start.pivot.x, Start.pivot.y);

}

void title_button_act() //�N���b�N�����Ƃ��̓���
{
    if (TRG(0) & L_CLICK)
    {
        if (click()) {
            game_start();
        }
    }

    // �}�E�X���{�^���ɐG�ꂽ��T�C�Y��ύX
    if (click())
    {
        Start.scale = { 1.3f, 1.3f };  // �G�ꂽ���̊g��T�C�Y
    }
    else
    {
        Start.scale = { 1.0f, 1.0f };  // �ʏ�T�C�Y
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
