#include "common.h"
#include "scene_tuto.h"
#include "system.h"
using namespace std;
using namespace input;


Sprite* sprBack;
Sprite* sprTuto;

TUTO Tuto;



void tuto_init()
{
    Tuto.tuto_timer = 0;
    Tuto.tuto_state = 0;
    Tuto.scroll_x = 0.0f; // スクロール位置の初期化
    Tuto.tuto_fadein = 1.0f;
    Tuto.tuto_fadeout = 0.0f;

    Tuto.isTuto_Fadein = false;
    Tuto.isTuto_Fadeout = false; // フェードアウトを開始していない状態
    Tuto.click_delay_timer = 0.0f; // タイマーの初期化
    Tuto.tuto_fadeTimer = 0.0f;
}

void tuto_update()
{
    switch (Tuto.tuto_state)
    {
    case 0:
        sprBack = sprite_load(L"./Data/Images/back(仮).png");
        sprTuto = sprite_load(L"./Data/Images/tuto.png");
        Tuto.tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Tuto.tuto_state++;
        /*fallthrough*/
    case 2:
        fadeIn_act();
        break;

    case 3:
        // 左クリックが押されたらフェードアウト遅延タイマーを開始
        if (TRG(0) & L_CLICK)
        {
            Tuto.isTuto_Fadeout = true;
            Tuto.click_delay_timer = 0.0f; // タイマーをリセット
            Tuto.tuto_state++;
        }
        break;

    case 4:
        fadeOut_act();
        break;
    }

    // スクロール処理
    Tuto.scroll_x += 0.5f; // スクロール速度
    if (Tuto.scroll_x >= 5464) { // 画像の幅でループ
        Tuto.scroll_x = 0.0f; // ループの初期化
    }

    Tuto.tuto_timer++;
}

void tuto_render()
{
    GameLib::clear(1.0, 0.0, 0.0);

    // 背景スクロール
    sprite_render(sprBack, -Tuto.scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // １枚目
    sprite_render(sprBack, -Tuto.scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // ２枚目(繰り返し)

    // フェードイン・フェードアウトに応じてスケール変更
    float scale_up = 1.0f + (1.0f - Tuto.tuto_fadein); // フェードインでスケールを大きくする
    float scale_down = 1.0f - Tuto.tuto_fadeout * 0.5f; // フェードアウトでスケールを小さくする
    VECTOR2 scale = { 1.5f * scale_up * scale_down ,1.5f * scale_up * scale_down };//最終的なスケール
    

    // 説明画像の描画
    sprite_render(sprTuto, SCREEN_W * 0.5f, SCREEN_H * 0.4f, scale.x, scale.y, 0, 0, 640, 500, 640 / 2, 500 / 2);

    // フェードインの四角形描画
    if (Tuto.tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadein);
    }

    // フェードアウトの四角形描画
    if (Tuto.tuto_fadeout > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadeout);
    }
}


void tuto_deinit()
{
    safe_delete(sprBack);
    safe_delete(sprTuto);
}

//フェードインの処理
void fadeIn_act() 
{
    if (!Tuto.isTuto_Fadein)
    {
        Tuto.isTuto_Fadein = true; // フェードイン開始
    }

    if (Tuto.isTuto_Fadein)
    {
        Tuto.tuto_fadein -= 0.03f; // フェードインの速度
        if (Tuto.tuto_fadein <= 0.0f)
        {
            Tuto.tuto_fadein = 0.0f;
            Tuto.isTuto_Fadein = false;
            Tuto.tuto_state++;
        }
    }
}

//フェードアウトの処理
void fadeOut_act() 
{
    // 遅延タイマーが一定時間に達したらフェードアウト開始
        if (Tuto.isTuto_Fadeout)
        {
            Tuto.click_delay_timer += 0.03f; // タイマーの増加
            if (Tuto.click_delay_timer >= 2.0f) // 2秒の遅延
            {
                Tuto.tuto_fadeout += 0.05f; // フェードアウトの速度
                if (Tuto.tuto_fadeout >= 1.0f)
                {
                    Tuto.tuto_fadeout = 1.0f;
                    Tuto.tuto_fadeTimer += 0.1f;
                    if (Tuto.tuto_fadeTimer >= 10.0f) {//フェードアウトしてから10経過したら
                        
                        game_start(); // フェードアウト完了後、ゲーム画面に移行
                        Tuto.tuto_fadeTimer = 0.0f;
                    }
                    
                }
            }
        }
}
