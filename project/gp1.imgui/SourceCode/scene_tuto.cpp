#include "common.h"
#include "scene_tuto.h"
#include "system.h"
using namespace std;
using namespace input;

Sprite* sprBack;
Sprite* sprPlay;
Sprite* sprSetu[5]; // �`���[�g���A���y�[�W�̔z��
Sprite* sprNextpage;
Sprite* sprBackpage;
//Sprite* sprSousa;

TUTO Tuto;
TutoButton tutoButton;

void tuto_init()
{
    Tuto.tuto_timer = 0;
    Tuto.tuto_state = 0;
    Tuto.scroll_x = 0.0f;
    Tuto.tuto_fadein = 1.0f;
    Tuto.tuto_fadeout = 0.0f;

    Tuto.isTuto_Fadein = false;
    Tuto.isTuto_Fadeout = false;
    Tuto.click_delay_timer = 0.0f;
    Tuto.tuto_fadeTimer = 0.0f;
    Tuto.currentPage = 0; // ���݂̃y�[�W

    tutoButton.position = { SCREEN_W*0.95f,SCREEN_H *0.5f };
    tutoButton.scale = { 1.0f, 1.0f };
    tutoButton.texPos = { 0, 0 };
    tutoButton.texSize = { BUTTON_TEX_W, BUTTON_TEX_H };
    tutoButton.pivot = { BUTTON_PIVOT_X, BUTTON_PIVOT_Y };
    tutoButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

}

void tuto_update()
{
    switch (Tuto.tuto_state)
    {
    case 0:
        
        sprBack = sprite_load(L"./Data/Images/back(��).png");
        sprPlay = sprite_load(L"./Data/Images/�Q�[���摜(��).png");
        sprSetu[0] = sprite_load(L"./Data/Images/����1.png");
        sprSetu[1] = sprite_load(L"./Data/Images/����2.png");
        sprSetu[2] = sprite_load(L"./Data/Images/����3.png");
        sprSetu[3] = sprite_load(L"./Data/Images/����4.png");
        sprSetu[4] = sprite_load(L"./Data/Images/����1.png");
        sprNextpage = sprite_load(L"./Data/images/nextpage.png");
        sprBackpage = sprite_load(L"./Data/Images/backpage.png");

        Tuto.tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        Tuto.tuto_state++;
        /*fallthrough*/
    case 2:
        fadeIn_act();
        break;

    case 3:
        //���N���b�N�Ńy�[�W�ړ�
        tuto_click_act();
            
        
        break;

    case 4:
        fadeOut_act();
        break;
    }

    Tuto.scroll_x += 0.5f;
    if (Tuto.scroll_x >= 5464) {
        Tuto.scroll_x = 0.0f;
    }

    Tuto.tuto_timer++;
}

void tuto_render()
{
    GameLib::clear(1.0, 0.0, 0.0);

    // �w�i�X�N���[��
    sprite_render(sprBack, -Tuto.scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2);
    sprite_render(sprBack, -Tuto.scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2);

    // �t�F�[�h�C���E�t�F�[�h�A�E�g�G�t�F�N�g
    float scale_up = 1.0f + (1.0f - Tuto.tuto_fadein);
    float scale_down = 1.0f - Tuto.tuto_fadeout * 0.5f;
    VECTOR2 scale = { 1.0f * scale_up * scale_down , 1.0f * scale_up * scale_down };

    // �`���[�g���A���y�[�W�`��
    sprite_render(sprSetu[Tuto.currentPage], SCREEN_W * 0.5f, SCREEN_H * 0.5f, scale.x, scale.y, 0, 0, 800, 550, 800 / 2, 550 / 2);

    // �X�P�[�����ő�i�Ⴆ��1.5�j�ɂȂ�������{�^����\��
    if (scale.x >= 2.0f && scale.y >= 2.0f)
    {
        sprite_render(sprNextpage, SCREEN_W - 100, SCREEN_H * 0.5,1.0f,1.0f, 0, 0, 103, 192, 103 / 2, 192 / 2);
    }


    
    if (Tuto.tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadein);
    }

    if (Tuto.tuto_fadeout > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadeout);
    }
}

void tuto_deinit()
{
    safe_delete(sprBack);
    for (int i = 0; i < 5; i++) safe_delete(sprSetu[i]);
    safe_delete(sprPlay);
}

// Fade-in process
void fadeIn_act()
{
    if (!Tuto.isTuto_Fadein)
    {
        Tuto.isTuto_Fadein = true;
    }

    if (Tuto.isTuto_Fadein)
    {
        Tuto.tuto_fadein -= 0.03f;
        if (Tuto.tuto_fadein <= 0.0f)
        {
            Tuto.tuto_fadein = 0.0f;
            Tuto.isTuto_Fadein = false;
            Tuto.tuto_state++;
        }
    }
}

// Fade-out process
void fadeOut_act()
{
    if (Tuto.isTuto_Fadeout)
    {
        Tuto.click_delay_timer += 0.03f;
        if (Tuto.click_delay_timer >= 2.0f)
        {
            Tuto.tuto_fadeout += 0.05f;
            if (Tuto.tuto_fadeout >= 1.0f)
            {
                Tuto.tuto_fadeout = 1.0f;
                Tuto.tuto_fadeTimer += 0.1f;
                if (Tuto.tuto_fadeTimer >= 10.0f)
                {
                    game_start();
                    Tuto.tuto_fadeTimer = 0.0f;
                }
            }
        }
    }
}

void tuto_click_act() 
{
    if ((TRG(0) & L_CLICK) && tuto_click()) 
    {
        tutoButton.scale = { 1.0f, 1.0f };  // �X�P�[��������������
        Tuto.currentPage++;
    }
    else if (!tuto_click())  // �N���b�N����Ă��Ȃ����͈͓��łȂ����
    {
        tutoButton.scale = { 1.0f, 1.0f };//���̂܂܂̃X�P�[��
    }
    if (tuto_click()) 
    {
        tutoButton.scale = { 1.3f, 1.3f };  // �X�P�[��������������
    }
    
    
    if(Tuto.currentPage>3) {//�Ō�̃y�[�W�̎��ɍ��N���b�N������
        Tuto.isTuto_Fadeout = true;
        Tuto.click_delay_timer = 0.0f;
        Tuto.tuto_state++;
    }
}

bool tuto_click() 
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = tutoButton.position.x + (tutoButton.texSize.x * tutoButton.scale.x) ;
    float button_left = tutoButton.position.x - (tutoButton.texSize.x * tutoButton.scale.x) ;
    float button_top = tutoButton.position.y - (tutoButton.texSize.y * tutoButton.scale.y) ;
    float button_bottom = tutoButton.position.y + (tutoButton.texSize.y * tutoButton.scale.y) ;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}
