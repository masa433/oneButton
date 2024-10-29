#include"scene_title.h"
#include"common.h"

int title_state;
int title_timer;

void title_init() 
{
	title_state = 0;
}

void title_deinit()
{

}

void title_update() 
{
	
		using namespace input;
		

		switch (title_state)
		{
		case 0:
			//////// 初期設定 ////////

			
			


			title_state++;
			/*fallthrough*/
		case 1:
			//////// パラメータの設定 ////////
			GameLib::setBlendMode(Blender::BS_ALPHA);
			
			title_state++;
			/*fallthrough*/
		case 2:
			//////// 通常時 ////////
			if (TRG(0) & L_CLICK) 
			{
				nextScene = SCENE_GAME;
			}
			
			break;
		}

		title_timer++;
	
}

void title_render() 
{
	GameLib::clear(0.0, 0.0, 0.0);

	text_out(6, "title", 100, 100, 1, 1, 1.0f, 1.0f, 1.0f);
}