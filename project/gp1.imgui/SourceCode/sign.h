#pragma once
#ifndef SING_H
#define SING_H
#include "../GameLib/sprite.h"


#define SIGN_TEX_W        (512.0f)   
#define SIGN_TEX_H        (512.0f)   
#define SIGN_PIVOT_X      (SIGN_TEX_W/2)  
#define SIGN_PIVOT_Y      (SIGN_TEX_H/2)  

class SIGN {
public:
	float angle;
	VECTOR2 speed;
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;

	int sign_state;
	float radius;
	VECTOR2 offset;

};


void sign_init();
void sign_update();
void sign_render();
void sign_deinit();
#endif