
#include "scene_game.h"
#include"common.h"
#include"player.h"
#include"back.h"
#include <sstream>

using namespace std;

int game_state = 0;    
int game_timer = 0;    
float FadeIn;
bool isFadeIn;
int countDown;
bool countdownComplete;


void game_init()
{
    game_state = 0;
    game_timer = 0;
    FadeIn = 1.0f;
    isFadeIn = false;
    countDown = 3;
    countdownComplete = false;
    player_init();
    back_init();
}


void game_update()
{
    back_update();

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

        if (countDown >= 0) {
            static int frameCounter = 0;
            frameCounter++;
            if (frameCounter >= 60) { // 1�b���ɃJ�E���g�_�E��
                frameCounter = 0;
                countDown--;
            }
        }

        else {
            countdownComplete = true;
            game_state++;
        }

    case 3:
            player_update();
                    
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


void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    // ��ʑS�̂Ƀt�F�[�h��K�p
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, FadeIn);

    if (countdownComplete) {
        back_render();
        player_render();

    }
    else if (countDown >= 0) {
        text_out(6, to_string(countDown), SCREEN_W / 2 + 120, SCREEN_H / 2, 20, 20, 1, 1, 1, 0.5f, TEXT_ALIGN::MIDDLE);
        back_render();
        player_render();
        
    }


}

void game_deinit()
{

    player_deinit();
    back_deinit();
}
