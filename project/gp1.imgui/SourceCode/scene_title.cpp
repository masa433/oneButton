#include"scene_title.h"
#include"common.h"
#include"system.h"
#include"button.h"

int title_state;
int title_timer;
float fadeBlack;
bool isFadeOut; // フェードアウトしているか
bool isClicked; // ボタンがクリックされたか
float fadeTimer;
float clickTimer;

StartButton Start;
Sprite* sprStart;

using namespace input;

void title_init() 
{
	title_state = 0;
	title_timer = 0;
	fadeBlack = 0.0f;
	isFadeOut = false; // フェードアウトしていない
	isClicked = false; // クリックされていない
	fadeTimer = 0.0f;
	clickTimer = 0.0f;
}

void title_deinit()
{
	safe_delete(sprStart);
	title_state = 0;
	title_timer = 0;
}

void title_update() 
{
	
	
		

		switch (title_state)
		{
		case 0:
			//////// 初期設定 ////////

			sprStart = sprite_load(L"./Data/Images/start(仮).png");
			
			title_state++;
			/*fallthrough*/
		case 1:
			//////// パラメータの設定 ////////
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
			//////// 通常時 ////////
            title_button_act();
			
			break;
		}

		title_timer++;
	
}

void title_render() 
{
	GameLib::clear(0.0, 0.0, 0.0);

	text_out(6, "title", 100, 100, 1, 1, 1.0f, 1.0f, 1.0f);

	sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
		Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
		Start.pivot.x, Start.pivot.y);
	debug::setString("clickTimer%f", clickTimer);
	debug::setString("faceBlack%f", fadeBlack);
	debug::setString("fadeTimer%f", fadeTimer);
	// 画面全体にフェードを適用
	primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, fadeBlack);

	
}

void title_button_act() // クリックしたときの動作
{
    if (!isFadeOut && (TRG(0) & L_CLICK) && click())
    {
        isClicked = true;
    }


    if (isClicked && !isFadeOut)
    {
        clickTimer += 0.1f;

        if (clickTimer >= 10.0f) // クリックされてから10.0f経過したらフェードアウト
        {
            isFadeOut = true;
            clickTimer = 0.0f; // タイマーリセット
        }
    }

    if (isFadeOut)
    {
        // フェードアウト
        fadeBlack += 0.05f;
        if (fadeBlack >= 1.0f) {
            fadeBlack = 1.0f;
            fadeTimer += 0.1f;

            if (fadeTimer >= 10.0f) {
                game_start(); // 完全にフェードアウトしたらシーン遷移
                fadeTimer = 0.0f;
            }
        }
    }
    else
    {
        // マウスがボタンに触れたらサイズを変更
        if (click())
        {
            Start.scale = { 1.3f, 1.3f };  // 触れた時のサイズ
        }
        else
        {
            Start.scale = { 1.0f, 1.0f };  // 通常サイズ
        }
    }
}

bool click() // マウスクリックの当たり判定
{
    // マウスカーソルの座標取得
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    // ボタンの四辺を取得 (サイズにスケールを考慮)
    float button_right = Start.position.x + (Start.texSize.x * Start.scale.x) / 2;
    float button_left = Start.position.x - (Start.texSize.x * Start.scale.x) / 2;
    float button_top = Start.position.y - (Start.texSize.y * Start.scale.y) / 2;
    float button_bottom = Start.position.y + (Start.texSize.y * Start.scale.y) / 2;

    // マウスの座標がボタンの範囲内かチェック
    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    // マウスがボタン範囲内ならクリックされたと判定
    return isWithinX && isWithinY;
}

