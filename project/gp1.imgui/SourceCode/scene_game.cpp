//******************************************************************************
//
//
//      scene_game
//
//
//******************************************************************************

//----< �C���N���[�h >-----------------------------------------------------------
#include "scene_game.h"
#include"common.h"
#include"player.h"
#include <sstream>

//------< �萔 >----------------------------------------------------------------

//------< �ϐ� >----------------------------------------------------------------
int game_state = 0;    // ���
int game_timer = 0;    // �^�C�}�[
float FadeIn;
bool isFadeIn;

//--------------------------------------
//  �����ݒ�
//--------------------------------------
void game_init()
{
    game_state = 0;
    game_timer = 0;
    FadeIn = 1.0f;
    isFadeIn = false;
    player_init();
}

//--------------------------------------
//  �X�V����
//--------------------------------------
void game_update()
{
    player_update();

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

        FadeIn = 1.0f;
        isFadeIn = false;

        game_state++;
        /*fallthrough*/
    case 2:
        //////// �ʏ펞 ////////
        if (!isFadeIn)
        {
            isFadeIn = true; // �t�F�[�h�C���J�n
        }

        if (isFadeIn)
        {
            // �t�F�[�h�C��
            FadeIn -= 0.05f;
            if (FadeIn <= 0.0f) {
                FadeIn = 0.0f;
                
            }
        }
        
        break;
    }

    game_timer++;


//#ifdef USE_IMGUI
//    ImGui::Begin("ImGUI");
//
//    static float value = 0;
//    ImGui::DragFloat("value", &value, 0.001f);
//
//    ImGui::End();
//#endif


}

//--------------------------------------
//  �`�揈��
//--------------------------------------
void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    // ��ʑS�̂Ƀt�F�[�h��K�p
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, FadeIn);

    player_render();
    //text_out(0, "Hello World", 0, 0);   // ���{


}

//--------------------------------------
//  �I������
//--------------------------------------
void game_deinit()
{
    player_deinit();
}
