
#ifndef PLAYER_H
#define PLAYER_H

#include "../GameLib/vector.h"


#define PLAYER_TEX_W        (169.0f)   // �v���C���[�̉摜1�̕�
#define PLAYER_TEX_H        (180.0f)   // �v���C���[�̉摜1�̍���
#define PLAYER_PIVOT_X      (PLAYER_TEX_W/2)    // �v���C���[�̉������̒��S
#define PLAYER_PIVOT_Y      (PLAYER_TEX_H/2)   // �v���C���[�̏c�����̉��[

#define PLAYER_SPEED        (5)

class PLAYER {
public:
	float angle;
	float speed;
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;



};


void player_init();
void player_deinit();
void player_update();
void player_render();

void player_act();

#endif//PlAYER_H