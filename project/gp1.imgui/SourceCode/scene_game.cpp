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

    player_init();
    back_init();
    count_init();
    bird_init();
    ring_init();
    sign_init();
}

void game_update()
{
    back_update(); // 背景は常に更新  
    player_update(); // プレイヤーの動作を再開

    using namespace input;

    switch (Game.game_state)
    {
    case 0:
        //////// 初期設定 ////////
        Game.game_state++;
        /*fallthrough*/
    case 1:
        //////// パラメータの設定 ////////
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Game.game_state++;
        /*fallthrough*/
    case 2:
        //////// フェードイン処理 ////////
        game_fade_act();
        break;

    case 3:
        //////// 待機状態 ////////
        Game.wait_timer++;
        if (Game.wait_timer >= 60) { // 1秒後に次の状態へ
            Game.game_state++;
        }
        break;

    case 4:
        //////// カウントダウン開始 ////////
        count_update(); // カウントダウンの更新を呼び出す
        
        if (TRG(0) & L_CLICK) { // 仮置き
            result_start();
        }
        Game.stay_timer++;
        if (Game.stay_timer >= 300) { // 5秒後に次の状態へ
            Game.game_state++;
        }
        break;

    case 5:
        //////// ゲームプレイ状態 ////////
        Game.wait_timer = 0;
        Game.stay_timer = 0;
        ring_update();
        sign_update();
        bird_update(); // カウントダウン後に鳥の更新を呼び出す
        break;
    }

    Game.game_timer++;
}


void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    back_render();  // 背景の描画
    

    if (Game.game_state == 2||Game.game_state==3) 
    {
        player_render();
    }
    
    // ゲームの状態がカウントダウン中ならカウントダウンを手前に表示
    if (Game.game_state >= 4)
    {   
        ring_render();   // リングは後ろに描画 
        player_render(); // プレイヤーを描画（リングの手前に描画）
        count_render();  // カウントダウンの描画
        
    }
    if (Game.game_state == 5) 
    {
        sign_render();
        bird_render();
    }

    

    // フェードインの矩形描画
    if (Game.FadeIn > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Game.FadeIn);
    }

    // デバッグ情報表示
    debug::setString("FadeIn%f", Game.FadeIn);
}

void game_deinit()
{
    player_deinit();
    back_deinit();
    bird_deinit();
    count_deinit();
    ring_deinit();
    sign_deinit();
}

void game_fade_act()
{
    if (!Game.isFadeIn)
    {
        Game.isFadeIn = true; // フェードイン開始
    }

    if (Game.isFadeIn)
    {
        Game.FadeIn -= 0.03f;
        if (Game.FadeIn <= 0.0f)
        {
            Game.FadeIn = 0.0f;
            Game.isFadeIn = false;
            Game.wait_timer = 0; // フェードインが完了したら待機タイマーをリセット
            Game.game_state++;   // 次の状態に進む
        }
    }
}

