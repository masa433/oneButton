#include "audio.h"
#include "../GameLib/game_lib.h"
#include "common.h"
void audio_init()
{
#if 1

#endif
    //ゲーム内
    music::load(0, L"./Data/Musics/ゲーム.wav");

    //タイトル
    music::load(1, L"./Data/Musics/タイトル.wav");

    //説明画面
    music::load(2, L"./Data/Musics/説明画面.wav");

    //リザルト
    music::load(3, L"./Data/Musics/リザルト.wav");

    //ボタン
    music::load(4, L"./Data/Musics/ボタン.wav");
   
    //リング
    music::load(5, L"./Data/Musics/リング.wav");

    //風船
    music::load(6, L"./Data/Musics/風船.wav");

    //水
    music::load(7, L"./Data/Musics/水.wav");

    //数
    music::load(8, L"./Data/Musics/リング数.wav");

    //ランク
    music::load(9, L"./Data/Musics/ランク.wav");

#if 21
#endif
}

void audio_deinit()
{
#if 2

#endif

    music::clear();
}