#pragma once
#ifndef BIRD_H
#define BIRD_H















#define BIRD_MAX   (500)
#define ENEMY_SPEED_X_MAX  (20.0f)   






//void game_over();
void bird_moveX();
void bird_init();
void bird_deinit();
void bird_update();
void bird_render();

#endif