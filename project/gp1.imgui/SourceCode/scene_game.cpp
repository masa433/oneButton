//******************************************************************************
//
//
//      scene_game
//
//
//******************************************************************************

//----< インクルード >-----------------------------------------------------------
#include "scene_game.h"
#include"common.h"
#include"player.h"
#include <sstream>

//------< 定数 >----------------------------------------------------------------

//------< 変数 >----------------------------------------------------------------
int game_state = 0;    // 状態
int game_timer = 0;    // タイマー
float FadeIn;
bool isFadeIn;

//--------------------------------------
//  初期設定
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
//  更新処理
//--------------------------------------
void game_update()
{
    player_update();

    using namespace input;

    switch (game_state)
    {
    case 0:
        //////// 初期設定 ////////


        game_state++;
        /*fallthrough*/
    case 1:
        //////// パラメータの設定 ////////
        GameLib::setBlendMode(Blender::BS_ALPHA);

        FadeIn = 1.0f;
        isFadeIn = false;

        game_state++;
        /*fallthrough*/
    case 2:
        //////// 通常時 ////////
        if (!isFadeIn)
        {
            isFadeIn = true; // フェードイン開始
        }

        if (isFadeIn)
        {
            // フェードイン
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
//  描画処理
//--------------------------------------
void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    // 画面全体にフェードを適用
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, FadeIn);

    player_render();
    //text_out(0, "Hello World", 0, 0);   // 見本


}

//--------------------------------------
//  終了処理
//--------------------------------------
void game_deinit()
{
    player_deinit();
}
