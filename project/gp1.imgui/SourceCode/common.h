#ifndef COMMON_H
#define COMMON_H

#include "../GameLib/game_lib.h"
#include "../GameLib/input_manager.h"
#include "../GameLib/obj2d_data.h"

using namespace GameLib;

//  �萔
extern const FLOAT PI;
extern const LONG SCREEN_W;
extern const LONG SCREEN_H;
extern const BOOL FULLSCREEN;
extern const LPCWSTR APPLICATION_NAME;

enum SCENE
{
	SCENE_NONE=-1,
	SCENE_TITLE=0,
	SCENE_GAME=1,
};

extern int curScene;
extern int nextScene;

//  �֐��|�C���^
extern float(* const ToRadian)(float);// �p�x�����W�A����
extern float(* const ToDegree)(float);// ���W�A�����p�x��

#endif // !COMMON_H
