#include "ring.h"
#include "common.h"
#include <cstdlib>  // rand() と srand() のためにインクルード
#include <algorithm>  // std::sort のためにインクルード

using namespace input;

// デフォルトのリングの数（ランタイムで変更可能）
int numRings = 50;  // リングの数の例
const int START_DELAY = 60 * 7; // 7秒の遅延（60FPSで7秒）

RING* goldRings; // ゴールドリング用の動的配列
RING redRing;
Sprite* sprRing_gold;
Sprite* sprRing_red;

int ring_start_timer = 0;  // リングの開始前のタイマー


// Z軸の最大値と最小値
const float MAX_Z = 0.0f; // Zの最大値
const float MIN_Z = -50.0f; // Zの最小値

// 手動で指定する位置の配列
std::vector<VECTOR2> ring_positions = {
    {100, 100}, {200, 150}, {300, 200}, {400, 250}, {500, 300},
    {600, 350}, {700, 400}, {800, 450}, {900, 500}, {1000, 550}
    // 必要に応じて座標を追加
};

// リングの初期化
void ring_init() {
    // ランダムのために乱数のシードを設定
    srand(static_cast<unsigned int>(time(nullptr)));

    // リングのテクスチャをロード
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");

    // ゴールドリング用のメモリを動的に確保
    goldRings = new RING[numRings];

    // リングの初期化
    for (int i = 0; i < numRings; ++i) {
        goldRings[i].scale = { 0.1f, 0.1f };  // スケール
        goldRings[i].angle = ToRadian(0);  // 角度
        goldRings[i].texPos = { 0, 0 };  // テクスチャの位置
        goldRings[i].texSize = { RING_TEX_W, RING_TEX_H };  // テクスチャのサイズ
        goldRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };  // ピボット位置

        // 手動で指定した位置を使ってリングの位置を設定
        if (i < ring_positions.size()) {
            goldRings[i].position.x = ring_positions[i].x;  // x座標を指定
            goldRings[i].position.y = ring_positions[i].y;  // y座標を指定
        }
        else {
            // もし位置が足りない場合、ランダムで配置（必要な数だけ手動で指定し、足りない場合のみ）
            goldRings[i].position.x = rand() % SCREEN_W;
            goldRings[i].position.y = rand() % SCREEN_H;
        }

        goldRings[i].position.z = MIN_Z; // 初期Z位置
        goldRings[i].update_delay = i * 60; // 更新を少し遅らせる
        goldRings[i].update_counter = 0;  // 更新カウンタ
    }

    // 赤いリングの初期位置を設定
    redRing.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };
    redRing.scale = { 0.5f, 0.5f };
    redRing.angle = ToRadian(0);
    redRing.texPos = { 0, 0 };
    redRing.texSize = { RING_TEX_W, RING_TEX_H };
    redRing.pivot = { RING_PIVOT_X, RING_PIVOT_Y };

    ring_start_timer = 0;  // タイマーをリセット
}


// リングの後始末
void ring_deinit() {
    // ゴールドリングのメモリを解放
    delete[] goldRings;

    // スプライトのリソースをクリーンアップ
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
}

// リングの更新
void ring_update() {
    // タイマーが開始遅延時間より短い場合、カウントアップして戻る
    if (ring_start_timer < START_DELAY) {
        ring_start_timer++;
        return;
    }

    // 各リングを更新
    for (int i = 0; i < numRings; ++i) {
        if (goldRings[i].update_counter >= goldRings[i].update_delay) {
            // Z位置を更新（リングをカメラに向かって移動、手前側が数値が小さい）
            goldRings[i].position.z -= 1.0f;

            // 最小Zに達したらリングの位置をリセット
            if (goldRings[i].position.z < MIN_Z) {
                goldRings[i].position.z = MAX_Z;
                goldRings[i].update_counter = 0; // カウンタをリセット
            }

            // Z位置に基づいてスケールを調整（近くのリングが大きく見えるように）
            float scale_factor = (MAX_Z - goldRings[i].position.z) / (MAX_Z - MIN_Z);
            scale_factor *= 3.0f;  // スケーリングを加速（この値を増加させると速くなる）

            goldRings[i].scale = { 0.1f + scale_factor * 0.2f, 0.1f + scale_factor * 0.2f };
        }
        else {
            goldRings[i].update_counter++;  // 更新カウンタをインクリメント
        }
    }
}

// リングの描画
void ring_render() {
    // タイマーが開始遅延時間より短い場合、描画をスキップ
    if (ring_start_timer < START_DELAY) {
        return;
    }

    // リングをZ位置に基づいてソート（遠いリングは後に描画される）
    std::sort(goldRings, goldRings + numRings, compareRingsByZ);

    // ソート後、リングを描画
    for (int i = 0; i < numRings; ++i) {
        if (goldRings[i].update_counter >= goldRings[i].update_delay) {
            sprite_render(
                sprRing_gold,
                goldRings[i].position.x, goldRings[i].position.y, // リングの位置
                goldRings[i].scale.x, goldRings[i].scale.y,
                goldRings[i].texPos.x, goldRings[i].texPos.y,
                goldRings[i].texSize.x, goldRings[i].texSize.y,
                goldRings[i].pivot.x, goldRings[i].pivot.y
            );
           
        }
    }

    // すべてのゴールドリングを描画した後に赤いリングを描画
    sprite_render(
        sprRing_red,
        redRing.position.x, redRing.position.y,
        redRing.scale.x, redRing.scale.y,
        redRing.texPos.x, redRing.texPos.y,
        redRing.texSize.x, redRing.texSize.y,
        redRing.pivot.x, redRing.pivot.y
    );
   
}

// Z軸で比較するための関数
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z > b.position.z;  // Z位置が大きいほど手前に描画される
}
