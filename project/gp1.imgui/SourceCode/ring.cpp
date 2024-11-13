#include "ring.h"
#include "common.h"
#include <cstdlib>  // rand() と srand() のためにインクルード
#include <algorithm>  // std::sort のためにインクルード

using namespace input;

// デフォルトのリングの数（ランタイムで変更可能）
int numRings = 20;  // リングの数の例
const int START_DELAY = 60 * 5; // 5秒の遅延（60FPSで5秒）

RING* goldRings; // ゴールドリング用の動的配列
RING* redRings; // レッドリングのオブジェクト
RING* rainbowRings;
Sprite* sprRing_gold; // ゴールドリングのスプライト
Sprite* sprRing_red;  // レッドリングのスプライト
Sprite* sprRing_rainbow;

int ring_start_timer = 0;  // リングの開始前のタイマー

// Z軸の最大値と最小値
const float MAX_Z = 300.0f; // Zの最大値
const float MIN_Z = 100.0f; // Zの最小値

const int RING_GROUP_SIZE = rand() % 6;      // グループごとのリングの数
const int GROUP_DISPLAY_DELAY = rand() % 180; // グループ間の遅延（フレーム単位、3秒を想定）

// ゴールドリング、赤リング、虹リングの位置を別々に設定
std::vector<VECTOR2> ring_positions_gold = {
    {300, 300}, {300, 250}, {300, 300}  // ゴールドリングの位置
};

std::vector<VECTOR2> ring_positions_red = {
    {800, 250}, {800, 300}, {800, 350}  // 赤リングの位置
};

std::vector<VECTOR2> ring_positions_rainbow = {
    {1000, 350}, {1000, 400}, {1000, 450}  // 虹リングの位置
};

// リングの初期化関数
void ring_init() {
    srand(static_cast<unsigned int>(time(nullptr))); // 乱数の初期化

    // スプライトの読み込み
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");
    sprRing_rainbow = sprite_load(L"./Data/Images/ring_rainbow.png");

    // ゴールドリングのメモリ割り当て
    goldRings = new RING[numRings];
    redRings = new RING[numRings];
    rainbowRings = new RING[numRings];

    // ゴールドリングの初期化
    for (int i = 0; i < numRings; ++i) {
        goldRings[i].scale = { 0.0f, 0.0f };
        goldRings[i].angle = ToRadian(0);
        goldRings[i].texPos = { 0, 0 };
        goldRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        goldRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // 位置の設定
        if (i < ring_positions_gold.size()) {
            goldRings[i].position.x = ring_positions_gold[i].x;
            goldRings[i].position.y = ring_positions_gold[i].y;
        }
        else {
            goldRings[i].position.x = ring_positions_gold.back().x; // 追加リングは最後の位置を使用
            goldRings[i].position.y = ring_positions_gold.back().y;
        }

        goldRings[i].position.z = MIN_Z;
        goldRings[i].update_delay = i * 20; // 更新遅延をリングごとに設定
        goldRings[i].update_counter = 0;
        goldRings[i].is_active = true;
    }

    // 赤リングの初期化
    for (int i = 0; i < numRings; ++i) {
        redRings[i].scale = { 0.0f, 0.0f };
        redRings[i].angle = ToRadian(0);
        redRings[i].texPos = { 0, 0 };
        redRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        redRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // 位置の設定
        if (i < ring_positions_red.size()) {
            redRings[i].position.x = ring_positions_red[i].x;
            redRings[i].position.y = ring_positions_red[i].y;
        }
        else {
            redRings[i].position.x = ring_positions_red.back().x; // 追加リングは最後の位置を使用
            redRings[i].position.y = ring_positions_red.back().y;
        }

        redRings[i].position.z = MIN_Z;
        redRings[i].update_delay = i * 20; // 更新遅延をリングごとに設定
        redRings[i].update_counter = 0;
        redRings[i].is_active = true;
    }

    // 虹リングの初期化
    for (int i = 0; i < numRings; ++i) {
        rainbowRings[i].scale = { 0.0f, 0.0f };
        rainbowRings[i].angle = ToRadian(0);
        rainbowRings[i].texPos = { 0, 0 };
        rainbowRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        rainbowRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // 位置の設定
        if (i < ring_positions_rainbow.size()) {
            rainbowRings[i].position.x = ring_positions_rainbow[i].x;
            rainbowRings[i].position.y = ring_positions_rainbow[i].y;
        }
        else {
            rainbowRings[i].position.x = ring_positions_rainbow.back().x; // 追加リングは最後の位置を使用
            rainbowRings[i].position.y = ring_positions_rainbow.back().y;
        }

        rainbowRings[i].position.z = MIN_Z;
        rainbowRings[i].update_delay = i * 20; // 更新遅延をリングごとに設定
        rainbowRings[i].update_counter = 0;
        rainbowRings[i].is_active = true;
    }

    ring_start_timer = 0;
}



void ring_deinit() {
    delete[] goldRings; // ゴールドリングのメモリ解放
    delete[] redRings;
    delete[] rainbowRings;

    // スプライトリソースのクリーンアップ
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
    safe_delete(sprRing_rainbow);
}

