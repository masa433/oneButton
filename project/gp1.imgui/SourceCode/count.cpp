#include "count.h"
#include "common.h"

using namespace std;

int count_state;
int count_timer;
int fade_state;     // State for fade effect
int fade_timer;     // Timer for fade effect
Sprite* sprCount;
COUNT Count;
bool count_done = false;  // Flag to indicate when the countdown is finished

void count_init()
{
    count_state = 0;
    count_timer = 0;
    fade_state = 0;
    fade_timer = 0;
    sprCount = sprite_load(L"./Data/Images/countdown.png");  // Load the countdown sprite only once
}

void count_deinit()
{
    safe_delete(sprCount);
}

void count_update()
{
    switch (count_state)
    {
    case 0:
        //////// 初期設定 ////////
        Count = {};
        Count.position = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
        Count.scale = { 1.0f, 1.0f };
        Count.pivot = { COUNT_PIVOT_X, COUNT_PIVOT_Y };
        Count.color = { 1, 1, 1, 1 };

        // Initialize texture size to display one number frame
        Count.texSize = { COUNT_TEX_W, COUNT_TEX_H };

        count_state++;  // Move to the countdown animation state
        /*fallthrough*/

    case 1:
        if (fade_state == 0) {
            //////// カウントダウンのアニメーション ////////
            // Display "3" with seven frames
            if (count_timer < 10) {
                Count.texPos = { 0, 0 };  // Position for first "3" frame
            }
            else if (count_timer < 12) {
                Count.texPos = { COUNT_TEX_W, 0 };  // Position for second "3" frame
            }
            else if (count_timer < 14) {
                Count.texPos = { 2 * COUNT_TEX_W, 0 };  // Position for third "3" frame
            }
            else if (count_timer < 16) {
                Count.texPos = { 3 * COUNT_TEX_W, 0 };  // Position for fourth "3" frame
            }
            else if (count_timer < 18) {
                Count.texPos = { 4 * COUNT_TEX_W, 0 };  // Position for fifth "3" frame
            }
            else if (count_timer < 20) {
                Count.texPos = { 5 * COUNT_TEX_W, 0 };  // Position for sixth "3" frame
            }
            else if (count_timer < 22) {
                Count.texPos = { 6 * COUNT_TEX_W, 0 };  // Position for seventh "3" frame
                fade_state = 1;  // Trigger fade-out after showing last "3" frame
            }

            // Display "2" with seven frames
            else if (count_timer < 23) { // Fixed range for number "2"
                Count.texPos = { 0, COUNT_TEX_H };  // Position for first "2" frame
            }
            else if (count_timer <25) {
                Count.texPos = { COUNT_TEX_W, COUNT_TEX_H };  // Position for second "2" frame
            }
            else if (count_timer < 27) {
                Count.texPos = { 2 * COUNT_TEX_W, COUNT_TEX_H };  // Position for third "2" frame
            }
            else if (count_timer < 29) {
                Count.texPos = { 3 * COUNT_TEX_W, COUNT_TEX_H };  // Position for fourth "2" frame
            }
            else if (count_timer < 31) {
                Count.texPos = { 4 * COUNT_TEX_W, COUNT_TEX_H };  // Position for fifth "2" frame
            }
            else if (count_timer < 33) {
                Count.texPos = { 5 * COUNT_TEX_W, COUNT_TEX_H };  // Position for sixth "2" frame
            }
            else if (count_timer < 35) {
                Count.texPos = { 6 * COUNT_TEX_W, COUNT_TEX_H };  // Position for seventh "2" frame
                fade_state = 1;  // Trigger fade-out after showing last "2" frame
            }

            // Display "1" with seven frames
            else if (count_timer < 36) { // Fixed range for number "1"
                Count.texPos = { 0, 2 * COUNT_TEX_H };  // Position for first "1" frame
            }
            else if (count_timer < 38) {
                Count.texPos = { COUNT_TEX_W, 2 * COUNT_TEX_H };  // Position for second "1" frame
            }
            else if (count_timer < 40) {
                Count.texPos = { 2 * COUNT_TEX_W, 2 * COUNT_TEX_H };  // Position for third "1" frame
            }
            else if (count_timer < 42) {
                Count.texPos = { 3 * COUNT_TEX_W, 2 * COUNT_TEX_H };  // Position for fourth "1" frame
            }
            else if (count_timer < 44) {
                Count.texPos = { 4 * COUNT_TEX_W, 2 * COUNT_TEX_H };  // Position for fifth "1" frame
            }
            else if (count_timer < 46) {
                Count.texPos = { 5 * COUNT_TEX_W, 2 * COUNT_TEX_H };  // Position for sixth "1" frame
            }
            else if (count_timer < 48) {
                Count.texPos = { 6 * COUNT_TEX_W, 2 * COUNT_TEX_H };  // Position for seventh "1" frame
                fade_state = 1;  // Trigger fade-out after showing last "1" frame
            }

            count_timer++;
        }
        else if (fade_state == 1) {
            //////// フェードアウト ////////
            fade_timer++;
            Count.color.w = 1.0f - (float)fade_timer / 20.0f;  // Gradually decrease alpha

            if (fade_timer >= 40) {
                fade_state = 0;  // Transition to fade-in
                fade_timer = 0;
                count_timer++;  // Move to the next number's first frame
            }
        }
        break;

    case 2:
        // Final state after countdown completes
        // Implement any logic needed after countdown finishes
        break;
    }
}


void count_render()
{
    if (!count_done) {  // Only render if the countdown is not complete
        sprite_render(sprCount, Count.position.x, Count.position.y,
            Count.scale.x, Count.scale.y,
            Count.texPos.x, Count.texPos.y,
            Count.texSize.x, Count.texSize.y,
            Count.pivot.x, Count.pivot.y,
            ToRadian(0),
            Count.color.x, Count.color.y, Count.color.z, Count.color.w
        );
    }
    debug::setString("count_timer%d",count_timer);
    debug::setString("fade_timer%d", fade_timer);
    debug::setString("fade_state%d",fade_state);
}
