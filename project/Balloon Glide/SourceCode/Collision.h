#pragma once


#include "player.h"
#include "ring.h"
#include "bird.h"

bool hitCheckRingCircle(VECTOR3 pos1, float r1, VECTOR3 pos2, float r2);
bool hitCheckBirdCircle(VECTOR2 pos1, float r1, VECTOR2 pos2, float r2);
bool hitCheckRing(PLAYER* obj1, RING* obj2);
bool hitCheckBird(PLAYER* obj1, BIRD* obj2);
void judge();
