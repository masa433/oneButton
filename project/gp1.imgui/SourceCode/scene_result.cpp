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
//#include "player.cpp"


using namespace input;
using namespace std;

extern int result_state;
int result_timer;



extern int score;

//Sprite* sprScore;
Sprite* sprRestart;
Sprite* sprResultback;

Restart restart;

void result_init()
{
	result_state = 0;
	result_timer = 0;
	restart.result_fadein = 1.0f; // フェードインの初期値
	restart.isResult_Fadein = true; // フェードインを有効化
}

void result_deinit()
{
	
	safe_delete(sprRestart);
	safe_delete(sprResultback);
}

void result_update()
{
	switch (result_state)
	{
	case 0:
		//sprScore = sprite_load();
		sprRestart = sprite_load(L"./Data/Images/タイトルへ戻る.png");
		sprResultback = sprite_load(L"./Data/Images/title_haikei.png");
		result_state++;
	case 1:
		GameLib::setBlendMode(Blender::BS_ALPHA);
		restart.position = { SCREEN_W * 0.5f, SCREEN_H * 0.9f };  // 中心位置
		restart.scale = { 1.0f, 1.0f };
		restart.texPos = { 0, 0 };
		restart.texSize = { RESTART_TEX_W, RESTART_TEX_H };
		restart.pivot = { RESTART_PIVOT_X, RESTART_PIVOT_Y };
		restart.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		result_state++;
	case 2:
		if (restart.isResult_Fadein)
		{
			restart.result_fadein -= 0.03f; // フェードインの進行
			if (restart.result_fadein <= 0.0f)
			{
				restart.result_fadein = 0.0f; // フェードイン完了
				restart.isResult_Fadein = false;
			}
		}
		result_click_act();
		break;

	case 3:
		result_fadeOut_act();
		break;
	}
	result_timer++;
}
void result_render()
{
	sprite_render(sprResultback, SCREEN_W * 0.5, SCREEN_H * 0.5, 0.7, 0.7, 0, 0, 2732, 2048, 2732 / 2, 2048 / 2,ToRadian(0),0.5, 0.5, 0.5, 0.2);

	sprite_render(sprRestart, restart.position.x, restart.position.y, restart.scale.x, restart.scale.y,
		restart.texPos.x, restart.texPos.y, restart.texSize.x, restart.texSize.y,
		restart.pivot.x, restart.pivot.y);
	
	
	
	if(result_state==2)
	primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, restart.result_fadein);
	if(result_state==3)
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
			restart.result_fadeout += 0.05f;
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