#include "common.h"
#include "scene_tuto.h"
#include "system.h"
#include "audio.h"
using namespace std;
using namespace input;

Sprite* sprBack;
Sprite* sprPlay;
Sprite* sprSetu[6]; // チュートリアルページの配列
Sprite* sprNextpage;
Sprite* sprBackpage;
Sprite* sprGamestart;
Sprite* sprSkip;


extern TUTO Tuto;
TutoButton tutoNextbutton;
TutoButton tutoBackbutton;
TutoButton gameButton;
TutoButton skipButton;

void tuto_init()
{
    Tuto.tuto_timer = 0;
    Tuto.tuto_state = 0;
    Tuto.scroll_x = 0.0f;
    Tuto.tuto_fadein = 1.0f;
    Tuto.tuto_fadeout = 0.0f;

    Tuto.isTuto_Fadein = false;
    Tuto.isTuto_Fadeout = false;
    Tuto.click_delay_timer = 0.0f;
    Tuto.tuto_fadeTimer = 0.0f;
    Tuto.currentPage = 0; // 現在のページ
    tutoNextbutton.clickTimer = 0.0f;
    tutoNextbutton.isClicked = false;

    tutoNextbutton.position = { SCREEN_W*0.95f,SCREEN_H *0.5f };
    tutoNextbutton.scale = { 1.0f, 1.0f };
    tutoNextbutton.texPos = { 0, 0 };
    tutoNextbutton.texSize = { BUTTON_TEX_W, BUTTON_TEX_H };
    tutoNextbutton.pivot = { BUTTON_PIVOT_X, BUTTON_PIVOT_Y };
    tutoNextbutton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    tutoBackbutton.position = { SCREEN_W * 0.05f,SCREEN_H * 0.5f };
    tutoBackbutton.scale = { 1.0f, 1.0f };
    tutoBackbutton.texPos = { 0, 0 };
    tutoBackbutton.texSize = { BUTTON_TEX_W, BUTTON_TEX_H };
    tutoBackbutton.pivot = { BUTTON_PIVOT_X, BUTTON_PIVOT_Y };
    tutoBackbutton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    gameButton.position = { SCREEN_W * 0.5f,SCREEN_H * 0.9f };
    gameButton.scale = { 1.0f, 1.0f };
    gameButton.texPos = { 0, 0 };
    gameButton.texSize = { GAME_BUTTON_TEX_W, GAME_BUTTON_TEX_H };
    gameButton.pivot = { GAME_BUTTON_PIVOT_X, GAME_BUTTON_PIVOT_Y };
    gameButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    skipButton.position = { SCREEN_W * 0.5f,SCREEN_H * 0.9f };
    skipButton.scale = { 1.0f, 1.0f };
    skipButton.texPos = { 0, 0 };
    skipButton.texSize = { GAME_BUTTON_TEX_W, GAME_BUTTON_TEX_H };
    skipButton.pivot = { GAME_BUTTON_PIVOT_X, GAME_BUTTON_PIVOT_Y };
    skipButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

}

void tuto_update()
{
    switch (Tuto.tuto_state)
    {
    case 0:
        
        sprBack = sprite_load(L"./Data/Images/back(仮).png");
        sprPlay = sprite_load(L"./Data/Images/ゲーム画像(仮).png");
        sprSetu[0] = sprite_load(L"./Data/Images/説明1.png");
        sprSetu[1] = sprite_load(L"./Data/Images/説明2.png");
        sprSetu[2] = sprite_load(L"./Data/Images/説明3.png");
        sprSetu[3] = sprite_load(L"./Data/Images/説明4.png");
        sprSetu[4] = sprite_load(L"./Data/Images/説明5.png");
        sprSetu[5] = sprite_load(L"./Data/Images/操作1.png");
        sprNextpage = sprite_load(L"./Data/images/nextpage.png");
        sprBackpage = sprite_load(L"./Data/Images/backpage.png");
        sprGamestart=sprite_load(L"./Data/Images/ゲームスタート.png");
        sprSkip = sprite_load(L"./Data/Images/スキップ.png");
        

        Tuto.tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        music::play(BGM_TUTO, true);
        Tuto.tuto_state++;
        /*fallthrough*/
    case 2:
        fadeIn_act();
        break;

    case 3:
        //左クリックでページ移動
        tuto_click_act();
        tuto_click_back_act();
        game_click_act();
        skip_click_act();
        
        break;

    case 4:
        fadeOut_act();
        
        break;
    }

    Tuto.scroll_x += 0.5f;
    if (Tuto.scroll_x >= 5464) {
        Tuto.scroll_x = 0.0f;
    }

    Tuto.tuto_timer++;
}

