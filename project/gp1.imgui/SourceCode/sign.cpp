#include "sign.h"
#include "bird.h"
#include "audio.h"
#include <cstdlib>
#include <ctime>
#include "common.h"

Sprite* sprSign;
SIGN sign;

int flash_count;     // 点滅回数を管理
float flash_timer ; // 点滅タイマー
bool flash_visible ; // 点滅しているか
bool bird_ready ;  // 鳥を飛ばす準備ができたか
float bird_spawn_timer; // 鳥のスポーンタイマー
bool sign_on_right ; // 標識の位置（true: 右, false: 左）
bool sign_visible ; // 標識が見えるか見えないか
float sign_spawn_timer ; // 標識表示までの遅延タイマー
float sign_spawn_interval ; // 標識の初期出現間隔
float min_spawn_interval ; // 最小出現間隔(最終的な間隔)
float interval_decrement ; // 出現間隔の減少量
float sign_display_timer ; // 標識表示後のタイマー


void sign_init() {
    srand((unsigned)time(NULL));
    sign.sign_state = 0;

    flash_count = 0;     
     flash_timer = 0.0f; 
    flash_visible = true;
    bird_ready = false;  
     bird_spawn_timer = 0.0f; 
    sign_on_right = true;
    sign_visible = true; 
     sign_spawn_timer = 0.0f; 
     sign_spawn_interval = 15.0f; 
     min_spawn_interval = 1.0f; 
     interval_decrement = 0.1f; 
     sign_display_timer = 0.0f; 


}

void sign_update() {
    // 出現間隔を徐々に短縮（最小間隔まで）
    if (sign_spawn_interval > min_spawn_interval) {
        sign_spawn_interval -= interval_decrement * (1.0f / 60.0f);
        if (sign_spawn_interval < min_spawn_interval) {
            sign_spawn_interval = min_spawn_interval;
        }
    }

    switch (sign.sign_state) {
    case 0:
        // 初期設定
        sprSign = sprite_load(L"./Data/Images/鳥(標識).png");
        sign.sign_state = 1;
        break;

    case 1:
        // 遅延タイマー処理
        sign_spawn_timer -= 1.0f / 60.0f; // タイマーを1フレーム分減算

        if (sign_spawn_timer <= 0.0f) {
            // 標識を表示
            sign.angle = ToRadian(0);
            sign_on_right = rand() % 2; // 0または1をランダムで決定

            sign.position = {
                sign_on_right ? SCREEN_W - SIGN_TEX_W * 0.3f : SIGN_TEX_W * 0.3f,
                SCREEN_H / 2.0f
            };

            sign.scale = { 0.5f, 0.5f };
            sign.texPos = { 0, 0 };
            sign.texSize = { SIGN_TEX_W, SIGN_TEX_H };
            sign.pivot = { SIGN_PIVOT_X, SIGN_PIVOT_Y };
            sign.color = { 1.0f, 1.0f, 1.0f, 1.0f };

            sign_spawn_timer = sign_spawn_interval; // 次の標識表示までの間隔を設定

            sign_visible = true; // 標識を可視化
            sign.sign_state = 2; // 点滅状態へ移行
        }
        break;

    case 2:
        // 点滅処理
        flash_timer += 1.0f / 60.0f;
        if (flash_timer >= 0.3f) {
            flash_visible = !flash_visible;
            flash_timer = 0.0f;
            if (!flash_visible) {
                music::play(BGM_BIRD, false); 
                flash_count++;
               
            }
        }

        if (flash_count >= 3) {
            flash_count = 0;
            flash_timer = 0.0f;
            sign_visible = true; // 1秒間標識を表示
            sign_display_timer = 1.0f; // 1秒の表示タイマー開始
            sign.sign_state = 4; // 1秒間表示後に非表示にする状態へ
        }
        break;

    case 4:
        // 標識を1秒間表示後非表示にする
        sign_display_timer -= 1.0f / 30.0f;

        if (sign_display_timer <= 0.0f) {
            sign_visible = false; // 標識を非表示に
            bird_ready = true; // 鳥の準備
            sign.sign_state = 5; // 鳥を飛ばす状態へ移行
        }
        break;

    case 5:
        // 鳥を飛ばす準備
        if (bird_ready) {
            bird_spawn_timer++;

            if (bird_spawn_timer >= 1.0f) {
                // 鳥を生成
                spawn_birds(sign_on_right, 1 + rand() % 4); // 鳥を1～4羽ランダムで生成
                bird_spawn_timer = 0.0f;
                bird_ready = false;

                // 次の標識の表示タイミングを現在の出現間隔で設定
                sign_spawn_timer = sign_spawn_interval;
                sign.sign_state = 1; // 再び標識の準備状態へ
            }
        }
        break;
    }
}

void sign_render() {
    // 標識が非表示の場合、描画しない
    if (!sign_visible || (sign.sign_state == 2 && !flash_visible)) {
        return;
    }

    sprite_render(
        sprSign,
        sign.position.x, sign.position.y,
        sign.scale.x, sign.scale.y,
        sign.texPos.x, sign.texPos.y,
        sign.texSize.x, sign.texSize.y,
        sign.pivot.x, sign.pivot.y,
        sign.angle,
        sign.color.x, sign.color.y, sign.color.z, sign.color.w
    );
}


void sign_deinit() {
    safe_delete(sprSign);
}
