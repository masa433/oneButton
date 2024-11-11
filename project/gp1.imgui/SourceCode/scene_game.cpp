#include "scene_game.h"
#include "common.h"
#include "player.h"
#include "back.h"
#include "count.h"
#include "ring.h"
#include <sstream>

using namespace std;

GAME Game;

void game_init()
{
    Game.game_state = 0;
    Game.game_timer = 0;
    Game.FadeIn = 1.0f;
    Game.isFadeIn = false;
    Game.wait_timer = 0;

    player_init();
    back_init();
    count_init();
    ring_init();
}

void game_update()
{
    back_update(); // �w�i�͏�ɍX�V  
    player_update(); // �v���C���[�̓�����ĊJ
   
    using namespace input;

    switch (Game.game_state)
    {
    case 0:
        //////// �����ݒ� ////////
        Game.game_state++;
        /*fallthrough*/
    case 1:
        //////// �p�����[�^�̐ݒ� ////////
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Game.game_state++;
        /*fallthrough*/
    case 2:
        //////// �t�F�[�h�C������ ////////
        game_fade_act();
        break;

    case 3:
        
        Game.wait_timer++;//�J�E���g�_�E���܂ł̂P�b�ԑҋ@
        if (Game.wait_timer >= 60) { // 1�b��Ɏ��̏�Ԃ�
            Game.wait_timer = 0;
            count_update(); // �J�E���g�_�E���̍X�V���Ăяo��
         
        }
        
        break;

    case 4:
       
        ring_update();
        
        break;
    case 5:
        
        break;
    }

    Game.game_timer++;
}

void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    back_render();
    
    player_render(); // �ҋ@�����v���C���[��`��
    

    if (Game.game_state >= 3)
    {
        count_render(); // �J�E���g�_�E���̕`����J�n
    }
    if (Game.game_state >= 4)
    {
        ring_render(); // �J�E���g�_�E���̕`����J�n
    }

    // �t�F�[�h�C���̋�`�`��
    if (Game.FadeIn > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Game.FadeIn);
    }

    debug::setString("FadeIn%f", Game.FadeIn);
}

void game_deinit()
{
    player_deinit();
    back_deinit();
    count_deinit();
    ring_deinit();
}

//�t�F�[�h�C���̏���
void game_fade_act() 
{
    if (!Game.isFadeIn)
    {
        Game.isFadeIn = true; // �t�F�[�h�C���J�n
    }

    if (Game.isFadeIn)
    {
        Game.FadeIn -= 0.03f;
        if (Game.FadeIn <= 0.0f)
        {
            Game.FadeIn = 0.0f;
            Game.isFadeIn = false;
            Game.wait_timer = 0; // �t�F�[�h�C��������������ҋ@�^�C�}�[�����Z�b�g
            Game.game_state++;   // ���̏�Ԃɐi��
        }
    }
}