#include "ring.h"
#include "common.h"

using namespace input;

int ring_state;
RING ring;
Sprite* sprRing_gold;
//リングの初期設定
void ring_init() {
	ring_state = 0;
	
	ring.position = { 500,200 };
}
//リングの終了設定
void ring_deinit() {
	safe_delete(sprRing_gold);
}
//リングの更新処理
void ring_update() {
	
	switch (ring_state)
	{
	case 0:
		sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
		ring_state;
	case 1:
		ring.angle = ToRadian(0);
		ring.scale = { 0.5f, 0.5f };
		ring.texPos = { 0, 0 };
		ring.texSize = { RING_TEX_W, RING_TEX_H };
		ring.pivot = { RING_PIVOT_X, RING_PIVOT_Y };

		++ring_state;
	}
	
}
void ring_render() {
	sprite_render(
		sprRing_gold,
		ring.scale.x, ring.scale.y,
		ring.texPos.x, ring.texPos.y,
		ring.texSize.x, ring.texSize.y,
		ring.pivot.x, ring.pivot.y
		
	);
}
