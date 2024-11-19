#pragma once
#ifndef BIRD_H
#define BIRD_H
#include "../GameLib/vector.h"
#include "../GameLib/sprite.h"

class BIRD {
public:
    VECTOR2 speed;
    VECTOR2 position;
    VECTOR2 scale;
    VECTOR2 texPos;
    VECTOR2 texSize;
    VECTOR2 pivot;
    VECTOR4 color;

    int bird_state;
    float radius;
    VECTOR2 offset;
    bool isColliding; // è’ìÀíÜÉtÉâÉO
};

#define BIRD_MAX           (30)
#define BIRD_SPEED_X_MAX   (30.0f)
#define BIRD_TEX_W         (512.0f)
#define BIRD_TEX_H         (512.0f)
#define NUM_LANES          (5)   // ÉåÅ[Éìêî

void bird_init();
void bird_deinit();
void bird_update();
void bird_render();
void spawn_birds(bool from_right, int count);
#endif