//#include"bird.h"
//#include"system.h"
//#include "common.h"
//#include    <iostream>
//#include    <stdlib.h>
//#include    <time.h>
//
//BIRD   bird[BIRD_MAX];
//
//class BIRD_DATA {
//public:
//    Sprite* spr;
//    const wchar_t* filePath;
//    VECTOR2         texPos;
//    VECTOR2         texSize;
//    VECTOR2         pivot;
//    float           radius;
//}
//birdData[] = {
//
//{ NULL, L"./Data/Images/boss.png",     { 0, 0 }, { 80, 80 }, { 40, 40 }, 45},};
//
//
//class BIRD_SET {
//public:
//    int             birdType;
//    VECTOR2         pos;
//}
//birdSet[] = {
//    { 0, { 1000, 600 }},
//    { 0, { 950, 600 }},
//    { 0, { 1200, 600 }},
//    { 0, { 1200, 600 }},
//    { 0, { 950, 600 }},
//    { 0, { 700, 600 }},
//    { 0, { 1200, 600 }},
//    { 0, { 950, 600 }},
//    { 0, { 700, 600 }},
//    { 1, { 1000, 600}},
//    { 2, { 1200, 600}},
//    { -1, { -1,  -1 }},
//};
//
//Sprite* sprBird;
//
//
//
//void bird_init()
//{
//    bird[BIRD_MAX].bird_state = 0;
//  
//
//}
//
//void bird_deinit()
//{
//    int dataNum = ARRAYSIZE(birdData);
//    for (int i = 0; i < dataNum; ++i) {
//        safe_delete(birdData[i].spr);
//    }
//}
//
//void bird_update()
//{
//    switch (bird[BIRD_MAX].bird_state)
//    {
//    case 0:
//    {
//        int dataNum = sizeof(birdData) / sizeof(BIRD_DATA);
//
//
//        for (int i = 0; i < dataNum; ++i) {
//            birdData[i].spr = sprite_load(birdData[i].filePath);
//
//        }
//    }
//
//    ++bird[BIRD_MAX].bird_state;
//
//    case 1:
//
//        for (int i = 0; i < BIRD_MAX; ++i) {
//            bird[i] = {};
//            bird[i].moveAlg = -1;
//        }
//
//        for (int i = 0; birdSet[i].birdType >= 0; i++) {
//           BIRD* p = searchSet0(bird, BIRD_MAX, birdSet[i].birdType, birdSet[i].pos);
//            if (!p)break;
//        
//        }
//        ++bird[BIRD_MAX].bird_state;
//
//    case 2:
//     
//         /*   ++bird[BIRD_MAX].bird.timer;
//      */
//
//        break;
//
//    }
//}
//void bird_render() {
//    for (int i = 0; i < BIRD_MAX; ++i)
//    {
//        if (bird[i].moveAlg == -1)continue;
//        sprite_render(
//            bird[i].spr,
//            bird[i].position.x, bird[i].position.y,
//            bird[i].scale.x, bird[i].scale.y,
//            bird[i].texPos.x, bird[i].texPos.y,
//            bird[i].texSize.x, bird[i].texSize.y,
//            bird[i].pivot.x, bird[i].pivot.y,
//            ToRadian(0),
//            bird[i].color.x, bird[i].color.y,
//            bird[i].color.z, bird[i].color.w
//        );
//
//    }
//}
//
//
//
////void game_over() {
////	HP--;
////	if (HP <= 0) {
////		nextScene = SCENE_OVER;
////	}