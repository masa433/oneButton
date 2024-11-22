#pragma once



#include "../GameLib/vector.h"
#include "../GameLib/sprite.h"

#define RING_TEX_W        (350.0f)   // �v���C���[�̉摜1�̕�
#define RING_TEX_H        (350.0f)   // �v���C���[�̉摜1�̍���
#define RING_PIVOT_X      (RING_TEX_W/2)    // �v���C���[�̉������̒��S
#define RING_PIVOT_Y      (RING_TEX_H/2)   // �v���C���[�̏c�����̉��[

#define MAX_RINGS   (50000)

enum class RING_TYPE
{
	GOLD,
	RED,
	RAINBOW
};

class RING {
public:
	

	VECTOR3 position;
	VECTOR2 scale;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;
	float radius;
	RING_TYPE type;
	VECTOR2 texPos;

	float update_counter;
	float update_delay;
	bool is_active;
	
	VECTOR3 offset;
	float angle;
	
};




void ring_init();
void ring_deinit();
void ring_update();
void ring_render();
bool compareRingsByZ(const RING& a, const RING& b);









