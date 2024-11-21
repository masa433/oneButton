#include "player.h"
#include "bird.h"

#include "common.h"
#include "system.h"
#include "Collision.h"
#include"audio.h"
using namespace input;

extern BIRD bird[BIRD_MAX];
extern int score;
int boostCount ;          // �u�[�X�g�񐔂��J�E���g
float boostMultiplier ; // �u�[�X�g���̑��x�{��
int boostFlame;
PLAYER player;

Sprite* sprPlayer;
Sprite* sprFinish;
POINT point; // �}�E�X�J�[�\���̈ʒu���i�[

// ���`��Ԋ֐� (lerp)
float lerp(float start, float end, float t) {
    return start + t * (end - start);
}



void player_init()
{
    player.player_state = 0;

    // �v���C���[�̏����ݒ�
    player.hp = 5;
    player.fadeAlpha = 0.0f;
    player.speed = { 0.0f, 0.0f };
    player.position = { static_cast<float>(SCREEN_W / 2), static_cast<float>(SCREEN_H / 2) };
    player.flashing = false;
    player.flashCounter = 0;
    player.finish_timer = 0.0f;
    score = 0;
    boostCount = 5;
    boostMultiplier = 2.0f;
    boostFlame = 0;
    // ���̏���������
    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;
    SetCursorPos(centerX, centerY);
}


//--------------------------------------
//  �v���C���[�̏I������
//--------------------------------------
void player_deinit()
{
    safe_delete(sprPlayer);
    safe_delete(sprFinish);
    music::stop(BGM_WATER);
    music::stop(BGM_BALLOON);
    // �}�E�X�J�[�\�����ĕ\������
    ShowCursor(TRUE);
}

void player_update()
{
    switch (player.player_state) {
    case 0:
        //////// �����ݒ� ////////
        sprPlayer = sprite_load(L"./Data/Images/player.png");
        sprFinish = sprite_load(L"./Data/Images/finish.png");
        ++player.player_state;
        /*fallthrough*/

    case 1:
        //////// �p�����[�^�̐ݒ� ////////
        player.angle = ToRadian(0);
        player.scale = { 3.0f, 3.0f };
        player.texPos = { 0, 0 };
        player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
        player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
        player.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        player.radius = 120;
        player.offset = { 0, 0 };
        player.flashing = false;
        player.flashCounter = 0;
        player.hp = 5;
        ++player.player_state;
        /*fallthrough*/

    case 2:
        //////// �ʏ펞 ////////
        if (player.flashing) {
            player.flashCounter++;
            if (player.flashCounter >= player.flashtimer) {
                player.flashing = false;
                player.flashCounter = 0;
            }
        }
        player_act();
        break;

    
    }
}

// �v���C���[�̕`�揈��
void player_render()
{
    // �v���C���[���t���b�V�����łȂ���Ε`��
    if (!(player.flashing && (player.flashCounter / 6 % 2 == 0))) {
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

    // �v���C���[��HP 0�ŉ�ʊO�ɏo���ꍇ�̂�finish.png��\��
    if (player.hp <= 0 && player.position.y > SCREEN_H) {
        sprite_render(sprFinish, SCREEN_W * 0.5, SCREEN_H * 0.5, 1.0f, 1.0f, 0, 0, 1020, 300, 1020 / 2, 300 / 2);
    }

    // �t�F�[�h�A�E�g�`��
    if (player.fadeAlpha > 0.0f) {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, player.fadeAlpha);
    }

   /* primitive::circle(player.position.x + player.offset.x,
        player.position.y + player.offset.y,
        player.radius, 1, 1, ToRadian(0), 1, 0, 0, 0.2f);*/
}


// �v���C���[�̍s������
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
    // ���N���b�N�Ńu�[�X�g�i�ő�5��j
    if ((TRG(0) & L_CLICK) && boostCount > 0) {
        boostFlame = 10; // �u�[�X�g��10�t���[������
        boostCount--;
    }

    // �u�[�X�g���̏���
    if (boostFlame > 0) {
        player.position.x = lerp(player.position.x, static_cast<float>(point.x), lerpSpeed * boostMultiplier);
        player.position.y = lerp(player.position.y, static_cast<float>(point.y), lerpSpeed * boostMultiplier);
        boostFlame--; // �t���[�����ƂɌ���
        
    }


    // �v���C���[��Y�̈ʒu���X�V
    player.position.y += player.speed.y;

    // ��ʒ[�ɂԂ������ꍇ�̐���
    if (player.position.y < 0 + PLAYER_PIVOT_Y * 2) {
        player.position.y = 0 + PLAYER_PIVOT_Y * 2;
        player.speed.y = 0.0f;
    }
    if (player.hp>0 && player.position.y > SCREEN_H - PLAYER_PIVOT_Y * 2) {
        player.position.y = SCREEN_H - PLAYER_PIVOT_Y * 2;
        player.speed.y = 0.0f;
    }
    if (player.position.x > SCREEN_W - PLAYER_PIVOT_X * 2) {
        player.position.x = SCREEN_W - PLAYER_PIVOT_X * 2;
        player.speed.x = 0.0f;
    }
    if (player.position.x < 0 + PLAYER_PIVOT_X * 2) {
        player.position.x = 0 + PLAYER_PIVOT_X * 2;
        player.speed.x = 0.0f;
    }

    // �Փˏ���
    for (int i = 0; i < BIRD_MAX; i++) {
        if (hitCheckBird(&player, &bird[i])) {
            if (!bird[i].isColliding) {
                music::play(BGM_BALLOON, false);
                bird[i].isColliding = true;

                // HP������
                player.hp--;
                if (player.hp > 0) {
                    player.flashing = true;
                    player.flashCounter = 0;
                    /*score -= 300;
                    if (score <= 0)score = 0;*/
                    
                }

                // �A�j���[�V���������ɂ��炷
                player.texPos.x += PLAYER_TEX_W;

                // �A�j���[�V�������Ō�̃t���[���𒴂����痎���J�n
                if (player.texPos.x >= PLAYER_TEX_W * 5) {
                    player.speed.y = 30.0f;
                }

                player.radius -= 10;
            }
        }
        else {
            bird[i].isColliding = false;
        }
    }

    // HP��0�ɂȂ����ꍇ�̏���
    if (player.hp <= 0) {
        player.hp = 0;
        if (player.position.y < SCREEN_H) {
            // �v���C���[����ʊO�ɗ�����܂ŗ���
            player.speed.y = 50.0f; // �������x
            player.speed.x = 0.0f;
            music::play(BGM_WATER, false);
        }
        else {
            // �v���C���[����ʊO�ɗ��������finish��\��
            
            sprite_render(sprFinish, SCREEN_W * 0.5, SCREEN_H * 0.5, 1.0f, 1.0f, 0, 0, 1020, 300, 1020 / 2, 300 / 2);

            // 2�b�ԕ\����Ƀt�F�[�h�A�E�g�J�n
            
            player.finish_timer += 0.0166f;

            // 2�b��Ƀt�F�[�h�A�E�g���J�n
            if (player.finish_timer >= 2.0f) {
                player.fadeAlpha += 0.02f; // ���X�Ƀt�F�[�h�A�E�g

                if (player.fadeAlpha >= 1.0f) {
                    // �t�F�[�h�A�E�g������Ɍ��ʉ�ʂ�\��
                    result_start();
                }
            }
        }
    }

}
