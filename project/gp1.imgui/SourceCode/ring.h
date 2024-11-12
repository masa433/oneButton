#ifndef RING_H
#define RING_H



#include "../GameLib/vector.h"


#define RING_TEX_W        (350.0f)   // �v���C���[�̉摜1�̕�
#define RING_TEX_H        (350.0f)   // �v���C���[�̉摜1�̍���
#define RING_PIVOT_X      (RING_TEX_W/2)    // �v���C���[�̉������̒��S
#define RING_PIVOT_Y      (RING_TEX_H/2)   // �v���C���[�̏c�����̉��[



class RING {
public:
	float angle;

	VECTOR3 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;


	float update_counter;
	float update_delay;
	bool is_active;

};


void ring_init();
void ring_deinit();
void ring_update();
void ring_render();
bool compareRingsByZ(const RING& a, const RING& b);

void ring_act();

#endif//RING_H



