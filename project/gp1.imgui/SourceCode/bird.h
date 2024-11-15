#pragma once
#ifndef BIRD_H
#define BIRD_H
#include "../GameLib/vector.h"
#include "../GameLib/sprite.h"

class  BIRD{
public:

	int timer;
	float angle;
	VECTOR2 speed;
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;
	GameLib::Sprite* spr;
	int bird_state;
	int moveAlg;



};

//float(* const ToRadian)(float) = DirectX::XMConvertToRadians;

#define BIRD_MAX   (20)
#define BIRD_SPEED_X_MAX  (20.0f)   
#define BIRD_TEX_W        (512.0f)   
#define BIRRD_TEX_H        (512.0f)   





//void game_over();
void bird_moveX();
void bird_init();
void bird_deinit();
void bird_update();
void bird_render();
void bird_delete();
void spr_load();
void spr_render();
void bird_move(BIRD* obj);
BIRD* searchSet0(BIRD arr[], int dataNum, int moveAlg, VECTOR2 pos);
#endif