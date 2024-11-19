#include "ring.h"
#include "player.h"
#include "common.h"
#include "Collision.h"
#include <cstdlib>  // rand() と srand() のためにインクルード
#include <algorithm>  // std::sort のためにインクルード
#include <ctime>  // time() のためにインクルード
#include <string>

using namespace input;
using namespace std;

int ring_state;
int gold_count;
int red_count;
int rainbow_count;
float game_timer ;
float next_ring_timer ;
extern int score;

extern PLAYER player;
RING goldRings[MAX_RINGS];
RING redRings[MAX_RINGS];
RING rainbowRings[MAX_RINGS];
Sprite* sprRing_gold;
Sprite* sprRing_red; 
Sprite* sprRing_rainbow;



// Z軸の最大値と最小値
const float MAX_Z = 0.0f; // Zの最大値
const float MIN_Z = -200.0f; // Zの最小値




// リングの初期化関数
void ring_init() {
 
    ring_state = 0;
    gold_count = 0;
    red_count = 0;
    rainbow_count = 0;
    game_timer = 0;
    next_ring_timer = 0;
    score = 0;
}



void ring_deinit() {
    
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
    safe_delete(sprRing_rainbow);
}

void spawn_ring(float x = 0.0f, float y = 0.0f)
{
    int random_value = rand() % 100; // 0〜99の乱数を生成
    int ring_type;

    if (random_value <= 70) {
        ring_type = 0; // 70% 金リング
    }
    else if (random_value>=71&&random_value <= 90) {
        ring_type = 1; // 20% 赤リング
    }
    else {
        ring_type = 2; // 10% 虹リング
    }

    switch (ring_type)
    {
    case 0: // 金リング
        if (gold_count < MAX_RINGS)
        {
            goldRings[gold_count] = {};
            goldRings[gold_count].position = { x, y, MIN_Z };
            goldRings[gold_count].scale = { 0.0f, 0.0f };
            goldRings[gold_count].texSize = { RING_TEX_W, RING_TEX_H };
            goldRings[gold_count].pivot = { RING_PIVOT_X, RING_PIVOT_Y };
            goldRings[gold_count].color = { 1.0f, 1.0f, 1.0f, 1.0f };
            goldRings[gold_count].radius = 150;
            goldRings[gold_count].offset = { 0,0,0 };
            gold_count++;
        }
        break;

    case 1: // 赤リング
        if (red_count < MAX_RINGS)
        {
            redRings[red_count] = {};
            redRings[red_count].position = { x, y, MIN_Z };
            redRings[red_count].scale = { 0.0f, 0.0f };
            redRings[red_count].texSize = { RING_TEX_W, RING_TEX_H };
            redRings[red_count].pivot = { RING_PIVOT_X, RING_PIVOT_Y };
            redRings[red_count].color = { 1.0f, 1.0f, 1.0f, 1.0f };
            redRings[red_count].radius = 150;
            redRings[red_count].offset = { 0,0,0 };
            red_count++;
        }
        break;

    case 2: // 虹リング
        if (rainbow_count < MAX_RINGS)
        {
            rainbowRings[rainbow_count] = {};
            rainbowRings[rainbow_count].position = { x, y, MIN_Z };
            rainbowRings[rainbow_count].scale = { 0.0f, 0.0f };
            rainbowRings[rainbow_count].texSize = { RING_TEX_W, RING_TEX_H };
            rainbowRings[rainbow_count].pivot = { RING_PIVOT_X, RING_PIVOT_Y };
            rainbowRings[rainbow_count].color = { 1.0f, 1.0f, 1.0f, 1.0f };
            rainbowRings[rainbow_count].radius = 150;
            rainbowRings[rainbow_count].offset = { 0,0,0 };
            rainbow_count++;
        }
        break;
    }
}


// 新しく追加するフラグ
static int ring_generate_count = 0;  // 生成されたリングの数を追跡

void spawn_ring_randomly(float x, float y) {
    spawn_ring(x, y);  // 位置を指定してリングを生成
}

