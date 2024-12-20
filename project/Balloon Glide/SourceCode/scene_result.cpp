/*足りないもの
・リザルト画面に行く動作（これはplayerのほうに作る）
・背景（ゲーム画面を薄暗くして使うのもあり）
・scoreのプラスマイナス（ringかplayer?）
・ランクの評価画像（仮でEをおいてる）
(S:金　A:青　B:赤紫　C:緑　D:オレンジ　E:灰）
・BGM
・RESTARTボタン
*/

#include "scene_result.h"
#include "common.h"
#include "system.h"
#include "audio.h"

using namespace input;
using namespace std;

extern int result_state;
int result_timer;

// リングデータ構造
struct RingInfo {
    Sprite* sprite;     // リングのスプライト
    int count;          // リングのカウント
    float posX;         // 描画位置 X
    float posY;         // 描画位置 Y
};

RingInfo rings[] = {
    { nullptr, 0, 220, 280 },  // ゴールドリング
    { nullptr, 0, 220, 470 },  // レッドリング
    { nullptr, 0, 220, 650 }   // レインボーリング
};

extern int score;
extern int gold_ring_count;
extern int red_ring_count;
extern int rainbow_ring_count;

int current_display_step = 0; // 現在の表示段階
int display_timer = 0;        // 表示用タイマー
int click_count;
static float scale = 2.0f;  // 初期スケール
bool bgm[] = { false, false, false, false,false };

Sprite* sprRestart;
Sprite* sprResultback;
Sprite* ringGold;
Sprite* ringRed;
Sprite* ringRainbow;
Sprite* sprRank[6]; // ランクのスプライト
Sprite* sprScore;

Restart restart;

void result_init() {
    result_state = 0;
    result_timer = 0;
    restart.result_fadein = 1.0f;
    restart.isResult_Fadein = true;
    restart.isClicked = false;
    restart.clickCount = 0;
    restart.clickTimer = 0.0f;
    restart.result_fadeout = 0.0f;
    restart.result_fadeTimer = 0.0f;
    scale = 2.0f;
    current_display_step = 0;
    display_timer = 0;
    click_count = 0;
    for (int i = 0; i < 5; i++) 
    {
        bgm[i] = false;
    }
    
}


void result_deinit() {
    safe_delete(sprRestart);
    safe_delete(sprResultback);
    safe_delete(ringGold);
    safe_delete(ringRed);
    safe_delete(ringRainbow);
    for (int i = 0; i < 6; i++) {
        safe_delete(sprRank[i]);
    }
    music::stop(BGM_RESULT);
    music::stop(BGM_BUTTON);
    music::stop(BGM_RANK);
    music::stop(BGM_NUMDER);
    safe_delete(sprScore);

    result_state = 0;  
}

void result_update() {
    switch (result_state) {
    case 0:
        rings[0].sprite = sprite_load(L"./Data/Images/ring_gold.png");
        rings[1].sprite = sprite_load(L"./Data/Images/ring_red.png");
        rings[2].sprite = sprite_load(L"./Data/Images/ring_rainbow.png");

        rings[0].count = gold_ring_count;
        rings[1].count = red_ring_count;
        rings[2].count = rainbow_ring_count;

        sprRank[0] = sprite_load(L"./Data/Images/S.png");
        sprRank[1] = sprite_load(L"./Data/Images/A.png");
        sprRank[2] = sprite_load(L"./Data/Images/B.png");
        sprRank[3] = sprite_load(L"./Data/Images/C.png");
        sprRank[4] = sprite_load(L"./Data/Images/D.png");
        sprRank[5] = sprite_load(L"./Data/Images/E.png");

        sprRestart = sprite_load(L"./Data/Images/タイトルへ戻る.png");
        sprResultback = sprite_load(L"./Data/Images/result.png");
        sprScore = sprite_load(L"./data/Images/total_score.png");
        
        result_state++;
        break;

    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        music::play(BGM_RESULT, true);
        restart.position = { SCREEN_W * 0.5f, SCREEN_H * 0.93f };
        restart.scale = { 1.0f, 1.0f };
        restart.texPos = { 0, 0 };
        restart.texSize = { RESTART_TEX_W, RESTART_TEX_H };
        restart.pivot = { RESTART_PIVOT_X, RESTART_PIVOT_Y };
        restart.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        result_state++;
        break;

    case 2:
        if (restart.isResult_Fadein) {
            restart.result_fadein -= 0.03f;
            if (restart.result_fadein <= 0.0f) {
                restart.result_fadein = 0.0f;
                restart.isResult_Fadein = false;
            }
        }
        display_timer++;

        if (display_timer >= 60) {
            current_display_step++;
            display_timer = 0;
        }
        if (TRG(0) & L_CLICK&&click_count==0) {
            current_display_step = 7;
            click_count = 1;
        }

        if (current_display_step > 6) {
            result_click_act();
        }

        break;

    case 3:
        result_fadeOut_act();
        break;
    }

    result_timer++;
}

