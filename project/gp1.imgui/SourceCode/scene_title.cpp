#include"scene_title.h"
#include"common.h"
#include"system.h"
#include"button.h"

int title_state;
int title_timer;




using namespace input;

void title_init() 
{
	title_state = 0;
	title_button_init();
}

void title_deinit()
{
	title_button_deinit();
}

void title_update() 
{
	
	title_button_update();
		

		switch (title_state)
		{
		case 0:
			//////// ‰Šúİ’è ////////

			
			


			title_state++;
			/*fallthrough*/
		case 1:
			//////// ƒpƒ‰ƒ[ƒ^‚Ìİ’è ////////
			GameLib::setBlendMode(Blender::BS_ALPHA);
			
			
			


			title_state++;
			/*fallthrough*/
		case 2:
			//////// ’Êí ////////
		
			
			break;
		}

		title_timer++;
	
}

void title_render() 
{
	GameLib::clear(0.0, 0.0, 0.0);

	text_out(6, "title", 100, 100, 1, 1, 1.0f, 1.0f, 1.0f);

	title_button_render();
}