void ring_update_positions() {
    for (int i = 0; i < gold_count; i++) {
        goldRings[i].position.z += 2.0f;  // Z軸方向に手前に移動
        
            if (goldRings[i].position.z > MAX_Z) {
                // リストの最後のリングと入れ替え、カウントを減らす
                goldRings[i] = goldRings[--gold_count];
            }
        

    }
    for (int i = 0; i < red_count; i++) {
        redRings[i].position.z += 2.0f;
        if (redRings[i].position.z > MAX_Z) {
            // リストの最後のリングと入れ替え、カウントを減らす
            redRings[i] = redRings[--red_count];
        }
    }
    for (int i = 0; i < rainbow_count; i++) {
        rainbowRings[i].position.z += 2.0f;
        if (rainbowRings[i].position.z > MAX_Z) {
            // リストの最後のリングと入れ替え、カウントを減らす
            rainbowRings[i] = rainbowRings[--rainbow_count];
        }
    }
}

void adjust_ring_scales() {
    for (int i = 0; i < gold_count; i++) {
        float scaleFactor = (goldRings[i].position.z - MIN_Z) / (MAX_Z - MIN_Z);
        goldRings[i].scale = { scaleFactor, scaleFactor };
    }
    for (int i = 0; i < red_count; i++) {
        float scaleFactor = (redRings[i].position.z - MIN_Z) / (MAX_Z - MIN_Z);
        redRings[i].scale = { scaleFactor, scaleFactor };
    }
    for (int i = 0; i < rainbow_count; i++) {
        float scaleFactor = (rainbowRings[i].position.z - MIN_Z) / (MAX_Z - MIN_Z);
        rainbowRings[i].scale = { scaleFactor, scaleFactor };
    }
}



// Z軸での比較関数
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z < b.position.z;  // Z位置が大きいほど手前に描画
}

void sort_rings_by_z() {
    std::sort(goldRings, goldRings + gold_count, compareRingsByZ);
    std::sort(redRings, redRings + red_count, compareRingsByZ);
    std::sort(rainbowRings, rainbowRings + rainbow_count, compareRingsByZ);
}
void ring_update() {
    game_timer += 0.0166f;

    switch (ring_state) {
    case 0:
        // スプライトの読み込み
        sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
        sprRing_red = sprite_load(L"./Data/Images/ring_red.png");
        sprRing_rainbow = sprite_load(L"./Data/Images/ring_rainbow.png");
        ring_state++;
        /*fallthrough*/
    case 1:
        // 1つずつリングを生成
        if (ring_generate_count < MAX_RINGS) {
            // 出現間隔を0.5～3秒のランダムに設定
            if (game_timer > next_ring_timer) {
                // 最初に出現する位置をランダムに決定
                float firstX = static_cast<float>((rand() % 1700) + 200);  // 100 ～ 1800
                float firstY = static_cast<float>((rand() % 700) + 200);   // 100 ～ 900

                // 最初のリングの近辺に出現させる
                float offsetX = static_cast<float>(rand() % 100 - 50);  // -50 ～ 50
                float offsetY = static_cast<float>(rand() % 100 - 50);  // -50 ～ 50
                spawn_ring_randomly(firstX + offsetX, firstY + offsetY);

                // 次のリングの出現時間を調整
                next_ring_timer = game_timer + 0.5f + static_cast<float>(rand() % 100) / 100.0f;

                // 生成したリング数をカウント
                ring_generate_count++;

                
                
            }
        }


        break;
    }

    ring_update_positions();  // Z軸の移動
    sort_rings_by_z();        // 描画順のソート
    adjust_ring_scales();     // スケール調整
    judge();                  // 当たり判定の処理
}


