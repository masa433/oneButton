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
    back_update(); // 背景は常に更新  
    player_update(); // プレイヤーの動作を再開
    ring_update();
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
        game_state++;
        /*fallthrough*/
    case 2:
        //////// フェードイン処理 ////////
        if (!isFadeIn)
        {
            isFadeIn = true; // フェードイン開始
        }

        if (isFadeIn)
        {
            FadeIn -= 0.03f;
            if (FadeIn <= 0.0f)
            {
                FadeIn = 0.0f;
                isFadeIn = false;
                wait_timer = 0; // フェードインが完了したら待機タイマーをリセット
                game_state++;   // 次の状態に進む
            }
        }
        break;

    case 3:
        //////// 待機状態 ////////
        wait_timer++;
        if (wait_timer >= 60) { // 1秒後に次の状態へ
            game_state++;
        }
        break;

    case 4:
        //////// カウントダウン開始 ////////
        count_update(); // カウントダウンの更新を呼び出す
        
        break;
    }

    game_timer++;
}

void game_render()
{
    GameLib::clear(0.0, 0.0, 0.4);

    back_render();
    ring_render();
    player_render(); // 待機中もプレイヤーを描画
    

    if (game_state >= 4)
    {
        count_render(); // カウントダウンの描画を開始
    }

    // フェードインの矩形描画
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
