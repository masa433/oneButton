#include "scene_title.h"
#include "common.h"
#include "system.h"

int title_state;
int title_timer;
const int MAX_FLASH_COUNT = 2; // 最大の点滅回数
int currentFlashCount = 0; // 現在の点滅回数


StartButton Start;
Sprite* sprStart;

using namespace input;

void title_init()
{
    title_state = 0;
    title_timer = 0;
    Start.fadeBlack = 0.0f;
    Start.isFadeOut = false; // フェードアウトしていない
    Start.isClicked = false; // クリックされていない
    Start.fadeTimer = 0.0f;
    Start.clickTimer = 0.0f;
    Start.clickCount = 0;
    Start.flashing = false;
    Start.flashCounter = 0;
    Start.flashTimer = 10; // 点滅の速さ調整
    currentFlashCount = 0;
}

void title_deinit()
{
    safe_delete(sprStart);
}

void title_update()
{
    switch (title_state)
    {
    case 0:
        sprStart = sprite_load(L"./Data/Images/start(仮).png");
        title_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };  // 中心位置
        Start.scale = { 1.0f, 1.0f };
        Start.texPos = { 0, 0 };
        Start.texSize = { TITLE_TEX_W, TITLE_TEX_H };
        Start.pivot = { TITLE_PIVOT_X, TITLE_PIVOT_Y };
        Start.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        title_state++;
        /*fallthrough*/
    case 2:
        click_act();
        break;
    }

    title_timer++;
}

void title_render()
{
    GameLib::clear(0.0, 0.0, 0.0);
    text_out(6, "title", 100, 100, 1, 1, 1.0f, 1.0f, 1.0f);

    // 点滅の描画処理
    if (!Start.flashing || (Start.flashCounter / 4 % 2 == 0)) {
        sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
            Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
            Start.pivot.x, Start.pivot.y);
    }

    for (int i = 0; i < 10; i++) {
        debug::setString("");
    }
    debug::setString("clickTimer%f", Start.clickTimer);
    debug::setString("fadeBlack%f", Start.fadeBlack);
    debug::setString("fadeTimer%f", Start.fadeTimer);
    debug::setString("clickConut%d", Start.clickCount);
    debug::setString("title_timer%d", title_timer);
    debug::setString("title_state%d", title_state);
    debug::setString("currentFlashCount%d", currentFlashCount);

    // 画面全体にフェードを適用
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Start.fadeBlack);
}

void click_act()
{
    if (Start.clickCount < 1 && !Start.isFadeOut && (TRG(0) & L_CLICK) && click())
    {
        Start.isClicked = true;
        Start.scale = { 1.0f, 1.0f };//スケールを元に戻す
        Start.clickCount = 1;//クリック回数をカウント
        Start.flashing = true;//点滅処理
        Start.flashCounter = 0;
        currentFlashCount = 0;
    }

    if (Start.isClicked && !Start.isFadeOut)
    {
        Start.clickTimer += 0.1f;

        if (Start.clickTimer >= 10.0f)//フェードアウトするまでの処理
        {
            Start.isFadeOut = true;
            Start.clickTimer = 0.0f;
        }
    }

    if (Start.isFadeOut)//フェードアウトの処理
    {
        Start.fadeBlack += 0.05f;
        if (Start.fadeBlack >= 1.0f) {
            Start.fadeBlack = 1.0f;
            Start.fadeTimer += 0.1f;
            if (Start.fadeTimer >= 10.0f) {//完全にフェードアウトしたら
                game_start();//game_start関数を呼び出し
                Start.fadeTimer = 0.0f;
            }
        }
    }
    else
    {
        if (Start.clickCount == 1 || click())
        {
            Start.scale = { 1.3f, 1.3f };
        }
        else if (!click() && Start.clickCount == 0)
        {
            Start.scale = { 1.0f, 1.0f };
        }
    }

    if (Start.flashing)//点滅処理
    {
        Start.flashCounter++;
        if (Start.flashCounter >= Start.flashTimer)
        {
            Start.flashCounter = 0;
            currentFlashCount++;
            Start.flashing = (currentFlashCount < MAX_FLASH_COUNT);
        }
    }
}



bool click()//マウスカーソルと画像の当たり判定
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = Start.position.x + (Start.texSize.x * Start.scale.x) / 2;
    float button_left = Start.position.x - (Start.texSize.x * Start.scale.x) / 2;
    float button_top = Start.position.y - (Start.texSize.y * Start.scale.y) / 2;
    float button_bottom = Start.position.y + (Start.texSize.y * Start.scale.y) / 2;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}
