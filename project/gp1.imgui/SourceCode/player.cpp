#include "player.h"
#include "bird.h"

#include "common.h"
#include "system.h"
#include "Collision.h"
#include"audio.h"
using namespace input;

extern BIRD bird[BIRD_MAX];
extern int score;
int boostCount ;          // ブースト回数をカウント
float boostMultiplier ; // ブースト時の速度倍率
int boostFlame;
PLAYER player;

Sprite* sprPlayer;
Sprite* sprFinish;
POINT point; // マウスカーソルの位置を格納

// 線形補間関数 (lerp)
float lerp(float start, float end, float t) {
    return start + t * (end - start);
}



void player_init()
{
    player.player_state = 0;

    // プレイヤーの初期設定
    player.hp = 5;
    player.fadeAlpha = 0.0f;
    player.speed = { 0.0f, 0.0f };
    player.position = { static_cast<float>(SCREEN_W / 2), static_cast<float>(SCREEN_H / 2) };
    player.flashing = false;
    player.flashCounter = 0;
    player.finish_timer = 0.0f;
    score = 0;
    boostCount = 5;
    boostMultiplier = 2.0f;
    boostFlame = 0;
    // 他の初期化処理
    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;
    SetCursorPos(centerX, centerY);
}


//--------------------------------------
//  プレイヤーの終了処理
//--------------------------------------
void player_deinit()
{
    safe_delete(sprPlayer);
    safe_delete(sprFinish);
    music::stop(BGM_WATER);
    music::stop(BGM_BALLOON);
    // マウスカーソルを再表示する
    ShowCursor(TRUE);
}

void player_update()
{
    switch (player.player_state) {
    case 0:
        //////// 初期設定 ////////
        sprPlayer = sprite_load(L"./Data/Images/player.png");
        sprFinish = sprite_load(L"./Data/Images/finish.png");
        ++player.player_state;
        /*fallthrough*/

    case 1:
        //////// パラメータの設定 ////////
        player.angle = ToRadian(0);
        player.scale = { 3.0f, 3.0f };
        player.texPos = { 0, 0 };
        player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
        player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
        player.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        player.radius = 120;
        player.offset = { 0, 0 };
        player.flashing = false;
        player.flashCounter = 0;
        player.hp = 5;
        ++player.player_state;
        /*fallthrough*/

    case 2:
        //////// 通常時 ////////
        if (player.flashing) {
            player.flashCounter++;
            if (player.flashCounter >= player.flashtimer) {
                player.flashing = false;
                player.flashCounter = 0;
            }
        }
        player_act();
        break;

    
    }
}

// プレイヤーの描画処理
void player_render()
{
    // プレイヤーがフラッシュ中でなければ描画
    if (!(player.flashing && (player.flashCounter / 6 % 2 == 0))) {
        sprite_render(
            sprPlayer,
            player.position.x, player.position.y,
            player.scale.x, player.scale.y,
            player.texPos.x, player.texPos.y,
            player.texSize.x, player.texSize.y,
            player.pivot.x, player.pivot.y,
            player.angle,
            player.color.x, player.color.y, player.color.z, player.color.w
        );
    }

    // プレイヤーがHP 0で画面外に出た場合のみfinish.pngを表示
    if (player.hp <= 0 && player.position.y > SCREEN_H) {
        sprite_render(sprFinish, SCREEN_W * 0.5, SCREEN_H * 0.5, 1.0f, 1.0f, 0, 0, 1020, 300, 1020 / 2, 300 / 2);
    }

    // フェードアウト描画
    if (player.fadeAlpha > 0.0f) {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, player.fadeAlpha);
    }

   /* primitive::circle(player.position.x + player.offset.x,
        player.position.y + player.offset.y,
        player.radius, 1, 1, ToRadian(0), 1, 0, 0, 0.2f);*/
}


