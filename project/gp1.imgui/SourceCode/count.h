#pragma once

#include "../GameLib/vector.h"

#define  COUNT_TEX_W        200
#define  COUNT_TEX_H        280
#define  COUNT_PIVOT_X      (COUNT_TEX_W/2)
#define  COUNT_PIVOT_Y      (COUNT_TEX_H/2)

#define  START_TEX_W        1020
#define  START_TEX_H        300
#define  START_PIVOT_X      (START_TEX_W/2)
#define  START_PIVOT_Y      (START_TEX_H/2)

#define  CRING_TEX_W        410
#define  CRING_TEX_H        410
#define  CRING_PIVOT_X      (CRING_TEX_W/2)
#define  CRING_PIVOT_Y      (CRING_TEX_H/2)


class COUNT 
{
public:
    VECTOR2         position;        
    VECTOR2         scale;      
    VECTOR2         texPos;     
    VECTOR2         texSize;    
    VECTOR2         pivot;      
    VECTOR4         color;      
    

    int count_state;
    int count_timer;
    bool count_done = false;

};



void count_init();
void count_deinit();
void count_update();
void count_render();

