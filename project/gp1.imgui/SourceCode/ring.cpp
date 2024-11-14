#include "ring.h"
#include "common.h"
#include <cstdlib>  // rand() と srand() のためにインクルード
#include <algorithm>  // std::sort のためにインクルード
#include <ctime>  // time() のためにインクルード

using namespace input;

// デフォルトのリングの数（ランタイムで変更可能）
int numRings_gold = 20;  // リングの数の例
int numRings_red = 10;  // リングの数の例
int numRings_rainbow = 3;  // リングの数の例
const int START_DELAY_GOLD = 60 * 5; // 5秒の遅延（60FPSで5秒）
const int START_DELAY_RED = 60 * 10; 
const int START_DELAY_RAINBOW = 60 * 15;

RING* goldRings; // ゴールドリング用の動的配列
RING* redRings; 
RING* rainbowRings;
Sprite* sprRing_gold; // ゴールドリングのスプライト
Sprite* sprRing_red;  // レッドリングのスプライト
Sprite* sprRing_rainbow;

int ring_start_timer_gold = 0;  // リングの開始前のタイマー
int ring_start_timer_red = 0;  
int ring_start_timer_rainbow = 0;  

// Z軸の最大値と最小値
const float MAX_Z = 300.0f; // Zの最大値
const float MIN_Z = 100.0f; // Zの最小値

// グループごとのリングの数と遅延
int RING_GROUP_SIZE_GOLD;
int RING_GROUP_SIZE_RED;
int RING_GROUP_SIZE_RAINBOW;
int GROUP_DISPLAY_DELAY_GOLD;
int GROUP_DISPLAY_DELAY_RED;
int GROUP_DISPLAY_DELAY_RAINBOW;

#include <vector>
#include <cstdlib>  // rand() と srand() のためにインクルード
#include <ctime>    // time() のためにインクルード

// VECTOR2 構造体の定義
struct POSITION {
    float x, y;
};

// 乱数の範囲を指定する関数（min から max の間）
int getRandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

// グループ1とグループ2のランダムな位置を生成する関数
void generateRingPositions(std::vector<VECTOR2>& ring_positions_gold) {
    srand(static_cast<unsigned int>(time(nullptr))); // 乱数の初期化

    // 1グループ目のランダムな位置設定（最初の3つ）
    for (int i = 0; i < 3; ++i) {
        float x = getRandomInRange(450, 650);  // x座標は450から650の範囲
        float y = getRandomInRange(200, 400);  // y座標は200から400の範囲
        ring_positions_gold.push_back({ x, y });
    }

    // 2グループ目のランダムな位置設定（次の3つ）
    for (int i = 0; i < 3; ++i) {
        float x = getRandomInRange(650, 850);  // x座標は650から850の範囲
        float y = getRandomInRange(400, 600);  // y座標は400から600の範囲
        ring_positions_gold.push_back({ x, y });
    }

    // 最後のゴールの位置を追加
    ring_positions_gold.push_back({ 700, 200 });  // ゴールの位置
}




std::vector<VECTOR2> ring_positions_red = {
    {960, 500}, {1000, 550}, {1040, 600},  // Red rings centered, creating a descending stair pattern
    {1080, 550}, {1120, 500}               // Additional rings to extend the stair pattern
};

std::vector<VECTOR2> ring_positions_rainbow = {
    {1500, 300}, {1550, 350}, {1600, 400}, // Rainbow rings on the right, positioned diagonally
    {1650, 350}, {1700, 300}               // Mirror the gold rings' curve for symmetry
};


