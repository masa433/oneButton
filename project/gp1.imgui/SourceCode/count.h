#ifndef COUNT_H
#define COUNT_H

#include "../GameLib/vector.h"

#define  COUNT_TEX_W        200
#define  COUNT_TEX_H        280
#define  COUNT_PIVOT_X      (COUNT_TEX_W/2)
#define  COUNT_PIVOT_Y      (COUNT_TEX_H/2)

class COUNT 
{
public:
    VECTOR2         position;        
    VECTOR2         scale;      
    VECTOR2         texPos;     
    VECTOR2         texSize;    
    VECTOR2         pivot;      
    VECTOR4         color;      
    int             act;        
    int             anime;      
    int             animeTimer; 
    int             timer;

};

enum COUNT_ACT 
{

};

void count_init();
void count_deinit();
void count_update();
void count_render();

#endif//COUNT_H