void ring_render() {
    for (int i = 0; i < gold_count; i++) {
        // 金リングのスプライトを描画
        sprite_render(
            sprRing_gold,
            goldRings[i].position.x, goldRings[i].position.y,
            goldRings[i].scale.x, goldRings[i].scale.y,
            goldRings[i].texPos.x, goldRings[i].texPos.y,
            goldRings[i].texSize.x, goldRings[i].texSize.y,
            goldRings[i].pivot.x, goldRings[i].pivot.y,
            ToRadian(0),
            goldRings[i].color.x, goldRings[i].color.y, goldRings[i].color.z, goldRings[i].color.w
        );

        //// スケールに応じた当たり判定の円を描画
        //primitive::circle(
        //    goldRings[i].position.x + goldRings[i].offset.x,
        //    goldRings[i].position.y + goldRings[i].offset.y,
        //    goldRings[i].radius * goldRings[i].scale.x, // スケールに応じて拡大した半径
        //    1, 1, ToRadian(0),
        //    1, 0, 0, 0.5f
        //);
    }

    for (int i = 0; i < red_count; i++) {
        // 赤リングのスプライトを描画
        sprite_render(
            sprRing_red,
            redRings[i].position.x, redRings[i].position.y,
            redRings[i].scale.x, redRings[i].scale.y,
            redRings[i].texPos.x, redRings[i].texPos.y,
            redRings[i].texSize.x, redRings[i].texSize.y,
            redRings[i].pivot.x, redRings[i].pivot.y,
            ToRadian(0),
            redRings[i].color.x, redRings[i].color.y, redRings[i].color.z, redRings[i].color.w
        );

        //// スケールに応じた当たり判定の円を描画
        //primitive::circle(
        //    redRings[i].position.x + redRings[i].offset.x,
        //    redRings[i].position.y + redRings[i].offset.y,
        //    redRings[i].radius * redRings[i].scale.x, // スケールに応じて拡大した半径
        //    1, 1, ToRadian(0),
        //    1, 0, 0, 0.5f
        //);
    }

    for (int i = 0; i < rainbow_count; i++) {
        // 虹リングのスプライトを描画
        sprite_render(
            sprRing_rainbow,
            rainbowRings[i].position.x, rainbowRings[i].position.y,
            rainbowRings[i].scale.x, rainbowRings[i].scale.y,
            rainbowRings[i].texPos.x, rainbowRings[i].texPos.y,
            rainbowRings[i].texSize.x, rainbowRings[i].texSize.y,
            rainbowRings[i].pivot.x, rainbowRings[i].pivot.y,
            ToRadian(0),
            rainbowRings[i].color.x, rainbowRings[i].color.y, rainbowRings[i].color.z, rainbowRings[i].color.w
        );

        //// スケールに応じた当たり判定の円を描画
        //primitive::circle(
        //    rainbowRings[i].position.x + rainbowRings[i].offset.x,
        //    rainbowRings[i].position.y + rainbowRings[i].offset.y,
        //    rainbowRings[i].radius * rainbowRings[i].scale.x, // スケールに応じて拡大した半径
        //    1, 1, ToRadian(0),
        //    1, 0, 0, 0.5f
        //);
    }
   //debug::setString("       %d", score);
    text_out(6, "SCORE"+to_string(score), 10, 50, 1, 1, 1, 1, 1, 1, TEXT_ALIGN::MIDDLE_LEFT);
}

void judge() 
{
    for (int i = 0; i < gold_count; i++) 
    {
        if (goldRings[i].position.z >= MAX_Z)//zの位置が最大値以上なら
        {
            if (hitCheckRing(&player, &goldRings[i])) //当たり判定の処理
            {
                score += 100;//スコアを増やす
                for (int j = i; j < gold_count - 1; j++) {
                    goldRings[j] = goldRings[j + 1];
                }
                gold_count--;
                i--;
            }
        }
        
    }

    for (int i = 0; i < red_count; i++)
    {
        if (redRings[i].position.z >= MAX_Z)
        {
            if (hitCheckRing(&player, &redRings[i]))
            {
                score += 500;
                for (int j = i; j < red_count - 1; j++) {
                    redRings[j] = redRings[j + 1];
                }
                red_count--;
                i--;
            }
        }

    }

    for (int i = 0; i < rainbow_count; i++)
    {
        if (rainbowRings[i].position.z >= MAX_Z)
        {
            if (hitCheckRing(&player, &rainbowRings[i]))
            {
                score += 1000;
                for (int j = i; j < rainbow_count - 1; j++) {
                    rainbowRings[j] = rainbowRings[j + 1];
                }
                rainbow_count--;
                i--;
            }
        }

    }
}



