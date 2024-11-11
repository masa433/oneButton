#include "ring.h"
#include "common.h"

using namespace input;

const int MAX_RINGS = 5; // 表示するリングの数
RING goldRings[MAX_RINGS];
RING redRing;
Sprite* sprRing_gold;
Sprite* sprRing_red;

void ring_init() {
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");

    // 各リングの初期設定
    for (int i = 0; i < MAX_RINGS; ++i) {
        goldRings[i].scale = { 0.1f, 0.1f };
        goldRings[i].angle = ToRadian(0);
        goldRings[i].texPos = { 0, 0 };
        goldRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        goldRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };
        
        goldRings[i].position.x = SCREEN_W * 0.5f ; 
        goldRings[i].position.y = SCREEN_H * 0.3f ; 
        goldRings[i].update_delay = i * 60; // 各リングの更新を少しずつ遅らせる
        goldRings[i].update_counter = 0;
    }
    redRing.position = { SCREEN_W * 0.5f ,SCREEN_H * 0.7f };
    redRing.scale = { 0.5f, 0.5f };
    redRing.angle = ToRadian(0);
    redRing.texPos = { 0, 0 };
    redRing.texSize = { RING_TEX_W, RING_TEX_H };
    redRing.pivot = { RING_PIVOT_X, RING_PIVOT_Y };


}

void ring_deinit() {
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
}

void ring_update() {
    for (int i = 0; i < MAX_RINGS; ++i) {
        // カウンターが指定の遅延時間に達したリングだけ更新する
        if (goldRings[i].update_counter >= goldRings[i].update_delay) {
            // リングが奥から手前に来るようにスケールを拡大
            goldRings[i].scale.x += 0.01f;
            goldRings[i].scale.y += 0.01f;

            // スケールが一定値に達したら初期化して奥に戻す
            if (goldRings[i].scale.x > 1.0f) {
                goldRings[i].scale = { 0.1f, 0.1f };
                goldRings[i].update_counter = 0; // カウンターもリセットして遅延再度適用
            }
        }
        else {
            goldRings[i].update_counter++;
        }
    }
}

void ring_render() {
    for (int i = 0; i < MAX_RINGS; ++i) {
        // 描画するリングのみ描画（カウンターが遅延時間を過ぎたリング）
        if (goldRings[i].update_counter >= goldRings[i].update_delay) {
            sprite_render(
                sprRing_gold,
                goldRings[i].position.x, goldRings[i].position.y, // 各リングの位置
                goldRings[i].scale.x, goldRings[i].scale.y,
                goldRings[i].texPos.x, goldRings[i].texPos.y,
                goldRings[i].texSize.x, goldRings[i].texSize.y,
                goldRings[i].pivot.x, goldRings[i].pivot.y
            );
        }
    }

    sprite_render(sprRing_red,
        redRing.position.x, redRing.position.y, // 各リングの位置
        redRing.scale.x, redRing.scale.y,
        redRing.texPos.x, redRing.texPos.y,
        redRing.texSize.x, redRing.texSize.y,
        redRing.pivot.x, redRing.pivot.y);
}
