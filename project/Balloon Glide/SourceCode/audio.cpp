#include "audio.h"
#include "../GameLib/game_lib.h"
#include "common.h"
void audio_init()
{

    //�Q�[����
    music::load(BGM_GAME, L"./Data/Musics/�Q�[��.wav");

    //�^�C�g��
    music::load(BGM_TITLE, L"./Data/Musics/�^�C�g��.wav");

    //�������
    music::load(BGM_TUTO, L"./Data/Musics/�������.wav");

    //���U���g
    music::load(BGM_RESULT, L"./Data/Musics/���U���g.wav");

    //�{�^��
    music::load(BGM_BUTTON, L"./Data/Musics/�{�^��.wav");
   
    //�����O
    music::load(BGM_RING, L"./Data/Musics/�����O.wav");

    //���D
    music::load(BGM_BALLOON, L"./Data/Musics/���D.wav");

    //��
    music::load(BGM_WATER, L"./Data/Musics/��.wav");

    //��
    music::load(BGM_NUMDER, L"./Data/Musics/�����O��.wav");

    //�����N
    music::load(BGM_RANK, L"./Data/Musics/�����N.wav");

    //�J�E���g�_�E��
    music::load(BGM_COUNTDOWN, L"./Data/Musics/�J�E���g�_�E��.wav");

    //���o��
    music::load(BGM_BIRD, L"./Data/Musics/���o��.wav");

    //�������O
    music::load(BGM_RAINBOW, L"./Data/Musics/�������O.wav");

    //�V�[�N���b�g�����O
    music::load(BGM_SECRET, L"./Data/Musics/�V�[�N���b�g.wav");

    //�u�[�X�g
    music::load(BGM_BOOST, L"./Data/Musics/�u�[�X�g.wav");

}

void audio_deinit()
{


    music::clear();
}