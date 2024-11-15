#include "scene_title.h"
#include "common.h"
#include "system.h"






StartButton Start;
Sprite* sprStart;
Sprite* sprLogo;
Sprite* sprTitleBack;
Sprite* sprBalloon;

const int BALLOON_MAX = 15;
Balloon balloons[BALLOON_MAX];


using namespace input;

void title_init()
{
    Start.title_state = 0;
    Start.title_timer = 0;
    Start.fadeBlack = 0.0f;
    Start.isFadeOut = false; // フェードアウトしていない
    Start.isClicked = false; // クリックされていない
    Start.fadeTimer = 0.0f;
    Start.clickTimer = 0.0f;
    Start.clickCount = 0;

    for (int i = 0; i < BALLOON_MAX; i++) {
       
        
        balloons[i].active = false;
    }
    
}

void title_deinit()
{
    safe_delete(sprStart);
    safe_delete(sprLogo);
    safe_delete(sprTitleBack);
    safe_delete(sprBalloon);
    
}

void title_update()
{
    switch (Start.title_state)
    {
    case 0:
        sprStart = sprite_load(L"./Data/Images/startButton1.png");
        sprLogo = sprite_load(L"./Data/Images/title.png");
        sprTitleBack = sprite_load(L"./Data/Images/title_haikei.png");
        sprBalloon = sprite_load(L"./Data/Images/balloon.png"); 
        Start.title_move_timer = 0;
        Start.title_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };  // 中心位置
        Start.scale = { 1.0f, 1.0f };
        Start.texPos = { 0, 0 };
        Start.texSize = { TITLE_TEX_W, TITLE_TEX_H };
        Start.pivot = { TITLE_PIVOT_X, TITLE_PIVOT_Y };
        Start.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        Start.title_state++;
        /*fallthrough*/
    case 2:
        click_act();
        title_act();
        balloon_act();

        break;
    }

    Start.title_timer++;
}

void title_render() {
    GameLib::clear(0.0, 0.0, 0.0);

    // 背景を描画
    sprite_render(sprTitleBack, SCREEN_W * 0.5, SCREEN_H * 0.5, 0.7, 0.7, 0, 0, 2732, 2048, 2732 / 2, 2048 / 2);

    // 風船を描画
    for (int i = 0; i < BALLOON_MAX; i++) {
        if (balloons[i].active) {
            int spriteIndex = i % 6; // 6色をループで選択
            int texWidth = 140;      // 風船の幅を設定
            int texHeight = 330;     // 風船の高さを設定
            int texX = spriteIndex * texWidth;
            int texY = 0;

            // 風船のスプライトを描画
            sprite_render(sprBalloon, balloons[i].position.x, balloons[i].position.y, balloons[i].scale, balloons[i].scale,
                texX, texY, texWidth, texHeight, texWidth / 2, texHeight / 2);
        }
    }

    // タイトルロゴを描画
    sprite_render(sprLogo, SCREEN_W * 0.5, Start.titlePos.y, 0.7, 0.7, 0, 0, 2800, 1000, 2800 / 2, 1000 / 2, ToRadian(5));

    // スタートボタンを描画
    sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
        Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
        Start.pivot.x, Start.pivot.y);

    // デバッグ用情報を表示
    debug::setString("clickTimer%f", Start.clickTimer);
    debug::setString("fadeBlack%f", Start.fadeBlack);
    debug::setString("fadeTimer%f", Start.fadeTimer);
    debug::setString("clickConut%d", Start.clickCount);
    debug::setString("title_timer%d", Start.title_timer);
    debug::setString("title_state%d", Start.title_state);

    // フェードアウトを描画
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Start.fadeBlack);
}


//クリックしたときの処理
void click_act()
{
    if (Start.clickCount ==0 && !Start.isFadeOut && (TRG(0) & L_CLICK) && click())
        //クリックされていないかつフェードアウトしていないかつ左クリックを範囲内でしていれば
    {
        Start.isClicked = true;
        Start.clickCount = 1;  //クリックカウントを増やす
        Start.scale = { 1.0f, 1.0f };  // スケールを小さくする
    }

    if (Start.isClicked && !Start.isFadeOut)
    {
        Start.clickTimer += 0.2f;

        if (Start.clickTimer >= 2.0f)  // クリックタイマーが２秒以上になったら
        {
            // スケールを大きくする
            Start.scale = { 1.3f, 1.3f };
        }

        if (Start.clickTimer >= 10.0f)  // １０経ったらフェードアウト処理へ
        {
            Start.isFadeOut = true;
            Start.clickTimer = 0.0f;
        }
    }

    if (Start.isFadeOut)  // フェードアウト処理
    {
        Start.fadeBlack += 0.05f;
        if (Start.fadeBlack >= 1.0f) {
            Start.fadeBlack = 1.0f;
            Start.fadeTimer += 0.1f;
            if (Start.fadeTimer >= 10.0f) {
                tuto_start();  //ゲーム画面へ移行する関数
                Start.fadeTimer = 0.0f;
            }
        }
    }
    else if (Start.clickCount == 0&&!click())  // クリックされていないかつ範囲内でなければ
    {
        Start.scale = { 1.0f, 1.0f };//そのままのスケール
    }
    if (Start.clickCount == 0 && click()) //クリックされているかつ範囲内なら
    {
        Start.scale = { 1.3f, 1.3f };//スケールを大きくする
    }
}



bool click()//マウスカーソルと画像の当たり判定
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = Start.position.x + (Start.texSize.x * Start.scale.x) / 2;
    float button_left = Start.position.x - (Start.texSize.x * Start.scale.x) / 2;
    float button_top = Start.position.y - (Start.texSize.y * Start.scale.y) / 1.5;
    float button_bottom = Start.position.y + (Start.texSize.y * Start.scale.y) / 3.1;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void title_act() 
{
    
    Start.title_move_timer++;
    //titlePos.x = SCREEN_W / 2 + cos(title_angle) * 20;
    Start.titlePos.y = SCREEN_H * 0.3f + sin(Start.title_angle) * 20;
    Start.title_angle += ToRadian(1);


}

void balloon_act() 
{
    for (int i = 0; i < BALLOON_MAX; i++) {
        if (!balloons[i].active) {

            balloons[i].active = true;


            balloons[i].position.x = rand() % SCREEN_W;
            balloons[i].position.y = SCREEN_H + (100+rand() % 150);
            balloons[i].speed = 1.0f + (rand() % 3);
            balloons[i].scale = 0.5f + (rand() % 5) * 0.1f;
        }

        // yの値をスピードで減らしていく
        balloons[i].position.y -= balloons[i].speed;

        // 画面外にいたっらリセット
        if (balloons[i].position.y < -150) {
            balloons[i].active = false;
        }
    }
}
