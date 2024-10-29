#include "../GameLib/vector.h"

#ifndef BUTTON_H
#define BUTTON_H

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

void title_button_init();
void title_button_deinit();
void title_button_update();
void title_button_render();
void title_button_act();
bool click();


#endif//BUTTON_H