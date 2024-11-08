#include "count.h"
#include "common.h"

using namespace std;

int count_state;
int count_timer;

Sprite* sprCount;
Sprite* sprGstart;
COUNT Count;
COUNT Start;
bool count_done = false;

void count_init()
{
    count_state = 0;
    count_timer = 0;
    
    sprCount = sprite_load(L"./Data/Images/countdown.png");
    sprGstart = sprite_load(L"./Data/Images/start.png");
}

void count_deinit()
{
    safe_delete(sprCount);
    safe_delete(sprGstart);
}

void count_update()
{
    switch (count_state)
    {
    case 0:
    {
        //////// 初期設定 ////////
        Count = {};
        Count.position = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
        Count.pivot = { COUNT_PIVOT_X, COUNT_PIVOT_Y };
        Count.color = { 1, 1, 1, 1 };
        Count.texSize = { COUNT_TEX_W, COUNT_TEX_H };

        Start = {};
        Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
        Start.pivot = { START_PIVOT_X, START_PIVOT_Y };
        Start.color = { 1, 1, 1, 1 };
        Start.texSize = { START_TEX_W, START_TEX_H };

        count_state++;
        /*fallthrough*/
    }
    case 1:
    {
        float count_index = count_timer / 60; // カウント番号 (0 = 3, 1 = 2, 2 = 1)
        float frame_index = (count_timer % 60) / 3; // アニメーションフレーム

        if (count_index < 3) {
            // カウント画像の表示位置 (texPos) を更新
            Count.texPos = { frame_index * COUNT_TEX_W, count_index * COUNT_TEX_H };

            // カウントダウンのスケールアニメーション設定
            if (count_timer % 60 < 20)
            {
                // 最初の20フレームでスケールを小さく開始
                float t = (count_timer % 20) / 20.0f; // 0から1への補間
                Count.scale = { 0.5f + 1.5f * t, 0.5f + 1.5f * t }; // 徐々に大きく
            }
            else if (count_timer % 60 < 40)
            {
                // 次の20フレームで最大スケールに達する
                Count.scale = { 2.0f, 2.0f };
            }
            else
            {
                // 最後の20フレームでスケールを小さく
                float t = (count_timer % 20) / 20.0f; // 0から1への補間
                Count.scale = { 2.0f - 0.7f * t, 2.0f - 0.7f * t }; // 徐々に小さく
            }
        }
        else if (count_index >= 3)
        {
            count_done = true;
            count_state++;
        }
        count_timer++; // カウントタイマーを進める
        break;
    }
    case 2:
    {
        float start_index = count_timer / 60; // カウント番号 (0 = 3, 1 = 2, 2 = 1)
        float frame_index2 = (count_timer % 60) / 3; // アニメーションフレーム

        // カウント画像の表示位置 (texPos) を更新
        Start.texPos = { frame_index2 * START_TEX_W, start_index * START_TEX_H };

        // カウントダウンのスケールアニメーション設定
        if (count_timer % 60 < 20)
        {
            // 最初の20フレームでスケールを小さく開始
            float t = (count_timer % 20) / 20.0f; // 0から1への補間
            Start.scale = { 0.5f + 1.5f * t, 0.5f + 1.5f * t }; // 徐々に大きく
        }
        else if (count_timer % 60 < 40)
        {
            // 次の20フレームで最大スケールに達する
            Start.scale = { 2.0f, 2.0f };
        }
        else
        {
            // 最後の20フレームでスケールを小さく
            float t = (count_timer % 60 - 40) / 20.0f; // 0から1への補間
            Start.scale = { 2.0f - 0.7f * t, 2.0f - 0.7f * t }; // 徐々に小さく
        }
        count_timer++; // カウントタイマーを進める
        break;
    }
    }
}



void count_render()
{
    if (!count_done) 
    {
        sprite_render(sprCount, Count.position.x, Count.position.y,
            Count.scale.x, Count.scale.y,
            Count.texPos.x, Count.texPos.y,
            Count.texSize.x, Count.texSize.y,
            Count.pivot.x, Count.pivot.y,
            ToRadian(0),
            Count.color.x, Count.color.y, Count.color.z, Count.color.w
        );
    }

    
        sprite_render(sprGstart, Start.position.x, Start.position.y,
            Start.scale.x, Start.scale.y,
            Start.texPos.x, Start.texPos.y,
            Start.texSize.x, Start.texSize.y,
            Start.pivot.x, Start.pivot.y,
            ToRadian(0),
            Start.color.x, Start.color.y, Start.color.z, Start.color.w
        );
    

    debug::setString("count_timer%d", count_timer);
    

}
