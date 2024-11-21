#pragma once


class GAME 
{

public:
	int game_state ;
	int game_timer ;
	float FadeIn;
	bool isFadeIn;
	int wait_timer ;
	int stay_timer;

};

void game_init();
void game_update();
void game_render();
void game_deinit();

void game_fade_act();



