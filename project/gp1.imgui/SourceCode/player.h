
#ifndef PLAYER_H
#define PLAYER_H

#include "../GameLib/vector.h"


#define PLAYER_TEX_W        (128.0f)   // �v���C���[�̉摜1�̕�
#define PLAYER_TEX_H        (128.0f)   // �v���C���[�̉摜1�̍���
#define PLAYER_PIVOT_X      (PLAYER_TEX_W/2)    // �v���C���[�̉������̒��S
#define PLAYER_PIVOT_Y      (PLAYER_TEX_H/2)   // �v���C���[�̏c�����̉��[



class PLAYER {
public:
	float angle;
	VECTOR2 speed;
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;
	
	int player_state;


};


void player_init();
void player_deinit();
void player_update();
void player_render();

void player_act();

#endif//PlAYER_H