void ring_update() {
    // 開始遅延の間はカウントを進めるだけ
    if (ring_start_timer < START_DELAY) {
        ring_start_timer++;
        return;
    }

    // 現在のアクティブグループ数を計算
    int active_groups = (ring_start_timer - START_DELAY) / GROUP_DISPLAY_DELAY + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE, numRings) - 1;

    // ゴールドリングの更新
    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active) {
            // 更新遅延を経過したリングを処理
            if (goldRings[i].update_counter >= goldRings[i].update_delay) {
                goldRings[i].position.z -= 1.0f; // Z軸の移動

                // Z位置が下限を超えた場合のリセット処理
                if (goldRings[i].position.z < MIN_Z) {
                    goldRings[i].position.z = MAX_Z;
                    goldRings[i].update_counter = 0;
                }

                // スケールをZ位置に基づいて調整
                float scale_factor = (MAX_Z - goldRings[i].position.z) / (MAX_Z - MIN_Z);
                scale_factor *= 3.0f;

                goldRings[i].scale = { 0.1f + scale_factor * 0.5f, 0.1f + scale_factor * 0.5f };

                // スケールが一定以上になったら非アクティブにする
                if (scale_factor >= 1.0f) {
                    goldRings[i].is_active = false;
                }
            }
            else {
                goldRings[i].update_counter++;
            }
        }
    }

    // レッドリングの更新 
   
        
        for (int i = 0; i <= max_active_ring_index; ++i) {
            if (redRings[i].is_active) {
                // 更新遅延を経過したリングを処理
                if (redRings[i].update_counter >= redRings[i].update_delay) {
                    redRings[i].position.z -= 1.0f; // Z軸の移動

                    // Z位置が下限を超えた場合のリセット処理
                    if (redRings[i].position.z < MIN_Z) {
                        redRings[i].position.z = MAX_Z;
                        redRings[i].update_counter = 0;
                    }

                    // スケールをZ位置に基づいて調整
                    float scale_factor = (MAX_Z - redRings[i].position.z) / (MAX_Z - MIN_Z);
                    scale_factor *= 3.0f;

                    redRings[i].scale = { 0.1f + scale_factor * 0.5f, 0.1f + scale_factor * 0.5f };

                    // スケールが一定以上になったら非アクティブにする
                    if (scale_factor >= 1.0f) {
                        redRings[i].is_active = false;
                    }
                }
                else {
                    redRings[i].update_counter++;
                }
            }
        }
    
    // 虹リングの更新 
    
        
        for (int i = 0; i <= max_active_ring_index; ++i) {
            if (rainbowRings[i].is_active) {
                // 更新遅延を経過したリングを処理
                if (rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
                    rainbowRings[i].position.z -= 1.0f; // Z軸の移動

                    // Z位置が下限を超えた場合のリセット処理
                    if (rainbowRings[i].position.z < MIN_Z) {
                        rainbowRings[i].position.z = MAX_Z;
                        rainbowRings[i].update_counter = 0;
                    }

                    // スケールをZ位置に基づいて調整
                    float scale_factor = (MAX_Z - rainbowRings[i].position.z) / (MAX_Z - MIN_Z);
                    scale_factor *= 3.0f;

                    rainbowRings[i].scale = { 0.1f + scale_factor * 0.5f, 0.1f + scale_factor * 0.5f };

                    // スケールが一定以上になったら非アクティブにする
                    if (scale_factor >= 1.0f) {
                        rainbowRings[i].is_active = false;
                    }
                }
                else {
                    rainbowRings[i].update_counter++;
                }
            }
        }
    
    // タイマーの増加
    ring_start_timer++;
}


// リングの描画処理
void ring_render() {
    if (ring_start_timer < START_DELAY) {
        return;
    }

    // Z軸で並べ替え
    std::sort(goldRings, goldRings + numRings, compareRingsByZ);
    std::sort(redRings, redRings + numRings, compareRingsByZ);
    std::sort(rainbowRings, rainbowRings + numRings, compareRingsByZ);

    // 現在アクティブなグループ数を計算
    int active_groups = (ring_start_timer - START_DELAY) / GROUP_DISPLAY_DELAY + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE, numRings) - 1;

    // アクティブなリングを描画
    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active && goldRings[i].update_counter >= goldRings[i].update_delay) {
            // ゴールドリングを描画
            sprite_render(sprRing_gold, goldRings[i].position.x, goldRings[i].position.y, goldRings[i].scale.x, goldRings[i].scale.y, goldRings[i].texPos.x,goldRings[i].texPos.y,goldRings[i].texSize.x,goldRings[i].texSize.y,goldRings[i].pivot.x, goldRings[i].pivot.y);
        }
        if (redRings[i].is_active && redRings[i].update_counter >= redRings[i].update_delay) {
            // レッドリングを描画（確率で表示）
            sprite_render(sprRing_red, redRings[i].position.x, redRings[i].position.y, redRings[i].scale.x, redRings[i].scale.y, redRings[i].texPos.x, redRings[i].texPos.y, redRings[i].texSize.x, redRings[i].texSize.y, redRings[i].pivot.x, redRings[i].pivot.y);
        }
        if (rainbowRings[i].is_active && rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
            // 虹リングを描画（確率で表示）
            sprite_render(sprRing_rainbow, rainbowRings[i].position.x, rainbowRings[i].position.y, rainbowRings[i].scale.x, rainbowRings[i].scale.y, rainbowRings[i].texPos.x, rainbowRings[i].texPos.y, rainbowRings[i].texSize.x, rainbowRings[i].texSize.y, rainbowRings[i].pivot.x, rainbowRings[i].pivot.y);
        }
    }
}



// Z軸での比較関数
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z > b.position.z;  // Z位置が大きいほど手前に描画される
}
