#include "back.h"
#include "common.h"

int back_state;
int back_frameCounter = 0; // �t���[���J�E���^�[

using namespace std;

struct Sky_DATA
{
    Sprite* spr;
    const wchar_t* filePath;
} skyData[] = {
    { NULL, L"./Data/Images/game_haikei/1.png"},
    { NULL, L"./Data/Images/game_haikei/2.png"},
    { NULL, L"./Data/Images/game_haikei/3.png"},
    { NULL, L"./Data/Images/game_haikei/4.png"},
    { NULL, L"./Data/Images/game_haikei/5.png"},
    { NULL, L"./Data/Images/game_haikei/6.png"},
    { NULL, L"./Data/Images/game_haikei/7.png"},
    { NULL, L"./Data/Images/game_haikei/8.png"},
    { NULL, L"./Data/Images/game_haikei/9.png"},
    { NULL, L"./Data/Images/game_haikei/10.png"},
    { NULL, L"./Data/Images/game_haikei/11.png"},
    { NULL, L"./Data/Images/game_haikei/12.png"},
    { NULL, L"./Data/Images/game_haikei/13.png"},
    { NULL, L"./Data/Images/game_haikei/14.png"},
    { NULL, L"./Data/Images/game_haikei/15.png"},
    { NULL, L"./Data/Images/game_haikei/16.png"},
    { NULL, L"./Data/Images/game_haikei/17.png"},
};

void back_init()
{
    back_state = 0;
    back_frameCounter = 0;
    for (int i = 0; i < SKY; i++) {
        skyData[i].spr = sprite_load(skyData[i].filePath);
    }
}

void back_deinit()
{
    for (int i = 0; i < SKY; i++) {
        safe_delete(skyData[i].spr);
    }
}

void back_update()
{
    const float FRAME_DELAY = 5.3f; // �w�i���؂�ւ��܂ł̃t���[����

    // �t���[���J�E���^�[���ݒ�l�ɒB������w�i��؂�ւ���
    if (back_frameCounter >= FRAME_DELAY) {
        back_frameCounter = 0; // �J�E���^�[�����Z�b�g
        if (back_state >= SKY-1 ) {
            back_state = 0; // �Ō�̉摜�̎��͍ŏ��̉摜�ɖ߂�
        }
        else {
            back_state++;
        }
    }
    else {
        back_frameCounter++; // �J�E���^�[�𑝉�
    }
}

void back_render()
{
    sprite_render(
        skyData[back_state].spr,
        SCREEN_W * 0.5, SCREEN_H * 0.5,
        0.8f, 0.6f,
        0, 0,
        2732, 2048,
        2732 / 2, 2048 / 2,
        ToRadian(0),
        1, 1, 1, 1
    );
}