// リングの初期化関数
void ring_init() {
    srand(static_cast<unsigned int>(time(nullptr))); // 乱数の初期化

    std::vector<VECTOR2> ring_positions_gold;

    // リングの位置を生成
    generateRingPositions(ring_positions_gold);

    // 動的なリングのグループサイズと遅延の設定
    RING_GROUP_SIZE_GOLD = 3;         // 4～6 の範囲
    RING_GROUP_SIZE_RED = 2;         // 1～6 の範囲
    RING_GROUP_SIZE_RAINBOW = 1;         // 3～6 の範囲
    GROUP_DISPLAY_DELAY_GOLD = 30 + rand() % 60;       // 60～119 の範囲
    GROUP_DISPLAY_DELAY_RED = 60 + rand() % 60;       // 0～119 の範囲
    GROUP_DISPLAY_DELAY_RAINBOW = rand() % 120;       // 0～119 の範囲

    // スプライトの読み込み
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");
    sprRing_rainbow = sprite_load(L"./Data/Images/ring_rainbow.png");

    // ゴールドリングのメモリ割り当て
    goldRings = new RING[numRings_gold];
    redRings = new RING[numRings_red];
    rainbowRings = new RING[numRings_rainbow];

    // ゴールドリングの初期化
    for (int i = 0; i < numRings_gold; ++i) {
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

    ring_start_timer_gold = 0;


    // 赤リングの初期化
    for (int i = 0; i < numRings_red; ++i) {
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

    ring_start_timer_red = 0;

    // 虹リングの初期化
    for (int i = 0; i < numRings_rainbow; ++i) {
        rainbowRings[i].scale = { 0.0f, 0.0f };
        rainbowRings[i].angle = ToRadian(0);
        rainbowRings[i].texPos = { 0, 0 };
        rainbowRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        rainbowRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // 位置の設定
        if (i < ring_positions_gold.size()) {
            rainbowRings[i].position.x = ring_positions_rainbow[i].x;
            rainbowRings[i].position.y = ring_positions_rainbow[i].y;
        }
        else {
            rainbowRings[i].position.x = ring_positions_rainbow.back().x; // 追加リングは最後の位置を使用
            rainbowRings[i].position.y = ring_positions_rainbow.back().y;
        }

        rainbowRings[i].position.z = MIN_Z;
        rainbowRings[i].update_delay = i * 100; // 更新遅延をリングごとに設定
        rainbowRings[i].update_counter = 0;
        rainbowRings[i].is_active = true;
    }

    ring_start_timer_rainbow = 0;
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

void update_gold_rings() {
    int active_groups = (ring_start_timer_gold - START_DELAY_GOLD) / GROUP_DISPLAY_DELAY_GOLD + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_GOLD, numRings_gold) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active) {
            if (goldRings[i].update_counter >= goldRings[i].update_delay) {
                goldRings[i].position.z -= 1.0f;

                if (goldRings[i].position.z < MIN_Z) {
                    goldRings[i].position.z = MAX_Z;
                    goldRings[i].update_counter = 0;
                }

                float scale_factor = (MAX_Z - goldRings[i].position.z) / (MAX_Z - MIN_Z) * 4.0f;
                goldRings[i].scale = { scale_factor, scale_factor };

                if (scale_factor >= 1.0f) {
                    goldRings[i].is_active = false;
                }
            }
            else {
                goldRings[i].update_counter += 3.0f;
            }
        }
    }
}

void update_red_rings() {
    int active_groups = (ring_start_timer_red - START_DELAY_RED) / GROUP_DISPLAY_DELAY_RED + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RED, numRings_red) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (redRings[i].is_active) {
            if (redRings[i].update_counter >= redRings[i].update_delay) {
                redRings[i].position.z -= 1.0f;

                if (redRings[i].position.z < MIN_Z) {
                    redRings[i].position.z = MAX_Z;
                    redRings[i].update_counter = 0;
                }

                float scale_factor = (MAX_Z - redRings[i].position.z) / (MAX_Z - MIN_Z) * 4.0f;
                redRings[i].scale = { scale_factor, scale_factor };

                if (scale_factor >= 1.0f) {
                    redRings[i].is_active = false;
                }
            }
            else {
                redRings[i].update_counter += 2.0f;
            }
        }
    }
}

