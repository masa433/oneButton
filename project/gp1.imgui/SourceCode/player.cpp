#include "player.h"
#include "common.h"

using namespace input;

int player_state;
PLAYER player;
Sprite* sprPlayer;
POINT point; // �}�E�X�J�[�\���̈ʒu���i�[



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
   
    

    // �}�E�X�J�[�\�����\���ɂ���
     //ShowCursor(FALSE);

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
    static int countdownTimer = 60*5; // 4�b�Ԃ̃J�E���g�_�E��(�v���C���[�������Ȃ�����) 

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
        //////// �ʏ펞 ////////
        if (countdownTimer >= 0) {
            countdownTimer--;

            // �}�E�X�J�[�\���̈ʒu���Œ肷��
            int centerX = SCREEN_W / 2;
            int centerY = SCREEN_H / 2;
            SetCursorPos(centerX, centerY);

            // �J�E���g�_�E�����͍s���ł��Ȃ�
        }
        else {
            player_act(); // �J�E���g�_�E�����I��������Ăяo��
        }
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

    
}

//--------------------------------------
//  �v���C���[�̍s������
//--------------------------------------
void player_act()
{
    

    // �}�E�X�J�[�\���̌��݈ʒu���擾
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // ��ԗ� (0.0 - 1.0�͈̔͂ő��x�𒲐��A0.1�ł������A1.0�ő���)
    float lerpSpeed = 0.03f;

    // �v���C���[�̈ʒu���}�E�X�J�[�\���̈ʒu�Ɍ������Ă������ړ�
    player.position.x = lerp(player.position.x, static_cast<float>(point.x), lerpSpeed);
    player.position.y = lerp(player.position.y, static_cast<float>(point.y), lerpSpeed);

    //// ��ɏd�͂�������
    //player.speed.y += GRAVITY;

    //// ���N���b�N�ŕ����オ��
    //if (TRG(0) & L_CLICK) {
    //    player.speed.y = -FLOAT_STRENGTH;
    //}

    //// �������x�̐���
    //if (player.speed.y > MAX_FALL_SPEED) {
    //    player.speed.y = MAX_FALL_SPEED;
    //}

    // �v���C���[��Y�̈ʒu���X�V
    player.position.y += player.speed.y;

    // ��ʏ�[�ɓ��������Ƃ��̏���
    if (player.position.y < 0 + PLAYER_PIVOT_Y / 2) {
        player.position.y = 0 + PLAYER_PIVOT_Y / 2;
        player.speed.y = 0.0f; // �~�܂�
    }

    if (player.position.x > SCREEN_W - PLAYER_PIVOT_X/2) {
        player.position.x = SCREEN_W - PLAYER_PIVOT_X/2 ;
        player.speed.x = 0.0f; // �~�܂�
    }

    if (player.position.x < 0 + PLAYER_PIVOT_X / 2) {
        player.position.x = 0 + PLAYER_PIVOT_X / 2;
        player.speed.x = 0.0f; // �~�܂�
    }
}