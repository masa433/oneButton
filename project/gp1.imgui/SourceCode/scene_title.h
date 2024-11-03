#include "../GameLib/vector.h"

#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#define TITLE_TEX_W   170
#define TITLE_TEX_H   50
#define TITLE_PIVOT_X (TITLE_TEX_W/2)
#define TITLE_PIVOT_Y (TITLE_TEX_H/2)

class StartButton {
public:

	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;
};


void title_button_act();
bool click();


void title_init();
void title_deinit();
void title_update();
void title_render();



#endif//SCENE_TITLE_H