//******************************************************************************
//
//
//      main
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "all.h"

//------< namespace >-----------------------------------------------------------
using namespace GameLib;

//------< 変数 >----------------------------------------------------------------

//------------------------------------------------------------------------------
//  WinMain（エントリポイント）
//------------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// 使用しない変数は記述しない
{
    // ゲームライブラリの初期設定
    GameLib::init(L"ゲームプログラミングⅠ", SCREEN_W, SCREEN_H, FULLSCREEN);

    game_init();

    while (GameLib::gameLoop())
    {
        // 入力処理
        input::update();

#ifdef USE_IMGUI
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
#endif

        // 現在のシーンを更新・描画
        game_update();
        game_render();

#ifdef USE_IMGUI
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

        // デバッグ文字列を描画
        debug::display(1.0f, 1.0f, 1.0f, 1, 1);

        //debug::setString("GP1_01 SAMPLE");

        // バックバッファの内容を表示
        GameLib::present(1, 0);
    }

    // ゲームシーンの終了
    game_deinit();

    // ゲームライブラリの終了処理
    GameLib::uninit();
}
