//#pragma once
//#ifndef BIRD_H
//#define BIRD_H
//#include "../GameLib/vector.h"
//
//
//
//
//
//
//
//
//
//
//
//class  BIRD{
//public:
//	float angle;
//	VECTOR2 speed;
//	VECTOR2 position;
//	VECTOR2 scale;
//	VECTOR2 texPos;
//	VECTOR2 texSize;
//	VECTOR2 pivot;
//	VECTOR4 color;
//	GameLib::Sprite* spr;
//	int bird_state;
//	int moveAlg;
//
//
//
//};
//
//
//#define BIRD_MAX   (200)
//#define BIRD_SPEED_X_MAX  (20.0f)   
//#define BIRD_TEX_W        (169.0f)   // プレイヤーの画像1つの幅
//#define BIRRD_TEX_H        (180.0f)   // プレイヤーの画像1つの高さ
//
//
//
//
//
////void game_over();
//void bird_moveX();
//void bird_init();
//void bird_deinit();
//void bird_update();
//void bird_render();
//
//BIRD* searchSet0(BIRD arr[], int dataNum, int moveAlg, VECTOR2 pos)
//{
//
//
//	for (int i = 0; i < dataNum; i++) {
//		if (arr[i].moveAlg != -1) continue;
//
//		arr[i] = {};
//		arr[i].moveAlg = moveAlg;
//		arr[i].position = pos;
//		return &arr[i];
//	}
//	return NULL;
//
//#endif