#include "count.h"
#include "common.h"

using namespace std;



Sprite* sprCount;
Sprite* sprGstart;
Sprite* sprRing;
COUNT Count;
COUNT Start;
COUNT Ring;



void count_init()
{
    Count.count_state = 0;
    Count.count_timer = 0;
    
    sprCount = sprite_load(L"./Data/Images/countdown.png");
    sprGstart = sprite_load(L"./Data/Images/start.png");
    sprRing = sprite_load(L"./Data/Images/countring.png");
}

void count_deinit()
{
    safe_delete(sprCount);
    safe_delete(sprGstart);
    safe_delete(sprRing);
}

void count_update()
{
    switch (Count.count_state)
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

        Ring = {};
        Ring.position = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
        Ring.pivot = { CRING_PIVOT_X, CRING_PIVOT_Y };
        Ring.color = { 1, 1, 1, 1 };
        Ring.texSize = { CRING_TEX_W, CRING_TEX_H };

        Count.count_state++;
        /*fallthrough*/
    }
    case 1:
    {
        float count_index = Count.count_timer / 60; //カウントダウン番号(0 = 3, 1 = 2, 2 = 1)
        float count_frame_index = (Count.count_timer % 60) / 3; // アニメーションフレーム
        float ring_frame_index = (Count.count_timer % 60) / 3; // リングアニメーションフレーム(ループさせる)

        if (count_index < 3) {
            // カウントダウンアニメーション更新
            Count.texPos = { count_frame_index * COUNT_TEX_W, count_index * COUNT_TEX_H };

            // リングアニメーション更新
            Ring.texPos = { ring_frame_index * CRING_TEX_W, 0 };

            // カウントダウンのスケール
            Count.scale = { 1.5f, 1.5f };

            // リングのスケールの処理
            float ring_scale = 1.5f + 3.0f * (Count.count_timer % 60) / 60.0f;
            Ring.scale = { ring_scale, ring_scale };
        }
        else if (count_index >= 3)
        {
            
            Count.count_done = true;
            Count.count_timer = 0;
            Count.count_state++;
        }

      
        Count.count_timer++;
        break;
    }


    case 2:
    {
        float start_index = Count.count_timer / 60; 
        float frame_index2 = (Count.count_timer % 60) / 4; 
        // カウント画像の表示位置 (texPos) を更新
        Start.texPos = { frame_index2 * START_TEX_W, start_index * START_TEX_H };

        //// カウントダウンのスケールアニメーション設定
        //if (count_timer % 60 < 20)
        //{
        //    float t = (count_timer % 20) / 20.0f;
        //    Start.scale = { 0.5f + 1.5f * t, 0.5f + 1.5f * t };
        //}
        //else if (count_timer % 60 < 40)
        //{
        //    Start.scale = { 2.0f, 2.0f };
        //}
        //else
        //{
        //    float t = (count_timer % 20) / 20.0f;
        //    Start.scale = { 2.0f - 0.7f * t, 2.0f - 0.7f * t };
        //}

        Start.scale = { 1.5f,1.5f };

        Count.count_timer++;

    }
    }
}

void count_render()
{
    if (!Count.count_done)
    {
        
        sprite_render(sprRing, Ring.position.x, Ring.position.y,
            Ring.scale.x, Ring.scale.y,
            Ring.texPos.x, Ring.texPos.y,
            Ring.texSize.x, Ring.texSize.y,
            Ring.pivot.x, Ring.pivot.y,
            ToRadian(0),
            Ring.color.x, Ring.color.y, Ring.color.z, Ring.color.w
        );

        
        sprite_render(sprCount, Count.position.x, Count.position.y,
            Count.scale.x, Count.scale.y,
            Count.texPos.x, Count.texPos.y,
            Count.texSize.x, Count.texSize.y,
            Count.pivot.x, Count.pivot.y,
            ToRadian(0),
            Count.color.x, Count.color.y, Count.color.z, Count.color.w
        );
    }
    else if (Count.count_state == 2)
    {
        

      
        sprite_render(sprGstart, Start.position.x, Start.position.y,
            Start.scale.x, Start.scale.y,
            Start.texPos.x, Start.texPos.y,
            Start.texSize.x, Start.texSize.y,
            Start.pivot.x, Start.pivot.y,
            ToRadian(0),
            Start.color.x, Start.color.y, Start.color.z, Start.color.w
        );
    }

   
}

