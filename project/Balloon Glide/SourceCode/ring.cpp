#include "ring.h"
#include "player.h"
#include "common.h"
#include "Collision.h"
#include"audio.h"
#include <cstdlib>  // rand() と srand() のためにインクルード
#include <algorithm>  // std::sort のためにインクルード
#include <ctime>  // time() のためにインクルード
#include<string>
#include<vector>

using namespace std;
using namespace input;

int ring_state;
int gold_count;
int red_count;
int rainbow_count;
int secret_count;
float game_timer ;
float next_ring_timer ;
int score;
int gold_ring_count;
int red_ring_count;
int rainbow_ring_count;
int secret_ring_count;
extern  int boostCount;


extern PLAYER player;
std::vector<RING> rings;
Sprite* sprRing_gold;
Sprite* sprRing_red; 
Sprite* sprRing_rainbow;
Sprite* sprRing_secret;



// Z軸の最大値と最小値
const float MAX_Z = 0.0f; // Zの最大値
const float MIN_Z = -200.0f; // Zの最小値

int ring_generate_count;  // 生成されたリングの数を追跡



// リングの初期化関数
void ring_init() {
 
    ring_state = 0;
    gold_count = 0;
    red_count = 0;
    rainbow_count = 0;
    secret_count = 0;
    game_timer = 0;
    next_ring_timer = 0;
    score = 0;
    gold_ring_count = 0;
    red_ring_count = 0;
    rainbow_ring_count = 0;
    secret_ring_count = 0;
    ring_generate_count = 0;
    
    rings.clear();
    
}



void ring_deinit() {
    
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
    safe_delete(sprRing_rainbow);
    safe_delete(sprRing_secret);
    music::stop(BGM_RING);
    music::stop(BGM_RAINBOW);
    music::stop(BGM_SECRET);
}

void spawn_ring(float x = 0.0f, float y = 0.0f) {
    float random_value = rand() % 2000; // 0〜1999の乱数
    RING_TYPE ringType;

    if (random_value < 1400) { // 0〜1399 (70%)
        ringType = RING_TYPE::GOLD;
    }
    else if (random_value < 1900) { // 1400〜1899 (25%)
        ringType = RING_TYPE::RED;
    }
    else if (random_value < 1999) { // 1900〜1998 (4.95%)
        ringType = RING_TYPE::RAINBOW;
    }
    else if (random_value < 2000) { // 1999〜1999 (0.05%)
        ringType = RING_TYPE::SECRET;
    }




    RING newRing = {
        {x, y, MIN_Z},
        {0.0f, 0.0f},
        {RING_TEX_W, RING_TEX_H},
        {RING_PIVOT_X, RING_PIVOT_Y},
        {1.0f, 1.0f, 1.0f, 1.0f},
        100.0f,
        ringType // リングの種類を設定
    };

    rings.push_back(newRing);
}







void spawn_ring_randomly(float x, float y) {
    spawn_ring(x, y);  // 位置を指定してリングを生成
}

// リングの位置更新
void ring_update_positions() {
    for (auto ring = rings.begin(); ring != rings.end();) {
        ring->position.z += 2.5f;
        if (ring->position.z > MAX_Z) {
            ring = rings.erase(ring);
        }
        else {
            ++ring;
        }
    }
}

// リングのスケール調整
void adjust_ring_scales() {
    for (auto& ring : rings) {
        float scaleFactor = (ring.position.z - MIN_Z) / (MAX_Z - MIN_Z);
        ring.scale = { scaleFactor, scaleFactor };
    }
}



// Z軸での比較関数
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z < b.position.z;  // Z位置が大きいほど手前に描画
}

// 描画順をZ軸でソート
void sort_rings_by_z() {
    auto compare_by_z = [](const RING& a, const RING& b) {
        return a.position.z < b.position.z;
    };

    std::sort(rings.begin(), rings.end(), compare_by_z);
   
}

// リングの更新
void ring_update() {
    // ゲームタイマーの更新
    game_timer += 0.0166f; // フレーム時間

    // リングの初期化と画像ロード
    if (ring_state == 0) {
        sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
        sprRing_red = sprite_load(L"./Data/Images/ring_red.png");
        sprRing_rainbow = sprite_load(L"./Data/Images/ring_rainbow.png");
        sprRing_secret = sprite_load(L"./Data/Images/ring_purple.png");
        ring_state = 1; // 状態を更新
    }

    // リングの生成（最大数を超えないように）
    if (ring_state == 1 && ring_generate_count < MAX_RINGS) {
        if (game_timer > next_ring_timer) {
            // ランダム座標の生成
            float x = static_cast<float>((rand() % 1500) + 200);  // X範囲: 200 ～ 1700
            float y = static_cast<float>((rand() % 700) + 200);   // Y範囲: 200 ～ 900

            // リングの生成
            spawn_ring(x, y);

            // 次の生成タイミングを設定
            next_ring_timer = game_timer + 0.5f + static_cast<float>(rand() % 100) / 100.0f;
            ring_generate_count++;
        }
    }

    
    ring_update_positions();// リングの位置更新と削除   
    sort_rings_by_z();// 描画順を調整
    adjust_ring_scales();// リングのスケール調整
    judge();// 当たり判定のチェック
}

// リングの描画
void ring_render() {
    for (const auto& ring : rings) {
        Sprite* sprite = nullptr;
        switch (ring.type) { // 各リングの種類を参照
        case RING_TYPE::GOLD:
            sprite = sprRing_gold;
            break;
        case RING_TYPE::RED:
            sprite = sprRing_red;
            break;
        case RING_TYPE::RAINBOW:
            sprite = sprRing_rainbow;
            break;
        case RING_TYPE::SECRET:
            sprite = sprRing_secret;
            break;
        }
        if (sprite) {
            sprite_render(sprite,
                ring.position.x, ring.position.y,
                ring.scale.x, ring.scale.y,
                0, 0, RING_TEX_W, RING_TEX_H,
                RING_PIVOT_X, RING_PIVOT_Y,
                ToRadian(0),
                ring.color.x, ring.color.y, ring.color.z, ring.color.w);
            /*primitive::circle(ring.position.x + ring.offset.x,
                ring.position.y + ring.offset.y,
                ring.radius, ring.scale.x, ring.scale.y, ToRadian(0), 1, 0, 0, 0.2f);*/
        }
    }
    text_out(6, "SCORE :" + std::to_string(score), 10, 10, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, TEXT_ALIGN::UPPER_LEFT);
    
}



void judge() {
    for (size_t i = 0; i < rings.size(); ) {
        if (rings[i].position.z >= MAX_Z) {
            if (hitCheckRing(&player, &rings[i])) { // 当たり判定
                switch (rings[i].type) { // 各リングの種類を参照
                case RING_TYPE::GOLD:
                    score += 100; 
                    gold_ring_count++;
                    music::play(BGM_RING, false);
                    break;
                case RING_TYPE::RED:
                    score += 500;
                    red_ring_count++;
                    music::play(BGM_RING, false);
                    break;
                case RING_TYPE::RAINBOW:
                    score += 1000;
                    rainbow_ring_count++;
                    music::play(BGM_RAINBOW, false);
                    music::setVolume(BGM_RAINBOW, 1.0f);
                    break;
                case RING_TYPE::SECRET:
                    score += 2000; // シークレットリング自体のスコア加算
                    secret_ring_count++;
                    music::play(BGM_SECRET, false); 
                    boostCount++;
                    break;
                }

                rings.erase(rings.begin() + i); // リングを削除
            }
            else {
                ++i;
            }
        }
        else {
            ++i;
        }
    }
}