void tuto_render()
{
    GameLib::clear(1.0, 0.0, 0.0);

    // 背景スクロール
    sprite_render(sprBack, -Tuto.scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2);
    sprite_render(sprBack, -Tuto.scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2);

    // フェードイン・フェードアウトエフェクト
    float scale_up = 1.0f + (1.0f - Tuto.tuto_fadein);
    float scale_down = 1.0f - Tuto.tuto_fadeout * 0.5f;
    VECTOR2 scale = { 1.0f * scale_up * scale_down , 1.0f * scale_up * scale_down };

    // チュートリアルページ描画
    sprite_render(sprSetu[Tuto.currentPage], SCREEN_W * 0.5f, SCREEN_H * 0.48f, scale.x, scale.y, 0, 0, 800, 550, 800 / 2.0f, 550 / 2.0f);

    // 矢印ボタンの表示制御
    if (scale.x >= 2.0f && scale.y >= 2.0f)
    {
        if (Tuto.currentPage > 0) // 最初のページでない場合は「戻るボタン」を表示
        {
            sprite_render(sprBackpage, tutoBackbutton.position.x, tutoBackbutton.position.y,
                tutoBackbutton.scale.x, tutoBackbutton.scale.y,
                tutoBackbutton.texPos.x, tutoBackbutton.texPos.y,
                tutoBackbutton.texSize.x, tutoBackbutton.texSize.y,
                tutoBackbutton.pivot.x, tutoBackbutton.pivot.y);
        }

        if (Tuto.currentPage < 5) // 最後のページでない場合は「次へボタン」を表示
        {
            sprite_render(sprNextpage, tutoNextbutton.position.x, tutoNextbutton.position.y,
                tutoNextbutton.scale.x, tutoNextbutton.scale.y,
                tutoNextbutton.texPos.x, tutoNextbutton.texPos.y,
                tutoNextbutton.texSize.x, tutoNextbutton.texSize.y,
                tutoNextbutton.pivot.x, tutoNextbutton.pivot.y);

            sprite_render(sprSkip, skipButton.position.x, skipButton.position.y,
                skipButton.scale.x, skipButton.scale.y,
                skipButton.texPos.x, skipButton.texPos.y,
                skipButton.texSize.x, skipButton.texSize.y,
                skipButton.pivot.x, skipButton.pivot.y);
                


        }

        if (Tuto.currentPage == 5) 
        {
            sprite_render(sprGamestart, gameButton.position.x, gameButton.position.y,
                gameButton.scale.x, gameButton.scale.y,
                gameButton.texPos.x, gameButton.texPos.y,
                gameButton.texSize.x, gameButton.texSize.y,
                gameButton.pivot.x, gameButton.pivot.y);

            
          
        }
    }

    // フェードイン描画
    if (Tuto.tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadein);
    }

    // フェードアウト描画
    if (Tuto.tuto_fadeout > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadeout);
    }

   
}


void tuto_deinit()
{
    safe_delete(sprBack);
    for (int i = 0; i < 6; i++) safe_delete(sprSetu[i]);
    safe_delete(sprPlay);
    safe_delete(sprNextpage);
    safe_delete(sprBackpage);
    safe_delete(sprGamestart);
    music::stop(BGM_TUTO);
    music::stop(BGM_BUTTON);
    safe_delete(sprSkip);
   
}

// フェードイン
void fadeIn_act()
{
    if (!Tuto.isTuto_Fadein)
    {
       
        Tuto.isTuto_Fadein = true;
    }

    if (Tuto.isTuto_Fadein)
    {
        Tuto.tuto_fadein -= 0.03f;
        if (Tuto.tuto_fadein <= 0.0f)
        {
            
            Tuto.tuto_fadein = 0.0f;
            Tuto.isTuto_Fadein = false;
            Tuto.tuto_state++;
        }
    }
}

// フェードアウト
void fadeOut_act()
{
    if (Tuto.isTuto_Fadeout)
    {
        Tuto.click_delay_timer += 0.03f;
        if (Tuto.click_delay_timer >= 2.0f)
        {
            Tuto.tuto_fadeout += 0.05f;
            if (Tuto.tuto_fadeout >= 1.0f)
            {
                Tuto.tuto_fadeout = 1.0f;
                Tuto.tuto_fadeTimer += 0.1f;
                if (Tuto.tuto_fadeTimer >= 7.0f)
                {
                    
                    game_start();
                    Tuto.tuto_fadeTimer = 0.0f;
                    Tuto.tuto_state = 0; 
                }
            }
        }
    }
}

