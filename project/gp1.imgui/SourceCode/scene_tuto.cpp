#include "common.h"
#include "scene_tuto.h"
#include"system.h"
using namespace std;

int tuto_state;
int tuto_timer;

Sprite* sprBack;

void tuto_init() 
{
    tuto_timer = 0;
    tuto_state = 0;
}
void tuto_update()
{
    switch (tuto_state)
    {
    case 0:
        sprBack = sprite_load(L"./Data/Images/back(��).png");
        tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        
        tuto_state++;
        /*fallthrough*/
    case 2:
        
        break;
    }

    tuto_timer++;
}
void tuto_render() 
{
    sprite_render(sprBack, 0,0, 0, 0, 2732, 2048, 2732 / 2, 2048 / 2,ToRadian(0),1,1,1,1);
    GameLib::clear(1.0, 0.0, 0.0);
    
}
void tuto_deinit() 
{
    safe_delete(sprBack);
}