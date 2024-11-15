#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "ring.h"
#include "bird.h"

bool hitCheckCircle(VECTOR3 pos1, float r1, VECTOR3 pos2, float r2);
bool hitCheck(PLAYER* obj1, RING* obj2);
bool hitCheck(PLAYER* obj1, BIRD* obj2);


#endif//COLLISION_H