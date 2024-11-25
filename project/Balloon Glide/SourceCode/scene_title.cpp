#include "scene_title.h"
#include "common.h"
#include "system.h"
#include "audio.h"






StartButton Start;
Sprite* sprStart;
Sprite* sprLogo;
Sprite* sprTitleBack;
Sprite* sprBalloon;

const int BALLOON_MAX = 20;
Balloon balloons[BALLOON_MAX];


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
    Start.fadeIn = 1.0f;
    Start.fadeAfterTimer = 0;
    
    Start.isFadeIn = false;

    for (int i = 0; i < BALLOON_MAX; i++) {
       
        
        balloons[i].active = false;
    }
    
}

void title_deinit()
{
    safe_delete(sprStart);
    safe_delete(sprLogo);
    safe_delete(sprTitleBack);
    safe_delete(sprBalloon);
    
    music::stop(BGM_TITLE);
    music::stop(BGM_BUTTON);
}

void title_update()
{
    switch (Start.title_state)
    {
    case 0:
        sprStart = sprite_load(L"./Data/Images/startButton1.png");
        sprLogo = sprite_load(L"./Data/Images/title.png");
        sprTitleBack = sprite_load(L"./Data/Images/title_haikei.png");
        sprBalloon = sprite_load(L"./Data/Images/balloon.png"); 
        Start.title_move_timer = 0;
        Start.title_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        music::play(BGM_TITLE, true);
        Start.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };  // ���S�ʒu
        Start.scale = { 1.0f, 1.0f };
        Start.texPos = { 0, 0 };
        Start.texSize = { TITLE_TEX_W, TITLE_TEX_H };
        Start.pivot = { TITLE_PIVOT_X, TITLE_PIVOT_Y };
        Start.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        Start.title_state++;
        /*fallthrough*/
    case 2:

        title_fadeIn_act();
        if (Start.fadeAfterTimer >= 20) 
        {
            click_act();
        }
        
        title_act();
        balloon_act();

        break;
    }

    Start.title_timer++;
}

void title_render() {
    GameLib::clear(0.0, 0.0, 0.0);

    // �w�i��`��
    sprite_render(sprTitleBack, SCREEN_W * 0.5f, SCREEN_H * 0.5f, 0.8f, 0.8f, 0, 0, 2732, 2048, 2732 / 2.0f, 2048 / 2.0f);

    // ���D��`��
    for (int i = 0; i < BALLOON_MAX; i++) {
        if (balloons[i].active) {
            int spriteIndex = i % 6; // 6�F�����[�v�őI��
            int texWidth = 140;      // ���D�̕���ݒ�
            int texHeight = 330;     // ���D�̍�����ݒ�
            int texX = spriteIndex * texWidth;
            int texY = 0;

            // ���D�̃X�v���C�g��`��
            sprite_render(sprBalloon, balloons[i].position.x, balloons[i].position.y, balloons[i].scale, balloons[i].scale,
                texX, texY, texWidth, texHeight, texWidth / 2, texHeight / 2);
        }
    }

    // �^�C�g�����S��`��
    sprite_render(sprLogo, SCREEN_W * 0.5f, Start.titlePos.y, 0.7f, 0.7f, 0, 0, 2800, 1000, 2800 / 2.0f, 1000 / 2.0f, ToRadian(5));

    // �X�^�[�g�{�^����`��
    sprite_render(sprStart, Start.position.x, Start.position.y, Start.scale.x, Start.scale.y,
        Start.texPos.x, Start.texPos.y, Start.texSize.x, Start.texSize.y,
        Start.pivot.x, Start.pivot.y);

    

    // �t�F�[�h�A�E�g��`��
    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Start.fadeBlack);

    primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Start.fadeIn);
}


//�N���b�N�����Ƃ��̏���
void click_act()
{
    if (Start.clickCount ==0 && !Start.isFadeOut && (TRG(0) & L_CLICK) && click())
        //�N���b�N����Ă��Ȃ����t�F�[�h�A�E�g���Ă��Ȃ������N���b�N��͈͓��ł��Ă����
    {
        Start.isClicked = true;
        Start.clickCount = 1;  //�N���b�N�J�E���g�𑝂₷
        Start.scale = { 1.0f, 1.0f };  // �X�P�[��������������
        music::play(BGM_BUTTON, false);
        

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
        Start.fadeBlack += 0.02f;

        // ���ʂ����X�ɉ����鏈��
        float volume = 1.0f - Start.fadeBlack; // ���ʂ��t�F�[�h�A�E�g�̐i�s�x�ɍ��킹�Ē���
        if (volume < 0.0f) volume = 0.0f; // �ŏ��l��0
        music::setVolume(BGM_TITLE, volume); // ���ʂ�ݒ�

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
    float button_top = Start.position.y - (Start.texSize.y * Start.scale.y) / 1.8;
    float button_bottom = Start.position.y + (Start.texSize.y * Start.scale.y) / 2.0;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void title_act() 
{
    
    Start.title_move_timer++;
    //titlePos.x = SCREEN_W / 2 + cos(title_angle) * 20;
    Start.titlePos.y = SCREEN_H * 0.3f + sin(Start.title_angle) * 20;
    Start.title_angle += ToRadian(1);


}

void balloon_act() 
{
    for (int i = 0; i < BALLOON_MAX; i++) {
        if (!balloons[i].active) {

            balloons[i].active = true;


            balloons[i].position.x = 200 + rand() % (1700 - 200 + 1); // 200�`1700 �͈̔͂ɐݒ�
            balloons[i].position.y = SCREEN_H + (100+rand() % 250);
            balloons[i].speed = 1.0f + (rand() % 3);
            balloons[i].scale = 0.5f + (rand() % 5) * 0.1f;
        }

        // y�̒l���X�s�[�h�Ō��炵�Ă���
        balloons[i].position.y -= balloons[i].speed;

        // ��ʊO�ɂ������烊�Z�b�g
        if (balloons[i].position.y < -150) {
            balloons[i].active = false;
        }
    }
}

// �t�F�[�h�C��
void title_fadeIn_act()
{
    if (!Start.isFadeIn)
    {

        Start.isFadeIn = true;
        Start.fadeAfterTimer++;
    }

    if (Start.isFadeIn)
    {
        Start.fadeIn -= 0.03f;
        if (Start.fadeIn <= 0.0f)
        {

            Start.fadeIn = 0.0f;
            Start.isFadeIn = false;
            
            
        }
    }
}
