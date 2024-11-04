#include "player.h"
#include "common.h"

using namespace input;

int player_state;
PLAYER player;
Sprite* sprPlayer;
POINT point; // マウスカーソルの位置を格納


const float COUNTDOWN_DURATION = 3.0f; // 3 seconds for countdown

float countdown_timer = COUNTDOWN_DURATION; // Start at 3 seconds

// 線形補間関数 (lerp)
float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

//--------------------------------------
//  プレイヤーの初期設定
//--------------------------------------
void player_init()
{
    player_state = 0;
    player.velocityY = 0.0f; // Initialize vertical velocity to 0
    countdown_timer = COUNTDOWN_DURATION; // Reset countdown timer

    // マウスカーソルを非表示にする
    // ShowCursor(FALSE);

    // 画面中央の座標を計算
    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;

    // マウスカーソルを画面中央に移動
    SetCursorPos(centerX, centerY);

    // プレイヤーの位置を画面中央に設定
    player.position = { static_cast<float>(centerX), static_cast<float>(centerY) };
}

//--------------------------------------
//  プレイヤーの終了処理
//--------------------------------------
void player_deinit()
{
    safe_delete(sprPlayer);

    // マウスカーソルを再表示する
    ShowCursor(TRUE);
}

//--------------------------------------
//  プレイヤーの更新処理
//--------------------------------------
void player_update()
{
    

    switch (player_state)
    {
    case 0:
        //////// 初期設定 ////////
        sprPlayer = sprite_load(L"./Data/Images/player.png");
        ++player_state;
        /*fallthrough*/

    case 1:
        //////// パラメータの設定 ////////
        player.angle = ToRadian(0);
        player.scale = { 0.5f, 0.5f };
        player.texPos = { 0, 0 };
        player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
        player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
        player.color = { 1.0f, 1.0f, 1.0f, 1.0f };

        ++player_state;
        /*fallthrough*/

    case 2:
        // Update countdown timer
            if (countdown_timer > 0) {
                countdown_timer -= 60*3; // Decrease countdown
                if (countdown_timer <= 0) {
                    countdown_timer = 0; // Ensure it doesn't go below 0
                }
            }

    case 3:
        //////// 通常時 ////////
        player_act();
        break;
    }
}

//--------------------------------------
//  プレイヤーの描画処理
//--------------------------------------
void player_render()
{
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

    // Optional: Display countdown on screen
    if (countdown_timer > 0) {
        text_out(6,"Starting in: " + std::to_string(static_cast<int>(countdown_timer) + 1), 100, 50); // Position and countdown display
    }
}

//--------------------------------------
//  プレイヤーの行動処理
//--------------------------------------
void player_act()
{
    // Check if countdown is still active
    if (countdown_timer > 0) {
        return; // If countdown is active, don't execute movement
    }

    // マウスカーソルの現在位置を取得
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // 補間率 (0.0 - 1.0の範囲で速度を調整、0.1でゆっくり、1.0で速い)
    float interpolationSpeed = 0.05f;

    // プレイヤーの位置をマウスカーソルの位置に向かってゆっくり移動
    player.position.x = lerp(player.position.x, static_cast<float>(point.x), interpolationSpeed);

    // 常に重力をかける
    player.speed.y += GRAVITY;

    // 左クリックで浮き上がる
    if (TRG(0) & L_CLICK) {
        player.speed.y = -FLOAT_STRENGTH;
    }

    // 最大落下速度の制限
    if (player.speed.y > MAX_FALL_SPEED) {
        player.speed.y = MAX_FALL_SPEED;
    }

    // プレイヤーのYの位置を更新
    player.position.y += player.speed.y;

    // 地面に到達したときの処理（画面下端で止まる）
    if (player.position.y > SCREEN_H - PLAYER_TEX_H * player.scale.y) {
        player.position.y = SCREEN_H - PLAYER_TEX_H * player.scale.y;
        player.speed.y = 0.0f; // 止まる
    }
}