void result_render() {
    sprite_render(sprResultback, SCREEN_W * 0.5f, SCREEN_H * 0.5f, 1.0f, 1.0f, 0, 0, 1920, 1080, 1920 / 2.0f, 1080 / 2.0f, ToRadian(0), 1.0f, 1.0f, 1.0f);
    
    // リングを順次表示
    for (int i = 0; i < current_display_step && i < 3; ++i) {
        // 音楽を一度だけ再生
        if (!bgm[i]) {
            music::play(BGM_NUMDER, false);  // 各リングの音楽を再生
            bgm[i] = true;
        }

        // リングを描画
        sprite_render(rings[i].sprite, rings[i].posX, rings[i].posY, 0.3f, 0.3f, 0, 0, 350, 350, 350 / 2.0f, 350 / 2.0f);

        // スコア倍率を設定
        int score_level= 0;  // スコア倍率
        switch (i) {
        case 0: score_level = 100; break; // 1回目は×100
        case 1: score_level = 500; break; // 2回目は×500
        case 2: score_level = 1000; break; // 3回目は×1000
        default: score_level = 100; break; // デフォルトは×100
        }

        // スコアを描画
        text_out(6, "x " + to_string(rings[i].count) + " = " + to_string(rings[i].count * score_level),
            rings[i].posX + 100, rings[i].posY - 30,
            2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, TEXT_ALIGN::UPPER_LEFT);
    }


    // リングを表示した後にスコアを表示
    if (current_display_step > 3) {
        sprite_render(sprScore, 200, 850, 0.3f, 0.3f, 0, 0, 2550, 300, 0, 300 / 2.0f);
        text_out(6, to_string(score), 1100, 830, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, TEXT_ALIGN::UPPER_LEFT);
        
        if (!bgm[4]) {
            music::play(BGM_NUMDER, false);
            bgm[4] = true;
        }
            
        
        
    }

    // ランクの表示
    if (current_display_step > 4) {
        // スコアに応じてランクを決定
        Sprite* rank_sprite = sprRank[5];  // デフォルトはEランク
        if (score >= 45000) rank_sprite = sprRank[0];  // Sランク
        else if (score >= 30000) rank_sprite = sprRank[1];  // Aランク
        else if (score >= 20000) rank_sprite = sprRank[2];  // Bランク
        else if (score >= 10000) rank_sprite = sprRank[3];  // Cランク
        else if (score >= 5000) rank_sprite = sprRank[4];  // Dランク

        // ランクスプライトの描画（スケールを小さくしていく）
        scale -= 0.03f;  // 徐々に縮小
        if (scale < 0.5f)
        {
            scale = 0.5f;
            
        }
        if (!bgm[3]&&scale==0.5f) 
        {
            music::play(BGM_RANK, false);
            bgm[3] = true;
        }
           
        

        sprite_render(rank_sprite, SCREEN_W * 0.75f, SCREEN_H * 0.5f, scale, scale, 0, 0, 1366, 1024, 1366 / 2.0f, 1024 / 2.0f);
    }
 
    // リスタートボタンの表示
    if (current_display_step > 6) {
        sprite_render(sprRestart, restart.position.x, restart.position.y, restart.scale.x, restart.scale.y,
            restart.texPos.x, restart.texPos.y, restart.texSize.x, restart.texSize.y, restart.pivot.x, restart.pivot.y);
    }

    // フェードイン/フェードアウトの描画
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, restart.result_fadein);
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, restart.result_fadeout);
}



// フェードアウト
void result_fadeOut_act()
{
	if (restart.isResult_Fadeout)
	{
		restart.click_delay_timer += 0.03f;
		if (restart.click_delay_timer >= 2.0f)
		{
            // 音量を徐々に下げる処理
            float volume = 1.0f - restart.result_fadeout; // 音量をフェードアウトの進行度に合わせて調整
            if (volume < 0.0f) volume = 0.0f; // 最小値は0
            music::setVolume(BGM_RESULT, volume); // 音量を設定
			restart.result_fadeout += 0.03f;
			if (restart.result_fadeout >= 1.0f)
			{
				restart.result_fadeout = 1.0f;
				restart.result_fadeTimer += 0.1f;
				if (restart.result_fadeTimer >= 7.0f)
				{
					title_start();
					restart.result_fadeTimer = 0.0f;
					result_state = 0;
				}
			}
		}
	}
}

void result_click_act() 
{
	if (restart.clickCount == 0 && (TRG(0) & L_CLICK) && result_click())
	{
		// クリックされていない && フェードアウトしていない && 左クリックが範囲内で発生した場合
		restart.isClicked = true;
		restart.clickCount = 1; // クリックカウントを増やす
		restart.scale = { 1.0f, 1.0f }; // スケールを小さくする
		music::play(BGM_BUTTON, false);
		
	}

	if (restart.isClicked)
	{
		restart.clickTimer += 0.2f;

		if (restart.clickTimer >= 2.0f)  // クリックタイマーが2秒以上になったら
		{
			// スケールを大きくする
			restart.scale = { 1.3f, 1.3f };
		}

		if (restart.clickTimer >= 5.0f)  // 5秒経過でフェードアウト処理へ
		{
			restart.isResult_Fadeout = true;
			restart.clickTimer = 0.0f;
			result_state++;
		}
	}


	else if (restart.clickCount == 0 && !result_click()) // クリックされていない && 範囲外の場合
	{
		restart.scale = { 1.0f, 1.0f }; // スケールを通常に戻す
	}

	if (restart.clickCount == 0 && result_click()) // クリックされていない && 範囲内の場合
	{
		restart.scale = { 1.3f, 1.3f }; // スケールを大きくする
	}
}

bool result_click() 
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window::getHwnd(), &point);

	float button_right = restart.position.x + (restart.texSize.x * restart.scale.x) / 2;
	float button_left = restart.position.x - (restart.texSize.x * restart.scale.x) / 2;
	float button_top = restart.position.y - (restart.texSize.y * restart.scale.y) / 1.5;
	float button_bottom = restart.position.y + (restart.texSize.y * restart.scale.y) / 3.1;

	bool isWithinX = (point.x >= button_left && point.x <= button_right);
	bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

	return isWithinX && isWithinY;
}