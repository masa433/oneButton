#include "audio.h"
#include "../GameLib/game_lib.h"
#include "common.h"
void audio_init()
{

    //ゲーム内
    music::load(BGM_GAME, L"./Data/Musics/ゲーム.wav");

    //タイトル
    music::load(BGM_TITLE, L"./Data/Musics/タイトル.wav");

    //説明画面
    music::load(BGM_TUTO, L"./Data/Musics/説明画面.wav");

    //リザルト
    music::load(BGM_RESULT, L"./Data/Musics/リザルト.wav");

    //ボタン
    music::load(BGM_BUTTON, L"./Data/Musics/ボタン.wav");
   
    //リング
    music::load(BGM_RING, L"./Data/Musics/リング.wav");

    //風船
    music::load(BGM_BALLOON, L"./Data/Musics/風船.wav");

    //水
    music::load(BGM_WATER, L"./Data/Musics/水.wav");

    //数
    music::load(BGM_NUMDER, L"./Data/Musics/リング数.wav");

    //ランク
    music::load(BGM_RANK, L"./Data/Musics/ランク.wav");

    //カウントダウン
    music::load(BGM_COUNTDOWN, L"./Data/Musics/カウントダウン.wav");

}

void audio_deinit()
{


    music::clear();
}