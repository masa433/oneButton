#include "scene_game.h"
#include "common.h"
#include "system.h"
#include "player.h"
#include "back.h"
#include "count.h"
#include "ring.h"
#include "bird.h"
#include "sign.h"
#include <sstream>
#include"audio.h"



using namespace std;

extern GAME Game;


void game_init()
{
    Game.game_state = 0;
    Game.game_timer = 0;
    Game.FadeIn = 1.0f;
    Game.isFadeIn = false;
    Game.wait_timer = 0;
    Game.stay_timer = 0;
    Game.bird_timer = 0;

    player_init();
    back_init();
    count_init();
    bird_init();
    ring_init();
    sign_init();
}

void game_update()
{
    back_update(); // �w�i�͏�ɍX�V  
    player_update(); // �v���C���[�̓�����ĊJ

    using namespace input;

    if (Game.game_state == 0) 
    {
        game_init();
    }

    switch (Game.game_state)
    {
    case 0:
        //////// �����ݒ� ////////
        Game.game_state++;
        /*fallthrough*/
    case 1:
        //////// �p�����[�^�̐ݒ� ////////
        GameLib::setBlendMode(Blender::BS_ALPHA);
        music::play(BGM_GAME, true);
        Game.game_state++;
        /*fallthrough*/
    case 2:
        //////// �t�F�[�h�C������ ////////
        
        game_fade_act();
        break;

    case 3:
        //////// �ҋ@��� ////////
        Game.wait_timer++;
        
        if (Game.wait_timer >= 60) { // 1�b��Ɏ��̏�Ԃ�
            Game.game_state++;
        }
        break;

    case 4:
        //////// �J�E���g�_�E���J�n ////////
        count_update(); // �J�E���g�_�E���̍X�V���Ăяo��
        
        Game.stay_timer++;
        if (Game.stay_timer >= 300) { // 5�b��Ɏ��̏�Ԃ�
            Game.game_state = 5; // ���ڃQ�[���v���C��Ԃɐi��
        }
        break;

    case 5:
        //////// �Q�[���v���C��� ////////
        ring_update();
        Game.bird_timer++;
        if (Game.bird_timer >= 240) //4�b��ɒ��̏���
        {
            sign_update();
            bird_update(); // �J�E���g�_�E����ɒ��̍X�V���Ăяo��
        }
        
        
        break;
    }

    Game.game_timer++;
}



void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    back_render();  // �w�i�̕`��
    

    if (Game.game_state == 2||Game.game_state==3) 
    {
        player_render();
    }
    
    // �Q�[���̏�Ԃ��J�E���g�_�E�����Ȃ�J�E���g�_�E������O�ɕ\��
    if (Game.game_state >= 4)
    {   
        ring_render();   // �����O�͌��ɕ`�� 
        player_render(); // �v���C���[��`��i�����O�̎�O�ɕ`��j
        count_render();  // �J�E���g�_�E���̕`��
        
    }
    if (Game.game_state == 5) 
    {
       
        sign_render();
        bird_render();
        player_render();
    }

    

    // �t�F�[�h�C���̋�`�`��
    if (Game.FadeIn > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Game.FadeIn);
    }

    
}

void game_deinit()
{
    player_deinit();
    back_deinit();
    bird_deinit();
    count_deinit();
    ring_deinit();
    sign_deinit();
    music::stop(BGM_GAME);

    // �O���[�o���ϐ��̃��Z�b�g
    Game.game_state = 0;
    Game.game_timer = 0;
    Game.FadeIn = 1.0f;
    Game.isFadeIn = false;
    Game.wait_timer = 0;
    Game.stay_timer = 0;
}


void game_fade_act()
{
    if (!Game.isFadeIn)
    {
        Game.isFadeIn = true; // �t�F�[�h�C���J�n
    }

    if (Game.isFadeIn)
    {
        Game.FadeIn -= 0.03f; // �t�F�[�h�C����i�s
        if (Game.FadeIn <= 0.0f)
        {
            Game.FadeIn = 0.0f; // �t�F�[�h�C������
            Game.isFadeIn = false;

            // �K�v�ȃ^�C�}�[���Ԃ����Z�b�g
            Game.wait_timer = 0;
            Game.stay_timer = 0;
            Game.game_timer = 0;

            Game.game_state++;   // ���̏�Ԃɐi��
        }
    }
}


