#include "audio.h"
#include "../GameLib/game_lib.h"
#include "common.h"
void audio_init()
{
#if 1

#endif
    //�Q�[����
    music::load(0, L"./Data/Musics/�Q�[��.wav");

    //�^�C�g��
    music::load(1, L"./Data/Musics/�^�C�g��.wav");

    //�������
    music::load(2, L"./Data/Musics/�������.wav");

    //���U���g
    music::load(3, L"./Data/Musics/���U���g.wav");

    //�{�^��
    music::load(4, L"./Data/Musics/�{�^��.wav");
   
    //�����O
    music::load(5, L"./Data/Musics/�����O.wav");

    //���D
    music::load(6, L"./Data/Musics/���D.wav");

    //��
    music::load(7, L"./Data/Musics/��.wav");

    //��
    music::load(8, L"./Data/Musics/�����O��.wav");

    //�����N
    music::load(9, L"./Data/Musics/�����N.wav");

#if 21
#endif
}

void audio_deinit()
{
#if 2

#endif

    music::clear();
}