void tuto_click_act()
{

    // 5枚目のページなら「戻る」ボタンの処理をスキップ
    if (Tuto.currentPage == 5)
    {
        tutoNextbutton.scale = { 1.0f, 1.0f }; // スケールを通常サイズにリセット
        return;
    }
    // クリックされて範囲内にある場合
    if ((TRG(0) & L_CLICK) && tuto_click())
    {
        tutoNextbutton.isClicked = true;          // クリック状態をセット
        tutoNextbutton.clickTimer = 0.0f;         // タイマーをリセット
        tutoNextbutton.scale = { 1.0f, 1.0f };  // スケールを小さくする

        // ページを進める
        if (Tuto.currentPage < 5) {
            Tuto.currentPage++;
            music::play(BGM_BUTTON, false);
        }
        else if (Tuto.currentPage == 5) {  // 最後のページのとき
            Tuto.isTuto_Fadeout = true;
            Tuto.click_delay_timer = 0.0f;
            Tuto.tuto_state++;
        }
    }

    // クリック中またはクリック状態で範囲内にある場合
    if (tutoNextbutton.isClicked && tuto_click())
    {
        tutoNextbutton.clickTimer += 0.2f;  // タイマーを進める

        // クリックタイマーが2秒以上ならスケールを大きくする
        if (tutoNextbutton.clickTimer >= 2.0f)
        {
            tutoNextbutton.scale = { 1.3f, 1.3f };
        }
    }
    else if (tuto_click())
    {
        // 範囲内にあるがクリックされていない場合
        tutoNextbutton.scale = { 1.3f, 1.3f }; // スケールを大きくする
    }
    else
    {
        // 範囲外にある場合
        tutoNextbutton.scale = { 1.0f, 1.0f }; // 通常のスケールに戻す
    }

    // タイマーが10.0以上になったらクリック状態をリセット
    if (tutoNextbutton.clickTimer >= 5.0f)
    {
        tutoNextbutton.isClicked = false;
        tutoNextbutton.clickTimer = 0.0f;
    }

}



bool tuto_click() 
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = tutoNextbutton.position.x + (tutoNextbutton.texSize.x * tutoNextbutton.scale.x)/2 ;
    float button_left = tutoNextbutton.position.x - (tutoNextbutton.texSize.x * tutoNextbutton.scale.x)/2 ;
    float button_top = tutoNextbutton.position.y - (tutoNextbutton.texSize.y * tutoNextbutton.scale.y) /2;
    float button_bottom = tutoNextbutton.position.y + (tutoNextbutton.texSize.y * tutoNextbutton.scale.y)/2 ;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void tuto_click_back_act()
{
    // 1枚目のページなら「戻る」ボタンの処理をスキップ
    if (Tuto.currentPage == 0)
    {
        tutoBackbutton.scale = { 1.0f, 1.0f }; // スケールを通常サイズにリセット
        return;
    }

    // クリックされて範囲内にある場合
    if ((TRG(0) & L_CLICK) && tuto_click_back())
    {
        tutoBackbutton.isClicked = true;          // クリック状態をセット
        tutoBackbutton.clickTimer = 0.0f;         // タイマーをリセット
        tutoBackbutton.scale = { 1.0f, 1.0f };    // スケールを小さくする

        // ページを戻す
        if (Tuto.currentPage > 0) {
            Tuto.currentPage--;
            music::play(BGM_BUTTON, false);
        }
    }

    // クリック中またはクリック状態で範囲内にある場合
    if (tutoBackbutton.isClicked && tuto_click_back())
    {
        tutoBackbutton.clickTimer += 0.2f;  // タイマーを進める

        // クリックタイマーが2秒以上ならスケールを大きくする
        if (tutoBackbutton.clickTimer >= 2.0f)
        {
            tutoBackbutton.scale = { 1.3f, 1.3f };
        }
    }
    else if (tuto_click_back())
    {
        // 範囲内にあるがクリックされていない場合
        tutoBackbutton.scale = { 1.3f, 1.3f }; // スケールを大きくする
    }
    else
    {
        // 範囲外にある場合
        tutoBackbutton.scale = { 1.0f, 1.0f }; // 通常のスケールに戻す
    }

    // タイマーが5.0以上になったらクリック状態をリセット
    if (tutoBackbutton.clickTimer >= 5.0f)
    {
        tutoBackbutton.isClicked = false;
        tutoBackbutton.clickTimer = 0.0f;
    }
}


