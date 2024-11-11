#include "scene_title.h"
#include "common.h"
#include "system.h"






StartButton Start;
Sprite* sprStart;
Sprite* sprLogo;



using namespace input;

void title_init()
{
    Start.title_state = 0;
    Start.title_timer = 0;
    Start.fadeBlack = 0.0f;
    Start.isFadeOut = false; // �t�F�[�h�A�E�g���Ă��Ȃ�
    Start.isClicked = false; // �N���b�N����Ă��Ȃ�
    Start.fadeTimer = 0.0f;
    Start.clickTimer = 0.0f;
    Start.clickCount = 0;

    
}

void title_deinit()
{
    safe_delete(sprStart);
    safe_delete(sprLogo);
}

void title_update()
{
    switch (Start.title_state)
    {
    case 0:
        sprStart = sprite_load(L"./Data/Images/startButton1.png");
        sprLogo = sprite_load(L"./Data/Images/titlelogo.png");
        Start.title_move_timer = 0;
        Start.title_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };  // ���S�ʒu
        Start.scale = { 1.0f, 1.0f };
        Start.texPos = { 0, 0 };
        Start.texSize = { TITLE_TEX_W, TITLE_TEX_H };
        Start.pivot = { TITLE_PIVOT_X, TITLE_PIVOT_Y };
        Start.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        Start.title_state++;
        /*fallthrough*/
    case 2:
        click_act();
        title_act();
        break;
    }

    Start.title_timer++;
}

void title_render()
{
    GameLib::clear(0.0, 0.0, 0.0);
    

        sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
            Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
            Start.pivot.x, Start.pivot.y);

        sprite_render(sprLogo,SCREEN_W*0.5, Start.titlePos.y, 3, 3, 0, 0, 469, 100, 469 / 2, 100 / 2);
    

    
    debug::setString("clickTimer%f", Start.clickTimer);
    debug::setString("fadeBlack%f", Start.fadeBlack);
    debug::setString("fadeTimer%f", Start.fadeTimer);
    debug::setString("clickConut%d", Start.clickCount);
    debug::setString("title_timer%d", Start.title_timer);
    debug::setString("title_state%d", Start.title_state);
 

    // ��ʑS�̂Ƀt�F�[�h��K�p
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Start.fadeBlack);
}

void click_act()
{
    if (Start.clickCount ==0 && !Start.isFadeOut && (TRG(0) & L_CLICK) && click())
        //�N���b�N����Ă��Ȃ����t�F�[�h�A�E�g���Ă��Ȃ������N���b�N��͈͓��ł��Ă����
    {
        Start.isClicked = true;
        Start.clickCount = 1;  //�N���b�N�J�E���g�𑝂₷
        Start.scale = { 1.0f, 1.0f };  // �X�P�[��������������
    }

    if (Start.isClicked && !Start.isFadeOut)
    {
        Start.clickTimer += 0.2f;

        if (Start.clickTimer >= 2.0f)  // �N���b�N�^�C�}�[���Q�b�ȏ�ɂȂ�����
        {
            // �X�P�[����傫������
            Start.scale = { 1.3f, 1.3f };
        }

        if (Start.clickTimer >= 10.0f)  // �P�O�o������t�F�[�h�A�E�g������
        {
            Start.isFadeOut = true;
            Start.clickTimer = 0.0f;
        }
    }

    if (Start.isFadeOut)  // �t�F�[�h�A�E�g����
    {
        Start.fadeBlack += 0.05f;
        if (Start.fadeBlack >= 1.0f) {
            Start.fadeBlack = 1.0f;
            Start.fadeTimer += 0.1f;
            if (Start.fadeTimer >= 10.0f) {
                tuto_start();  //�Q�[����ʂֈڍs����֐�
                Start.fadeTimer = 0.0f;
            }
        }
    }
    else if (Start.clickCount == 0&&!click())  // �N���b�N����Ă��Ȃ����͈͓��łȂ����
    {
        Start.scale = { 1.0f, 1.0f };//���̂܂܂̃X�P�[��
    }
    if (Start.clickCount == 0 && click()) //�N���b�N����Ă��邩�͈͓��Ȃ�
    {
        Start.scale = { 1.3f, 1.3f };//�X�P�[����傫������
    }
}



bool click()//�}�E�X�J�[�\���Ɖ摜�̓����蔻��
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = Start.position.x + (Start.texSize.x * Start.scale.x) / 2;
    float button_left = Start.position.x - (Start.texSize.x * Start.scale.x) / 2;
    float button_top = Start.position.y - (Start.texSize.y * Start.scale.y) / 2;
    float button_bottom = Start.position.y + (Start.texSize.y * Start.scale.y) / 2;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void title_act() {
    /*
    VECTOR2 titlePos;
    int title_move_timer;
    float title_angle;
    */
    Start.title_move_timer++;
    //titlePos.x = SCREEN_W / 2 + cos(title_angle) * 20;
    Start.titlePos.y = SCREEN_H * 0.3f + sin(Start.title_angle) * 20;
    Start.title_angle += ToRadian(1);


}