void update_rainbow_rings() {
    int active_groups = (ring_start_timer_rainbow - START_DELAY_RAINBOW) / GROUP_DISPLAY_DELAY_RAINBOW + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RAINBOW, numRings_rainbow) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (rainbowRings[i].is_active) {
            if (rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
                rainbowRings[i].position.z -= 1.0f;

                if (rainbowRings[i].position.z < MIN_Z) {
                    rainbowRings[i].position.z = MAX_Z;
                    rainbowRings[i].update_counter = 0;
                }

                float scale_factor = (MAX_Z - rainbowRings[i].position.z) / (MAX_Z - MIN_Z) * 4.0f;
                rainbowRings[i].scale = { scale_factor, scale_factor };

                if (scale_factor >= 1.0f) {
                    rainbowRings[i].is_active = false;
                }
            }
            else {
                rainbowRings[i].update_counter++;
            }
        }
    }
}

void ring_update() {
    // タイマーの増加
    ring_start_timer_gold++;
    ring_start_timer_red++;
    ring_start_timer_rainbow++;

    // 開始遅延が終了したら、色ごとにリングを更新
    if (ring_start_timer_gold >= START_DELAY_GOLD) {
        update_gold_rings();
    }

    if (ring_start_timer_red >= START_DELAY_RED) {
        update_red_rings();
    }

    if (ring_start_timer_rainbow >= START_DELAY_RAINBOW) {
        update_rainbow_rings();
    }
}


        


// ゴールドリングの描画処理
void render_gold_rings() {
    std::sort(goldRings, goldRings + numRings_gold, compareRingsByZ);

    int active_groups = (ring_start_timer_gold - START_DELAY_GOLD) / GROUP_DISPLAY_DELAY_GOLD + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_GOLD, numRings_gold) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active && goldRings[i].update_counter >= goldRings[i].update_delay) {
            sprite_render(sprRing_gold, goldRings[i].position.x, goldRings[i].position.y,
                goldRings[i].scale.x, goldRings[i].scale.y,
                goldRings[i].texPos.x, goldRings[i].texPos.y,
                goldRings[i].texSize.x, goldRings[i].texSize.y,
                goldRings[i].pivot.x, goldRings[i].pivot.y);
        }
    }
}

// レッドリングの描画処理
void render_red_rings() {
    std::sort(redRings, redRings + numRings_red, compareRingsByZ);

    int active_groups = (ring_start_timer_red - START_DELAY_RED) / GROUP_DISPLAY_DELAY_RED + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RED, numRings_red) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (redRings[i].is_active && redRings[i].update_counter >= redRings[i].update_delay) {
            sprite_render(sprRing_red, redRings[i].position.x, redRings[i].position.y,
                redRings[i].scale.x, redRings[i].scale.y,
                redRings[i].texPos.x, redRings[i].texPos.y,
                redRings[i].texSize.x, redRings[i].texSize.y,
                redRings[i].pivot.x, redRings[i].pivot.y);
        }
    }
}

// 虹リングの描画処理
void render_rainbow_rings() {
    std::sort(rainbowRings, rainbowRings + numRings_rainbow, compareRingsByZ);

    int active_groups = (ring_start_timer_rainbow - START_DELAY_RAINBOW) / GROUP_DISPLAY_DELAY_RAINBOW + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RAINBOW, numRings_rainbow) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (rainbowRings[i].is_active && rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
            sprite_render(sprRing_rainbow, rainbowRings[i].position.x, rainbowRings[i].position.y,
                rainbowRings[i].scale.x, rainbowRings[i].scale.y,
                rainbowRings[i].texPos.x, rainbowRings[i].texPos.y,
                rainbowRings[i].texSize.x, rainbowRings[i].texSize.y,
                rainbowRings[i].pivot.x, rainbowRings[i].pivot.y);
        }
    }
}

// 全リングの描画処理
void ring_render() {
    // ゴールドリングの描画タイミングをチェック
    if (ring_start_timer_gold >= START_DELAY_GOLD) {
        render_gold_rings();
    }

    // レッドリングの描画タイミングをチェック
    if (ring_start_timer_red >= START_DELAY_RED) {
        render_red_rings();
    }

    // レインボーリングの描画タイミングをチェック
    if (ring_start_timer_rainbow >= START_DELAY_RAINBOW) {
        render_rainbow_rings();
    }
}





// Z軸での比較関数
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z > b.position.z;  // Z位置が大きいほど手前に描画される
}