bool tuto_click_back()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = tutoBackbutton.position.x + (tutoBackbutton.texSize.x * tutoBackbutton.scale.x)/2;
    float button_left = tutoBackbutton.position.x - (tutoBackbutton.texSize.x * tutoBackbutton.scale.x)/2;
    float button_top = tutoBackbutton.position.y - (tutoBackbutton.texSize.y * tutoBackbutton.scale.y)/2;
    float button_bottom = tutoBackbutton.position.y + (tutoBackbutton.texSize.y * tutoBackbutton.scale.y)/2;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void game_click_act()
{
    if (Tuto.currentPage != 5)
    {
       
        gameButton.scale = { 1.0f, 1.0f }; // スケールを通常サイズにリセット
        gameButton.isClicked = false;     // クリック状態をリセット
        gameButton.clickTimer = 0.0f;     // タイマーをリセット
        gameButton.clickCount = 0;        // クリックカウントをリセット
        return;
    }

    if (gameButton.clickCount == 0 && (TRG(0) & L_CLICK) && game_click())
    {
        // クリックされていない && 左クリックが範囲内で発生した場合
        gameButton.isClicked = true;
        gameButton.clickCount = 1;        // クリックカウントを増やす
        gameButton.scale = { 1.0f, 1.0f }; // スケールを通常サイズに
        music::play(BGM_BUTTON, false);
    }

    if (gameButton.isClicked)
    {
        gameButton.clickTimer += 0.2f;
        if (gameButton.clickTimer >= 2.0f)  // クリックタイマーが2秒以上になったら
        {
            gameButton.scale = { 1.3f, 1.3f }; // スケールを大きくする
        }

        if (gameButton.clickTimer >= 5.0f)  // 5秒経過でフェードアウト処理へ
        {
            Tuto.isTuto_Fadeout = true;
            gameButton.clickTimer = 0.0f; // タイマーをリセット
            gameButton.isClicked = false; // クリック状態をリセット
            gameButton.clickCount = 0;    // クリックカウントをリセット
            Tuto.tuto_state++;            // チュートリアルの状態を進める
        }
    }
    else if (gameButton.clickCount == 0 && !game_click()) // クリックされていない && 範囲外の場合
    {
        gameButton.scale = { 1.0f, 1.0f }; // スケールを通常に戻す
    }

    if (gameButton.clickCount == 0 && game_click()) // クリックされていない && 範囲内の場合
    {
        gameButton.scale = { 1.3f, 1.3f }; // スケールを大きくする
    }
}

bool game_click()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = gameButton.position.x + (gameButton.texSize.x * gameButton.scale.x) / 2;
    float button_left = gameButton.position.x - (gameButton.texSize.x * gameButton.scale.x) / 2;
    float button_top = gameButton.position.y - (gameButton.texSize.y * gameButton.scale.y) / 1.5;
    float button_bottom = gameButton.position.y + (gameButton.texSize.y * gameButton.scale.y) / 3.1;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void skip_click_act() 
{
    if (Tuto.currentPage == 5)
    {
        skipButton.scale = { 1.0f, 1.0f }; // スケールを通常サイズにリセット
        return;
    }

    if (skipButton.clickCount == 0 && (TRG(0) & L_CLICK) && skip_click())
    {
        // クリックされていない && フェードアウトしていない && 左クリックが範囲内で発生した場合
        skipButton.isClicked = true;
        skipButton.clickCount = 1; // クリックカウントを増やす
        skipButton.scale = { 1.0f, 1.0f }; // スケールを小さくする
        music::play(BGM_BUTTON, false);
    }

    if (skipButton.isClicked)
    {
        skipButton.clickTimer += 0.2f;

        if (skipButton.clickTimer >= 2.0f)  // クリックタイマーが2秒以上になったら
        {
            // スケールを大きくする
            skipButton.scale = { 1.3f, 1.3f };
        }

        if (skipButton.clickTimer >= 5.0f)  // 5秒経過でフェードアウト処理へ
        {
            Tuto.isTuto_Fadeout = true;
            skipButton.clickTimer = 0.0f;
            skipButton.isClicked = false; // クリック状態をリセット
            skipButton.clickCount = 0;    // クリックカウントをリセット
            Tuto.tuto_state++;
        }
    }


    else if (skipButton.clickCount == 0 && !skip_click()) // クリックされていない && 範囲外の場合
    {
        skipButton.scale = { 1.0f, 1.0f }; // スケールを通常に戻す
    }

    if (skipButton.clickCount == 0 && skip_click()) // クリックされていない && 範囲内の場合
    {
        skipButton.scale = { 1.3f, 1.3f }; // スケールを大きくする
    }
}

bool skip_click()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = skipButton.position.x + (skipButton.texSize.x * skipButton.scale.x) / 2;
    float button_left = skipButton.position.x - (skipButton.texSize.x * skipButton.scale.x) / 2;
    float button_top = skipButton.position.y - (skipButton.texSize.y * skipButton.scale.y) / 1.5;
    float button_bottom = skipButton.position.y + (skipButton.texSize.y * skipButton.scale.y) / 3.1;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

