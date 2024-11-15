#include"bird.h"
#include "common.h"
#include    <cstdlib>
#include    <ctime>
#include"sign.h"
using namespace GameLib;

BIRD   bird[BIRD_MAX];

class BIRD_DATA {
public:
    Sprite* spr;
    const wchar_t* filePath;
    VECTOR2         texPos;
    VECTOR2         texSize;
    VECTOR2         pivot;
    float           radius;
}
birdData[] = {

{ NULL, L"./Data/Images/êeíπ.png",     { 0, 0 }, { 512, 512 }, { 256, 256 }, 45},};


class BIRD_SET {
public:
    int             birdType;
    VECTOR2         pos;
}
birdSet[] = {
    { 0, { 1000, 600 }},
    { 0, { 950, 600 }},
    { 0, { 1200, 600 }},
    { 0, { 1200, 600 }},
    { 0, { 950, 600 }},
    { 0, { 700, 600 }},
    { 0, { 1200, 600 }},
    { 0, { 950, 600 }},
    { 0, { 700, 600 }},
    { 1, { 1000, 600}},
    { 2, { 1200, 600}},
    { -1, { -1,  -1 }},
};

Sprite* sprBird;



void bird_init()
{
    srand((unsigned)time(NULL));
    for(int i=0;i<BIRD_MAX;i++){
    bird[i].bird_state = 0;
    }
  
    int rndX = 0,rndY = 0;
    for (int i = 0; birdSet[i].birdType >= 0; i++) {
        rndX = rand() % 1921;
        birdSet[i].pos.x = rndX;
    }


    for (int i = 0; birdSet[i].birdType >= 0; i++) {
        rndY = rand() % 2;
        if (rndY == 1) {
            rndY = rndY * 1081;
        }
        birdSet[i].pos.y = rndY;
    }

}
template <typename T>
void bird_delete(T*& p){
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}
void bird_deinit()
{
    int dataNum = ARRAYSIZE(birdData);
    for (int i = 0; i < dataNum; ++i) {
        bird_delete(birdData[i].spr);
    }
}

void bird_update()
{
    for (int i = 0; i < BIRD_MAX; i++) {
        switch (bird[i].bird_state)
        {
        case 0:
        {
            int dataNum = sizeof(birdData) / sizeof(BIRD_DATA);


            for (int i = 0; i < dataNum; ++i) {
                birdData[i].spr = sprite_load(birdData[i].filePath);

            }
        }

        ++bird[i].bird_state;

        case 1:

            for (int i = 0; i < BIRD_MAX; ++i) {
                bird[i] = {};
                bird[i].moveAlg = -1;
            }

            for (int i = 0; birdSet[i].birdType >= 0; i++) {
                BIRD* p = searchSet0(bird, BIRD_MAX, birdSet[i].birdType, birdSet[i].pos);
                if (!p)break;

            }
            ++bird[i].bird_state;

        case 2:

            for (int i = 0; i < BIRD_MAX; i++) {
                if (bird[i].moveAlg == -1)continue;
                switch (bird[i].moveAlg)
                {
                    case 0:
                        bird_move(&bird[i]);
                    break;
                }
               ++bird[i].timer;
            }

         

            break;

        }
    }
}
void bird_render() {
    for (int i = 0; i < BIRD_MAX; ++i)
    {
        if (bird[i].moveAlg == -1)continue;
        sprite_render(
            bird[i].spr,
            bird[i].position.x, bird[i].position.y,
            bird[i].scale.x, bird[i].scale.y,
            bird[i].texPos.x, bird[i].texPos.y,
            bird[i].texSize.x, bird[i].texSize.y,
            bird[i].pivot.x, bird[i].pivot.y,
            ToRadian(0),
            bird[i].color.x, bird[i].color.y,
            bird[i].color.z, bird[i].color.w
        );

    }




}
void spr_load() {

}
void spr_render() {

}

void bird_move(BIRD* obj)
{
    switch (obj->bird_state)
    {
    case 0:

        obj->scale = { 0.5f,0.5f };
        obj->color = { 1,1,1,1 };
        obj->spr = birdData[0].spr;
        obj->texPos = birdData[0].texPos;
        obj->texSize = birdData[0].texSize;
        obj->pivot = birdData[0].pivot;


        ++obj->bird_state;
    case 1:

        break;
    }
}

BIRD* searchSet0(BIRD arr[], int dataNum, int moveAlg, VECTOR2 pos)
{


    for (int i = 0; i < dataNum; i++) {
        if (arr[i].moveAlg != -1) continue;

        arr[i] = {};
        arr[i].moveAlg = moveAlg;
        arr[i].position = pos;
        return &arr[i];
    }
    return NULL;
}