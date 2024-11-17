#include"sign.h"
#include"bird.h"
#include <cstdlib>
#include <ctime>
#include "common.h"

Sprite* sprSign;
SIGN sign;


int timr = rand() % 5 + 3;
int tim = 100;



void sign_init() {
	srand((unsigned)time(NULL));
	sign.sign_state = 0;
}
void sign_update(){ 


	switch (sign.sign_state)
	{
	case 0:
		//////// 初期設定 ////////
		sprSign = sprite_load(L"./Data/Images/鳥(標識).png");
		++sign.sign_state;
		/*fallthrough*/

	case 1:
		//////// パラメータの設定 ////////
		sign.angle = ToRadian(0);
		sign.position = { 0,SCREEN_H / 2.0f };
		sign.scale = { 0.4f, 0.4f };
		sign.texPos = { 0, 0 };
		sign.texSize = { SIGN_TEX_W, SIGN_TEX_H };
		sign.pivot = { SIGN_PIVOT_X, SIGN_PIVOT_Y };
		sign.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		++sign.sign_state;
		/*fallthrough*/

	case 2:
		for (timr; timr >= 0; timr--) {
			if (tim >= 0) {
				tim--;
			}
		}
		break;
	}
}
void sign_render(){
	sprite_render(
	sprSign,
		sign.position.x, sign.position.y,
		sign.scale.x, sign.scale.y,
		sign.texPos.x, sign.texPos.y,
		sign.texSize.x, sign.texSize.y,
		sign.pivot.x, sign.pivot.y,
		sign.angle,
		sign.color.x, sign.color.y, sign.color.z, sign.color.w
		);
}
void sign_deinit(){
	safe_delete(sprSign);
}