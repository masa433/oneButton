#include "player.h"
#include "common.h"

using namespace input;

int player_state;
PLAYER player;
Sprite* sprPlayer;
POINT point; // �}�E�X�J�[�\���̈ʒu���i�[


const float COUNTDOWN_DURATION = 3.0f; // 3 seconds for countdown

float countdown_timer = COUNTDOWN_DURATION; // Start at 3 seconds

// ���`��Ԋ֐� (lerp)
float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

//--------------------------------------
//  �v���C���[�̏����ݒ�
//--------------------------------------
void player_init()
{
    player_state = 0;
    player.velocityY = 0.0f; // Initialize vertical velocity to 0
    countdown_timer = COUNTDOWN_DURATION; // Reset countdown timer

    // �}�E�X�J�[�\�����\���ɂ���
    // ShowCursor(FALSE);

    // ��ʒ����̍��W���v�Z
    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;

    // �}�E�X�J�[�\������ʒ����Ɉړ�
    SetCursorPos(centerX, centerY);

    // �v���C���[�̈ʒu����ʒ����ɐݒ�
    player.position = { static_cast<float>(centerX), static_cast<float>(centerY) };
}

//--------------------------------------
//  �v���C���[�̏I������
//--------------------------------------
void player_deinit()
{
    safe_delete(sprPlayer);

    // �}�E�X�J�[�\�����ĕ\������
    ShowCursor(TRUE);
}

//--------------------------------------
//  �v���C���[�̍X�V����
//--------------------------------------
void player_update()
{
    

    switch (player_state)
    {
    case 0:
        //////// �����ݒ� ////////
        sprPlayer = sprite_load(L"./Data/Images/player.png");
        ++player_state;
        /*fallthrough*/

    case 1:
        //////// �p�����[�^�̐ݒ� ////////
        player.angle = ToRadian(0);
        player.scale = { 0.5f, 0.5f };
        player.texPos = { 0, 0 };
        player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
        player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
        player.color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ++player_state;
        /*fallthrough*/

    case 2:
        // Update countdown timer
            if (countdown_timer > 0) {
                countdown_timer -= 60*3; // Decrease countdown
                if (countdown_timer <= 0) {
                    countdown_timer = 0; // Ensure it doesn't go below 0
                }
            }

    case 3:
        //////// �ʏ펞 ////////
        player_act();
        break;
    }
}

//--------------------------------------
//  �v���C���[�̕`�揈��
//--------------------------------------
void player_render()
{
    sprite_render(
        sprPlayer,
        player.position.x, player.position.y,
        player.scale.x, player.scale.y,
        player.texPos.x, player.texPos.y,
        player.texSize.x, player.texSize.y,
        player.pivot.x, player.pivot.y,
        player.angle,
        player.color.x, player.color.y, player.color.z, player.color.w
    );

    // Optional: Display countdown on screen
    if (countdown_timer > 0) {
        text_out(6,"Starting in: " + std::to_string(static_cast<int>(countdown_timer) + 1), 100, 50); // Position and countdown display
    }
}

//--------------------------------------
//  �v���C���[�̍s������
//--------------------------------------
void player_act()
{
    // Check if countdown is still active
    if (countdown_timer > 0) {
        return; // If countdown is active, don't execute movement
    }

    // �}�E�X�J�[�\���̌��݈ʒu���擾
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // ��ԗ� (0.0 - 1.0�͈̔͂ő��x�𒲐��A0.1�ł������A1.0�ő���)
    float interpolationSpeed = 0.05f;

    // �v���C���[�̈ʒu���}�E�X�J�[�\���̈ʒu�Ɍ������Ă������ړ�
    player.position.x = lerp(player.position.x, static_cast<float>(point.x), interpolationSpeed);

    // ��ɏd�͂�������
    player.speed.y += GRAVITY;

    // ���N���b�N�ŕ����オ��
    if (TRG(0) & L_CLICK) {
        player.speed.y = -FLOAT_STRENGTH;
    }

    // �ő嗎�����x�̐���
    if (player.speed.y > MAX_FALL_SPEED) {
        player.speed.y = MAX_FALL_SPEED;
    }

    // �v���C���[��Y�̈ʒu���X�V
    player.position.y += player.speed.y;

    // �n�ʂɓ��B�����Ƃ��̏����i��ʉ��[�Ŏ~�܂�j
    if (player.position.y > SCREEN_H - PLAYER_TEX_H * player.scale.y) {
        player.position.y = SCREEN_H - PLAYER_TEX_H * player.scale.y;
        player.speed.y = 0.0f; // �~�܂�
    }
}
