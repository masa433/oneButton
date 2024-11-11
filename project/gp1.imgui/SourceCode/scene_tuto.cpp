#include "common.h"
#include "scene_tuto.h"
#include "system.h"
using namespace std;
using namespace input;

int tuto_state;
int tuto_timer;
float tuto_fadein;
float tuto_fadeout;
float tuto_fadeTimer;

bool isTuto_Fadein;
bool isTuto_Fadeout;

Sprite* sprBack;
Sprite* sprTuto;



float scroll_x; // 横スクロールの位置
float click_delay_timer = 0.0f; // クリック後のフェードアウト遅延タイマー

void tuto_init()
{
    tuto_timer = 0;
    tuto_state = 0;
    scroll_x = 0.0f; // スクロール位置の初期化
    tuto_fadein = 1.0f;
    tuto_fadeout = 0.0f;

    isTuto_Fadein = false;
    isTuto_Fadeout = false; // フェードアウトを開始していない状態
    click_delay_timer = 0.0f; // タイマーの初期化
    tuto_fadeTimer = 0.0f;
}

void tuto_update()
{
    switch (tuto_state)
    {
    case 0:
        sprBack = sprite_load(L"./Data/Images/back(仮).png");
        sprTuto = sprite_load(L"./Data/Images/tuto.png");
        tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        tuto_state++;
        /*fallthrough*/
    case 2:
        fadeIn_act();
        break;

    case 3:
        // 左クリックが押されたらフェードアウト遅延タイマーを開始
        if (TRG(0) & L_CLICK)
        {
            isTuto_Fadeout = true;
            click_delay_timer = 0.0f; // タイマーをリセット
            tuto_state++;
        }
        break;

    case 4:
        fadeOut_act();
        break;
    }

    // スクロール処理
    scroll_x += 0.5f; // スクロール速度
    if (scroll_x >= 5464) { // 画像の幅でループ
        scroll_x = 0.0f; // ループの初期化
    }

    tuto_timer++;
}

void tuto_render()
{
    GameLib::clear(1.0, 0.0, 0.0);

    // 背景スクロール
    sprite_render(sprBack, -scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // First part
    sprite_render(sprBack, -scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // Repeated part

    // フェードイン・フェードアウトに応じてスケール変更
    float scale_up = 1.0f + (1.0f - tuto_fadein); // フェードインでスケールを大きくする
    float scale_down = 1.0f - tuto_fadeout * 0.5f; // フェードアウトでスケールを小さくする
    VECTOR2 scale = { 1.5f * scale_up * scale_down ,1.5f * scale_up * scale_down };//最終的なスケール
    

    // 説明画像の描画
    sprite_render(sprTuto, SCREEN_W * 0.5f, SCREEN_H * 0.4f, scale.x, scale.y, 0, 0, 640, 500, 640 / 2, 500 / 2);

    // フェードインの四角形描画
    if (tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, tuto_fadein);
    }

    // フェードアウトの四角形描画
    if (tuto_fadeout > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, tuto_fadeout);
    }
}


void tuto_deinit()
{
    safe_delete(sprBack);
    safe_delete(sprTuto);
}

void fadeIn_act() 
{
    if (!isTuto_Fadein)
    {
        isTuto_Fadein = true; // フェードイン開始
    }

    if (isTuto_Fadein)
    {
        tuto_fadein -= 0.03f; // フェードインの速度
        if (tuto_fadein <= 0.0f)
        {
            tuto_fadein = 0.0f;
            isTuto_Fadein = false;
            tuto_state++;
        }
    }
}

void fadeOut_act() 
{
    // 遅延タイマーが一定時間に達したらフェードアウト開始
        if (isTuto_Fadeout)
        {
            click_delay_timer += 0.03f; // タイマーの増加
            if (click_delay_timer >= 2.0f) // 2秒の遅延
            {
                tuto_fadeout += 0.05f; // フェードアウトの速度
                if (tuto_fadeout >= 1.0f)
                {
                    tuto_fadeout = 1.0f;
                    tuto_fadeTimer += 0.1f;
                    if (tuto_fadeTimer >= 10.0f) {//フェードアウトしてから10経過したら
                        
                        game_start(); // フェードアウト完了後、ゲーム画面に移行
                        tuto_fadeTimer = 0.0f;
                    }
                    
                }
            }
        }
}
