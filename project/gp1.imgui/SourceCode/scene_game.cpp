#include "scene_game.h"
#include "common.h"
#include "player.h"
#include "back.h"
#include "count.h"
#include "ring.h"
#include <sstream>

using namespace std;

int game_state = 0;
int game_timer = 0;
float FadeIn;
bool isFadeIn;
int wait_timer = 0;

void game_init()
{
    game_state = 0;
    game_timer = 0;
    FadeIn = 1.0f;
    isFadeIn = false;
    wait_timer = 0;

    player_init();
    back_init();
    count_init();
    ring_init();
}

void game_update()
{
    back_update(); // �w�i�͏�ɍX�V  
    player_update(); // �v���C���[�̓�����ĊJ
    ring_update();
    using namespace input;

    switch (game_state)
    {
    case 0:
        //////// �����ݒ� ////////
        game_state++;
        /*fallthrough*/
    case 1:
        //////// �p�����[�^�̐ݒ� ////////
        GameLib::setBlendMode(Blender::BS_ALPHA);
        game_state++;
        /*fallthrough*/
    case 2:
        //////// �t�F�[�h�C������ ////////
        if (!isFadeIn)
        {
            isFadeIn = true; // �t�F�[�h�C���J�n
        }

        if (isFadeIn)
        {
            FadeIn -= 0.03f;
            if (FadeIn <= 0.0f)
            {
                FadeIn = 0.0f;
                isFadeIn = false;
                wait_timer = 0; // �t�F�[�h�C��������������ҋ@�^�C�}�[�����Z�b�g
                game_state++;   // ���̏�Ԃɐi��
            }
        }
        break;

    case 3:
        //////// �ҋ@��� ////////
        wait_timer++;
        if (wait_timer >= 60) { // 1�b��Ɏ��̏�Ԃ�
            game_state++;
        }
        break;

    case 4:
        //////// �J�E���g�_�E���J�n ////////
        count_update(); // �J�E���g�_�E���̍X�V���Ăяo��
        
        break;
    }

    game_timer++;
}

void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    back_render();
    ring_render();
    player_render(); // �ҋ@�����v���C���[��`��
    

    if (game_state >= 4)
    {
        count_render(); // �J�E���g�_�E���̕`����J�n
    }

    // �t�F�[�h�C���̋�`�`��
    if (FadeIn > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, FadeIn);
    }

    debug::setString("FadeIn%f", FadeIn);
}

void game_deinit()
{
    player_deinit();
    back_deinit();
    count_deinit();
    ring_deinit();
}
