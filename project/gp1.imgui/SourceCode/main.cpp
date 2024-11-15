//******************************************************************************
//
//
//      main
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "common.h"
#include"scene_game.h"
#include"scene_title.h"
#include"scene_tuto.h"
#include"scene_result.h"

//------< namespace >-----------------------------------------------------------
using namespace GameLib;

//------< 変数 >----------------------------------------------------------------
int curScene = SCENE_NONE;
int nextScene = SCENE_TITLE;
SCENE Scene;

//------------------------------------------------------------------------------
//  WinMain（エントリポイント）
//------------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// 使用しない変数は記述しない
{
    // ゲームライブラリの初期設定
    GameLib::init(L"ゲームプログラミングⅠ", SCREEN_W, SCREEN_H, true);



    while (GameLib::gameLoop())
    {
        if (curScene != nextScene) 
        {
            switch (curScene) 
            {
            case SCENE_TITLE:
                title_deinit();
                break;
            case SCENE_TUTO:
                tuto_deinit();
                break;
            case SCENE_GAME:
                game_deinit();
                break;
            case SCENE_RESULT:
                result_deinit();
                break;
            }
       
            // 次のシーンに応じた初期設定処理
            switch (nextScene)
            {
            case SCENE_TITLE: 
                title_init();
                break;
            case SCENE_TUTO:
                tuto_init();
                break;
            case SCENE_GAME:
                game_init();
                break;
            case SCENE_RESULT:
                result_init();
                break;
            }
            // nextScene が curScene になる
            curScene = nextScene;
        }
            // 入力処理
            input::update();

        // 現在のシーンに応じた更新・描画処理
        switch (curScene)
        {
        case SCENE_TITLE:
            title_update();
            title_render();
            break;

        case SCENE_TUTO:
            tuto_update();
            tuto_render();
            break;

        case SCENE_GAME:
            game_update();
            game_render();
            break;

        case SCENE_RESULT:
            result_update();
            result_render();
            break;
        }
//#ifdef USE_IMGUI
//        ImGui_ImplDX11_NewFrame();
//        ImGui_ImplWin32_NewFrame();
//        ImGui::NewFrame();
//#endif

       

//#ifdef USE_IMGUI
//        ImGui::Render();
//        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//#endif

        // デバッグ文字列を描画
        debug::display(1.0f, 1.0f, 1.0f, 1, 1);

        //debug::setString("GP1_01 SAMPLE");

        // バックバッファの内容を表示
        GameLib::present(1, 0);
    }


    // ゲームシーンの終了
    // 現在のシーンに応じた終了処理を行う
    switch (curScene)
    {
    case SCENE_TITLE:
        title_deinit();
        break;

    case SCENE_TUTO:
        tuto_deinit();
        break;

    case SCENE_GAME:
        game_deinit();
        break;

    case SCENE_RESULT:
        result_deinit();
        break;
    
    }

    // ゲームライブラリの終了処理
    GameLib::uninit();

    return 0;
}
