#include "player.h"
#include "common.h"

using namespace input;

int player_state;

PLAYER player;
Sprite* sprPlayer;
POINT point; // マウスカーソルの位置を格納

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

    // マウスカーソルを非表示にする
    //ShowCursor(FALSE);
    

    // 画面中央の座標を計算
    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;

    // マウスカーソルを画面中央に移動
    SetCursorPos(centerX, centerY);

    // プレイヤーの位置を画面中央に設定
    player.position = { static_cast<float>(centerX),static_cast<float>(centerY) };


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
}

//--------------------------------------
//  プレイヤーの行動処理
//--------------------------------------
void player_act()
{
    // マウスカーソルの現在位置を取得
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // 補間率 (0.0 - 1.0の範囲で速度を調整、0.1でゆっくり、1.0で速い)
    float interpolationSpeed = 0.05f;

    // プレイヤーの位置をマウスカーソルの位置に向かってゆっくり移動
    player.position.x = lerp(player.position.x, static_cast<float>(point.x), interpolationSpeed);
    player.position.y = lerp(player.position.y, static_cast<float>(point.y), interpolationSpeed);
}