// プレイヤーの行動処理
void player_act()
{
    // マウスカーソルの現在位置を取得
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // 補間率 (0.0 - 1.0の範囲で速度を調整、0.1でゆっくり、1.0で速い)
    float lerpSpeed = 0.03f;

    // プレイヤーの位置をマウスカーソルの位置に向かってゆっくり移動
    player.position.x = lerp(player.position.x, static_cast<float>(point.x), lerpSpeed);
    player.position.y = lerp(player.position.y, static_cast<float>(point.y), lerpSpeed);
    // 左クリックでブースト（最大5回）
    if ((TRG(0) & L_CLICK) && boostCount > 0) {
        boostFlame = 10; // ブーストを10フレーム持続
        boostCount--;
    }

    // ブースト中の処理
    if (boostFlame > 0) {
        player.position.x = lerp(player.position.x, static_cast<float>(point.x), lerpSpeed * boostMultiplier);
        player.position.y = lerp(player.position.y, static_cast<float>(point.y), lerpSpeed * boostMultiplier);
        boostFlame--; // フレームごとに減少
        
    }


    // プレイヤーのYの位置を更新
    player.position.y += player.speed.y;

    // 画面端にぶつかった場合の制限
    if (player.position.y < 0 + PLAYER_PIVOT_Y * 2) {
        player.position.y = 0 + PLAYER_PIVOT_Y * 2;
        player.speed.y = 0.0f;
    }
    if (player.hp>0 && player.position.y > SCREEN_H - PLAYER_PIVOT_Y * 2) {
        player.position.y = SCREEN_H - PLAYER_PIVOT_Y * 2;
        player.speed.y = 0.0f;
    }
    if (player.position.x > SCREEN_W - PLAYER_PIVOT_X * 2) {
        player.position.x = SCREEN_W - PLAYER_PIVOT_X * 2;
        player.speed.x = 0.0f;
    }
    if (player.position.x < 0 + PLAYER_PIVOT_X * 2) {
        player.position.x = 0 + PLAYER_PIVOT_X * 2;
        player.speed.x = 0.0f;
    }

    // 衝突処理
    for (int i = 0; i < BIRD_MAX; i++) {
        if (hitCheckBird(&player, &bird[i])) {
            if (!bird[i].isColliding) {
                music::play(BGM_BALLOON, false);
                bird[i].isColliding = true;

                // HPを減少
                player.hp--;
                if (player.hp > 0) {
                    player.flashing = true;
                    player.flashCounter = 0;
                    /*score -= 300;
                    if (score <= 0)score = 0;*/
                    
                }

                // アニメーションを横にずらす
                player.texPos.x += PLAYER_TEX_W;

                // アニメーションが最後のフレームを超えたら落下開始
                if (player.texPos.x >= PLAYER_TEX_W * 5) {
                    player.speed.y = 30.0f;
                }

                player.radius -= 10;
            }
        }
        else {
            bird[i].isColliding = false;
        }
    }

    // HPが0になった場合の処理
    if (player.hp <= 0) {
        player.hp = 0;
        if (player.position.y < SCREEN_H) {
            // プレイヤーが画面外に落ちるまで落下
            player.speed.y = 50.0f; // 落下速度
            player.speed.x = 0.0f;
            music::play(BGM_WATER, false);
        }
        else {
            // プレイヤーが画面外に落ちた後にfinishを表示
            
            sprite_render(sprFinish, SCREEN_W * 0.5, SCREEN_H * 0.5, 1.0f, 1.0f, 0, 0, 1020, 300, 1020 / 2, 300 / 2);

            // 2秒間表示後にフェードアウト開始
            
            player.finish_timer += 0.0166f;

            // 2秒後にフェードアウトを開始
            if (player.finish_timer >= 2.0f) {
                player.fadeAlpha += 0.02f; // 徐々にフェードアウト

                if (player.fadeAlpha >= 1.0f) {
                    // フェードアウト完了後に結果画面を表示
                    result_start();
                }
            }
        }
    }

}
