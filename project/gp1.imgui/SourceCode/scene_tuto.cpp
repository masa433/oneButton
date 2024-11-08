#include "common.h"
#include "scene_tuto.h"
#include "system.h"
using namespace std;

int tuto_state;
int tuto_timer;
float tuto_fadein;

bool isTuto_Fadein;

Sprite* sprBack;
Sprite* sprTuto;

float scroll_x; // 横スクロールの位置

void tuto_init()
{
    tuto_timer = 0;
    tuto_state = 0;
    scroll_x = 0.0f; // スクロール位置の初期化
    tuto_fadein = 1.0f;

    isTuto_Fadein = false;
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
                tuto_state++;   // 次の状態に進む
            }
        }

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

    // 背景横スクロール
    sprite_render(sprBack, -scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // 1枚目
    sprite_render(sprBack, -scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2); // 2枚目（繰り返し）

    // チュートリアル画像の描画（フェードインとスケーリング）
    VECTOR2 scale = { 2.0f + (1.0f * (1.0f - tuto_fadein)), 1.5f + (1.0f * (1.0f - tuto_fadein)) }; // スケールをフェードインに合わせて大きくする
    sprite_render(sprTuto, SCREEN_W * 0.5f, SCREEN_H * 0.4f, scale.x, scale.y, 0, 0, 640, 500, 640 / 2, 500/2);

    // フェードインの矩形描画
    if (tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, tuto_fadein);
    }
}

void tuto_deinit()
{
    safe_delete(sprBack);
}
